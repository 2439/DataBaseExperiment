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

    writeOne(first, blk_begin, &location);
    writeOne(second, blk_begin, &location);
    writeOne(third, blk_begin, &location);
    writeOne(forth, blk_begin, &location);
    return 0;
}

void writeOne(int num, unsigned char* blk_begin, int* location)
{
    *(blk_begin+*location) = '0' + num;
    if(num != 0)
        *location = *location + 1;
    return;
}

void freeAllBlockInBuffer(Buffer* buf)
{
    memset(buf->data, 0, buf->bufSize*sizeof(unsigned char*));
    for(int i=0; i<buf->numAllBlk; i++)
    {
        freeBlockInBuffer(buf->data+i*65+1, buf);
    }
    return 0;
}
