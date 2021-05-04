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
        printf("基于线性搜索的关系选择算法：S.C = ");
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
    printf("注：结果写入磁盘%d\n",*write_blk);
    if(writeBlockToDisk(re_blk, *write_blk, buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    *write_blk = *write_blk+1;

    // IOcount
    printf("满足选择条件的元组一共%d个\n", count);
    printf("IO读写一共%ld次\n", buf->numIO);
    printf("\n");
    return 0;
}

int findInBlk(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* blk_count, unsigned int* write_blk, int* count_num)
{
    unsigned char* blk;
    int X, Y;
    int re_blk_count = *blk_count;
    int addr;

    printf("读入数据块%d\n", blk_num);
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
            writeToBlk(buf, X, Y, re_blk, &re_blk_count, write_blk);
        }
    }
    *blk_count = re_blk_count;

    // addr
    getAddr(&addr, blk);

    freeBlockInBuffer(blk, buf);
    return addr;
}
