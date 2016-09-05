#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <string.h>

#include "logger.h"

#define NAME "wd"
#define VERSION "0.0.1"

#define EXIT_WARP 0
#define EXIT_ERROR 1
#define EXIT_INFO 2

extern int QUIET_FLAG;
extern int ARGC;
extern char **ARGV;

char *get_rc_file();
void set_rc_file(char *file);

#endif
