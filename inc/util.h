#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

void die(const char *fmt, ...);
void *emalloc(size_t size);
void *erealloc(void *ptr, size_t size);

#endif
