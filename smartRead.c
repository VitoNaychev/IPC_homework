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

    struct smart_block* mem = mmap( NULL, 512 * sizeof(struct smart_block), PROT_READ, MAP_SHARED, memFd, 0 );
    if( mem == NULL )
    {
        perror("Can't mmap");
        return -1;
    }   
    int i = 0; 
    uint32_t prev_seed = verify((void*)((mem + i)->gen));

    while(prev_seed == -1){
        prev_seed = verify((void*)(mem + i)->gen);
    }
    ++ i;

    while(true){
        uint32_t cur_seed = verify((void*)(mem + i)->gen);
        uint64_t pos = mem->pos;

        if(cur_seed - 1 == prev_seed){
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            prev_seed = cur_seed;
        }else{
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            usleep(1);
            //if(pos + 512 < mem->pos){
            //    printf("U fucked up!\n");
            //    printf("norm pos %d cur pos %d\n", pos, mem->pos);
            //    return -1;
            //}
            continue;
        }
        ++ i;
        i %= 512;
    }

    return 0;
}

