/*
name : vikee salunkhe

description : A07 - WAP to create child process to execute command passed through command line

Requirements:

Create child and execute a command passed from command-line arguments.  
If no arguments passed print a usage message.
In case any wrong command passed, print an error message.
After child terminates print the exit status of child process.
Sample execution: -
1. No args passed (Print usage info)
./exe_child Usage: ./exe_child args...
2. Valid arg. passed
./exe_child date
This is the CHILD process, with id 11612
Wed Apr  4 13:27:19 IST 2012
Child exited with status 0
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    pid_t pid;
    char *command[argc];
    int i;

    if (argc < 2){

        printf("Please pass valid argument\n");
        printf("Insuficient arguments\n");
        exit(1);
    }

    for (i = 0; i < argc -1 ; i++){
        command[i] = argv[i+1];
    }

    command[i] = NULL;

    pid = fork();

    if (pid > 0){
        int status;
        int wpid = wait(&status);

        if(WIFEXITED(status))			
        {                
            printf("child terminated normally\n");			
            printf("Child exited with status -> %d \n", WEXITSTATUS(status));			
        }			
        else			
        {				
            printf("Child terminated abnormally \n");				               
            printf("Child exited with code %d \n",WEXITSTATUS(status));			
        }
    }

    else if(pid == 0)
    {
        printf("This is the child process, with id %d\n",getpid()); 
        sleep(2);
        //exec        
        if(execvp(argv[1], command) < 0)              
        {                  
            perror(command[0]);                
            exit(1);             
        }   
    }
    else
    {    
        perror("fork");
        exit(1);
    }
    return 0;
}