#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linear_search.h"
#include "TPMMS.h"
#include "index_search.h"
#include "join_with_sort.h"

#define LINEARSEARCHWRITEBLK 101
#define TPMMSWRITEBLK 201
#define INDEXBLK 301
#define INDEXSEARCHBLK 351 
#define JOINBLK 401

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
