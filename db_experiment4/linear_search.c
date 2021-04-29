#include "linear_search.h"

int linearSearch(Buffer* buf, int blk_start, int blk_end)
{
    int equal_num;
    int blk_num = blk_start;

    // get equal number
    do
    {
        printf("-------------------------------------\n");
        printf("基于线性搜索的关系选择算法：R.A = ");
        equal_num = getNumber();
    } while(equal_num == -1);
    printf("-------------------------------------\n");

    // linearSearch
    while(blk_num != blk_end)
    {
        blk_num = findInBlk(equal_num, blk_num, buf);
    }
    findInBlk(equal_num, blk_num, buf);

    return 0;
}

/** \brief 获得0-9999的数字
 *
 * \return 成功则返回数字，否则返回-1
 *
 */
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

/** \brief 在blk_num的块中查找第一个元素等于equal_num的元组，返回下一块地址
 *
 * \param   equal_num：等于的值
 * \param   blk_num：查找的块
 * \return 下一块地址，错误返回-1
 *
 */

int findInBlk(int equal_num, int blk_num, Buffer* buf)
{
    printf("blk_num\n");
    reuturn 0;
}
