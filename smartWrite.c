#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "smartBlock.h"
#include "gen.h"

int main()
{
    int memFd = shm_open( "simple_memory", O_CREAT | O_RDWR, S_IRWXU );
    if( memFd == -1 )
    {
        perror("Can't open file");
        return 1;
    }

    int res;
    res = ftruncate( memFd, sizeof(struct smart_block));
    if( res == -1 )
    {
        perror("Can't truncate file");
        return res;
    }   
   
    struct smart_block* mem = mmap( NULL, sizeof(struct smart_block), PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0 );
    if( mem == NULL )
    {
        perror("Can't mmap");
        return -1;
    }
    
    uint32_t seed = 1;
    mem->pos = 0;
    while(true){   
        
        generate((void*)mem->gen[mem->pos % 512], seed);
        seed += 1; 
        
        printf("Seed: %d Pos: %d\n", verify((void*)mem->gen[mem->pos % 512]), mem->pos);
        
        ++ mem->pos;
        
    }   

    return 0;
}
