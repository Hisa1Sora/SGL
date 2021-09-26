#ifndef SGL_RENDER_H
#define SGL_RENDER_H

#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <sgl_shader.h>
#include <sgl_matrix.h>

namespace sgl {

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

		template< typename T, typename Alloc >
		static GLuint makeGLBuffer(GLenum const type, GLenum const usage, std::vector< T, Alloc > const& vec) {
			GLuint id;
			glGenBuffers(1, &id);
			glBindBuffer(type, id);
			auto const size = std::size(vec);
			glBufferData(type, sizeof(vec[0]) * size, size ? &vec[0] : nullptr, usage);
			return id;
		}

		void render() {

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgramID);

			mvpMat = getMVP();

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

			shaderProgramID = sgl::LoadShaders("./shader/simpleshader.vert", "./shader/simpleshader.frag");

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
