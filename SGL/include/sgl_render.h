#ifndef SGL_RENDER_H
#define SGL_RENDER_H

#include <vector>
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <sgl_shader.h>

namespace sgl {

	class SGLRender {
	public:
		SGLRender(const int w, const int h)
			: viewportWidth(w), viewportHeight(h)
		{}

		~SGLRender() {}

		void init() {
			sglInitBuffers();
			sglInitVertexArrays();
			sglInitPrograms();
			sglInitUniforms();
			sglSetGLParams();
		}

		void finish() {
			sglDeleteBuffers();
			sglDeleteVertexArrays();
			sglDeletePrograms();
		}

		void render() {

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgramID);

			glm::mat4 mvpMat = getMVP();

			glUniformMatrix4fv(uniformIDs[0], 1, GL_FALSE, &mvpMat[0][0]);

			glBindVertexArray(vertexArrayID);
			glDrawArrays(GL_TRIANGLES, 0, triangle.size());

		}

	private:

		template< typename T, typename Alloc >
		static GLuint makeGLBuffer(GLenum const type, GLenum const usage, std::vector< T, Alloc > const& vec) {
			GLuint id;
			glGenBuffers(1, &id);
			glBindBuffer(type, id);
			auto const size = std::size(vec);
			glBufferData(type, sizeof(vec[0]) * size, size ? &vec[0] : nullptr, usage);
			return id;
		}

		glm::mat4 getMVP() {
			glm::mat4 mvpMat;

			float fov = 45.0f;
			float aspectRate = viewportWidth / viewportHeight;
			float znear = 0.001f;
			float zfar = 1000.0f;
			glm::vec3 cameraPos(4, 3, 3);
			glm::vec3 lookatPos(0, 0, 0);
			glm::vec3 cameraUp(0, 1, 0);

			glm::mat4 Projection = glm::perspective(glm::radians(fov), aspectRate, znear, zfar);
			glm::mat4 View = glm::lookAt(cameraPos, lookatPos, cameraUp);
			glm::mat4 Model = glm::mat4(1.0f);

			mvpMat = Projection * View * Model;

			return mvpMat;
		}

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

			std::string shaderFolderPath = "./shader/";
			std::string vertexExp = ".vert";
			std::string fragmentExp = ".frag";
			std::string shaderName = "simpleshader";
			std::string vertexFilePath = shaderFolderPath + shaderName + vertexExp;
			std::string fragmentFilePath = shaderFolderPath + shaderName + fragmentExp;

			shaderProgramID = sgl::LoadShaders(vertexFilePath, fragmentFilePath);

		}

		void sglInitUniforms() {

			GLuint mvpID = glGetUniformLocation(shaderProgramID, "mvp");
			uniformIDs.push_back(mvpID);

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
		const int viewportWidth;
		const int viewportHeight;
		GLuint vertexBufferID;
		GLuint shaderProgramID;
		GLuint vertexArrayID;
		std::vector<GLuint> uniformIDs;

		std::vector<GLfloat> triangle = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f,  1.0f, 0.0f
		};

	};

}
#endif // !SGL_RENDER_H
