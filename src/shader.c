#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

static char *file_read_text(const char *path);

char *
file_read_text(const char *path)
{
	FILE *file;
	char *text;
	size_t size;

	/* TODO files larger than chunk size */
	const size_t CHUNK_SIZE = 1024;

	file = fopen(path, "r");
	if (NULL == file) {
		die("Unable to open %s", path);
	}

	text = emalloc(CHUNK_SIZE);
	size = fread(text, 1, CHUNK_SIZE, file);
	fclose(file);

	text = erealloc(text, size + 1);
	text[size] = '\0';

	return text;
}

unsigned int
shader_build(const char *vpath, const char *fpath)
{
	unsigned int vid, fid, id;
	int success;
	char info_log[512], *vtext, *ftext;

	vtext = file_read_text(vpath);
	ftext = file_read_text(fpath);

	vid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vid, 1, &vtext, NULL);
	glCompileShader(vid);
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vid, 512, NULL, info_log);
		die(info_log);
	}

	fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fid, 1, &ftext, NULL);
	glCompileShader(fid);
	glGetShaderiv(fid, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fid, 512, NULL, info_log);
		die(info_log);
	}

	id = glCreateProgram();
	glAttachShader(id, vid);
	glAttachShader(id, fid);
	glLinkProgram(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, info_log);
		die(info_log);
	}

	glDeleteShader(vid);
	glDeleteShader(fid);
	free(vtext);
	free(ftext);

	return id;
}

void
shader_use(int id)
{
	glUseProgram(id);
}

void
shader_set_bool(int id, const char *name, bool value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void
shader_set_int(int id, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void
shader_set_float(int id, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

