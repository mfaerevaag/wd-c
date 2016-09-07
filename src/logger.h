/* Zed's Awesome Debug Macros */
/* http://c.learncodethehardway.org/book/ex20.html */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef DEBUG
#define debugf(M, ...) fprintf(stderr, "debug: %s:%s:%d: " M, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define debug(M) fprintf(stderr, "debug: %s:%s:%d: " M "\n", __FILE__, __func__, __LINE__)
#define todof(M, ...) fprintf(stderr, "todo: %s:%d: " M, __FILE__, __LINE__, ##__VA_ARGS__)
#define todo(M) fprintf(stderr, "todo: %s:%d: " M "\n", __FILE__, __LINE__)
#else
#define debugf(M, ...)
#define debug(M)
#define todof(M, ...)
#define todo(M)
#endif

#define clean_errno() (errno == 0 ? "none" : strerror(errno))

#define log_errf(M, ...) fprintf(stderr, "error: " M, ##__VA_ARGS__)

#define log_err(M) fprintf(stderr, "error: " M "\n")

#define log_warnf(M, ...) fprintf(stderr, "warning: " M, ##__VA_ARGS__)

#define log_warn(M) fprintf(stderr, "warning: " M "\n")

#define log_infof(M, ...) fprintf(stderr, "info: " M, ##__VA_ARGS__)

#define log_info(M) fprintf(stderr, "info: " M "\n")

#define checkf(A, M, ...) if(!(A)) { log_errf(M, ##__VA_ARGS__); errno=0; goto error; }

#define check(A, M) if(!(A)) { log_err(M); errno=0; goto error; }

#define sentinelf(M, ...)  { log_errf(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M)  { log_err(M); errno=0; goto error; }

#endif
