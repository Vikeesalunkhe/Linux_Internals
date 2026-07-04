#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    //check if the source and destination files are provided as command line arguments
    if(argc != 3){
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    //open the source file in read mode
    int src_fd = open(argv[1], O_RDONLY);
    if(src_fd < 0){
        perror("Error opening source file");
        return 1;
    }

    //open the destination file in write mode, create it if it doesn't exist
    int dest_fd = open(argv[2], O_CREAT | O_WRONLY, 0777);
    if(dest_fd < 0){
        perror("Error opening destination file");
        close(src_fd);
        return 1;
    }

    char buffer[1024];
    ssize_t bytes_read;

    //read from source file and write to destination file
    while((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0){
        write(dest_fd, buffer, bytes_read);
    }

    //close the file descriptors
    close(src_fd);
    close(dest_fd);

    return 0;


}

