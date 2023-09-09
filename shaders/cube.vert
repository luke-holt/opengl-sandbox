#version 330 core

layout (location = 0) in vec3 pos;

uniform float time;

void main()
{
	float pi = 3.14159265359;
	float a = pi / 10 * time;
	float b = pi / 10 * time;
	float c = pi / 10 * time;

	mat3 yaw = mat3(cos(a), -sin(a), 0,
				  	sin(a), cos(a), 0,
				  	0, 0, 1);

	mat3 pitch = mat3(cos(b), 0, sin(b),
				  	  0, 1, 0,
				  	  -sin(b), 0, cos(b));

	mat3 roll = mat3(1, 0, 0,
					 0, cos(c), -sin(c),
			 		 0, sin(c), cos(c));
	
	/* rotate and scale */
	vec3 res = yaw * pitch * roll * (pos - 0.5);

    gl_Position = vec4(res, 4);
}

