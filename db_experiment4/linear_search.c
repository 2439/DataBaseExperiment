#include "linear_search.h"

int linearSearch(Buffer* buf, int blk_start, int blk_end)
{
    int equal_num;
    int blk_num = blk_start;

    // get equal number
    do
    {
        printf("-------------------------------------\n");
        printf("�������������Ĺ�ϵѡ���㷨��R.A = ");
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

/** \brief ���0-9999������
 *
 * \return �ɹ��򷵻����֣����򷵻�-1
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

/** \brief ��blk_num�Ŀ��в��ҵ�һ��Ԫ�ص���equal_num��Ԫ�飬������һ���ַ
 *
 * \param   equal_num�����ڵ�ֵ
 * \param   blk_num�����ҵĿ�
 * \return ��һ���ַ�����󷵻�-1
 *
 */

int findInBlk(int equal_num, int blk_num, Buffer* buf)
{
    printf("blk_num\n");
    reuturn 0;
}
