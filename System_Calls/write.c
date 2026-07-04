#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){

    char buffer[100] = "Hello, World!";
    //open the f1.txt in write mode
    int fd = open("f2.txt", O_CREAT | O_WRONLY, 0777);  

    //write the content of the buffer to the file
    write(fd, buffer, strlen(buffer));

    close(fd);   //close the file descriptor
    return 0;
}