#include <math.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.h"

const char *vertex_shader_path = "/home/txm/src/opengl/shaders/shader.vert";
const char *fragment_shader_path = "/home/txm/src/opengl/shaders/shader.frag";

void framebuffer_size_callback(GLFWwindow *w, int width, int height);
void process_input(GLFWwindow *w);

int
main(void)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Hello, world!", NULL, NULL);
	if (NULL == window) {
		printf("Failed to create window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to init GLAD.\n");
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/**************************************************/
	/* Data */

	float vertices[] = {
	/*   points 			colors */
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, /* first triangle */
		// 1, 2, 3, /* second triangle */
	};

	/**************************************************/
	/* Shaders */

	unsigned int shader_program;
	shader_program = shader_build(vertex_shader_path, fragment_shader_path);
	shader_use(shader_program);

	/**************************************************/
	/* Vertex Buffer Object */

	/* Vertex Buffer Object. Generate buffer and bind it to the VBO variable */
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	/**************************************************/
	/* Vertex Array Object */

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	/* Bind array */
	glBindVertexArray(VAO);
	/* Copy vertices array in buffer for OpenGL */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Summary:
	 *   - Create all objects we want to draw (VAOs + VBO and attribute pointers)
	 *   - To draw an object:
	 *     - take VAO
	 *     - bind it
	 *     - draw it
	 *     - unbind it
	 */

	/**************************************************/
	/* Element Buffer Object */

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Binding a EBO to a VAO automatically binds that EBO.
	 * The last EBO that is bound to a VAO is set as it's EBO.
	 */

	/* NOTE: A VAO stores the glBindBuffer calls when the target it GL_ELEMENT_ARRAY_BUFFER.
	 * This means it also stores the unbind calls.
	 * Don't unbind the EBO before unbinding the VAO.
	 * Otherwise, the VAO won't have an EBO configured.
	 */

	/**************************************************/
	/* Shader tutorial page */

	int max_vertex_attributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attributes);
	printf("Max vertex attributes: %i\n", max_vertex_attributes);

	/* in GLSL (shader language), access datatype elements with swizzling.
	 * vec2 a_v2;
	 * vec4 a_v4 = a_v2.xyxy;
	 * vec3 a_v3 = a_v4.xyy + a_v4.yxy;
	 * Strange feature... i'll take it lol
	 */

#if 0 /* moved to render loop */
	float time_value, green_value;
	int vertex_color_location;
	time_value = glfwGetTime();
	green_value = sin(time_value) / 2.0f + 0.5f;
	vertex_color_location = glGetUniformLocation(shader_program, "our_color");
	glUseProgram(shader_program);
	glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);
#endif

	/**************************************************/
	/* Render Loop */

	/* Set vertex attributes pointers */
	/* position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	/* color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor((23.0f/255.0f), (16.0f/255.0f), (43.0f/255.0f), 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* activate shader */
		shader_use(shader_program);

		/* update uniform color */
		float time_value;
		int vertex_time_location;
		time_value = glfwGetTime();
		vertex_time_location = glGetUniformLocation(shader_program, "time_value");
		glUniform1f(vertex_time_location, time_value);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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

