#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int wstatus;

    pid = fork();
    
    
    if(pid > 0)
    {
        printf("A child created with pid %d\n",getpid());
        while(waitpid(pid, &wstatus, WNOHANG) == 0)
        {
            printf("Parent is Running\n");
        }
    }
    else if(pid == 0)
    {

        sleep(2);
        printf("Child %d terminated normally with exit status %d parent terminating\n",getpid(),pid);
    }
    else
    {

        perror("Fork");
        exit(0);
    }
    return 0;
}
