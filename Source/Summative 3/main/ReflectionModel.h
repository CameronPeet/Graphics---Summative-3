#pragma once

#include "glew/glew.h"
#include "freeglut/freeglut.h"
#include "soil/SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include <iostream>
#include "ShaderLoader.h"
#include "Utils.h"

#include "Camera.h"
#include "Cubemap.h"


class ReflectionModel
{
public:
	ReflectionModel(GLuint program, Camera* _camera, Cubemap* skybox);
	~ReflectionModel();

	Camera* camera;
	Cubemap* skybox;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	std::vector<VertexFormat> vertices;
	std::vector<GLuint> indices;

	void render();
};