#include "base.h"

int getNumber()
{
    char ch[4];
    char c;
    int re = -1;
    int i = 0;

    c = (char)getchar();
    while(c != '\n')
    {
        if(c > '9' || c < '0')
        {
            printf("input error,please input again\n");
            fflush(stdin);
            return -1;
        }
        if(i < 4)
        {
            ch[i++] = c;
        }
        else
        {
            printf("too big number\n");
            fflush(stdin);
            return -1;
        }
        c = (char)getchar();
    }
    re = atoi(ch);
    fflush(stdin);
    return re;
}

void getXY(int* X, int* Y, unsigned char* blk, int i)
{
    *X = getFour(blk+i*8);
    *Y = getFour(blk+i*8+4);
}

void getAddr(int* addr, unsigned char* blk)
{
    *addr = getFour(blk+7*8);
}

int getFour(unsigned char* blk_begin)
{
    char str[5];
    for(int k = 0; k < 4; k++)
    {
        str[k] = *(blk_begin + k);
    }
    return atoi(str);
}

int writeXY(int X, int Y, unsigned char* blk, int i)
{
    if(writeFour(X, blk+i*8) == -1)
    {
        return -1;
    }
    if(writeFour(Y, blk+i*8+4) == -1)
    {
        return -1;
    }
    return 0;
}

int writeAddr(int addr, unsigned char* blk)
{
    return writeFour(addr, blk+7*8);
}

int writeFour(int num, unsigned char* blk_begin)
{
    int first = num / 1000;
    int second = (num % 1000) / 100;
    int third = (num % 100) / 10;
    int forth = num % 10;
    int location = 0;

    if(first > 9)
    {
        printf("too big number\n");
        return -1;
    }

    if(first != 0)
    {
        *(blk_begin+location) = '0' + first;
        *(blk_begin+location+1) = '0' + second;
        *(blk_begin+location+2) = '0' + third;
        *(blk_begin+location+3) = '0' + forth;
        return 0;
    }
    if(second != 0)
    {
        *(blk_begin+location) = '0' + second;
        *(blk_begin+location+1) = '0' + third;
        *(blk_begin+location+2) = '0' + forth;
        return 0;
    }
    if(third != 0)
    {
        *(blk_begin+location) = '0' + third;
        *(blk_begin+location+1) = '0' + forth;
        return 0;
    }
    *(blk_begin+location) = '0' + forth;
    return 0;
}

void freeAllBlockInBuffer(Buffer* buf)
{
    memset(buf->data, 0, buf->bufSize*sizeof(unsigned char));
    buf->numFreeBlk = buf->numAllBlk;
    return;
}

unsigned char* getXILocationFromBuf(Buffer* buf, int i)
{
    int blk_count;
    int blk_avail_count = 0;
    
    for(blk_count = 0; blk_count < buf->numAllBlk; blk_count++)
    {
        if(ifUsingBlk(buf, blk_count))
            blk_avail_count++;
        if(blk_avail_count*7 >= i)
            break;
    }

    int blk_in_addr = ((i - 1) % 7) * 8 + 1;
    int blk_addr = blk_count * (buf->blkSize + 1);
    return buf->data+blk_addr+blk_in_addr;
}

int ifUsingBlk(Buffer* buf, int blk_count)
{
    return *(buf->data + blk_count * (buf->blkSize + 1));
}

unsigned char* getBlkFromBuf(Buffer* buf, int blk_count)
{
    return buf->data + blk_count * (buf->blkSize + 1) + 1;
}

void swapIJInBuf(Buffer* buf, int i, int j)
{
    unsigned char* i_location = getXILocationFromBuf(buf, i);
    unsigned char* j_location = getXILocationFromBuf(buf, j);
    
    int iX = getFour(i_location);
    int iY = getFour(i_location + 4);

    writeFour(getFour(j_location), i_location);
    writeFour(getFour(j_location+4), i_location+4);

    writeFour(iX, j_location);
    writeFour(iY, j_location+4);
    return;
}

int writeAddrBlockToDisk(Buffer* buf, unsigned char* blk, int* write_blk)
{
    if(writeAddr(*write_blk+1, blk) == -1)
    {
        return -1;
    }
    if(writeBlockToDisk(blk, *write_blk, buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    printf("注：将结果写入磁盘%d\n",*write_blk);
    *write_blk = *write_blk+1;
    return 0;
}

void writeToBlk(Buffer* buf, int X, int Y, unsigned char* blk, int* blk_count, unsigned int* write_blk)
{
    // blk is full, write to disk and update blk
    if(*blk_count == 7)
    {
        writeAddrBlockToDisk(buf, blk, write_blk);
        blk = getNewBlockInBuffer(buf);
        memset(blk, 0, buf->blkSize*sizeof(unsigned char));
        *blk_count = 0;
    }

    // write
    writeXY(X, Y, blk, *blk_count);
    *blk_count = *blk_count + 1;
}