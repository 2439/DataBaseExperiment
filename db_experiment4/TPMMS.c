#include "TPMMS.h"

int TPMMS(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk)
{
    unsigned char* blk;
    unsigned int blk_num = blk_start;
    unsigned int write_blk_temp = write_blk +100;
    int blk_count = 0;

    printf("-----------------------------------------------\n");
    printf("两阶段多路归并排序算法：数据块%d.blk到%d.blk\n", blk_start, blk_end);
    printf("-----------------------------------------------\n");

    // inline sorting
    while(blk_num != blk_end)
    {
        blk = readBlockFromDisk(blk_num, buf);
        blk_num = getFour(blk+7*8);
        if(buf->numFreeBlk == 0)
        {
            inSort(buf, &write_blk_temp);
            freeAllBlockInBuffer(buf);
        }
    }
    blk = readBlockFromDisk(blk_num, buf);
    inSort(buf, &write_blk_temp);
    freeAllBlockInBuffer(buf);

    return 0;
}

int inSort(Buffer* buf, unsigned int* write_blk)
{
    int blk_count = buf->numAllBlk - buf->numFreeBlk;
    int couple_count = blk_count * 7;
    int min_data, temp, min_location;

    // sorting in buf
    for(int i = 0; i < couple_count - 1; i++)
    {
        min_data = getXIFromBuf(buf, i);
        min_location = i;
        for(int j = i+1; j < couple_count; j++)
        {
            temp = getXIFromBuf(buf, j);
            if(temp < min_data)
            {
                min_data = temp;
                min_location = j;
            }
        }
        if(i != j)
        {
            swapIJInBuf(buf, i, j);
        }
    }

    // write to disk

}

