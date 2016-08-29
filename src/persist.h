#ifndef PERSIST_H
#define PERSIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "config.h"

struct wpoint_t
{
    char *name;
    char *dir;
};
typedef struct wpoint_t wpoint;

void wp_store();
void wp_free();

void wp_print_all();
wpoint **wp_all();

wpoint *wp_find(char *name);
void wp_add(char *name, char *dir);
void wp_remove(char *name);

#endif
