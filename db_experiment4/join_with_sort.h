#ifndef JOINWITHSORT_H_INCLUDED
#define JOINWITHSORT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"
#include "base.h"

/** \brief 基于排序结果的连接操作
 *
 * \param buf：缓存区指针
 * \param R_start: 表R块开始地址，表R为被连接
 * \param R_end: 表R块块结束地址
 * \param S_start: 表S块开始地址，表S为连接
 * \param S_end: 表S块块结束地址
 * \param write_blk: 结果写入开始地址
 * \return 保存成功返回0，失败返回-1
 *
 */
int joinWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, int* write_blk);

/** \brief 获得blk对应的下一个元组，如果块有更新，且blk_num不为NULL，则blk_num为更新后blk位置
 *
 * \param buf：缓存区指针
 * \param blk：块指针
 * \param count：元组在块内位置
 * \param blk_end：子集最后一块位置
 * \param blk_num：当前块的位置
 * \return 返回更新后块位置
 *
 */
unsigned char* getNextInJoin(Buffer* buf, unsigned char* blk, int* count, int blk_end, unsigned int* blk_num);
#endif // LINEARSEARCH_H_INCLUDED
