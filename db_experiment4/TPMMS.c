#include "TPMMS.h"

int TPMMS(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk)
{
    unsigned int blk_num = blk_start;
    GroupList* head = (GroupList*) malloc(sizeof(GroupList));
    unsigned char* blk;
    int blk_count;
    int X, Y;
    GroupList* small;

    printf("-----------------------------------------------\n");
    printf("两阶段多路归并排序算法：数据块%d.blk到%d.blk\n", blk_start, blk_end);
    printf("-----------------------------------------------\n");

    // inline sorting
    inlineSort(buf, blk_num, blk_end, head);

    // merge
    blk = getNewBlockInBuffer(buf);
    blk_count = 0;
    mergeInit(buf, head);
    while(head != NULL)
    {
        small = findSmall(buf, head, &X, &Y);
        head = small->next;
        // small couple next or delete
        getNext(small, buf);
        writeToBlk(buf, X, Y, blk, &blk_count, write_blk);
        // only one blk compare
        if(head->next == head)
        {
            int addr;
            writeBlkLastToBlkBuf(buf, head->blk, head->count, blk, &blk_count, write_blk);
            getAddr(&addr, head->blk);
            while(addr <= head->blk_end)
            {
                head->blk = readBlockFromDisk(addr, buf);
                head->count = 0;
                writeBlkLastToBlkBuf(buf, head->blk, head->count, blk, &blk_count, write_blk);
            }
            free(head);
            head = NULL;
            break;
        }
    }
    if(blk_count != 0)
    {
        writeAddrBlockToDisk(buf, blk, write_blk);
    }

    printf("IO读写一共%ld次\n", buf->numIO);
    printf("\n");
    return 0;
}

void inlineSort(Buffer* buf, unsigned int blk_num, unsigned int blk_end, GroupList* head)
{
    GroupList* temp = head;
    unsigned char* blk;
    unsigned int write_blk_temp = TPMMSWRITETEMP;
    
    groupNodeInit(temp, write_blk_temp, head);
    while(blk_num != blk_end)
    {
        // read
        printf("读入数据块%d\n", blk_num);
        blk = readBlockFromDisk(blk_num, buf);
        getAddr(&blk_num, blk);

        // buf is full
        if(buf->numFreeBlk == 0)
        {
            inSort(buf, &write_blk_temp);
            freeAllBlockInBuffer(buf);
            // temp->blk_end = blk_num + TPMMSWRITETEMP - 1 - 1;   // 201开始所以-1，full所以-1
            temp->blk_end = write_blk_temp - 1;   // 201开始所以-1，full所以-1
            GroupList* node = (GroupList*)malloc(sizeof(GroupList));
            groupNodeInit(node, write_blk_temp, head);
            temp->next = node;
            temp = temp->next;
        }
    }
    // the last inline Group
    printf("读入数据块%d\n", blk_num);
    blk = readBlockFromDisk(blk_num, buf);
    inSort(buf, &write_blk_temp);
    freeAllBlockInBuffer(buf);
    temp->blk_end = write_blk_temp - 1;
}

int inSort(Buffer* buf, unsigned int* write_blk)
{
    int blk_count = buf->numAllBlk - buf->numFreeBlk;
    int couple_count = blk_count * 7;
    int min_data, temp, min_location;

    // sorting in buf
    for(int i = 0; i < couple_count - 1; i++)
    {
        min_data = getFour(getXILocationFromBuf(buf, i));
        min_location = i;
        for(int j = i+1; j < couple_count; j++)
        {
            temp = getFour(getXILocationFromBuf(buf, j));
            if(temp < min_data)
            {
                min_data = temp;
                min_location = j;
            }
        }
        if(i != min_location)
        {
            swapIJInBuf(buf, i, min_location);
        }
    }

    // write to disk
    for(int i = 0; i < buf->numAllBlk; i++)
    {
        if(ifUsingBlk(buf, i))
        {
            writeAddrBlockToDisk(buf, getBlkFromBuf(buf, i), write_blk);
        }
    }
}

void groupNodeInit(GroupList* node, unsigned int blk_num, GroupList* head)
{
    node->blk_start = blk_num;
    node->blk_end = blk_num;
    node->next = head;
    node->blk = NULL;
    node->count = 0;
}

void mergeInit(Buffer* buf, GroupList* head)
{
    GroupList* temp = head;
    printf("读入数据块%d\n", temp->blk_start);
    temp->blk = readBlockFromDisk(temp->blk_start, buf);
    while(temp->next != head)
    {
        temp = temp->next;
        printf("读入数据块%d\n", temp->blk_start);
        temp->blk = readBlockFromDisk(temp->blk_start, buf);
    }

}

GroupList* findSmall(Buffer* buf, GroupList* head, int* X, int* Y)
{
    GroupList* temp = head;
    GroupList* small;
    int count = 0;

    // findSmall
    small = temp;
    getXY(X, Y, small->blk, small->count);
    for(temp = temp->next; temp != head; temp = temp->next)
    {
        int x, y;
        getXY(&x, &y, temp->blk, temp->count);
        if(x < *X)
        {
            small = temp;
            *X = x;
            *Y = y;
        }
        if(count > 10)
        {
            break;
        }
        count++;
    }
    return small;
}

void getNext(GroupList* small, Buffer* buf)
{
    int addr;
    GroupList* temp;

    // 当前块没比较完
    if(small->count < 6)
    {
        small->count++;
        return;
    }
    
    // 获得下一个块
    getAddr(&addr, small->blk);
    if(addr <= small->blk_end)
    {
        freeBlockInBuffer(small->blk, buf);
        printf("读入数据块%d\n", addr);
        small->blk = readBlockFromDisk(addr, buf);
        small->count = 0;
        return;
    }

    // 该子集所有块比较完成，删除节点
    freeBlockInBuffer(small->blk, buf);
    temp = small;
    while(temp->next != small)
    {
        temp = temp->next;
    }
    temp->next = temp->next->next;
    free(small);
    return;
}

void writeBlkLastToBlkBuf(Buffer* buf, unsigned char* src_blk, int src_blk_count, unsigned char* dst_blk, int* dst_blk_count, int* write_blk)
{
    int X, Y;
    while(src_blk_count < 7)
    {
        getXY(&X, &Y, src_blk, src_blk_count);
        writeToBlk(buf, X, Y, dst_blk, dst_blk_count, write_blk);
        src_blk_count++;
    }
}