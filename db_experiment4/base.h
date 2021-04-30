#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"

/** \brief 获得0-9999的数字
 *
 * \return 成功则返回数字，否则返回-1
 *
 */
int getNumber();

/** \brief 从blk中获得第i+1个元组的X，Y
 *
 * \param X：元组的第一个元素，大小4字节
 * \param Y：元组的第二个元素，大小4字节
 * \param blk：数据块
 * \param i：第i+1个元组
 * \return
 *
 */
void getXY(int* X, int* Y, unsigned char* blk, int i);

/** \brief 获得blk块的地址
 *
 * \param addr：返回的地址
 * \param blk：当前的blk块
 * \return
 *
 */
void getAddr(int* addr, unsigned char* blk);

/** \brief 从blk_begin的位置开始获得4字节的数据，每个字节代表一位数
 *
 * \param blk_begin：读取开始位置
 * \return 返回对应数字，错误返回-1
 *
 */
int getFour(unsigned char* blk_begin);

/** \brief 将（X,Y）元组写入blk的第i+1个中
 *
 * \param X,Y：元组的两个元素
 * \param blk：数据块
 * \param i：第i+1个元组
 * \return 成功返回0，否则返回-1
 *
 */
int writeXY(int X, int Y, unsigned char* blk, int i);

/** \brief 写入blk块的地址addr
 *
 * \param addr：写入的地址
 * \param blk：当前的blk块
 * \return
 *
 */
int writeAddr(int addr, unsigned char* blk);

/** \brief 从blk_begin的位置开始写入数据，每位数一个字节
 *
 * \param num：写入代表的数据
 * \param blk_begin：写入开始位置
 * \return 正确返回0，错误返回-1
 *
 */
int writeFour(int num, unsigned char* blk_begin);

/** \brief 将num写入blk_begin+location的位置，如果num不为0，则location++
 *
 * \param num：写入的一位数字
 * \param blk_begin：开始写入的blk_begin位置
 * \param location：num对应偏移量
 * \return 无
 *
 */
void writeOne(int num, unsigned char* blk_begin, int* location);

/** \brief buf清零并释放所有缓存区域
 *
 * \param buf：缓存指针
 * \return 无
 *
 */
void freeAllBlockInBuffer(Buffer* buf);
#endif // BASE_H_INCLUDED
