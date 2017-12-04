#ifndef SMART_BLOCK
#define SMART_BLOCK 

#include "gen.h"

struct smart_block{
    char gen_buff[GEN_BLOCK_SIZE];
    char buff[GEN_BLOCK_SIZE];
};

#endif
