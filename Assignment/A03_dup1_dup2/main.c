/*
Name : Vikee Salunkhe
Description : A03 - WAP to understand usage of dup and dup2 system calls 


Using dup or dup2 redirect printf out to a given file instead of printing to stdout.
Pass the file name using command-line arguments.
Try using both system calls (dup and dup2).
Sample Execution:
./dup_dup2 f1.txt
Print this message into STDOUT
 
Note : printf() should be diverted on file and one on STDOUT
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{


    int option;
    
    if(argc < 2)
    {
        printf("Please pass valid command line aruments\n");
        return -1;
    }
    //   select menu
    printf("1. dup\n");
    printf("2. dup2\n");
    

    printf("choose the option : ");
    scanf("%d",&option);

    if (option == 1)
    {
        int fd = open(argv[1],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(fd < 0)
        {
            perror("open");
            return -1;
        }
        int new_fd = dup(fd);
        if(new_fd < 0)
        {
            perror("dup");
            return -1;
        }
        close(fd);
        printf("This message is printed to file using dup\n");
    }
    else if(option == 2)
    {
        int fd = open(argv[1],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(fd < 0)
        {
            perror("open");
            return -1;
        }
        int new_fd = dup2(fd,STDOUT_FILENO);
        if(new_fd < 0)
        {
            perror("dup2");
            return -1;
        }
        close(fd);
        printf("This message is printed to file using dup2\n");
    }
    else
    {
        printf("Invalid option\n");
    }

    
    
    return 0;
}
