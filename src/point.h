#ifndef POINT_H
#define POINT_H

#include <stdlib.h>

struct wpoint_t
{
    char *name;
    char *dir;
};
typedef struct wpoint_t wpoint;

struct wtab_t
{
    size_t size;
    wpoint **points;
};
typedef struct wtab_t wtab;

#endif
