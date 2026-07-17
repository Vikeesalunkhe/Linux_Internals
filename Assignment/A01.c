/*
 * my_copy.c
 *
 * A01 - Implement a cp (copy) command with -p option, using only
 * basic system calls (open, read, write, close, fstat, fchmod).
 *
 * Usage:
 *   ./my_copy source dest
 *   ./my_copy -p source dest
 *
 * Requirements implemented:
 *  - Copies source file to destination file (byte-for-byte, including
 *    zero-sized files).
 *  - Shows usage info if arguments are missing.
 *  - my_copy(source_fd, dest_fd) does the actual read/write copy loop.
 *  - -p option copies source file's permissions to destination (fstat + fchmod).
 *  - If destination already exists, asks for overwrite confirmation
 *    (Y/n). Enter or 'Y'/'y' overwrites; 'n'/'N' aborts without changes.
 *  - Handles error conditions (missing file, permission errors, I/O errors,
 *    partial writes, etc).
 *
 * Compile:
 *   gcc -Wall -o my_copy my_copy.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 8192

/*
 * my_copy()
 * ----------
 * Reads from source_fd and writes to dest_fd until EOF.
 * Handles partial writes (write() may write fewer bytes than requested).
 * Returns 0 on success, -1 on failure (errno / message printed by caller
 * context where possible).
 */
int my_copy(int source_fd, int dest_fd)
{
    char buf[BUF_SIZE];
    ssize_t bytes_read, bytes_written, total_written;

    while ((bytes_read = read(source_fd, buf, BUF_SIZE)) > 0) {
        total_written = 0;

        /* Handle partial writes: keep writing until all bytes_read
         * bytes have actually been written out. */
        while (total_written < bytes_read) {
            bytes_written = write(dest_fd,
                                   buf + total_written,
                                   bytes_read - total_written);

            if (bytes_written < 0) {
                if (errno == EINTR) {
                    /* interrupted by a signal, retry */
                    continue;
                }
                perror("write");
                return -1;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read < 0) {
        perror("read");
        return -1;
    }

    /* bytes_read == 0 means EOF was reached; copy completed successfully.
     * Note: if the source file is empty, this loop body never executes,
     * and dest_fd (already created via O_CREAT|O_TRUNC) remains a
     * correctly-sized zero-byte file. */
    return 0;
}

static void usage(const char *prog)
{
    fprintf(stderr, "Usage:- %s [option]\n", prog);
    fprintf(stderr, "  %s source_file dest_file\n", prog);
    fprintf(stderr, "  %s -p source_file dest_file\n", prog);
}

int main(int argc, char *argv[])
{
    int preserve_perms = 0;
    char *positional[2];
    int pos_count = 0;

    /* ---------- 1. Check for insufficient arguments up-front ---------- */
    if (argc < 2) {
        fprintf(stderr, "Insufficient arguments\n");
        usage(argv[0]);
        return 1;
    }

    /* ---------- 2. Parse arguments: options + positional filenames ---------- */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            preserve_perms = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "%s: invalid option -- '%s'\n", argv[0], argv[i]);
            usage(argv[0]);
            return 1;
        } else {
            if (pos_count < 2) {
                positional[pos_count] = argv[i];
            }
            pos_count++;
        }
    }

    if (pos_count < 2) {
        fprintf(stderr, "Insufficient arguments\n");
        usage(argv[0]);
        return 1;
    }

    if (pos_count > 2) {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        usage(argv[0]);
        return 1;
    }

    const char *source_file = positional[0];
    const char *dest_file = positional[1];

    /* ---------- 3. Open source file ---------- */
    int source_fd = open(source_file, O_RDONLY);
    if (source_fd < 0) {
        fprintf(stderr, "%s: cannot open '%s': %s\n",
                argv[0], source_file, strerror(errno));
        return 1;
    }

    /* fstat on source to know its permissions/mode (needed for -p,
     * and also useful to validate it's a regular file). */
    struct stat source_stat;
    if (fstat(source_fd, &source_stat) < 0) {
        perror("fstat");
        close(source_fd);
        return 1;
    }

    /* ---------- 4. If destination exists, ask for confirmation ---------- */
    if (access(dest_file, F_OK) == 0) {
        printf("File \"%s\" already exists. Do you want to overwrite (Y/n) ",
               dest_file);
        fflush(stdout);

        char response[16];
        if (fgets(response, sizeof(response), stdin) == NULL) {
            /* EOF or read error on stdin: treat as "no" for safety */
            fprintf(stderr, "\n%s: no response, aborting.\n", argv[0]);
            close(source_fd);
            return 1;
        }

        char c = response[0];
        if (c == 'n' || c == 'N') {
            printf("Not overwriting. Exiting.\n");
            close(source_fd);
            return 0;
        }
        /* Any of 'Y', 'y', or just Enter ('\n') => proceed to overwrite */
    }

    /* ---------- 5. Open/create destination file ---------- */
    /* Default mode 0644 if we are creating a fresh file; if -p is given
     * we will fchmod() to the exact source permissions afterwards anyway. */
    int dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        fprintf(stderr, "%s: cannot create '%s': %s\n",
                argv[0], dest_file, strerror(errno));
        close(source_fd);
        return 1;
    }

    /* ---------- 6. Perform the actual copy ---------- */
    if (my_copy(source_fd, dest_fd) != 0) {
        fprintf(stderr, "%s: copy failed\n", argv[0]);
        close(source_fd);
        close(dest_fd);
        return 1;
    }

    /* ---------- 7. If -p was passed, copy permissions to destination ---------- */
    if (preserve_perms) {
        /* Use only the permission bits (mask with 07777 to drop file-type bits) */
        if (fchmod(dest_fd, source_stat.st_mode & 07777) < 0) {
            perror("fchmod");
            close(source_fd);
            close(dest_fd);
            return 1;
        }
        printf("Permissions copied from '%s' to '%s'\n", source_file, dest_file);
    }

    /* ---------- 8. Clean up ---------- */
    close(source_fd);
    close(dest_fd);

    printf("'%s' copied to '%s' successfully\n", source_file, dest_file);

    return 0;
}