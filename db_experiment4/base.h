#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void getAddr(unsigned int* addr, unsigned char* blk);

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

/** \brief buf清零并释放所有缓存区域
 *
 * \param buf：缓存指针
 * \return 无
 *
 */
void freeAllBlockInBuffer(Buffer* buf);

/** \brief 将缓存区buf中的数据看作数组，获得第i个数的位置
 *
 * \param buf：缓存区
 * \param i：需要获得的数据位置
 * \return 返回buf内第i个元祖的位置
 *
 */
unsigned char* getXILocationFromBuf(Buffer* buf, int i);

/** \brief 缓存区buf的第blk_count块（0-7）是否可用
 *
 * \param buf：缓存区
 * \param i：判断的块
 * \return 0为不在用，1为在用
 *
 */
int ifUsingBlk(Buffer* buf, int blk_count);

/** \brief 缓存区buf的第blk_count块（0-7）的起始位置
 *
 * \param buf：缓存区
 * \param i：判断的块
 * \return 数据块的起始位置
 *
 */
unsigned char* getBlkFromBuf(Buffer* buf, int blk_count);

/** \brief 交换buf中第i和第j个元祖的数据
 *
 * \param buf：缓存区
 * \param i：第i个元祖
 * \param j：第j个元祖
 * \return 无
 *
 */
void swapIJInBuf(Buffer* buf, int i, int j);

/** \brief 将缓存区buf对应块blk中写入下一块地址，然后写入磁盘write_blk
 *
 * \param buf：缓存区
 * \param blk：写入块
 * \param write_blk：写入磁盘
 * \return 正确返回0，错误返回-1
 *
 */
int writeAddrBlockToDisk(Buffer* buf, unsigned char* blk, unsigned int* write_blk);

/** \brief 将X，Y写入blk，如果blk_count>=7即blk已满，则存入磁盘write_blk，获取新的blk
 *
 * \param buf：缓存区
 * \param blk：写入块
 * \param X：写入数据
 * \param Y：写入数据
 * \param blk_count：写入快blk已有的元组数
 * \param write_blk：写入磁盘的位置，如果写入，自动加一
 * \return
 *
 */
void writeToBlk(Buffer* buf, int X, int Y, unsigned char* blk, int* blk_count, unsigned int* write_blk);

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
unsigned char* getNextCouple(Buffer* buf, unsigned char* blk, int* count, int blk_end, unsigned int* blk_num);

#endif // BASE_H_INCLUDED
