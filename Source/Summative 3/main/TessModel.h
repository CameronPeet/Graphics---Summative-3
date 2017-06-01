#pragma once

#include "glew/glew.h"

#include "glm/gtc/type_ptr.hpp"

class Camera;

class TessModel
{
public:
	TessModel(GLuint program, Camera* camera);
	~TessModel();

	void render();
	void setPosition(glm::vec3 _position);

private:

	GLuint vao;
	GLuint vbo;
	GLuint program;
	Camera* camera;
	glm::vec3 position;
};


