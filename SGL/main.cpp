#include <cstdio>
#include <vector>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <shader.h>

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
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			std::cerr << "Can't initilize GLEW." << std::endl;
			return -1;
		}
	}

	GLuint vertexBufferID;
	GLuint shaderProgramID;
	GLuint mvpID;
	GLuint vertexArrayID;
	glm::mat4 mvpMat;

	std::vector<GLfloat> triangle = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f
	};

	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
	}

	{
		vertexBufferID = makeGLBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, triangle);
		shaderProgramID = LoadShaders("./shader/simpleshader.vert", "./shader/simpleshader.frag");
		mvpID = glGetUniformLocation(shaderProgramID, "mvp");

		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgramID);


		float fov = 45.0f;
		float aspectRate = width / height;
		float znear = 0.001f;
		float zfar = 1000.0f;
		glm::vec3 cameraPos(4, 3, 3);
		glm::vec3 lookatPos(0, 0, 0);
		glm::vec3 cameraUp(0, 1, 0);

		glm::mat4 Projection = glm::perspective(glm::radians(fov), aspectRate, znear, zfar);
		glm::mat4 View = glm::lookAt(
			cameraPos,
			lookatPos,
			cameraUp
		);
		glm::mat4 Model = glm::mat4(1.0f);
		mvpMat = Projection * View * Model;
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpMat[0][0]);

		glBindVertexArray(vertexArrayID);
		glDrawArrays(GL_TRIANGLES, 0, triangle.size());

		glfwSwapBuffers(window);

	}

	{
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteVertexArrays(1, &vertexArrayID);
		glDeleteProgram(shaderProgramID);
	}

	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	return 0;

}