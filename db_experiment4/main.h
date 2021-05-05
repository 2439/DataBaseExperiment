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


/** \brief 初始化R和S表的开始和结束数据块
 *
 * \param R：R表
 * \param S：S表
 *
 */
void initTableRS(Table* R, Table* S);

/** \brief 打印菜单并选择算法
 *
 * \return 选择1-7，退出0，其余重新输入
 *
 */
int menuList();

#endif // MAIN_H_INCLUDED
