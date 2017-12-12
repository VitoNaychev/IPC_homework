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
    uint32_t prev_seed = verify((void*)(mem->gen));

    while(prev_seed == -1){
        prev_seed = verify((void*)(mem->gen));
    }
    mem += 1;
    while(true){
        
        uint32_t cur_seed = verify((void*)mem->gen);
        uint64_t pos = mem->pos;

        if(cur_seed - 1 == prev_seed){
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            prev_seed = cur_seed;
        }else{
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            usleep(1);
            if(pos + 2 * 512 < mem->pos){
                printf("U fucked up! Do u want to continue? Y/n: ");
                char cont = 'Y';
                scanf("%c", &cont);
                printf("\n");
                if(cont == 'Y'){
                    prev_seed = verify((void*)mem->gen);
                    if((mem->pos + 1) % 512 == 0){ 
                        mem -= 511;
                    }else{
                        mem += 1;
                    }
                    continue;
                }else{
                    return 0;
                }
                //printf("norm pos %d cur pos %d\n", pos, mem->pos);
            }
            continue;
        }
        
        if((mem->pos + 1) % 512 == 0){ 
            mem -= 511;
        }else{
            mem += 1;
        }
        

    }

    return 0;
}

