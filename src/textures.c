#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <math.h>
#include <stdio.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.h"
#include "util.h"


const char *vertex_shader_path = "./shaders/textures.vert";
const char *fragment_shader_path = "./shaders/textures.frag";
const char *texture_filename = "./textures/night-traffic.jpg";

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
		glfwTerminate();
		die("Failed to create window.\n");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		die("Failed to init GLAD.\n");
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowOpacity(window, 0.5);

	/**************************************************/
	/* Shader */

	unsigned int shader_program;
	shader_program = shader_build(vertex_shader_path, fragment_shader_path);

	/**************************************************/
	/* Data */

	float vertices[] = {
	/*   points (x,y,z)       colors (r,g,b)       texture coords (s,t) */
		 0.9f,  0.9f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.5f,
		 0.9f, -0.9f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.9f, -0.9f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.9f,  0.9f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  2.5f,
	};

	/* One row of the vertices array contains 8 floats */
	size_t stride = 8 * sizeof(float);
	size_t point_offset = 0;
	size_t color_offset = 3 * sizeof(float);
	size_t texture_coords_offset = 6 * sizeof(float);

	unsigned int indices[] = {
		0, 1, 2, /* first triangle */
		0, 2, 3, /* second triangle */
	};

	/**************************************************/
	/* VBO, VAO, EBO */

	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	/* Copy vertices array in buffer for OpenGL */
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Set vertex attributes pointers */
	/* position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)point_offset);
	glEnableVertexAttribArray(0);

	/* color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)color_offset);
	glEnableVertexAttribArray(1);

	/* texture attribute */
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)texture_coords_offset);
	glEnableVertexAttribArray(2);

	/**************************************************/
	/* Textures */

	/* Generate and configure texture */
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Load image */
	int width, height, nchannels;
	unsigned char *image_data;
	stbi_set_flip_vertically_on_load_thread(true);
	image_data = stbi_load(texture_filename, &width, &height, &nchannels, 0);
	if (NULL == image_data) {
		die("Failed to load image.\n");
	}

	/* Link image data to texture */
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image_data);

	/**************************************************/
	/* Render Loop */

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {
		process_input(window);

		// glClearColor((23.0f/255.0f), (16.0f/255.0f), (43.0f/255.0f), 0.1f);
		// glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, texture);

		/* activate shader */
		shader_use(shader_program);

		glfwSwapBuffers(window);
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

