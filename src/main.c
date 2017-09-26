#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "logger.h"
#include "engine.h"
#include "config.h"

int parse_args(int argc, char **argv);
void print_help();
void print_version();

int DO_WARP = 1;

int main(int argc, char **argv)
{
    int i, slash;
    char *name, *path;

    /* parse args */
    if (parse_args(argc, argv) != 0) {
        log_err("failed to parse args");
        exit(EXIT_ERROR);
    }

    /* do warp? */
    if (DO_WARP == 1) {
        debug("do warp");

        if(ARGC > 1) {
            log_warn("cannot warp to multiple points");
            exit(EXIT_ERROR);
        } else if(ARGC < 1) {
            log_warn("no warp point given");
            exit(EXIT_ERROR);
        }

        /* TODO: clean args, buffer overflow */
        name = ARGV[0];

        /* extract name from input */
        slash = -1;
        for (i = 0; i < strlen(name); ++i) {
            if (name[i] == '/') {
                slash = i;

                /* end name string */
                name[i] = '\0';
            }
        }

        /* find path to point */
        path = wd_path(name);

        /* print warp path */
        if (path == NULL) {
            log_warnf("no warp point named '%s'\n", ARGV[0]);
        } else {
            /* check if slash is found */
            if (slash > 0) { /* TODO: check for slash in input */
                printf("%s/%s", path, &(name[slash + 1]));
            } else {
                printf("%s", path);
            }

        }
    }

    /* clean */
    rc_free();
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
                    {"show",    no_argument,       0, 's'},
                    {"config",  required_argument, 0, 'c'},
                    {"remove",  required_argument, 0, 'r'},
                    {"add",     required_argument, 0, 'a'},
                    {"path",    required_argument, 0, 'p'},
                    {0, 0, 0, 0}
                };

            c = getopt_long(argc, argv, "hlsc:r:a:p:",
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
                    rc_set_file(optarg);
                    break;

                case 'a':
                    debug("do add");

                    /* TODO: clean args, env */
                    wd_add(optarg, getenv("PWD"));
                    DO_WARP = 0;
                    break;

                case 'r':
                    debug("do remove");

                    wd_remove(optarg);
                    DO_WARP = 0;
                    break;

                case 's':
                    debug("do show");

                    char *pwd = getenv("PWD");
                    char *name = wd_show(pwd);

                    if (name == NULL) {
                        log_warn("no warp point to current dir");
                    } else {
                        printf("\t%10s -> %10s\n", name, pwd);
                    }

                    DO_WARP = 0;
                    break;

                case 'p':
                    debug("do path");

                    /* TODO: clean arg */
                    char *path = wd_path(optarg);

                    if (path == NULL) {
                        log_warnf("no warp point named '%s'\n", optarg);
                    } else {
                        printf("%s\n", path);
                    }

                    DO_WARP = 0;
                    break;

                case 'l':
                    debug("do list");

                    wd_print_all();
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
           "-a | --add <point>\tAdds the current working directory to your warp points\n"
           /* "--add! <point>\tOverwrites existing warp point\n" */
           "-r | --remove <point>\tRemoves the given warp point\n"
           "-l | --list\t\tPrint all stored warp points\n"
           "-s | --show\t\tPrint warp points to current directory\n"
           "-p | --path <point>\tShow the path to given warp point\n"
           /* "--ls  <point>\tShow files from given warp point\n" */
           /* "--clean!\tRemove points warping to nonexistent directories\n" */
           "\n"
           "Other:\n"
           "-h, --help\t\tShow this extremely helpful text\n"
           "-v, --version\t\tPrint version\n"
           "-c, --config <file>\tSpecify config file (default ~/.warprc)\n"
           "-q, --quiet\t\tSuppress all output\n"
           "\n"
           "Licenced under MIT\n"
           "Report bugs to github.com/mfaerevaag/wd-c\n"
           );
}

void print_version()
{
    printf("wd (in c) version 0.1.2\n");
}
