/*
name : Vikee Salunkhe

description : A09 - Implement communication between two related processes using one pipe

Requirements: 

Create two child process and execute commands passed from command-line arguments 
Each command is separated by a '|' (pipe) character. 
First child execute first command (with or without options) and pass o/p to next. 
Second child executes second command (after '|') will reads I/p from first cmd. 
Parent will wait for both child process to finish .
Sample execution: 

./pipe (No arguments)
    Error: No arguments passed Usage: ./pipe   <command1 > '|'   <command2>   

2. ./pipe ls  

    Error: Insufficient arguments passed Usage: ./pipe  <command1 > '|'   <command2>   

3. ./pipe ls '|' wc

    5 25 4. 

4. ./pipe ls ­ -l -­a '|' wc -­l ­-w 

    10 15
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pos;
    
    if(argc < 3)
    {
        printf("Error: No arguments passed Usage: ./pipe   <command1 > '|'   <command2>\n");
        exit(1);
    }    
    else if((strcmp(argv[2],"|"))  == 0)             
    {          
            
        int fd[2];

        if( pipe(fd)== -1)
        {
            perror("pipe");
            exit(1);
        }
        int pid1 = fork();
        int pid2 = fork();
           
        if(pid2 > 0)
        {
             sleep(4);
             exit(1);
        }
        else if(pid1 == 0)
        {
            dup2(fd[1] , 1);
            close(fd[0]);
            close(fd[1]);
            execlp(argv[1],argv[1],(char*)NULL);
        }  
        else if(pid2 == 0)
        {        
            dup2(fd[0] , 0);
            close(fd[0]);
            close(fd[1]);
            execlp(argv[3],argv[3],(char*)NULL);
        }          

    }
    else
    {            
        printf("Error: No arguments passed Usage: ./pipe   <command1 > '|'   <command2>\n");
        exit(1);
    }
    return 0;
}