#ifndef LINEARSEARCH_H_INCLUDED
#define LINEARSEARCH_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "base.h"

/** \brief �������������Ĺ�ϵѡ���㷨
 *
 * \param Buffer* buf��������ָ��
 * \param blk_start: �鿪ʼ��ַ
 * \param blk_end: �������ַ
 * \param write_blk: ���д�뿪ʼ��ַ
 * \return ����ɹ�����0��ʧ�ܷ���-1
 *
 */
int linearSearch(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk);

/** \brief ��blk_num�Ŀ��в��ҵ�һ��Ԫ�ص���equal_num��Ԫ�飬������һ���ַ
 *
 * \param   equal_num�����ڵ�ֵ
 * \param   blk_num�����ҵĿ�
 * \param   buf:������ָ��
 * \param   re_blk:�����ָ��
 * \param   re_blk_count:������Ѵ���Ŀ
 * \param   write_blk:д��Ĵ��̵�ַ
 * \return ��һ���ַ�����󷵻�-1
 *
 */
int findInBlk(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* blk_count, int* write_blk, int* count_num);

#endif // LINEARSEARCH_H_INCLUDED
