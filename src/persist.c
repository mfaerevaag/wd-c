#include "persist.h"

static int WPOINTS_COUNT = 0;
static wpoint **WPOINTS;
static int WPOINTS_PARSED = 0;
static int WPOINTS_CHANGED = 0;
static FILE *RC_FP;

static char *DELIM = ":";

wpoint **wp_all()
{
    if (WPOINTS_PARSED == 0) {
        debug("init points");
        wp_parse();
    }

    return WPOINTS;
}

void wp_parse()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    debug("parsing points...");

    /* open file */
    if (RC_FP == NULL) {
        RC_FP = fopen(get_rc_file(), "r");

        if (RC_FP == NULL) {
            printf("could not open file '%s'\n", get_rc_file()); // TODO: logging
            exit(EXIT_ERROR); // TODO: error
        }
    }

    /* count lines */
    while ((read = getline(&line, &len, RC_FP)) != -1) {
        count++;
    }
    WPOINTS_COUNT = count;

    /* allocate */
    WPOINTS = malloc(count * sizeof(wpoint *));

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

        WPOINTS[i] = p;
        i++;
    }

    /* clean */
    if (line) {
        free(line);
    }

    /* finish */
    WPOINTS_PARSED = 1;
    debugf("finished parsing %d points\n", count);
}

void wp_free()
{
    if (WPOINTS_PARSED == 1) {
        debug("freeing");

        if (WPOINTS_CHANGED == 1) {
            wp_store();
        }

        /* free pointers */
        wpoint **wps = wp_all();
        wpoint *wp;
        for (int i = 0; i < WPOINTS_COUNT; i++) {
            wp = wps[i];

            free(wp->name);
            free(wp->dir);
            free(wp);
        }
        free(WPOINTS);
        WPOINTS_COUNT = 0;

        /* close file handle */
        if (fclose(RC_FP)) {
            log_err("error closing config file");
            exit(EXIT_ERROR);
        }
        RC_FP = NULL;

        WPOINTS_CHANGED = 0;
        WPOINTS_PARSED = 0;
    }
}

void wp_store()
{
    wpoint **wps = wp_all();
    wpoint *wp;
    int rc;

    debug("storing points...");

    /* truncate and get write permission */
    RC_FP = freopen(NULL, "w+", RC_FP);

    /* check */
    if (RC_FP == NULL) {
        log_errf("error opening config '%s'\n", get_rc_file());
        exit(EXIT_ERROR);
    }

    /* write to file */
    for (int i = 0; i < WPOINTS_COUNT; i++)
    {
        wp = wps[i];
        /* debugf("writing %s:%s\n", wp->name, wp->dir); */

        rc = fprintf(RC_FP, "%s:%s\n", wp->name, wp->dir);
        if (rc < 0) {
            log_errf("failed to write to config '%s'\n", get_rc_file());
        }
    }

    debugf("finished storing %d points\n", WPOINTS_COUNT);
}

int wp_find_index(char *name)
{
    int index = -1;
    wpoint **wps = wp_all();

    wpoint *wp;
    for (int i = 0; i < WPOINTS_COUNT; i++)
    {
        wp = wps[i];

        if (strcmp(name, wp->name) == 0) {
            /* debugf("match at index %i\n", i); */
            index = i;
            break;
        }
    }

    return index;
}

wpoint *wp_find(char *name)
{
    int index = wp_find_index(name);

    if (index < 0) return NULL;

    wpoint *wp = wp_all()[index];

    return wp;
}

void wp_add(char *name, char *dir)
{
    int len;

    /* check if exists */
    if (wp_find(name) != NULL) {
        log_err("point already exists");
        exit(EXIT_ERROR);
    }

    debugf("adding '%s' -> '%s'\n", name, dir);

    WPOINTS_COUNT++;

    /* allocate mem */
    WPOINTS = realloc(WPOINTS, sizeof(wpoint *) * WPOINTS_COUNT);
    if (WPOINTS == NULL) {
        log_err("failed to reallocate memory for points");
        exit(EXIT_ERROR);
    }

    /* new point */
    WPOINTS[WPOINTS_COUNT - 1] = malloc(sizeof(wpoint));
    wpoint *new = WPOINTS[WPOINTS_COUNT - 1];

    /* set name */
    len = strlen(name);
    new->name = (char *) calloc(sizeof(char), len);
    memcpy(new->name, name, len);

    /* set dir */
    len = strlen(dir);
    new->dir = (char *) calloc(sizeof(char), len);
    memcpy(new->dir, dir, len);

    WPOINTS_CHANGED = 1;
}

void wp_remove(char* name)
{
    int index = wp_find_index(name);
    if (index < 0) {
        log_errf("no warp point named '%s'\n", name);
        exit(EXIT_ERROR);
    }

    wpoint **wps = wp_all();

    debugf("removing '%s'\n", name);

    /* check if last */
    if (index == WPOINTS_COUNT - 1) {
        wps[index] = NULL;
    } else {
        for (int i = index; i < WPOINTS_COUNT - 1; i++) {
            wps[i] = wps[i + 1];
        }
    }

    WPOINTS_COUNT--;
    WPOINTS_CHANGED = 1;
}

void wp_print_all()
{
    wpoint **wps = wp_all();
    wpoint *wp = wps[0];

    if (wp == NULL) {
        printf("no points\n");
        return;
    }

    for (int i = 0; i < WPOINTS_COUNT; i++)
    {
        printf("\t%10s -> %10s\n", wps[i]->name, wps[i]->dir);
    }
}
