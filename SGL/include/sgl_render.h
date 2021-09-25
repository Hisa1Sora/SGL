#ifndef SGL_RENDER_H
#define SGL_RENDER_H

#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <shader.h>

namespace sgl {

	template< typename T, typename Alloc >
	GLuint makeGLBuffer(GLenum const type, GLenum const usage, std::vector< T, Alloc > const& vec) {
		GLuint id;
		glGenBuffers(1, &id);
		glBindBuffer(type, id);
		auto const size = std::size(vec);
		glBufferData(type, sizeof(vec[0]) * size, size ? &vec[0] : nullptr, usage);
		return id;
	}


	class SGLRender {
	public:
		SGLRender(const int w, const int h)
			: viewportWidth(w), viewportHeight(h)
		{

			sglInitBuffers();
			sglInitVertexArrays();
			sglInitPrograms();
			sglInitUniforms();
			sglSetGLParams();

		}

		~SGLRender() {
			sglDeleteBuffers();
			sglDeleteVertexArrays();
			sglDeletePrograms();
		}

		void render() {

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgramID);


			float fov = 45.0f;
			float aspectRate = viewportWidth / viewportHeight;
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

		}

	private:
		void sglInitBuffers() {
			vertexBufferID = makeGLBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, triangle);
		}

		void sglInitVertexArrays() {

			glGenVertexArrays(1, &vertexArrayID);
			glBindVertexArray(vertexArrayID);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindVertexArray(0);

		}

		void sglInitPrograms() {

			shaderProgramID = LoadShaders("./shader/simpleshader.vert", "./shader/simpleshader.frag");

		}

		void sglInitUniforms() {

			mvpID = glGetUniformLocation(shaderProgramID, "mvp");

		}

		void sglSetGLParams() {

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);

		}

		void sglDeleteBuffers() {
			glDeleteBuffers(1, &vertexBufferID);
		}

		void sglDeleteVertexArrays() {
			glDeleteVertexArrays(1, &vertexArrayID);
		}

		void sglDeletePrograms() {
			glDeleteProgram(shaderProgramID);
		}

	private:
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

		const int viewportWidth;
		const int viewportHeight;
	};


}
#endif // !SGL_RENDER_H
