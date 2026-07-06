/*
 * word_count.c
 *
 * A simplified 'wc' implementation using raw system calls
 * (open, read, write, close) instead of standard I/O buffered functions.
 *
 * Usage:
 *   ./word_count                     -> reads from stdin till EOF (Ctrl+D)
 *   ./word_count file.txt            -> counts lines/words/bytes of file
 *   ./word_count f1.txt f2.txt ...   -> counts each file + prints total
 *   ./word_count -l -w -c [files]    -> print only selected counts
 *
 * Compile:
 *   gcc -Wall -o word_count main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4096

/*
 * word_count()
 * -------------
 * Reads from the given file descriptor till EOF and calculates
 * the number of lines, words and bytes (characters) read.
 * Results are stored in the caller-supplied addresses.
 *
 * Returns 0 on success, -1 on a read error.
 */
int word_count(int fd, int *lines, int *words, int *bytes)
{
    char buf[BUF_SIZE];
    ssize_t n;
    int in_word = 0;

    *lines = 0;
    *words = 0;
    *bytes = 0;

    while ((n = read(fd, buf, BUF_SIZE)) > 0) {
        *bytes += (int)n;

        for (ssize_t i = 0; i < n; i++) {
            char c = buf[i];

            if (c == '\n') {
                (*lines)++;
            }

            if (c == ' ' || c == '\t' || c == '\n') {
                in_word = 0;
            } else {
                if (!in_word) {
                    (*words)++;
                    in_word = 1;
                }
            }
        }
    }

    if (n < 0) {
        perror("read");
        return -1;
    }

    return 0;
}

/*
 * print_values()
 * ---------------
 * Prints the requested counts (based on show_l/show_w/show_c flags)
 * followed by the file name (if provided, i.e. non-NULL).
 */
void print_values(int lines, int words, int bytes,
                   int show_l, int show_w, int show_c,
                   const char *name)
{
    int printed_any = 0;

    if (show_l) {
        printf("%s%d", printed_any ? " " : "", lines);
        printed_any = 1;
    }
    if (show_w) {
        printf("%s%d", printed_any ? " " : "", words);
        printed_any = 1;
    }
    if (show_c) {
        printf("%s%d", printed_any ? " " : "", bytes);
        printed_any = 1;
    }

    if (name != NULL) {
        printf(" %s", name);
    }

    printf("\n");
}

void print_usage(const char *prog_name)
{
    fprintf(stderr,
            "Usage: %s [-l] [-w] [-c] [file1 file2 ...]\n"
            "  -l   print line count only\n"
            "  -w   print word count only\n"
            "  -c   print byte(character) count only\n"
            "  (any combination of -l -w -c may be given)\n"
            "If no file is given, input is read from stdin.\n",
            prog_name);
}

int main(int argc, char *argv[])
{
    int show_l = 0, show_w = 0, show_c = 0;
    int option_given = 0;

    /* file name arguments will be collected here */
    char *files[argc];
    int file_count = 0;

    /* ---------- 1. Parse command line arguments ---------- */
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            /* Option string, e.g. "-l", "-w", "-c", or combined "-lwc" */
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'l':
                        show_l = 1;
                        option_given = 1;
                        break;
                    case 'w':
                        show_w = 1;
                        option_given = 1;
                        break;
                    case 'c':
                        show_c = 1;
                        option_given = 1;
                        break;
                    default:
                        fprintf(stderr,
                                "%s: invalid option -- '%c'\n",
                                argv[0], argv[i][j]);
                        print_usage(argv[0]);
                        exit(EXIT_FAILURE);
                }
            }
        } else {
            /* Treat as a file name */
            files[file_count++] = argv[i];
        }
    }

    /* If no option was explicitly given, show all three counts */
    if (!option_given) {
        show_l = show_w = show_c = 1;
    }

    int total_lines = 0, total_words = 0, total_bytes = 0;
    int exit_status = EXIT_SUCCESS;

    /* ---------- 2. No files passed -> read from stdin ---------- */
    if (file_count == 0) {
        int lines, words, bytes;

        if (word_count(STDIN_FILENO, &lines, &words, &bytes) != 0) {
            fprintf(stderr, "%s: error reading from stdin\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        print_values(lines, words, bytes, show_l, show_w, show_c, NULL);
        return EXIT_SUCCESS;
    }

    /* ---------- 3. One or more files passed ---------- */
    int valid_file_count = 0;

    for (int i = 0; i < file_count; i++) {
        int fd = open(files[i], O_RDONLY);

        if (fd < 0) {
            /* Handle error condition: file cannot be opened */
            fprintf(stderr, "%s: cannot open '%s': %s\n",
                    argv[0], files[i], strerror(errno));
            exit_status = EXIT_FAILURE;
            continue;
        }

        int lines, words, bytes;

        if (word_count(fd, &lines, &words, &bytes) != 0) {
            fprintf(stderr, "%s: error reading '%s'\n", argv[0], files[i]);
            close(fd);
            exit_status = EXIT_FAILURE;
            continue;
        }

        close(fd);

        print_values(lines, words, bytes, show_l, show_w, show_c, files[i]);

        total_lines += lines;
        total_words += words;
        total_bytes += bytes;
        valid_file_count++;
    }

    /* ---------- 4. Print total only if more than one valid file ---------- */
    if (valid_file_count > 1) {
        print_values(total_lines, total_words, total_bytes,
                      show_l, show_w, show_c, "total");
    }

    return exit_status;
}