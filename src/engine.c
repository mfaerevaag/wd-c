#include "engine.h"

void wd_warp(char *name)
{
    wpoint *p = rc_find(name);

    if (p == NULL) {
        log_warnf("no warp point named '%s'\n", name);
        exit(EXIT_ERROR);
    }

    debugf("warping to '%s'\n", name);

    printf("%s", p->dir);
}

void wd_add(char *name, char *dir)
{
    /* check if exists */
    if (rc_find(name) != NULL) {
        log_warn("point already exists");
        return;
    }

    debugf("adding '%s' -> '%s'\n", name, dir);

    rc_add_point(name, dir);
}

void wd_remove(char* name)
{
    int index = rc_find_index(name);
    if (index < 0) {
        log_warnf("no warp point named '%s'\n", name);
        return;
    }

    debugf("removing '%s'\n", name);

    rc_remove_point(index);
}

void wd_show(char *dir)
{
    wtab *tab = rc_tab();
    for (size_t i = 0; i < tab->size; i++) {
        if (strcmp(tab->points[i]->dir, dir) == 0) {
            printf("\t%10s -> %10s\n", tab->points[i]->name, tab->points[i]->dir);
        }
    }
}

void wd_path(char *dir)
{
    wpoint *wp = rc_find(dir);

    if (wp == NULL) {
        log_warnf("no warp point named '%s'\n", dir);
        exit(EXIT_ERROR);
    }

    printf("%s\n", wp->dir);
}

void wd_list()
{
    wtab *tab = rc_tab();

    if (tab->size == 0) {
        printf("no points\n");
        return;
    }

    printf("total %zu\n", tab->size);
    for (size_t i = 0; i < tab->size; i++)
    {
        printf("\t%10s -> %10s\n", tab->points[i]->name, tab->points[i]->dir);
    }
}
