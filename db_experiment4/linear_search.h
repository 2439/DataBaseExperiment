#ifndef LINEARSEARCH_H_INCLUDED
#define LINEARSEARCH_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"

/** \brief �������������Ĺ�ϵѡ���㷨
 *
 * \param Buffer* buf��������ָ��
 * \return ����ɹ�����0��ʧ�ܷ���-1
 *
 */

int linearSearch(Buffer* buf, int blk_start, int blk_end);
int findInBlk(int equal_num, int blk_num, Buffer* buf);
int getNumber();

#endif // LINEARSEARCH_H_INCLUDED
