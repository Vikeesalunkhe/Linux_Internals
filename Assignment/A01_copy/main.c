#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 255

int my_copy(int source_fd, int dest_fd)
{

    int data_len = 0;
    char buffer[BUF_SIZE];

    //read data from source file and write to destination file

    while ((data_len = read(source_fd, buffer, BUF_SIZE)) > 0) {
        if (write(dest_fd, buffer, data_len) != data_len) {
            perror("write");
            return -1;
        }
    }

    if (data_len < 0) {
        perror("read");
        return -1;
    }

    // check file contents present in destination file
    if (dest_fd == -1) {
        printf("Successfully copied the contents from source file to destination file\n");
        return -1;
    }

    return 0;


}


int main(int argc, char *argv[])
{

    int source_fd, dest_fd, read_len;
    char choice;

    //-p to copy the permission mode
	struct stat sb;

    if (argc < 3) {
        printf("Insufficient arguments\n");
        printf("Usage: ./my_copy [option] <source_file> <dest_file>\n");
        printf("Please pass valid cmd line arguments\n");

        return -1;
    }

    // check if the source file exists
    if (strcmp(argv[1], "-p") == 0) {

        printf("Permissions also copied from source to destination file.\n");
        source_fd = open(argv[2], O_RDONLY);
        if (source_fd < 0) {
            printf("Error opening source file\n");
            return -1;
        }

        // get the permission mode of source file
        if (stat(argv[2], &sb) == -1) {
            perror("stat");
            return -1;
        }

        // check if the destination file exists
        if ( (dest_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1 ) {

            printf("File %s is already exists.\n Do you want to overwrite (Y/n) ", argv[3]);
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                printf("Operation cancelled by user.\n");
                return -1;
            }
            else if (choice == 'y' || choice == 'Y') {
                dest_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0664);
                if (dest_fd == -1) {
                    perror("open");
                    return -1;
                }
            }
        }

        // copy the permission mode from source to destination file
        if (chmod(argv[3], sb.st_mode & 0777)) {
            perror("chmod");
            return -1;
        }

    }
    else {
        //read the source file
        source_fd = open(argv[1], O_RDONLY);
        if (source_fd < 0) {
            printf("Error opening source file\n");
            return -1;
        }


        //write the source file to destination file
        if ( (dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1 ) {

            printf("File %s is already exists.\n Do you want to overwrite (Y/n) ", argv[2]);
            scanf(" %c", &choice);
            if (choice == 'n' || choice == 'N') {
                printf("Operation cancelled by user.\n");
                return -1;
            }
            else if (choice == 'y' || choice == 'Y') {
                dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
                if (dest_fd == -1) {
                    perror("open");
                    return -1;
                }
            }
        }


    }

    // copy the contents from source file to destination file
    my_copy(source_fd, dest_fd);
    close(source_fd);
    close(dest_fd);
    return 0;
}

