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
    uint64_t cur_pos = mem->pos - 100;
    
    uint32_t prev_seed = verify((void*)mem->gen[cur_pos % 512]);

    while(prev_seed == -1){
        prev_seed = verify((void*)mem->gen[cur_pos % 512]);
    }
    ++ cur_pos;
    while(true){
        
        uint32_t cur_seed = verify((void*)mem->gen[cur_pos % 512]);

        if(cur_seed - 1 == prev_seed){
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            prev_seed = cur_seed;
        }else{
            printf("Prev: %d Curr: %d\n", prev_seed, cur_seed);
            usleep(1);
            //proverqvame dali ne sme napravili neshto koeto
            //da e zabavilo reada i write da e izburzalo
            //s edin cikul napred(512 pozicii)
            if(cur_pos  + 512 < mem->pos){
                printf("cur_pos: %d mem_pos: %d\n", cur_pos, mem->pos);
                printf("Seems like write is faster than SaNiC... Do you want to continue 1 == Yes/0 == No: ");
                uint8_t ans = 0;
                scanf("%d", &ans);
                if(ans > 0){
                    //ako potrebitelq reshi che iska da produlji
                    //izspulnenieto na programata cur_pos stava
                    //raven na nqkakvo chislo koeto da e okolo mem_pos
                    // za da dadem prednina na write i seeda se
                    //vizma ot tekushtata poziciq
                    cur_pos = mem->pos - 100;
                    prev_seed = verify((void*)mem->gen[cur_pos % 512]);
                    ++ cur_pos;
                }else{
                    return 0;
                }
            }
            continue;
        }
        ++ cur_pos;    
    }

    return 0;
}

