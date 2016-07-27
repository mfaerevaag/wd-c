#include <stdio.h>
#include <stdlib.h>

char *RC_FILE = "/home/markus/.warprc";

int read_rc()
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    /* open file */
    fp = fopen(RC_FILE, "r");
    if (fp == NULL)
        return(1);

    /* read file */
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%2d[%2zu]: %s", count, read, line);
        count++;
    }

    /* free */
    fclose(fp);
    if (line)
        free(line);

    return 0;
}

int main()
{
    printf("hello, world!\n");

    return 0;
}
