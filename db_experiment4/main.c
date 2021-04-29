#include "main.h"

int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    Table R, S;
    int choose = 0;
    int re = 0;

    initTableRS(&R, &S);
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    choose = menuList();    // print list and choose algorithm
    while(choose != 0)
    {
        switch(choose)
        {
        case 1:
            re = linearSearch(&buf, R.blk_start, R.blk_end);
            if(re == -1)
                printf("there is something error in linearSearch\n");
            break;
        case 2:
            printf("两阶段多路归并排序算法\n");
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

/** \brief 初始化R和S表的开始和结束数据块
 *
 * \param R：R表
 * \param S：S表
 *
 */

void initTableRS(Table* R, Table* S)
{
    R->blk_start = 1;
    R->blk_end = 16;
    S->blk_start = 17;
    S->blk_end = 48;
}

/** \brief 打印菜单并选择算法
 *
 * \return 选择1-7，退出0，其余重新输入
 *
 */

int menuList()
{
    char ch;
    int input;
    printf("1.基于线性搜索的关系选择算法\n");
    printf("2.两阶段多路归并排序算法\n");
    printf("3.基于索引的关系选择算法\n");
    printf("4.基于排序的连接操作算法\n");
    printf("5.集合并算法\n");
    printf("6.集合交算法\n");
    printf("7.集合差算法\n");
    printf("请输入算法选择：");

    ch = (char)getchar();
    fflush(stdin);
    while(ch > '7' || ch < '0')
    {
        printf("input error, please input again\n");
        ch = (char)getchar();
        fflush(stdin);
    }
    sscanf(&ch, "%d", &input);
    return input;
}
