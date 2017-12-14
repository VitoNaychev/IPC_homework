#ifndef SMART_BLOCK
#define SMART_BLOCK 

#include "gen.h"
#include <stdint.h>

struct smart_block{
    uint64_t pos;
    volatile uint8_t gen[512][4097];
};

#endif
