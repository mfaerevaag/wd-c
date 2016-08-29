#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "logger.h"
#include "persist.h"
#include "config.h"

int parse_args(int argc, char **argv);
void print_help();
void print_version();

int main(int argc, char **argv)
{
    /* rc = read_rc(); */

    /* parse args */
    if (parse_args(argc, argv) != 0) {
        log_err("failed to parse args");
        exit(EXIT_ERROR);
    }

    /* if here, no other command given; warp */
    if (ARGC == 1) {
        debugf("warping to '%s'\n", ARGV[0]);

        wpoint *point = wp_find(ARGV[0]);

        printf("%s", point->dir);
        exit(EXIT_SUCCESS);
    } else if(ARGC > 1) {
        log_err("cannot warp to multiple points"); // TODO: logging
    } else {
        log_err("no warp point given"); // TODO: logging
    }

    /* clean */
    wp_free();

    return 0;
}

int parse_args(int argc, char **argv)
{
    int c, ret;
    ret = 0; /* all goond so far */
    opterr = 1; /* prevent error messages if 0 */

    while (1)
    {
        int option_index = 0;

        static struct option long_options[] =
            {
                {"quiet",   no_argument,       &QUIET_FLAG, 1},
                {"version", no_argument,       0,  0 },
                {"help",    no_argument,       0, 'h'},
                {"config",  required_argument, 0, 'c'},
                {0, 0, 0, 0}
            };

        c = getopt_long(argc, argv, "hc:",
                        long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            print_version();
            exit(EXIT_INFO);
            break;

        case 'h':
            print_help();
            exit(EXIT_INFO);
            break;

        case 'c':
            // TODO: config file
            printf("TODO: config file `%s'\n", optarg);
            break;

        case '?':
            // TODO: logging
            break;

        default:
            abort(); // TODO: error
        }
    }

    /* store rest arguments */
    if (optind < argc) {
        ARGV = calloc(sizeof(char*), argc - optind);

        char *opt;
        while (optind < argc) {
            opt = argv[optind++];
            debugf("opt: %s\n", opt);
            size_t size = strlen(opt);

            ARGV[ARGC] = malloc(size);
            memcpy((char *) ARGV[ARGC], (char *) opt, size);

            ARGC++;
        }
    }

    return ret;
}

void print_help()
{
    printf("TODO: wow, such help\n");
}

void print_version()
{
    printf("TODO: version\n");
}
