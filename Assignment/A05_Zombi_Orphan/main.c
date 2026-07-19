/*
Name : vikee salunkhe

description : A05 - WAP to make zombie process become orphan, print status of each state

Requirements: 

Create a child process and print status that process is running
After some time print status that process is zombie state
After some time print zombie process cleared by init.
To print status use help of /proc//status file.
For eg: if child pid is 1234, open file /proc/1234/status and print first 3 lines
      5. If that file is not available means that process is cleared.
 
Sample execution:
 
1. ./zomb_orph
A child created with pid 1234
Name:  ./zomb_orph
State: S (sleeping)
Name:  /zomb_orph   (After some time)
State: Z (zombie)
Process 1234 cleared by init  (After some time)

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main() {

      pid_t pid = fork();

      if (pid > 0){

            sleep(2);
            char path[50];

            sprintf(path, "cat /proc/%d/status | head -2", pid);
            printf("%s\n", path);

            system(path);

            int pid1 = fork();

            if (pid1 > 0){

                  printf("Process %d cleared by init\n", pid);
                  sleep(2);
                  exit(1);
            }
      }
      else if (pid == 0){

            char path[50];
            int ret = getpid();

            printf("Chiled created with pid %d\n", ret);

            sprintf(path, "cat /proc/%d/status | head -3", ret);
            printf("%s\n", path);
            system(path);
            exit(1);
      }
      else 
      {
            perror("Fork");
            exit(1);
      }

      return 0;

}

