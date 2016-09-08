#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "rc.h"
#include "logger.h"
#include "config.h"

void wp_warp(char *name);
void wp_add(char *name, char *dir);
void wp_remove(char *name);
void wp_show(char *dir);
void wp_path(char *dir);
void wp_list();

#endif
