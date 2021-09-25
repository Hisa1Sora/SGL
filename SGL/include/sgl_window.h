#ifndef SGL_WINDOW_H
#define SGL_WINDOW_H

#include <cstdio>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <sgl_render.h>	

namespace sgl {

	class SGLWindow {
	public:
		SGLWindow() {
			sglGLFWInit();
			sglGLEWInit();
		}

		SGLWindow(const int w, const int h, const char* n)
			: windowWidth(w), windowHeight(h), windowName(n)
		{
			sglGLFWInit();
			sglGLEWInit();
		}

		~SGLWindow() {

			glfwDestroyWindow(window);
			glfwTerminate();

		}

		void open() {

			auto ren = sgl::SGLRender(windowWidth, windowHeight);

			while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {

				glfwPollEvents();

				ren.render();

				glfwSwapBuffers(window);

			}

		}


	private:

		static void glfwErrorCallback(int error, const char* description) {
			fprintf(stderr, "Glfw Error %d: %s\n", error, description);
		}

		void sglGLFWInit() {

			glfwSetErrorCallback(glfwErrorCallback);

			if (!glfwInit()) {
				std::cerr << "Can't initilize GLFW." << std::endl;
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);

			window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);

			if (!window) {
				glfwTerminate();
				std::cerr << "Can't create GLFW window." << std::endl;
				return;
			}

			glfwMakeContextCurrent(window);
			glfwSwapInterval(glSwapIntervalNum);

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
		GLFWwindow* window;
		const int windowWidth = 720;
		const int windowHeight = 540;
		const char* windowName = "SGLWindow";
		const int glVersionMajor = 4;
		const int glVersionMinor = 6;
		const int glSwapIntervalNum = 1;
	};

}

#endif // !SGL_WINDOW_H
