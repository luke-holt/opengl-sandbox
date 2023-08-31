#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

const char *vertex_shader_source = "#version 330 core \n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragment_shader_source = "#version 330 core \n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

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
		/* first triangle */
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, /* first triangle */
		1, 2, 3, /* second triangle */
	};

	/**************************************************/
	/* Shaders */

	int success;
	char info_log[512];

	/* Vertex shader. Create shader to get handle ref number. Load source. Compile shader. */
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

 	/* Verify compilation success */
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glad_glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
		printf("%s\n", info_log);
	}

	/* fragment shader. Creation, loading and compilation are the same. */
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

 	/* Verify compilation success */
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glad_glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
		printf("%s\n", info_log);
	}

	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
	}

	glUseProgram(shader_program);

	/* NOTE once we've linked the shaders into the program object,
	 *     make sure to delete them
	 */
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

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
	/* Render Loop */

	/* Set vertex attributes pointers */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor((23.0f/255.0f), (16.0f/255.0f), (43.0f/255.0f), 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

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






