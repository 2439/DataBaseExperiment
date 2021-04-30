#ifndef TPMMS_H_INCLUDED
#define TPMMS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "extmem.h"
#include "base.h"

int TPMMS(Buffer* buf, unsigned int blk_start, unsigned int blk_end, int* write_blk);
int inSort(Buffer* buf, unsigned int* write_blk);

#endif // TPMMS_H_INCLUDED
