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

/** \brief ��blk_begin��λ�ÿ�ʼ���4�ֽڵ����ݣ�ÿ���ֽڴ���һλ��
 *
 * \param blk_begin����ȡ��ʼλ��
 * \return ���ض�Ӧ���֣����󷵻�-1
 *
 */
int getFour(unsigned char* blk_begin);

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
#endif // BASE_H_INCLUDED
