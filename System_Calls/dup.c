#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(){
    //open the "f1.txt" in write mode
    int fd = open("f1.txt", O_WRONLY);

    //close the STDOUT (fd 1), freeing up fd number 1 for reuse
    close(1);
    //dup() always returns the lowest available FD number.
    //Since fd 1 was just closed, dup(fd) makes fd 1 point to the
    //same open file as fd (f1.txt) -- stdout is now the file, not the terminal.
    dup(fd);

    //this printf() writes to fd 1, which now points to f1.txt, not the screen
    printf("Print this message into FILE\n");

    //stdout is STILL redirected to f1.txt at this point (nothing restored it),
    //so despite the comment/name, this also lands in f1.txt, not the terminal
    printf("Print this message into STDOUT\n");

    return 0;
}
