#include <stdio.h>
#include <fcntl.h>

int main(){

    //open the f1.txt in write mode 

    int fd = open("f1.txt", O_CREAT | O_WRONLY, 0777);

    printf("FD -> %d\n", fd);

    close(fd);

    return 0;
}

