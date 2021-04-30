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

/** \brief 从blk_begin的位置开始获得4字节的数据，每个字节代表一位数
 *
 * \param blk_begin：读取开始位置
 * \return 返回对应数字，错误返回-1
 *
 */
int getFour(unsigned char* blk_begin);

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
#endif // BASE_H_INCLUDED
