#include "set_operation.h"

int intersection(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk)
{
    unsigned char* blk_R;
    unsigned char* blk_S;
    unsigned char* blk;
    int R_count, S_count, blk_count, R_begin_i;
    int count;
    int RX, RY, SX, SY;
    unsigned int R_begin, R_num;

    printf("------------------------------\n");
    printf("基于排序的交操作：\n");
    printf("------------------------------\n");

    // init
    count = 0;
    blk_R = readBlockFromDisk(R_start, buf);
    R_count = 0;
    blk_S = readBlockFromDisk(S_start, buf);
    S_count = 0;
    blk = getNewBlockInBuffer(buf);
    blk_count = 0;
    R_num = R_start;
    R_begin = R_start;
    R_begin_i = R_count;

    // intersection
    while(blk_R != NULL && blk_S != NULL)
    {
        getXY(&RX, &RY, blk_R, R_count);
        getXY(&SX, &SY, blk_S, S_count);

        // equal
        if(RX == SX && RY == SY)
        {
            // save
            printf("(X=%d,Y=%d)\n",RX, RY);
            writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
            writeToBlk(buf, RX, RY, blk, &blk_count, write_blk);
            count++;
            // next S
            blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
            if(blk_S == NULL)
                break;

            int tempX, tempY;
            getXY(&tempX, &tempY, blk_S, S_count);
            // R turn to R_begin
            if(tempX == SX)
            {
                if(R_num == R_begin)
                {
                    R_count = R_begin_i;
                }
                else
                {
                    freeBlockInBuffer(blk_R, buf);
                    blk_R = readBlockFromDisk(R_begin, buf);
                    R_count = R_begin_i;
                    R_num = R_begin;
                }
            }
            // R_begin turn to R, R to next
            else
            {
                blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
                R_begin = R_num;
                R_begin_i = R_count;
            }
        }

        // only X equal,R go next
        else if(RX == SX)
        {
            blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
        }
        // RX < SX
        else if(RX < SX)
        {
            blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
            R_begin = R_num;
            R_begin_i = R_count;
        }
        // RX > SX
        else
        {
            // next S
            blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
            if(blk_S == NULL)
                break;

            int tempX, tempY;
            getXY(&tempX, &tempY, blk_S, S_count);
            // R turn to R_begin
            if(tempX == SX)
            {
                if(R_num == R_begin)
                {
                    R_count = R_begin_i;
                }
                else
                {
                    freeBlockInBuffer(blk_R, buf);
                    blk_R = readBlockFromDisk(R_begin, buf);
                    R_count = R_begin_i;
                    R_num = R_begin;
                }
            }
            // R_begin turn to R, R to next
            else
            {
                R_begin = R_num;
                R_begin_i = R_count;
                // blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
            }
        }
    }

    // write finial result
    if(writeAddrBlockToDisk(buf, blk, write_blk) == -1)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }

    printf("R和S的交集有%d个元组\n", count);
    printf("\n");
    return 0;
}

int unionWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk)
{
    unsigned char* blk_R;
    unsigned char* blk_S;
    unsigned char* blk;
    int R_count, S_count, blk_count, R_begin_i;
    int RX, RY, SX, SY;
    unsigned int R_begin, R_num;
    int count;

    printf("------------------------------\n");
    printf("基于排序的并操作：\n");
    printf("------------------------------\n");

    // init
    count = 0;
    blk_R = readBlockFromDisk(R_start, buf);
    R_count = 0;
    blk_S = readBlockFromDisk(S_start, buf);
    S_count = 0;
    blk = getNewBlockInBuffer(buf);
    blk_count = 0;
    R_num = R_start;
    R_begin = R_start;
    R_begin_i = R_count;
    getXY(&RX, &RY, blk_R, R_count);
    getXY(&SX, &SY, blk_S, S_count);

    while(blk_R != NULL && blk_S != NULL)
    {
        // RX < SX, write RX
        if(RX < SX)
        {
            writeToBlk(buf, RX, RY, blk, &blk_count, write_blk);
            count++;
            blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
            if(blk_R == NULL)
            {
                break;
            }
            R_begin = R_num;
            R_begin_i = R_count;
            getXY(&RX, &RY, blk_R, R_count);
        }
        // RX > SX
        else if(RX > SX)
        {
            writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
            count++;
            blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
            if(blk_S == NULL)
            {
                break;
            }
            getXY(&SX, &SY, blk_S, S_count);
        }
        // RX == SX
        else
        {
            int equal_num = RX;

            // write all RX equal
            while(RX == equal_num)
            {
                writeToBlk(buf, RX, RY, blk, &blk_count, write_blk);
                count++;
                blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
                if(blk_R == NULL)
                    break;
                getXY(&RX, &RY, blk_R, R_count);
            }

            // find unequal SX and write
            while(SX == equal_num)
            {
                // reset R
                if(blk_R != NULL)
                {
                    freeBlockInBuffer(blk_R, buf);
                }
                blk_R = readBlockFromDisk(R_begin, buf);
                R_count = R_begin_i;
                R_num = R_begin;
                getXY(&RX, &RY, blk_R, R_count);

                while(RX == equal_num)
                {
                    if(RY != SY)
                    {
                        blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
                        if(blk_R == NULL)
                        {
                            RX = 0;
                            break;
                        }
                        getXY(&RX, &RY, blk_R, R_count);
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                // if S couple is in R
                if(RX == equal_num)
                {
                    blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
                    if(blk_S == NULL)
                    {
                        break;
                    }
                    getXY(&SX, &SY, blk_S, S_count);
                    continue;
                }
                // not in
                else
                {
                    writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
                    count++;
                    blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
                    if(blk_S == NULL)
                    {
                        break;
                    }
                    getXY(&SX, &SY, blk_S, S_count);
                }
            }
            // update begin
            while(RX == equal_num)
            {
                blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
                if(blk_R == NULL)
                {
                    RX = 0;
                    break;
                }
                getXY(&RX, &RY, blk_R, R_count);
            }
            R_begin = R_num;
            R_begin_i = R_count;
        }
    }
    // the last
    if(blk_R == NULL)
    {
        while(blk_S != NULL)
        {
            getXY(&SX, &SY, blk_S, S_count);
            writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
            count++;
            blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
        }
    }
    else
    {
        while(blk_R != NULL)
        {
            getXY(&RX, &RY, blk_R, R_count);
            writeToBlk(buf, RX, RY, blk, &blk_count, write_blk);
            count++;
            blk_R = getNextCouple(buf, blk_R, &R_count, R_end, NULL);
        }
    }

    // write finial result
    if(writeAddrBlockToDisk(buf, blk, write_blk) == -1)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    printf("R和S的并集有%d个元组\n", count);
    printf("\n");
    return 0;
}

int minusWithSort(Buffer* buf, unsigned int R_start, unsigned int R_end, unsigned int S_start, unsigned int S_end, unsigned int* write_blk)
{
    unsigned char* blk_R;
    unsigned char* blk_S;
    unsigned char* blk;
    int R_count, S_count, blk_count, R_begin_i;
    int RX, RY, SX, SY;
    unsigned int R_begin, R_num;
    int count;

    printf("------------------------------\n");
    printf("基于排序的差操作：\n");
    printf("------------------------------\n");

    // init
    count = 0;
    blk_R = readBlockFromDisk(R_start, buf);
    R_count = 0;
    blk_S = readBlockFromDisk(S_start, buf);
    S_count = 0;
    blk = getNewBlockInBuffer(buf);
    blk_count = 0;
    R_num = R_start;
    R_begin = R_start;
    R_begin_i = R_count;
    getXY(&RX, &RY, blk_R, R_count);
    getXY(&SX, &SY, blk_S, S_count);

    while(blk_R != NULL && blk_S != NULL)
    {
        // RX < SX
        if(RX < SX)
        {
            blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
            if(blk_R == NULL)
            {
                break;
            }
            R_begin = R_num;
            R_begin_i = R_count;
            getXY(&RX, &RY, blk_R, R_count);
        }
        // RX > SX
        else if(RX > SX)
        {
            writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
            count++;
            blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
            if(blk_S == NULL)
            {
                break;
            }
            getXY(&SX, &SY, blk_S, S_count);
        }
        // RX == SX
        else
        {
            int equal_num = RX;
            // find unequal SX and write
            while(SX == equal_num)
            {
                // reset R
                if(blk_R != NULL)
                {
                    freeBlockInBuffer(blk_R, buf);
                }
                blk_R = readBlockFromDisk(R_begin, buf);
                R_count = R_begin_i;
                R_num = R_begin;
                getXY(&RX, &RY, blk_R, R_count);

                while(RX == equal_num)
                {
                    if(RY != SY)
                    {
                        blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
                        if(blk_R == NULL)
                        {
                            RX = 0;
                            break;
                        }
                        getXY(&RX, &RY, blk_R, R_count);
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }

                if(RX == equal_num)
                {
                    blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
                    if(blk_S == NULL)
                    {
                        break;
                    }
                    getXY(&SX, &SY, blk_S, S_count);
                    continue;
                }
                else
                {
                    writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
                    count++;
                    blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
                    if(blk_S == NULL)
                    {
                        break;
                    }
                    getXY(&SX, &SY, blk_S, S_count);
                }
            }
            // update begin
            while(RX == equal_num)
            {
                blk_R = getNextCouple(buf, blk_R, &R_count, R_end, &R_num);
                if(blk_R == NULL)
                {
                    RX = 0;
                    break;
                }
                getXY(&RX, &RY, blk_R, R_count);
            }
            R_begin = R_num;
            R_begin_i = R_count;
        }
    }
    // the last
    if(blk_R == NULL)
    {
        while(blk_S != NULL)
        {
            getXY(&SX, &SY, blk_S, S_count);
            writeToBlk(buf, SX, SY, blk, &blk_count, write_blk);
            count++;
            blk_S = getNextCouple(buf, blk_S, &S_count, S_end, NULL);
        }
    }
    // write finial result
    if(writeAddrBlockToDisk(buf, blk, write_blk) == -1)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    
    printf("S-R集有%d个元组\n", count);
    printf("\n");
    return 0;
}
