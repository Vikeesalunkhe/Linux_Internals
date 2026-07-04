#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(){
    //open the "f1.txt" in write mode
    int fd = open("f1.txt", O_WRONLY);

    //Copy the STDOUT & close the STDOUT
    int dupstdout = dup(1);
    close(1);
    //duplicate FD
    dup(fd);
    printf("Print this message into FILE\n");
    fflush(stdout);

    //Replace the f1.tx in FD = 1 with STDOUT
    close(1);
    dup(dupstdout);
    printf("Print this message into STDOUT\n");

    return 0;
}
