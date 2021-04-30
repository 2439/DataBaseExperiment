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

int getFour(unsigned char* blk_begin)
{
    char str[5];
    for(int k = 0; k < 4; k++)
    {
        str[k] = *(blk_begin + k);
    }
    return atoi(str);
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
