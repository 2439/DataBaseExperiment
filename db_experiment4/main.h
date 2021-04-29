#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "linear_search.h"

typedef struct table
{
    unsigned int blk_start;
    unsigned int blk_end;
} Table;

void initTableRS(Table* R, Table* S);
int menuList();

#endif // MAIN_H_INCLUDED
