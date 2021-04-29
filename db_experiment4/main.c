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
            printf("���׶ζ�·�鲢�����㷨\n");
            break;
        case 3:
            printf("���������Ĺ�ϵѡ���㷨\n");
            break;
        case 4:
            printf("������������Ӳ����㷨\n");
            break;
        case 5:
            printf("���ϲ��㷨\n");
            break;
        case 6:
            printf("���Ͻ��㷨\n");
            break;
        case 7:
            printf("���ϲ��㷨\n");
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

/** \brief ��ʼ��R��S��Ŀ�ʼ�ͽ������ݿ�
 *
 * \param R��R��
 * \param S��S��
 *
 */

void initTableRS(Table* R, Table* S)
{
    R->blk_start = 1;
    R->blk_end = 16;
    S->blk_start = 17;
    S->blk_end = 48;
}

/** \brief ��ӡ�˵���ѡ���㷨
 *
 * \return ѡ��1-7���˳�0��������������
 *
 */

int menuList()
{
    char ch;
    int input;
    printf("1.�������������Ĺ�ϵѡ���㷨\n");
    printf("2.���׶ζ�·�鲢�����㷨\n");
    printf("3.���������Ĺ�ϵѡ���㷨\n");
    printf("4.������������Ӳ����㷨\n");
    printf("5.���ϲ��㷨\n");
    printf("6.���Ͻ��㷨\n");
    printf("7.���ϲ��㷨\n");
    printf("�������㷨ѡ��");

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
