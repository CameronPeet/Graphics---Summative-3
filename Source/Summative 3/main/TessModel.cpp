#include "TessModel.h"

#include "Camera.h"

TessModel::TessModel(GLuint program, Camera* camera)
{
	this->program = program;
	this->camera = camera;

	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,		
	};

	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}

TessModel::~TessModel()
{
}

void TessModel::render()
{
	glUseProgram(this->program);

	glm::mat4 model;
	model = glm::translate(model, position);
	glm::mat4 mvp = camera->getprojectionMatrix() * camera->getViewMatrix() * model;

	GLint mvLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}

void TessModel::setPosition(glm::vec3 _position)
{
	position = _position;
}