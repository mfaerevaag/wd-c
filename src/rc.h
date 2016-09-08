#ifndef RC_H
#define RC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "logger.h"
#include "config.h"

char *rc_get_file();
void rc_set_file(char *file);

wtab *rc_tab();
void rc_parse();
void rc_store();
void rc_free();

int rc_find_index(char *name);
wpoint *rc_find(char *name);
void rc_add_point(char *name, char *dir);
void rc_remove_point(size_t index);

#endif
