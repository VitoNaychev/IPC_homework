#ifndef SMART_BLOCK
#define SMART_BLOCK 

#include "gen.h"
#include <stdint.h>

struct smart_block{
    volatile uint64_t gen[512];
};

#endif
