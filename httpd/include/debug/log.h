#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define DEBUG
#ifdef DEBUG
#define exit_if(r, ...)                  \
    if (r) {                                        \
        printf(__VA_ARGS__);    \
        printf("%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); \
        exit(1);                                 \
    }
#endif
#endif