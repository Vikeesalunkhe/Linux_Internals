/*
 * dup_dup2.c
 *
 * Demonstrates redirecting the output of printf() (which normally
 * goes to fd 1, the terminal) into a file using dup()/dup2(),
 * and then restoring STDOUT back to the terminal.
 *
 * Both system calls are used:
 *   - dup()  : used to save a copy of the original STDOUT fd.
 *   - dup2() : used (a) to redirect STDOUT to the file, and
 *              (b) to restore STDOUT back from the saved copy.
 *
 * Usage:
 *   ./dup_dup2 f1.txt
 *
 * Expected behaviour:
 *   - A message is written into f1.txt (not visible on the terminal).
 *   - A second message "Print this message into STDOUT" is printed
 *     on the terminal (STDOUT), after STDOUT has been restored.
 *
 * Compile:
 *   gcc -Wall -o dup_dup2 main.c
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    /* ---------- 1. Open the target file ---------- */
    int file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) {
        fprintf(stderr, "%s: cannot open '%s': %s\n", argv[0], filename, strerror(errno));
        return 1;
    }

    /* ---------- 2. Save a copy of the original STDOUT using dup() ----------
     * dup() returns a new file descriptor that refers to the SAME
     * open file description as fd 1 (i.e. the terminal).
     * We keep this aside so we can restore STDOUT later.
     */
    int stdout_copy = dup(1);
    if (stdout_copy < 0) {
        perror("dup");
        close(file_fd);
        return 1;
    }

    /* ---------- 3. Redirect STDOUT to the file using dup2() ----------
     * dup2(file_fd, 1) closes the current fd 1 (STDOUT)
     * and makes it a duplicate of file_fd. From this point on, anything
     * written to fd 1 (including printf, which writes to STDOUT) goes
     * into the file instead of the terminal.
     */
    if (dup2(file_fd, 1) < 0) {
        perror("dup2 (redirect to file)");
        close(file_fd);
        close(stdout_copy);
        return 1;
    }

    /* file_fd is no longer needed separately; fd 1 (STDOUT) is now
     * an equivalent duplicate of it. */
    close(file_fd);

    /* ---------- 4. This printf goes into the file, not the terminal ---------- */
    printf("This message is redirected into the file using dup2()\n");
    fflush(stdout);   /* make sure it is flushed before we switch fds back */

    /* ---------- 5. Restore STDOUT back to the terminal using dup2() ----------
     * dup2(stdout_copy, 1) makes fd 1 point back to the
     * original terminal output again.
     */
    if (dup2(stdout_copy, 1) < 0) {
        perror("dup2 (restore stdout)");
        close(stdout_copy);
        return 1;
    }

    close(stdout_copy);

    /* ---------- 6. This printf goes to the terminal (STDOUT) again ---------- */
    printf("Print this message into STDOUT\n");

    return 0;
}