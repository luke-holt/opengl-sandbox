#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texcoord;

out vec3 verto_color;
out vec2 verto_texcoord;

void main()
{
    gl_Position = vec4(in_pos, 1.0);
	verto_color = in_color;
	verto_texcoord = in_texcoord;
}

