#include "linear_search.h"

int linearSearch(Buffer* buf, unsigned int blk_start, unsigned int blk_end, unsigned int* write_blk)
{
    int equal_num;
    unsigned int blk_num = blk_start;
    unsigned char *re_blk;
    int count = 0;

    int re_blk_count = 0;

    // get equal number
    do
    {
        printf("-------------------------------------\n");
        printf("�������������Ĺ�ϵѡ���㷨��S.C = ");
        equal_num = getNumber();
    } while(equal_num == -1);
    printf("-------------------------------------\n");

    // block for result
    re_blk = getNewBlockInBuffer(buf);
    memset(re_blk, 0, buf->blkSize*sizeof(unsigned char));
    if(!re_blk)
        return -1;

    // linear search
    while(blk_num != blk_end)
    {
        blk_num = findInBlk(equal_num, blk_num, buf, re_blk, &re_blk_count, write_blk, &count);
        if(blk_num == -1)
        {
            return -1;
        }
    }
    blk_num = findInBlk(equal_num, blk_num, buf, re_blk, &re_blk_count, write_blk, &count);
    if(blk_num == -1)
    {
        return -1;
    }

    // write final result
    printf("ע�����д�����%d\n",*write_blk);
    if(writeBlockToDisk(re_blk, *write_blk, buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    *write_blk = *write_blk+1;

    // IOcount
    printf("����ѡ��������Ԫ��һ��%d��\n", count);
    printf("IO��дһ��%ld��\n", buf->numIO);
    printf("\n");
    return 0;
}

int findInBlk(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* blk_count, unsigned int* write_blk, int* count_num)
{
    unsigned char* blk;
    int X, Y;
    int re_blk_count = *blk_count;
    int addr;

    printf("�������ݿ�%d\n", blk_num);
    if((blk = readBlockFromDisk(blk_num, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }

    // (X,Y)
    for(int i = 0; i < 7; i++)
    {
        getXY(&X, &Y, blk, i);

        // equal
        if(X == equal_num)
        {
            printf("(X=%d, Y=%d)\n", X, Y);
            *count_num = *count_num + 1;

            // re_blk is full, write to disk and update re_blk
            if(re_blk_count >= 7)
            {
                printf("ע�����д�����%d\n",*write_blk);
                if(writeAddr(*write_blk+1, re_blk) == -1)
                {
                    return -1;
                }
                if(writeBlockToDisk(re_blk, *write_blk, buf) != 0)
                {
                    perror("Writing Block Failed!\n");
                    return -1;
                }
                *write_blk = *write_blk+1;
                re_blk = getNewBlockInBuffer(buf);
                memset(re_blk, 0, buf->blkSize*sizeof(unsigned char));
                re_blk_count = 0;
            }

            // write
            if(writeXY(X, Y, re_blk, re_blk_count) == -1)
            {
                return -1;
            }
            re_blk_count++;
        }
    }
    *blk_count = re_blk_count;

    // addr
    getAddr(&addr, blk);

    freeBlockInBuffer(blk, buf);
    return addr;
}
