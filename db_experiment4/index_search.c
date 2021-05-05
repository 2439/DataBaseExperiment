#include "index_search.h"

void makeIndex(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk)
{
    printf("创建索引文件\n");
    unsigned char* re_blk;
    unsigned int blk_num = blk_start;
    unsigned char* blk;
    int count = 0;
    int X;

    re_blk = getNewBlockInBuffer(buf);
    while(blk_num != blk_end)
    {
        blk = readBlockFromDisk(blk_num, buf);
        X = getFour(blk + 6 * 8);   // 块内最后一个值
        writeToBlk(buf, X, blk_num, re_blk, &count, write_blk); // write couple into index
        getAddr(&blk_num, blk);
        freeBlockInBuffer(blk, buf);
    }
    blk = readBlockFromDisk(blk_num, buf);
    X = getFour(blk);
    writeToBlk(buf, X, blk_num, re_blk, &count, write_blk);     // write couple from blk_num block into index
    if(count != 0)
    {
        writeAddrBlockToDisk(buf, re_blk, write_blk);  // write index in buffer to disk
    }
    return;
}

int indexSearch(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk)
{
    int equal_num;
    unsigned char* re_blk;
    unsigned int blk_num = blk_start - 1;
    int re_blk_count = 0, count = 0;

    // get equal number
    do
    {
        printf("----------------------------------\n");
        printf("基于索引的关系选择算法：S.C = ");
        equal_num = getNumber();
    } while (equal_num == -1);
    printf("----------------------------------\n");

    // block for result
    re_blk = getNewBlockInBuffer(buf);
    if(!re_blk)
        return -1;
    memset(re_blk, 0, buf->bufSize*sizeof(unsigned char));
    
    // index search
    // index find
    while(blk_num < blk_end)
    {
        blk_num++;
        if(findIndexInBlk(equal_num, &blk_num, buf) == 1)
        {
            break;
        }
    }

    // data find
    while(blk_num != 0)
    {
        blk_num = findInBlkWithSort(equal_num, blk_num, buf, re_blk, &re_blk_count, write_blk, &count);
        if(blk_num < 0)
        {
            return -1;
        }
    }

    // write finial result
    if(writeAddrBlockToDisk(buf, re_blk, write_blk) == -1)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }

    // IOcount
    printf("满足选择条件的元组一共%d个\n", count);
    printf("IO读写一共%ld次\n", buf->numIO);
    printf("\n");
    return 0;
}

int findIndexInBlk(int equal_num, unsigned int* blk_num, Buffer* buf)
{
    unsigned char* blk;
    int X, Y;
    int flag = 0;

    // read
    printf("读入索引块%d\n", *blk_num);
    if((blk = readBlockFromDisk(*blk_num, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }    
    
    // (X,Y)
    for(int i = 0; i < 7; i++)
    {
        getXY(&X, &Y, blk, i);

        // find data blk_num
        if(X >= equal_num)
        {
            *blk_num = Y;
            return 1;
        }
    }

    // don't find, the largest num in the block is smaller than equal_num
    return 0;
}

int findInBlkWithSort(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* re_blk_count, unsigned int* write_blk, int* count)
{
    unsigned char* blk;
    int X, Y;
    int addr;
    int flag = 0;
    
    // read
    printf("读入数据块%d\n", blk_num);
    if((blk = readBlockFromDisk(blk_num, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return -1;
    }

    // (X，Y)
    for(int i = 0; i < 7; i++)
    {
        getXY(&X, &Y, blk, i);

        // unequal
        if(X != equal_num)
        {
            // small
            if(flag == 0)
            {
                continue;
            }
            // big
            return 0;
        }
        // equal
        printf("(X=%d, Y=%d)\n", X, Y);
        *count = *count + 1;
        writeToBlk(buf, X, Y, re_blk, re_blk_count, write_blk);
        flag = 1;
    }

    // addr
    getAddr(&addr, blk);
    freeBlockInBuffer(blk, buf);
    return addr;
}
