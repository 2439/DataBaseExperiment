#ifndef INDEXSEARCH_H_INCLUDED
#define INDEXSEARCH_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"
#include "base.h"

/** \brief ����blk_start��blk_end֮���Ѿ�����õ�S�����������ļ���write_blk
 *
 * \param buf��������ָ��
 * \param blk_start: �鿪ʼ��ַ
 * \param blk_end: �������ַ
 * \param write_blk: ���д�뿪ʼ��ַ
 * \return
 *
 */
void makeIndex(Buffer* buf, unsigned int blk_start, unsigned int blk_end, unsigned int* write_blk);

/** \brief ����blk_start��blk_end֮��������ļ�������S.C=�����д��write_blk��
 *
 * \param buf��������ָ��
 * \param blk_start: �鿪ʼ��ַ
 * \param blk_end: �������ַ
 * \param write_blk: ���д�뿪ʼ��ַ
 * \return ����ɹ�����0��ʧ�ܷ���-1
 *
 */
int indexSearch(Buffer* buf, unsigned int blk_start, unsigned int blk_end, unsigned int* write_blk);

/** \brief ����ֵ���ڵ���equal_num������Ԫ�飬����ҵ�����1�����򷵻�0
 *
 * \param buf��������ָ��
 * \param equal_num���Ƚϵ�ֵ
 * \param blk_num�����ҵ�������
 * \return �ҵ�����1��û�ҵ�����0�����󷵻�-1
 *
 */
int findIndexInBlk(int equal_num, unsigned int* blk_num, Buffer* buf);

/** \brief ��blk_num�Ŀ���˳����Ҳ������һ��Ԫ��Ϊequal_num���������ǵ�Ԫ�鷵��0�����򷵻���һ���ַ
 *
 * \param   equal_num�����ڵ�ֵ
 * \param   blk_num�����ҵĿ�
 * \param   buf:������ָ��
 * \param   re_blk:�����ָ��
 * \param   re_blk_count:������Ѵ���Ŀ
 * \param   write_blk:д��Ĵ��̵�ַ
 * \param   count_num�����ҵ�Ԫ��ͳ��
 * \return ������һ���ַ���� ���󷵻�-1
 *
 */
int findInBlkWithSort(int equal_num, unsigned int blk_num, Buffer* buf, unsigned char* re_blk, int* re_blk_count, unsigned int* write_blk, int* count);
#endif // LINEARSEARCH_H_INCLUDED
