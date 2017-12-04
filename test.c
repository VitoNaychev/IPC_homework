#include "gen.h"
#include <stdio.h>
#include <string.h>

int main(){
    char str[] = "LOL";
    char tmp[GEN_BLOCK_SIZE];
    strcpy(tmp, str);
    printf("%s\n", str);
    printf("%s\n", tmp);
    generate(tmp, 1);
    printf("%s\n", tmp);
    verify(tmp);
    printf("%s\n", tmp);
    return 0;
}
