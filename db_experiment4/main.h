#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linear_search.h"
#include "TPMMS.h"
#include "index_search.h"
#include "join_with_sort.h"
#include "set_operation.h"

#define LINEARSEARCHWRITEBLK 101
#define TPMMSWRITEBLK 201
#define INDEXBLK 301
#define INDEXSEARCHBLK 351
#define JOINBLK 401
#define UNIONBLK 501
#define INTERSECTIONBLK 601
#define MINUSBLK 701

typedef struct table
{
    unsigned int blk_start;
    unsigned int blk_end;
} Table;


/** \brief ��ʼ����R��S
 *
 * \param R����ϵR
 * \param S����ϵS
 *
 */
void initTableRS(Table* R, Table* S);

/** \brief ��ӡ�˵�����ȡѡ��
 *
 * \return ����ѡ�񣬴���Ϊ-1
 *
 */
int menuList();

#endif // MAIN_H_INCLUDED
