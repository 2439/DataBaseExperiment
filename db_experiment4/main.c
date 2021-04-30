#include "main.h"

int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    Table R, S;
    unsigned int linearSearchWriteBlk = LINEARSEARCHWRITEBLK;
    unsigned int TPMMSWriteBlk = TPMMSWRITEBLK;
    int choose = 0;
    int re = 0;

    initTableRS(&R, &S);

    choose = menuList();    // print list and choose algorithm
    while(choose != 0)
    {
        /* Initialize the buffer */
        if (!initBuffer(520, 64, &buf))
        {
            perror("Buffer Initialization Failed!\n");
            return -1;
        }
        switch(choose)
        {
        case 1:
            re = linearSearch(&buf, S.blk_start, S.blk_end, &linearSearchWriteBlk);
            if(re == -1)
                printf("there is something error in linearSearch\n");
            break;
        case 2:
            re = TPMMS(&buf, R.blk_start, R.blk_end, &TPMMSWriteBlk);
            if(re == -1)
                printf("There is something error in TPMMS\n");
            re = TPMMS(&buf, S.blk_start, S.blk_end, &TPMMSWriteBlk);
            if(re == -1)
                printf("There is something error in TPMMS\n");
            break;
        case 3:
            printf("基于索引的关系选择算法\n");
            break;
        case 4:
            printf("基于排序的连接操作算法\n");
            break;
        case 5:
            printf("集合并算法\n");
            break;
        case 6:
            printf("集合交算法\n");
            break;
        case 7:
            printf("集合差算法\n");
            break;
        case 0:
            break;
        default:
            printf("input error, please input again\n");
            break;
        }
        if(choose == 0)
            break;
        else
        {
            choose = menuList();
        }
    }

    return 0;
}

void initTableRS(Table* R, Table* S)
{
    R->blk_start = 1;
    R->blk_end = 16;
    S->blk_start = 17;
    S->blk_end = 48;
}

int menuList()
{
    char ch;
    char re;
    int input;
    int count = 0;
    printf("1.基于线性搜索的关系选择算法\n");
    printf("2.两阶段多路归并排序算法\n");
    printf("3.基于索引的关系选择算法\n");
    printf("4.基于排序的连接操作算法\n");
    printf("5.集合并算法\n");
    printf("6.集合交算法\n");
    printf("7.集合差算法\n");
    printf("请输入算法选择：");

    ch = (char)getchar();
    while(ch != '\n')
    {
        if(ch > '7' || ch < '0')
        {
            printf("input error, please input again\n");
            fflush(stdin);
            ch = (char)getchar();
            count = 0;
            continue;
        }
        if(count != 0)
        {
            printf("input too long, please input again\n");
            fflush(stdin);
            ch = (char)getchar();
            count = 0;
            continue;
        }
        count++;
        re = ch;
        ch = (char)getchar();
    }
    sscanf(&re, "%d", &input);
    return input;
}
