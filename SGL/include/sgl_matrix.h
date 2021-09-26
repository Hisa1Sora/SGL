#ifndef SGL_MATRIX_H
#define SGL_MATRIX_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace sgl {

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

}

#endif // !SGL_MATRIX_H
