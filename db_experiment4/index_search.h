#ifndef INDEXSEARCH_H_INCLUDED
#define INDEXSEARCH_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"
#include "base.h"

/** \brief 基于blk_start到blk_end之间已经排序好的S表，建立索引文件到write_blk
 *
 * \param buf：缓存区指针
 * \param blk_start: 块开始地址
 * \param blk_end: 块结束地址
 * \param write_blk: 结果写入开始地址
 * \return
 *
 */
void makeIndex(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk);

/** \brief 基于blk_start到blk_end之间的索引文件，查找S.C=，结果写入write_blk块
 *
 * \param buf：缓存区指针
 * \param blk_start: 块开始地址
 * \param blk_end: 块结束地址
 * \param write_blk: 结果写入开始地址
 * \return 保存成功返回0，失败返回-1
 *
 */
int indexSearch(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk);

/** \brief 查找值大于等于equal_num的索引元组，如果找到返回1，否则返回0
 *
 * \param buf：缓存区指针
 * \param equal_num：比较的值
 * \param blk_num：查找的索引块
 * \return 找到返回1，没找到返回0，错误返回-1
 *
 */
int findIndexInBlk(int equal_num, unsigned int* blk_num, Buffer* buf);

/** \brief 在blk_num的块中顺序查找并保存第一个元素为equal_num，遇到不是的元组返回0，否则返回下一块地址
 *
 * \param   equal_num：等于的值
 * \param   blk_num：查找的块
 * \param   buf:缓存区指针
 * \param   re_blk:结果块指针
 * \param   re_blk_count:结果块已存数目
 * \param   write_blk:写入的磁盘地址
 * \param   count_num：已找到元组统计
 * \return 返回下一块地址，， 错误返回-1
 *
 */
int findInBlkWithSort(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* re_blk_count, unsigned int* write_blk, int* count);
#endif // LINEARSEARCH_H_INCLUDED
