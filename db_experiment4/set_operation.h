#ifndef SETOPERATION_H_INCLUDED
#define SETOPERATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"
#include "base.h"

// /** \brief 两阶段多路归并排序算法
//  *
//  * \param Buffer* buf：缓存区指针
//  * \param blk_start: 块开始地址
//  * \param blk_end: 块结束地址
//  * \param write_blk: 结果写入开始地址
//  * \return 保存成功返回0，失败返回-1
//  *
//  */
// int TPMMS(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk);

// /** \brief 缓存区buf内排，从blk_num到blk_end的块，分子集并内排，将子集结果写入head中
//  *
//  * \param Buffer* buf：缓存区指针
//  * \param blk_num：当前块
//  * \param blk_end：最后一块
//  * \param head：子集列表的指针
//  * \return
//  *
//  */
// void inlineSort(Buffer* buf, unsigned int blk_num, unsigned int blk_end, GroupList* head);

// /** \brief 缓存区buf内排，并写入write_blk为起始的块内
//  *
//  * \param buf：缓存区指针
//  * \param write_blk: 结果写入开始地址
//  * \return 保存成功返回0，失败返回-1
//  *
//  */
// int inSort(Buffer* buf, unsigned int* write_blk);

// /** \brief 初始化节点数据
//  *
//  * \param node：被初始化的节点
//  * \param blk_num：初始化的开始和结束块数据
//  * \param head：节点的下一个节点
//  * \return 
//  *
//  */
// void groupNodeInit(GroupList* node, unsigned int blk_num, GroupList* head);

// /** \brief 归并初始化
//  *
//  * \param buf：缓存区
//  * \param head：子集信息
//  * \return 
//  *
//  */
// void mergeInit(Buffer* buf, GroupList* head);

// /** \brief 查找缓存区buf内head对应子集的最小值，返回对应元祖(X,Y)，更新对应head的信息
//  *
//  * \param buf：缓存区
//  * \param head：子集信息
//  * \param X：元祖第一个信息
//  * \param Y：元组第二个信息
//  * \return 最小值所在子集
//  *
//  */
// GroupList* findSmall(Buffer* buf, GroupList* head, int* X, int* Y);

// /** \brief 更新small到下一个需比较的元组，如果没有则返回-1
//  *
//  * \param small：最小值对应子集信息
//  * \param buf：缓存区
//  * \return
//  *
//  */
// void getNext(GroupList* small, Buffer* buf);

// /** \brief 将src块内剩余元组写入缓存区的dst内，如果dst不足，则写入write_blk内，
//  * \param buf：缓存区
//  * \param src_blk：写入的块
//  * \param src_blk_count：写入的块的起始写入元组地址
//  * \param dst_blk：被写入的块
//  * \param dst_blk_count：被写入的块的起始写入元组地址
//  * \param write_blk：写入磁盘位置
//  * \return 
//  *
//  */
// void writeBlkLastToBlkBuf(Buffer* buf, unsigned char* src_blk, int src_blk_count, unsigned char* dst_blk, int* dst_blk_count, int* write_blk);

#endif // TPMMS_H_INCLUDED
