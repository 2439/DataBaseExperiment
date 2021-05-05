#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void getAddr(unsigned int* addr, unsigned char* blk);

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

/** \brief buf���㲢�ͷ����л�������
 *
 * \param buf������ָ��
 * \return ��
 *
 */
void freeAllBlockInBuffer(Buffer* buf);

/** \brief ��������buf�е����ݿ������飬��õ�i������λ��
 *
 * \param buf��������
 * \param i����Ҫ��õ�����λ��
 * \return ����buf�ڵ�i��Ԫ���λ��
 *
 */
unsigned char* getXILocationFromBuf(Buffer* buf, int i);

/** \brief ������buf�ĵ�blk_count�飨0-7���Ƿ����
 *
 * \param buf��������
 * \param i���жϵĿ�
 * \return 0Ϊ�����ã�1Ϊ����
 *
 */
int ifUsingBlk(Buffer* buf, int blk_count);

/** \brief ������buf�ĵ�blk_count�飨0-7������ʼλ��
 *
 * \param buf��������
 * \param i���жϵĿ�
 * \return ���ݿ����ʼλ��
 *
 */
unsigned char* getBlkFromBuf(Buffer* buf, int blk_count);

/** \brief ����buf�е�i�͵�j��Ԫ�������
 *
 * \param buf��������
 * \param i����i��Ԫ��
 * \param j����j��Ԫ��
 * \return ��
 *
 */
void swapIJInBuf(Buffer* buf, int i, int j);

/** \brief ��������buf��Ӧ��blk��д����һ���ַ��Ȼ��д�����write_blk
 *
 * \param buf��������
 * \param blk��д���
 * \param write_blk��д�����
 * \return ��ȷ����0�����󷵻�-1
 *
 */
int writeAddrBlockToDisk(Buffer* buf, unsigned char* blk, unsigned int* write_blk);

/** \brief ��X��Yд��blk�����blk_count>=7��blk��������������write_blk����ȡ�µ�blk
 *
 * \param buf��������
 * \param blk��д���
 * \param X��д������
 * \param Y��д������
 * \param blk_count��д���blk���е�Ԫ����
 * \param write_blk��д����̵�λ�ã����д�룬�Զ���һ
 * \return
 *
 */
void writeToBlk(Buffer* buf, int X, int Y, unsigned char* blk, int* blk_count, unsigned int* write_blk);

/** \brief ���blk��Ӧ����һ��Ԫ�飬������и��£���blk_num��ΪNULL����blk_numΪ���º�blkλ��
 *
 * \param buf��������ָ��
 * \param blk����ָ��
 * \param count��Ԫ���ڿ���λ��
 * \param blk_end���Ӽ����һ��λ��
 * \param blk_num����ǰ���λ��
 * \return ���ظ��º��λ��
 *
 */
unsigned char* getNextCouple(Buffer* buf, unsigned char* blk, int* count, int blk_end, unsigned int* blk_num);

#endif // BASE_H_INCLUDED
