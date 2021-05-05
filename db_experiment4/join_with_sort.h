#ifndef JOINWITHSORT_H_INCLUDED
#define JOINWITHSORT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extmem.h"
#include "base.h"

/** \brief ���������������Ӳ���
 *
 * \param buf��������ָ��
 * \param R_start: ��R�鿪ʼ��ַ����RΪ������
 * \param R_end: ��R��������ַ
 * \param S_start: ��S�鿪ʼ��ַ����SΪ����
 * \param S_end: ��S��������ַ
 * \param write_blk: ���д�뿪ʼ��ַ
 * \return ����ɹ�����0��ʧ�ܷ���-1
 *
 */
int joinWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);
#endif // LINEARSEARCH_H_INCLUDED
