#include <stdio.h>
#include <stdlib.h>
#include "../include/t2fs.h"

int main()
{
    printf("Hello world!\n");

    char groupName[SIZEIDENTIFY+1];
    int errorCode = identify2(groupName, SIZEIDENTIFY);
    if(errorCode == -1) {
        printf("Main error");
        return -1;
    }

    char path[59];
    getcwd2(path,sizeof(path));
    printf("cwd:\n%s\n", path);
    printf("Nome do grupo:\n%s\n", groupName);

    return 0;
}
