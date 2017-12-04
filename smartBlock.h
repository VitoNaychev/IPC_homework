#ifndef SMART_BLOCK
#define SMART_BLOCK 

#include "gen.h"

struct smart_block{
    volatile unsigned int seed;
    volatile char buff[GEN_BLOCK_SIZE];
};

#endif
