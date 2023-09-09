#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <math.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.h"
#include "util.h"

#include <cglm/cglm.h>

const char *vertex_shader_path = "./shaders/cube.vert";
const char *fragment_shader_path = "./shaders/cube.frag";

void framebuffer_size_callback(GLFWwindow *w, int width, int height);
void process_input(GLFWwindow *w);

int
main(void)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *w = glfwCreateWindow(800, 600, "Hello, world!", NULL, NULL);
	if (NULL == w) {
		glfwTerminate();
		die("Failed to create window.\n");
	}

	glfwMakeContextCurrent(w);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		die("Failed to init GLAD.\n");
	}

	glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);
	glfwSetWindowOpacity(w, 0.5);

	/**************************************************/
	/* Shader */

	unsigned int shader_program;
	shader_program = shader_build(vertex_shader_path, fragment_shader_path);

	/**************************************************/
	/* Vertices */

	vec3 cube_vertices[8] = {
		{0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{0.0f, 1.0f, 1.0f},
	};

	ivec3 indices[12] = {
		{0, 1, 2},
		{1, 5, 6},
		{4, 5, 7},
		{0, 3, 4},

		{0, 2, 3},
		{1, 2, 6},
		{5, 6, 7},
		{3, 4, 7},

		{2, 6, 7},
		{2, 3, 7},
		{0, 4, 5},
		{0, 1, 5},
	};

	/**************************************************/
	/* GL items */

	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	/* VBO will contain the cube vertices */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	/* EBO will contain the combinations of vertices used to draw cube */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* in VAO, configure data layout in VBO */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(w)) {
		process_input(w);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time;
		time = glfwGetTime();
		glUniform1f(glGetUniformLocation(shader_program, "time"), time);

		glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);

		shader_use(shader_program);

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void
framebuffer_size_callback(GLFWwindow *w, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
process_input(GLFWwindow *w)
{
	if (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(w, 1);
	}
}

