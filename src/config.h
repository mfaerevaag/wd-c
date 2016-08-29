#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

#define NAME "wd"
#define VERSION "0.0.1"

#define EXIT_WARP 0
#define EXIT_ERROR 1
#define EXIT_INFO 2

extern int QUIET_FLAG;
extern char *RC_FILE;
extern int ARGC;
extern char **ARGV;

#endif
