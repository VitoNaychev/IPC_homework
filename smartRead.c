#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "gen.h"
#include "smartBlock.h"

int main()
{
    int memFd = shm_open( "simple_memory", O_RDONLY, 0 );
    if( memFd == -1 )
    {
        perror("Can't open file");
        return 1;
    }

    struct smart_block* mem = mmap( NULL, sizeof(struct smart_block), PROT_READ, MAP_SHARED, memFd, 0 );
    if( mem == NULL )
    {
        perror("Can't mmap");
        return -1;
    }   
    struct smart_block* mem_begin = mem;
    int i = 0; 
    uint32_t prev_seed = verify((void*)(mem->gen + i));
    ++ i;
    while( true )
    {
        uint32_t cur_seed = verify((void*)(mem->gen + i));

        if(cur_seed == prev_seed + 1)
        {
            prev_seed = cur_seed;
            printf("Penis\n"); 
        }else{
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            usleep(1);
            -- i;
            i %= 512;
            continue;              
        }
        ++ i;
        i %= 512;
    }

    return 0;
}

