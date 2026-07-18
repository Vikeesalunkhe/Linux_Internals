/*
Name : Vikee Salunkhe
DEscription : A08 - WAP to create three child from the same parent



• To understand how to use fork system calls.

 Requirements: 

Create three child process from same parent. 
Parent has to wait for all three child process.
Print exit status of each child when they terminates.
Sample execution: 

./three_child 

Child 1 with pid 2020 created 
Child 2 with pid 2021 created
Child 3 with pid 2022 created 
Child 2020 is terminated with code 0 
Child 2021 is terminated with code 0 
Child 2022 is terminated with code 0

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() 
{
    int process_id1 = fork();

    if (process_id1 > 0) {

        int process_id2 = fork();
        if (process_id2 > 0) {

            int process_id3 = fork();
            if (process_id3 > 0) {

                sleep(2);
                printf("child 1 with pid %d created\n",process_id1);
                printf("child 2 with pid %d created\n",process_id2);
                printf("child 3 with pid %d created\n",process_id3);
                sleep(2);

            } 
            else
            {
                sleep(1);
                printf("child 3 pid %d was terminated with code %d\n",getpid(),process_id3);
            }

        }
        else
        {
            sleep(1);
            printf("child 2 pid %d was terminated with code %d\n",getpid(),process_id2);
        }



    }
    else
    {
        sleep(1);
        printf("child 1 pid %d was terminated with code %d\n",getpid(),process_id1);

    }


}
