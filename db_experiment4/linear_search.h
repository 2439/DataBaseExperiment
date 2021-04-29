#ifndef LINEARSEARCH_H_INCLUDED
#define LINEARSEARCH_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"

/** \brief 基于线性搜索的关系选择算法
 *
 * \param Buffer* buf：缓存区指针
 * \return 保存成功返回0，失败返回-1
 *
 */

int linearSearch(Buffer* buf, int blk_start, int blk_end);
int findInBlk(int equal_num, int blk_num, Buffer* buf);
int getNumber();

#endif // LINEARSEARCH_H_INCLUDED
