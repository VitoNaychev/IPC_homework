#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
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
    struct smart_block* mem_begin = mem;
     
    char str[4] = "lol";
    unsigned int seed = 1;
    while(true)
    {
        char tmp[GEN_BLOCK_SIZE];
        strcpy(tmp, str);
        generate(tmp, seed);
        
        strcpy(mem->gen_buff, tmp);
        strcpy(mem->buff, str);
        seed += 1; 
        
        if(mem + 1 >= mem_begin + 512){
            mem = mem_begin;
        }else{
            mem += 1;
        }
    }   

    return 0;
}
