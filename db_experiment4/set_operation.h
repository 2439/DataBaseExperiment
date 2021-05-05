#ifndef SETOPERATION_H_INCLUDED
#define SETOPERATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"
#include "base.h"

/** \brief �����������Ľ�����
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
int intersection(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);

/** \brief �����������Ĳ�����
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
int unionWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);

/** \brief �����������Ĳ����
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
int minusWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk);
#endif // TPMMS_H_INCLUDED
