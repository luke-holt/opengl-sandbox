#version 330 core

out vec4 frago_color;

in vec3 verto_color;
in vec2 verto_texcoord;

uniform sampler2D uni_texture;

void main()
{
	frago_color = vec4(verto_color, 1.0);
    frago_color = texture(uni_texture, -verto_texcoord) * frago_color;
}

