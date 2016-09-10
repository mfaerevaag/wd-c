#ifndef ENGINE_H
#define ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "rc.h"
#include "logger.h"
#include "config.h"

void wd_warp(char *name);
void wd_add(char *name, char *dir);
void wd_remove(char *name);
void wd_show(char *dir);
void wd_path(char *dir);
void wd_list();

#endif
