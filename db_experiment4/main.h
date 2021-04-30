#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linear_search.h"

#define LINEARSEARCHWRITEBLK 101
#define TPMMSWRITEBLK 301

typedef struct table
{
    unsigned int blk_start;
    unsigned int blk_end;
} Table;


/** \brief ��ʼ��R��S��Ŀ�ʼ�ͽ������ݿ�
 *
 * \param R��R��
 * \param S��S��
 *
 */
void initTableRS(Table* R, Table* S);

/** \brief ��ӡ�˵���ѡ���㷨
 *
 * \return ѡ��1-7���˳�0��������������
 *
 */
int menuList();

#endif // MAIN_H_INCLUDED
