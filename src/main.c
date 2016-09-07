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

int DO_WARP = 1;

int main(int argc, char **argv)
{
    /* parse args */
    if (parse_args(argc, argv) != 0) {
        log_err("failed to parse args");
        exit(EXIT_ERROR);
    }

    /* do warp? */
    if (DO_WARP == 1) {
        debug("do warp");

        if (ARGC == 1) {
            /* TODO: clean args, buffer overflow */
            char *name = ARGV[0];

            debugf("warping to '%s'\n", name);

            wpoint *point = wp_find(name);

            if (point == NULL) {
                log_errf("no warp point named '%s'\n", name);
                exit(EXIT_ERROR);
            }

            printf("%s", point->dir);
        } else if(ARGC > 1) {
            log_err("cannot warp to multiple points");
        } else {
            log_err("no warp point given");
        }
    }

    /* clean */
    wp_free();
    exit(DO_WARP ? EXIT_SUCCESS : EXIT_INFO);
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
                {"list",    no_argument,       0, 'l'},
                {"config",  required_argument, 0, 'c'},
                {"remove",  required_argument, 0, 'r'},
                {"add",     required_argument, 0, 'a'},
                {0, 0, 0, 0}
            };

        c = getopt_long(argc, argv, "hlc:r:a:",
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
            debugf("setting rc file to '%s'\n", optarg);
            set_rc_file(optarg);
            break;

        case 'a':
            debug("do add");

            /* TODO: clean args */
            wp_add(optarg, getenv("PWD"));
            DO_WARP = 0;
            break;

        case 'r':
            debug("do remove");

            wp_remove(optarg);
            DO_WARP = 0;
            break;

        case 'l':
            debug("do list");

            wp_print_all();
            DO_WARP = 0;
            break;

        case '?':
            // TODO: logging
            break;

        default:
            exit(EXIT_ERROR);
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
    printf(
        "Usage: wd [command] <point>\n"
        "\n"
        "Example:\n"
        "\t$ wd --add foo \t\t# points to current dir\n"
        "\t$ cd ~/far/far/away\n"
        "\t$ wd foo \t\t# and you're back!\n"
        "\n"
        "Commands:\n"
        "--add <point>\tAdds the current working directory to your warp points\n"
        "--add! <point>\tOverwrites existing warp point\n"
        "--rm <point>\tRemoves the given warp point\n"
        /* "--show\t\tPrint warp points to current directory\n" */
        /* "--show <point>\tPrint path to given warp point\n" */
        "--list\t\tPrint all stored warp points\n"
        /* "--ls  <point>\tShow files from given warp point\n" */
        "--path <point>\tShow the path to given warp point\n"
        /* "--clean!\tRemove points warping to nonexistent directories\n" */
        "\n"
        "Other:\n"
        "-h, --help\t\tShow this extremely helpful text\n"
        "-v, --version\tPrint version\n"
        "-c, --config <file>\tSpecify config file (default ~/.warprc)\n"
        "-q, --quiet\tSuppress all output\n"
        "\n"
        "Licenced under MIT\n" // TODO
        "Report bugs to github.com/mfaerevaag/wdc\n" // TODO
        );
}

void print_version()
{
    printf("wd (in c) version 0.1\n");
}
