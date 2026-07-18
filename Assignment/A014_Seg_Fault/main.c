/*
Name : vikee salunkhe
Description : A14 - WAP to print the address which is causing Segmentation fault

Requirements: 

Write a signal handler function to print address which caused seg-fault(SIGSEGV). 
Use sigaction system call to register signal. 
Use struct siginfo from sa_sigaction to print address (Read man page). 
Create a segmentation fault from code. 
When error occurs program should print address and exit. 
Sample execution: 

 ./sigsegv 

Segmentation fault ..!! 

Address 0x123456 caused error
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig, siginfo_t *siginfo, void *context)
{
    printf("Segmentation fault ..!!\n");
    printf("Address %p caused error\n", siginfo->si_addr);
    signal(SIGSEGV, SIG_DFL);
}

int main()
{
    struct sigaction newact;
    char *str = "Hello World";

    memset(&newact, 0, sizeof(newact));

    newact.sa_sigaction = handler;
    newact.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &newact, NULL);

    *str= 'W'; // This will cause segmentation fault

    return 0;
}