#include "rc.h"

static wtab *TAB;

static FILE *RC_FP;
static int RC_PARSED = 0;
static int RC_CHANGED = 0;

static char *DELIM = ":";


char *RC_FILE_NAME = ".warprc";
char *RC_FILE = NULL;

wtab *rc_tab()
{
    /* lazy init tab */
    if (RC_PARSED == 0) {
        debug("init points");
        rc_parse();
    }

    return TAB;
}

int rc_find_index(char *name)
{
    int i;
    wtab *tab = rc_tab();

    /* find point */
    for (i = 0; i < tab->size; i++) {
        if (strcmp(name, tab->points[i]->name) == 0) {
            return i;
        }
    }

    return -1;
}

wpoint *rc_find(char *name)
{
    int index = rc_find_index(name);

    /* check not found */
    if (index < 0) {
        return NULL;
    }

    return rc_tab()->points[index];
}

void rc_parse()
{
    char *line = NULL, *tok;
    size_t len = 0;
    ssize_t read;
    int count, tok_len;
    wpoint *p;

    debug("parsing points...");

    /* open file */
    if (RC_FP == NULL) {
        RC_FP = fopen(rc_get_file(), "r");

        if (RC_FP == NULL) {
            printf("could not open file '%s'\n", rc_get_file()); // TODO: logging
            exit(EXIT_ERROR); // TODO: error
        }
    }

    /* count lines */
    count = 0;
    while ((read = getline(&line, &len, RC_FP)) != -1) {
        count++;
    }

    /* allocate for number of lines (points) PLUS ONE */
    TAB = malloc(sizeof(wtab));
    TAB->points = malloc((count + 1) * sizeof(wpoint *));
    TAB->size = count;

    /* read points */
    rewind(RC_FP);
    int i = 0;
    while ((read = getline(&line, &len, RC_FP)) != -1) {
        p = malloc(sizeof(wpoint));

        /* debugf("found [%zu] %s", read, line); */

        /* name token */
        tok = strtok(line, DELIM); // token will point to name
        tok_len = strlen(tok);

        /* debugf("name tok: [%i] %s\n", len, tok); */
        p->name = (char *) calloc(sizeof(char), tok_len);
        memcpy(p->name, tok, tok_len);

        /* dir token */
        tok = strtok(NULL, DELIM); // token will point to dir
        tok_len = strlen(tok);

        p->dir = (char *) calloc(sizeof(char), tok_len);
        memcpy(p->dir, tok, tok_len);
        p->dir[tok_len - 1] = 0; // remove newline char

        /* test corrupt */
        tok = strtok(NULL, DELIM); // should be NULL
        if (tok != NULL) {
            log_errf("rc file corrupt ('%s')\n", RC_FILE);
            rc_free();
            exit(EXIT_ERROR);
        }

        TAB->points[i] = p;
        i++;
    }

    /* clean */
    if (line) {
        free(line);
    }

    /* finish */
    RC_PARSED = 1;
    debugf("finished parsing %d points\n", count);
}

void rc_free()
{
    int i;
    wpoint *wp;

    /* dont free if not alloced */
    if (RC_PARSED != 1) {
        return;
    }

    debug("freeing");

    /* store unsaved changes */
    if (RC_CHANGED == 1) {
        rc_store();
    }

    /* free pointers */
    for (i = 0; i < TAB->size; i++) {
        wp = TAB->points[i];

        free(wp->name);
        free(wp->dir);
        free(wp);
    }
    free(TAB->points);
    free(TAB);

    /* close file handle */
    if (fclose(RC_FP)) {
        log_err("error closing config file");
        exit(EXIT_ERROR);
    }

    /* reset state */
    RC_FP = NULL;
    RC_CHANGED = 0;
    RC_PARSED = 0;
}

void rc_store()
{
    int i, rc;
    wpoint *wp;

    debug("storing points...");

    /* truncate and get write permission */
    RC_FP = freopen(rc_get_file(), "w+", RC_FP);

    /* check */
    if (RC_FP == NULL) {
        log_errf("error opening config '%s'\n", rc_get_file());
        exit(EXIT_ERROR);
    }

    /* write to file */
    for (i = 0; i < TAB->size; i++)
    {
        wp = TAB->points[i];
        /* debugf("writing %s:%s\n", wp->name, wp->dir); */

        rc = fprintf(RC_FP, "%s:%s\n", wp->name, wp->dir);
        if (rc < 0) {
            log_errf("failed to write to config '%s'\n", rc_get_file());
        }
    }

    debugf("finished storing %zu points\n", TAB->size);
}

char *rc_get_file()
{
    char *home;

    /* already there */
    if (RC_FILE != NULL) {
        return RC_FILE;
    }

    home = getenv("HOME");

    RC_FILE = malloc(strlen(home) + strlen(RC_FILE_NAME) + 1);
    sprintf(RC_FILE, "%s/%s", home, RC_FILE_NAME);

    return RC_FILE;
}

void rc_set_file(char *file)
{
    int len;

    len = strlen(file);

    if (len > 248) {
        log_err("config file path too long");
        exit(EXIT_ERROR);
    } else if (len <= 0) {
        log_err("missing config file path");
        exit(EXIT_ERROR);
    }

    RC_FILE = strdup(file);
}

int rc_add_point(char *name, char *dir)
{
    int len;
    wpoint *new;
    wtab *tab = rc_tab();

    /* check for currupt */
    if (strpbrk(name, ":") != 0 || strpbrk(dir, ":") != 0) {
        log_err("cannot add point containing colon (':')");
        return 1;
    }

    tab->size++;
    /* already space for one more point in tab->points, see rc_parse */

    /* new point */
    tab->points[tab->size - 1] = malloc(sizeof(wpoint));
    new = tab->points[tab->size - 1];

    /* set name */
    len = strlen(name);
    new->name = (char *) calloc(sizeof(char), len);
    memcpy(new->name, name, len);

    /* set dir */
    len = strlen(dir);
    new->dir = (char *) calloc(sizeof(char), len);
    memcpy(new->dir, dir, len);

    RC_CHANGED = 1;

    return 0;
}

int rc_remove_point(size_t index)
{
    int i;
    wtab *tab = rc_tab();

    /* check if last */
    if (index == tab->size - 1) {
        tab->points[index] = NULL;
    } else {
        for (i = index; i < tab->size - 1; i++) {
            tab->points[i] = tab->points[i + 1];
        }
    }

    tab->size--;
    RC_CHANGED = 1;

    return 0;
}
