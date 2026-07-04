#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
int main(int argc, char *argv[]){
    struct stat my_stat;

    //open the argv[1] in write mode
    int fd = open(argv[1], O_WRONLY);
    if(fd == -1){
        perror("open");
        return -1;
    }

    //get the information of FD using fstat()
    if(fstat(fd, &my_stat) == -1){
        perror("fstat");
        return -1;
    }

    printf("Size of %s is %lu\n", argv[1], my_stat.st_size);
    printf("Mode of %s is %o\n", argv[1], my_stat.st_mode);

    return 0;
}
