#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(1);
}

void *
emalloc(size_t size)
{
	void *p;
	if (!(p = malloc(size)))
		die("malloc:");
	return p;
}

void *
erealloc(void *ptr, size_t size)
{
	void *p;
	if (!(p = realloc(ptr, size)))
		die("realloc:");
	return p;
}

