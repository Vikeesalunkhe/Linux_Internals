#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){

    char buffer[100];
    //open the f1.txt in read mode
    int fd = open("f1.txt", O_RDONLY);

    //read the content of the file and store it in the buffer
    read(fd, buffer, sizeof(buffer));

    printf("Content of the file: %s\n", buffer);

    close(fd);   //close the file descriptor
    return 0;


}