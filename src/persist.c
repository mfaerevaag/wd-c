#include "persist.h"

static int WPOINTS_PARSED = 0;
static int WPOINTS_COUNT = 0;
static wpoint **WPOINTS = NULL;

static char *DELIM = ":";


int wp_parse();

wpoint **wp_all()
{
    if (WPOINTS_PARSED == 0) {
        debug("init points...");
        int rc = wp_parse();
        if (rc != 0) {
            printf("failed to parse warp points\n"); // TODO: logging
            abort(); // TODO: error
        }
    }

    return WPOINTS;
}

int wp_parse()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    debug("parsing points...");

    /* open file */
    fp = fopen(RC_FILE, "r");
    if (fp == NULL) {
        printf("could not open file '%s'\n", RC_FILE); // TODO: logging
        return(1); // TODO: error
    }

    /* count lines */
    while ((read = getline(&line, &len, fp)) != -1) {
        count++;
    }
    WPOINTS_COUNT = count;

    /* allocate */
    WPOINTS = malloc(count * sizeof(wpoint *));

    /* read points */
    rewind(fp);
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        debugf("found %s", line);

        char *token;
        wpoint *p = malloc(sizeof(wpoint));

        /* name token */
        token = strtok(line, DELIM); // token will point to name
        p->name = (char *) malloc(strlen(token));
        memcpy(p->name, token, read);

        /* dir token */
        token = strtok(NULL, DELIM); // token will point to dir
        p->dir = (char *) malloc(strlen(token));
        memcpy(p->dir, token, read);
        p->dir[strlen(p->dir) - 1] = 0; // remove newline char

        WPOINTS[i] = p;
        i++;
    }

    /* clean */
    fclose(fp);
    if (line) {
        free(line);
    }

    /* finish */
    WPOINTS_PARSED = 1;
    debugf("finished parsing %d points\n", count);

    return 0;
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
            index = i;
            break;
        }
    }

    if (index < 0) {
        log_errf("no warp point named '%s'\n", ARGV[0]);
        exit(EXIT_ERROR);
    }

    return index;
}

wpoint *wp_find(char *name)
{
    int index = wp_find_index(name);
    wpoint *wp = wp_all()[index];

    return wp;
}

void wp_add(char *name, char *dir)
{
    printf("TODO: add '%s' -> '%s'\n", name, dir);
}

void wp_remove(char* name)
{
    int index = wp_find_index(name);
    wpoint **wps = wp_all();

    debugf("removing '%s'\n", name);
    for (int i = index; i < WPOINTS_COUNT - 1; i++) {
        wps[i] = wps[i + 1];
    }
}

void wp_store()
{
    printf("TODO: store\n");
}

void wp_free()
{
    if (WPOINTS_PARSED == 1) {
        debug("freeing wpoints");

        free(WPOINTS);
        WPOINTS_COUNT = 0;
        WPOINTS_PARSED = 0;
    }
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
