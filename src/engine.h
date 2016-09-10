#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "rc.h"
#include "logger.h"
#include "config.h"

void wd_add(char *name, char *dir);
void wd_remove(char *name);
char *wd_show(char *dir);
char *wd_path(char *dir);
void wd_print_all();

#endif
