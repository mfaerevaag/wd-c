#include "engine.h"

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

char *wd_show(char *dir)
{
    wtab *tab = rc_tab();
    for (size_t i = 0; i < tab->size; i++) {
        if (strcmp(tab->points[i]->dir, dir) == 0) {
            return tab->points[i]->name;
        }
    }

    return NULL;
}

char *wd_path(char *dir)
{
    wpoint *wp = rc_find(dir);

    if (wp == NULL) {
        return NULL;
    }

    return wp->dir;
}

void wd_print_all()
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
