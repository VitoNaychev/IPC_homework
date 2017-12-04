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
    res = ftruncate( memFd, 512 * sizeof(char) * GEN_BLOCK_SIZE );
    if( res == -1 )
    {
        perror("Can't truncate file");
        return res;
    }   
    char* mem = mmap( NULL, 512 * sizeof(char) * GEN_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0 );
    if( mem == NULL )
    {
        perror("Can't mmap");
        return -1;
    }
    char* mem_begin = mem;
     
    char str[4] = "lol";
    unsigned int seed = 1;
    while(true)
    {
        char tmp[GEN_BLOCK_SIZE];
        strcpy(tmp, str);
        generate(tmp, seed);
        memcpy(mem, tmp, strlen(tmp));
        seed += 1; 
        
        if(mem + GEN_BLOCK_SIZE >= mem_begin + (512 * GEN_BLOCK_SIZE)){
            mem = mem_begin;
        }else{
            verify(tmp);
            printf("%s\n", tmp);
            mem += GEN_BLOCK_SIZE;
        }
    }   

    return 0;
}
