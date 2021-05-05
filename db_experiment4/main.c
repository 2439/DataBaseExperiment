#include "main.h"

int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    Table R, S, index_S, inline_R, inline_S;
    unsigned int linearSearchWriteBlk = LINEARSEARCHWRITEBLK;
    unsigned int TPMMSWriteBlk = TPMMSWRITEBLK;
    unsigned int indexBlk = INDEXBLK;
    unsigned int indexSearchBlk = INDEXSEARCHBLK;
    unsigned int joinBlk = JOINBLK;
    int choose = 0;
    int re = 0;
    int flag = 0;

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    
    initTableRS(&R, &S);

    choose = menuList();    // print list and choose algorithm
    while(choose != 0)
    {
        if(choose > 2 && flag == 0)
        {
            printf("You must make choose 2 first\n");
            choose = 2;
        }

        switch(choose)
        {
        case 1:
            // linearSearch
            re = linearSearch(&buf, S.blk_start, S.blk_end, &linearSearchWriteBlk);
            if(re == -1)
                printf("there is something error in linearSearch\n");
            break;
        case 2:
            // TPMMS
            // R sort and inline_R
            inline_R.blk_start = TPMMSWriteBlk;
            re = TPMMS(&buf, R.blk_start, R.blk_end, &TPMMSWriteBlk);
            inline_R.blk_end = TPMMSWriteBlk - 1;

            buf.numIO = 0;
            freeAllBlockInBuffer(&buf);
            if(re == -1)
                printf("There is something error in TPMMS\n");

            // S sort and inline_S
            inline_S.blk_start = TPMMSWriteBlk;
            re = TPMMS(&buf, S.blk_start, S.blk_end, &TPMMSWriteBlk);
            inline_S.blk_end = TPMMSWriteBlk - 1;

            if(re == -1)
                printf("There is something error in TPMMS\n");
            freeAllBlockInBuffer(&buf);

            // make index S
            index_S.blk_start = indexBlk;
            makeIndex(&buf, inline_S.blk_start, inline_S.blk_end, &indexBlk);
            index_S.blk_end = indexBlk - 1;

            flag = 1;
            break;
        case 3:
            // indexSearch
            re = indexSearch(&buf, index_S.blk_start, index_S.blk_end, &indexSearchBlk);
            if(re == -1)
                printf("There is something error in indexSearch\n");
            break;
        case 4:
            // with sorted R S, join R.A == S.C
            re = joinWithSort(&buf, inline_R.blk_start, inline_R.blk_end, inline_S.blk_start, inline_S.blk_end, &joinBlk);
            if(re == -1)
                printf("There is something error in joinWithSort\n");
            break;
        case 5:
            // 
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
            buf.numIO = 0;
            freeAllBlockInBuffer(&buf);
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
