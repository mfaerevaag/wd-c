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
    if (RC_PARSED == 0) {
        debug("init points");
        rc_parse();
    }

    return TAB;;
}

int rc_find_index(char *name)
{
    int index = -1;
    wtab *tab = rc_tab();

    for (size_t i = 0; i < tab->size; i++)
    {
        if (strcmp(name, tab->points[i]->name) == 0) {
            /* debugf("match at index %i\n", i); */
            index = i;
            break;
        }
    }

    return index;
}

wpoint *rc_find(char *name)
{
    int index = rc_find_index(name);

    if (index < 0) return NULL;

    return rc_tab()->points[index];
}

void rc_parse()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

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
        char *token;
        int len;
        wpoint *p = malloc(sizeof(wpoint));

        /* debugf("found [%zu] %s", read, line); */

        /* name token */
        token = strtok(line, DELIM); // token will point to name
        len = strlen(token);

        /* debugf("name token: [%i] %s\n", len, token); */
        p->name = (char *) calloc(sizeof(char), len);
        memcpy(p->name, token, len);

        /* dir token */
        token = strtok(NULL, DELIM); // token will point to dir
        len = strlen(token);

        p->dir = (char *) calloc(sizeof(char), len);
        memcpy(p->dir, token, len);
        p->dir[len - 1] = 0; // remove newline char

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
    if (RC_PARSED == 1) {
        debug("freeing");

        if (RC_CHANGED == 1) {
            rc_store();
        }

        /* free pointers */
        wpoint *wp;
        for (int i = 0; i < (int) TAB->size; i++) {
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
        RC_FP = NULL;

        RC_CHANGED = 0;
        RC_PARSED = 0;
    }
}

void rc_store()
{
    wpoint *wp;
    int rc;

    debug("storing points...");

    /* truncate and get write permission */
    RC_FP = freopen(NULL, "w+", RC_FP);

    /* check */
    if (RC_FP == NULL) {
        log_errf("error opening config '%s'\n", rc_get_file());
        exit(EXIT_ERROR);
    }

    /* write to file */
    for (int i = 0; i < (int) TAB->size; i++)
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
    /* already there */
    if (RC_FILE != NULL) {
        return RC_FILE;
    }

    char *home = getenv("HOME");

    RC_FILE = malloc(strlen(home) + strlen(RC_FILE_NAME) + 1);
    sprintf(RC_FILE, "%s/%s", home, RC_FILE_NAME);

    return RC_FILE;
}

void rc_set_file(char *file)
{
    int len = strlen(file);
    if (len > 248) {
        log_err("config file path too long");
        exit(EXIT_ERROR);
    } else if (len <= 0) {
        log_err("missing config file path");
        exit(EXIT_ERROR);
    }

    RC_FILE = strdup(file);
}

void rc_add_point(char *name, char *dir)
{
    int len;
    wtab *tab = rc_tab();

    tab->size++;
    /* already space for one more point in tab->points, see rc_parse */

    /* new point */
    tab->points[tab->size - 1] = malloc(sizeof(wpoint));
    wpoint *new = tab->points[tab->size - 1];

    /* set name */
    len = strlen(name);
    new->name = (char *) calloc(sizeof(char), len);
    memcpy(new->name, name, len);

    /* set dir */
    len = strlen(dir);
    new->dir = (char *) calloc(sizeof(char), len);
    memcpy(new->dir, dir, len);

    RC_CHANGED = 1;
}

void rc_remove_point(size_t index)
{
    wtab *tab = rc_tab();

    /* check if last */
    if (index == tab->size - 1) {
        tab->points[index] = NULL;
    } else {
        for (size_t i = index; i < tab->size - 1; i++) {
            tab->points[i] = tab->points[i + 1];
        }
    }

    tab->size--;
    RC_CHANGED = 1;
}
