#ifndef SGL_WINDOW_H
#define SGL_WINDOW_H

#include <cstdio>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <sgl_render.h>	
#include <sgl_constant.h>

namespace sgl {

	class SGLWindow {
	public:
		SGLWindow(const int w, const int h, const char* n)
			: windowWidth(w), windowHeight(h), windowName(n), window(nullptr)
		{}

		~SGLWindow() {}

		void open() {
			sglGLFWInit();
		}

		void close() {
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		void draw() {

			sglGLEWInit();

			auto ren = sgl::SGLRender(windowWidth, windowHeight);

			while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

				glfwPollEvents();

				ren.render();

				glfwSwapBuffers(window);

			}

		}


	private:

		static void glfwErrorCallback(int error, const char* description) {
			fprintf(stderr, "GLFW Error %d: %s\n", error, description);
		}

		void sglGLFWInit() {

			glfwSetErrorCallback(glfwErrorCallback);

			if (!glfwInit()) {
				std::cerr << "Can't initilize GLFW." << std::endl;
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, SGL_VERSION_MAJOR);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, SGL_VERSION_MINOR);

			window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);

			if (!window) {
				glfwTerminate();
				std::cerr << "Can't create GLFW window." << std::endl;
				return;
			}

			glfwMakeContextCurrent(window);
			glfwSwapInterval(SGL_SWAPINTERVAL_NUM);

		}

		void sglGLEWInit() {

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) {
				glfwTerminate();
				std::cerr << "Can't initilize GLEW." << std::endl;
				return;
			}

		}

	private:
		const int windowWidth;
		const int windowHeight;
		const char* windowName;
		GLFWwindow* window;
	};

}

#endif // !SGL_WINDOW_H
