#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <stdbool.h>
#include <stddef.h>

unsigned int shader_build(const char *vpath, const char *fpath);
void shader_use(int id);
void shader_set_bool(int id, const char *name, bool value);
void shader_set_int(int id, const char *name, int value);
void shader_set_float(int id, const char *name, float value);

#endif
