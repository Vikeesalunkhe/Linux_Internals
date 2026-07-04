#include <stdio.h>
#include <fcntl.h>

int main(){

    //open the f1.txt in write mode 

    //fd : file descriptor is an integer value which is used to identify a file or a socket. It is returned by the open() system call and is used in subsequent read(), write(), and close() system calls to perform operations on the file or socket.
    //O_CREAT : This flag is used to create a new file if it does not exist. If the file already exists, this flag has no effect.
    //O_WRONLY : This flag is used to open the file in write-only mode.
    //0777 : This is the file permission mode, which specifies the read, write, and execute permissions for the file.
    int fd = open("f1.txt", O_CREAT | O_WRONLY, 0777);

    printf("FD -> %d\n", fd);

    close(fd);   //close the file descriptor

    return 0;
}

