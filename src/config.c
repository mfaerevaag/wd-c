#include "config.h"

int QUIET_FLAG = 0;
int ARGC = 0;
char **ARGV = NULL;

char *RC_FILE_SUFFIX = "/.warprc";
char *RC_FILE = NULL;

char *get_rc_file()
{
    /* already there */
    if (RC_FILE != NULL) {
        return RC_FILE;
    }

    char *home = getenv("HOME");
    char *rc = ".warprc";

    RC_FILE = malloc(strlen(home) + strlen(rc) + 1);
    sprintf(RC_FILE, "%s/%s", home, rc);

    return RC_FILE;
}

void set_rc_file(char *file)
{
    int len = strlen(file);
    if (len > 248) {
        log_err("config file path too long");
        exit(EXIT_ERROR);
    } else if (len <= 0) {
        log_err("missing config file path");
        exit(EXIT_ERROR);
    }

    RC_FILE = strdup(file);
}
