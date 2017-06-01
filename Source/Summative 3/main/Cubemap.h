#pragma once

#include "glew/glew.h"
#include "freeglut/freeglut.h"
#include "soil/SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Utils.h"
#include <vector>

#include "Camera.h"


class Cubemap
{
public:
	Cubemap(GLuint program, Camera* camera);
	~Cubemap();

	void render();

	Camera* camera;

	std::vector<VertexFormat> vertices;
	std::vector<GLuint> indices;

	GLuint loadCubemap(std::vector<const GLchar*> faces);

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	GLuint getTextureID();
};