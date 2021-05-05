#ifndef TPMMS_H_INCLUDED
#define TPMMS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"
#include "base.h"

#define TPMMSWRITETEMP 1001

// �Ӽ�����blk_start��ʼ��blk_end�Ŀ�Ϊһ���Ӽ�
// blkΪ��ǰ���бȽϵĿ飬countΪ�Ƚϵ�Ԫ��(0-6)
typedef struct group_list
{
    unsigned int blk_start;
    unsigned int blk_end;
    unsigned char* blk;
    int count;
    struct group_list* next;
} GroupList;

/** \brief ���׶ζ�·�鲢�����㷨
 *
 * \param Buffer* buf��������ָ��
 * \param blk_start: �鿪ʼ��ַ
 * \param blk_end: �������ַ
 * \param write_blk: ���д�뿪ʼ��ַ
 * \return ����ɹ�����0��ʧ�ܷ���-1
 *
 */
int TPMMS(Buffer* buf, unsigned int blk_start, unsigned int blk_end, unsigned int* write_blk);

/** \brief ������buf���ţ���blk_num��blk_end�Ŀ飬���Ӽ������ţ����Ӽ����д��head��
 *
 * \param Buffer* buf��������ָ��
 * \param blk_num����ǰ��
 * \param blk_end�����һ��
 * \param head���Ӽ��б��ָ��
 * \return
 *
 */
void inlineSort(Buffer* buf, unsigned int blk_num, unsigned int blk_end, GroupList* head);

/** \brief ������buf���ţ���д��write_blkΪ��ʼ�Ŀ���
 *
 * \param buf��������ָ��
 * \param write_blk: ���д�뿪ʼ��ַ
 * \return ����ɹ�����0��ʧ�ܷ���-1
 *
 */
int inSort(Buffer* buf, unsigned int* write_blk);

/** \brief ��ʼ���ڵ�����
 *
 * \param node������ʼ���Ľڵ�
 * \param blk_num����ʼ���Ŀ�ʼ�ͽ���������
 * \param head���ڵ����һ���ڵ�
 * \return
 *
 */
void groupNodeInit(GroupList* node, unsigned int blk_num, GroupList* head);

/** \brief �鲢��ʼ��
 *
 * \param buf��������
 * \param head���Ӽ���Ϣ
 * \return
 *
 */
void mergeInit(Buffer* buf, GroupList* head);

/** \brief ���һ�����buf��head��Ӧ�Ӽ�����Сֵ�����ض�ӦԪ��(X,Y)�����¶�Ӧhead����Ϣ
 *
 * \param buf��������
 * \param head���Ӽ���Ϣ
 * \param X��Ԫ���һ����Ϣ
 * \param Y��Ԫ��ڶ�����Ϣ
 * \return ��Сֵ�����Ӽ�
 *
 */
GroupList* findSmall(Buffer* buf, GroupList* head, int* X, int* Y);

/** \brief ����small����һ����Ƚϵ�Ԫ�飬���û���򷵻�-1
 *
 * \param small����Сֵ��Ӧ�Ӽ���Ϣ
 * \param buf��������
 * \return
 *
 */
void getNext(GroupList* small, Buffer* buf);

/** \brief ��src����ʣ��Ԫ��д�뻺������dst�ڣ����dst���㣬��д��write_blk�ڣ�
 * \param buf��������
 * \param src_blk��д��Ŀ�
 * \param src_blk_count��д��Ŀ����ʼд��Ԫ���ַ
 * \param dst_blk����д��Ŀ�
 * \param dst_blk_count����д��Ŀ����ʼд��Ԫ���ַ
 * \param write_blk��д�����λ��
 * \return
 *
 */
void writeBlkLastToBlkBuf(Buffer* buf, unsigned char* src_blk, int src_blk_count, unsigned char* dst_blk, int* dst_blk_count, unsigned int* write_blk);

#endif // TPMMS_H_INCLUDED
