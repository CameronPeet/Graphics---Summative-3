#pragma once

#include "Camera.h"

class GeometryModel
{
public:
	GeometryModel() {}
	GeometryModel::GeometryModel(GLuint program, Camera* camera);
	~GeometryModel() {}

	void render();
	void setPosition(glm::vec3 _position);

	GLuint program;
	Camera* camera;

	GLuint vao;
	GLuint vbo;

	glm::vec3 position;

};
