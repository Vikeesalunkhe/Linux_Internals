/*
 * three_child.c
 *
 * Creates three child processes from the same parent using fork().
 * The parent waits for all three children (in whatever order they
 * actually terminate) and prints each child's exit status.
 *
 * Usage:
 *   ./three_child
 *
 * Compile:
 *   gcc -Wall -o three_child three_child.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

int main(void)
{
    /* Make stdout unbuffered so parent/child messages always print
     * in true chronological order, even when not on a terminal. */
    setvbuf(stdout, NULL, _IONBF, 0);

    /* ---------- 1. Create three child processes ---------- */
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            /* ---- Child process ---- */
            /* Each child could do its own task here; for this
             * demonstration it simply exits with a status code
             * equal to its child number (0 => success/normal exit
             * is also fine — change as required). */
            _exit(0);
        } else {
            /* ---- Parent process ---- */
            printf("Child %d with pid %d created\n", i + 1, pid);
        }
    }

    /* ---------- 2. Parent waits for ALL three children ---------- */
    /* wait() returns as soon as ANY child terminates, so children
     * may finish in a different order than they were created; we
     * simply loop NUM_CHILDREN times to reap all of them. */
    for (int i = 0; i < NUM_CHILDREN; i++) {
        int status;
        pid_t terminated_pid = wait(&status);

        if (terminated_pid < 0) {
            perror("wait");
            continue;
        }

        if (WIFEXITED(status)) {
            printf("Child %d is terminated with code %d\n",
                   terminated_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child %d is terminated by signal %d\n",
                   terminated_pid, WTERMSIG(status));
        } else {
            printf("Child %d terminated abnormally\n", terminated_pid);
        }
    }

    return EXIT_SUCCESS;
}