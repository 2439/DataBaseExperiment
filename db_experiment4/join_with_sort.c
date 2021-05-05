#include "join_with_sort.h"

int joinWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, int* write_blk)
{
    unsigned char* blk_R;
    unsigned char* blk_S;
    unsigned char* blk;
    int R_count, S_count, blk_count, R_begin_i;
    int join_count;
    int RX, RY, SX, SY;
    unsigned int R_begin, R_num;

    printf("----------------------------------\n");
    printf("基于排序的连接操作：\n");
    printf("select S.C, S.D, R.A, R.B from S inner join R on S.C = R.A\n");
    printf("----------------------------------\n");

    // init
    join_count = 0;
    blk_R = readBlockFromDisk(R_start, buf);
    R_count = 0;
    blk_S = readBlockFromDisk(S_start, buf);
    S_count = 0;
    blk = getNewBlockInBuffer(buf);
    blk_count = 0;
    R_num = R_start;
    R_begin = R_start;
    R_begin_i = R_count;

    // join
    while(blk_R != NULL && blk_S != NULL)
    {
        getXY(&RX, &RY, blk_R, R_count);
        getXY(&SX, &SY, blk_S, S_count);
        // RX < SX，R go next, update R_begin
        if(RX < SX)
        {
            blk_R = getNextInJoin(buf, blk_R, &R_count, R_end, &R_num);
            R_begin = R_num;
            R_begin_i = R_count;
        }
        // RX == SX，R go next;
        else if(RX == SX)
        {
            writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
            writeToBlk(buf, RX, RY, blk, &blk_count, write_blk);
            join_count++;
            blk_R = getNextInJoin(buf, blk_R, &R_count, R_end, &R_num);
        }
        // RX > SX, S to next
        // if SX don't change, R turn to R_begin
        // else R_begin turn to R;
        else
        {
            int tempX, tempY;
            blk_S = getNextInJoin(buf, blk_S, &S_count, S_end, NULL);
            if(blk_S == NULL)
                break;
            getXY(&tempX, &tempY, blk_S, S_count);

            // R turn to R_begin
            if(tempX == SX)
            {
                if(R_num == R_begin)
                {
                    R_count = R_begin_i;
                }
                else
                {
                    freeBlockInBuffer(blk_R, buf);
                    blk_R = readBlockFromDisk(R_begin, buf);
                    R_count = R_begin_i;
                    R_num = R_begin;
                }
            }
            // R_begin turn to R
            else
            {
                R_begin = R_num;
                R_begin_i = R_count;
            }
        }
    }
    printf("总共连接%d次\n", join_count);
    printf("\n");
    return 0;
}

unsigned char* getNextInJoin(Buffer* buf, unsigned char* blk, int* count, int blk_end, unsigned int* blk_num)
{
    int addr;

    // 当前块未比较完
    if(*count < 6)
    {
        *count = *count + 1;
        return blk;
    }

    // 获得下一个块
    getAddr(&addr, blk);
    if(addr <= blk_end)
    {
        freeBlockInBuffer(blk, buf);
        blk = readBlockFromDisk(addr, buf);
        *count = 0;
        if(blk_num != NULL)
        {
            *blk_num = addr;
        }
        return blk;
    }

    // 比较完成所有
    return NULL;
}
