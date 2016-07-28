#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

static int QUIET_FLAG;
/* static char *RC_FILE = "~/.warprc"; */
static int ARGC = 0;
static char **ARGV = NULL;

int parse_args(int argc, char **argv);

int main(int argc, char **argv)
{
    int rc;

    /* rc = read_rc(); */

    rc = parse_args(argc, argv);

    return rc;
}

int parse_args(int argc, char **argv)
{
    int c, ret;
    ret = 0; /* all good so far */
    opterr = 1; /* prevent error messages if 0 */

    while(1)
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
            printf("TODO: version\n");
            exit(0); // TODO: ret code
            break;

        case 'h':
            printf("TODO: wow, such help\n");
            exit(0); // TODO: ret code
            break;

        case 'c':
            // TODO: config file
            printf("TODO: config file `%s'\n", optarg);
            break;

        case '?':
            // TODO: logging
            break;

        default:
            abort();
        }
    }

    /* store rest arguments */
    if (optind < argc) {
        ARGV = calloc(sizeof(char*), argc - optind);

        char *opt;
        while (optind < argc) {
            opt = argv[optind++];
            printf("opt: %s\n", opt); // TODO: debug
            size_t size = strlen(opt);

            ARGV[ARGC] = malloc(size);
            memcpy((char *) ARGV[ARGC], (char *) opt, size);

            ARGC++;
        }
    }

    return ret;
}
