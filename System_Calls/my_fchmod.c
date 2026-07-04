#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
int main(int argc, char *argv[]){
    struct stat my_stat;

    //open the argv[1] in write mode
    int fd = open(argv[1], O_WRONLY);
    int fd2 = open(argv[2], O_WRONLY);
    if(fd == -1 || fd2 == -1){
        perror("open");
        return -1;
    }

    //get the information of FD using fstat()
    if(fstat(fd, &my_stat) == -1){
        perror("fstat");
        return -1;
    }

    //change file2 permission with file1
    if(fchmod(fd2, my_stat.st_mode) == -1){
        perror("fchmod");
        return -1;
    }
    printf("File %s permission changed successfully\n", argv[2]);

    return 0;
}
