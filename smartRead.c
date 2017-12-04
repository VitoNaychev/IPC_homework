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
    struct smart_block* mem_begin = mem;
    char tmp[GEN_BLOCK_SIZE];
    strcpy(tmp, mem->gen_buff);
    int prev_seed = verify(tmp);
    printf("%s\n", mem->buff);
    mem += 1;
    while( true )
    {
        strcpy(tmp, mem->gen_buff);
        int cur_seed = verify(tmp);
        if(cur_seed == prev_seed + 1)
        {
            prev_seed = cur_seed;
            printf("%s\n", mem->buff);
            
        }else{
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            sleep(1);
            mem -= 1;
            continue;              
        }
        if(mem + 1 >= mem_begin + 512){
            mem = mem_begin;
        }else{
            mem += 1;
        }
    }

    return 0;
}

