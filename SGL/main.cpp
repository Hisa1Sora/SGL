#include <cstdio>
#include <vector>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

template< typename T, typename Alloc >
GLuint makeGLBuffer(GLenum const type, GLenum const usage, std::vector< T, Alloc > const& vec) {
	GLuint id;
	glGenBuffers(1, &id);
	glBindBuffer(type, id);
	auto const size = std::size(vec);
	glBufferData(type, sizeof(vec[0]) * size, size ? &vec[0] : nullptr, usage);
	return id;
}

void glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int main() {

	int width = 1280;
	int height = 720;
	const char* name = "SGL";

	GLFWwindow* window;

	// glfw setting
	{
		glfwSetErrorCallback(glfwErrorCallback);

		if (!glfwInit()) {
			std::cerr << "Can't initilize GLFW." << std::endl;
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		window = glfwCreateWindow(width, height, name, NULL, NULL);

		if (!window) {
			glfwTerminate();
			std::cerr << "Can't create GLFW window." << std::endl;
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
	}


	// glew setting
	{
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			std::cerr << "Can't initilize GLEW." << std::endl;
			return -1;
		}
	}

	GLuint vertexBufferID;
	GLuint vertexArrayID;
	GLuint shaderProgramID;
	GLuint mvpID;
	glm::mat4 mvpMat;

	return 0;

}