#ifndef LINEARSEARCH_H_INCLUDED
#define LINEARSEARCH_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "base.h"

/** \brief 基于线性搜索的关系选择算法
 *
 * \param Buffer* buf：缓存区指针
 * \param blk_start: 块开始地址
 * \param blk_end: 块结束地址
 * \param write_blk: 结果写入开始地址
 * \return 保存成功返回0，失败返回-1
 *
 */
int linearSearch(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk);

/** \brief 在blk_num的块中查找第一个元素等于equal_num的元组，返回下一块地址
 *
 * \param   equal_num：等于的值
 * \param   blk_num：查找的块
 * \param   buf:缓存区指针
 * \param   re_blk:结果块指针
 * \param   re_blk_count:结果块已存数目
 * \param   write_blk:写入的磁盘地址
 * \return 下一块地址，错误返回-1
 *
 */
int findInBlk(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* blk_count, int* write_blk, int* count_num);

#endif // LINEARSEARCH_H_INCLUDED
