#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 vertex_color;

uniform float time_value;

void main()
{
	float sinval = (sin(time_value) + 1.0) * 0.25;

	// xy to polar

	vec2 point = vec2(pos.xy);
	vec2 center = vec2(0.0, 0.0);

	float dist = sqrt(dot(point, point));
	float angle = atan(point.y / point.x); // radians

	if (point.y < 0 && point.x < 0) {
		angle += 3.141592;
	}

	angle += time_value;

	vec2 result = vec2(dist * cos(angle), dist * sin(angle));

    gl_Position = vec4(result.x, result.y, pos.z, 1.0);

    vertex_color = color;
}

