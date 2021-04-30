#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"

/** \brief ���0-9999������
 *
 * \return �ɹ��򷵻����֣����򷵻�-1
 *
 */
int getNumber();

/** \brief ��blk�л�õ�i+1��Ԫ���X��Y
 *
 * \param X��Ԫ��ĵ�һ��Ԫ�أ���С4�ֽ�
 * \param Y��Ԫ��ĵڶ���Ԫ�أ���С4�ֽ�
 * \param blk�����ݿ�
 * \param i����i+1��Ԫ��
 * \return
 *
 */
void getXY(int* X, int* Y, unsigned char* blk, int i);

/** \brief ���blk��ĵ�ַ
 *
 * \param addr�����صĵ�ַ
 * \param blk����ǰ��blk��
 * \return
 *
 */
void getAddr(int* addr, unsigned char* blk);

/** \brief ��blk_begin��λ�ÿ�ʼ���4�ֽڵ����ݣ�ÿ���ֽڴ���һλ��
 *
 * \param blk_begin����ȡ��ʼλ��
 * \return ���ض�Ӧ���֣����󷵻�-1
 *
 */
int getFour(unsigned char* blk_begin);

/** \brief ����X,Y��Ԫ��д��blk�ĵ�i+1����
 *
 * \param X,Y��Ԫ�������Ԫ��
 * \param blk�����ݿ�
 * \param i����i+1��Ԫ��
 * \return �ɹ�����0�����򷵻�-1
 *
 */
int writeXY(int X, int Y, unsigned char* blk, int i);

/** \brief д��blk��ĵ�ַaddr
 *
 * \param addr��д��ĵ�ַ
 * \param blk����ǰ��blk��
 * \return
 *
 */
int writeAddr(int addr, unsigned char* blk);

/** \brief ��blk_begin��λ�ÿ�ʼд�����ݣ�ÿλ��һ���ֽ�
 *
 * \param num��д����������
 * \param blk_begin��д�뿪ʼλ��
 * \return ��ȷ����0�����󷵻�-1
 *
 */
int writeFour(int num, unsigned char* blk_begin);

/** \brief ��numд��blk_begin+location��λ�ã����num��Ϊ0����location++
 *
 * \param num��д���һλ����
 * \param blk_begin����ʼд���blk_beginλ��
 * \param location��num��Ӧƫ����
 * \return ��
 *
 */
void writeOne(int num, unsigned char* blk_begin, int* location);

/** \brief buf���㲢�ͷ����л�������
 *
 * \param buf������ָ��
 * \return ��
 *
 */
void freeAllBlockInBuffer(Buffer* buf);
#endif // BASE_H_INCLUDED
