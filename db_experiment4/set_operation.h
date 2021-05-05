#ifndef SETOPERATION_H_INCLUDED
#define SETOPERATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"
#include "base.h"

/** \brief 基于排序结果的交操作
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
int intersection(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);

/** \brief 基于排序结果的并操作
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
int unionWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);

/** \brief 基于排序结果的差操作
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
int minusWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);
#endif // TPMMS_H_INCLUDED
