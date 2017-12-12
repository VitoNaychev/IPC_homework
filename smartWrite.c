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
    res = ftruncate( memFd, 512 * sizeof(struct smart_block));
    if( res == -1 )
    {
        perror("Can't truncate file");
        return res;
    }   
   
    struct smart_block* mem = mmap( NULL, 512 * sizeof(struct smart_block), PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0 );
    if( mem == NULL )
    {
        perror("Can't mmap");
        return -1;
    }

    for(int i = 0 ; i < 512 ; ++ i){
        (mem + i)->pos = i;
    }
    
    uint32_t seed = 1;
    
    while(true){   
        
        if((mem->pos + 1) % 512 == 0){
            mem -= 511;
        }else{
            mem += 1;
        }
        mem->pos += 512;
        generate((void*)mem->gen, seed);
        seed += 1; 
        
        printf("Seed: %d Pos: %d\n", seed, mem->pos);
    }   

    return 0;
}
