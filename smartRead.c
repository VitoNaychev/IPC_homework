#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "gen.h"

int main()
{
    int memFd = shm_open( "simple_memory", O_RDONLY, 0 );
    if( memFd == -1 )
    {
        perror("Can't open file");
        return 1;
    }

    char* mem = mmap( NULL, 512 * sizeof(char) * GEN_BLOCK_SIZE, PROT_READ, MAP_SHARED, memFd, 0 );
    if( mem == NULL )
    {
        perror("Can't mmap");
        return -1;
    }   
    char *mem_begin = mem;
    char tmp[GEN_BLOCK_SIZE];
    strcpy(tmp, mem);
    int prev_seed = verify(tmp);
    printf("%s\n", mem);
    mem += GEN_BLOCK_SIZE;
    while( true )
    {
        strcpy(tmp, mem);
        int cur_seed = verify(tmp);
        if(cur_seed == prev_seed + 1)
        {
            prev_seed = cur_seed;
            printf("%s\n", mem);
            
        }else{
            sleep(1);
            mem -= GEN_BLOCK_SIZE;
            continue;   
        }
        if(mem + GEN_BLOCK_SIZE == mem_begin + (512 * GEN_BLOCK_SIZE)){
            mem = mem_begin;
        }else{
            mem += GEN_BLOCK_SIZE;
        }
    }

    return 0;
}

