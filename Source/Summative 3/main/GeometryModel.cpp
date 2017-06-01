#include "glm/gtc/type_ptr.hpp"

#include "GeometryModel.h"

GeometryModel::GeometryModel(GLuint program, Camera* camera)
{
	this->program = program;
	this->camera = camera;

	// passing ing 1 point
	GLfloat points[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, }; 

	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);

	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void GeometryModel::render()
{
	glUseProgram(this->program);

	glm::mat4 model;
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, position);

	glm::mat4 mvp = camera->getprojectionMatrix() * camera->getViewMatrix() * model;

	GLint vpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void GeometryModel::setPosition(glm::vec3 _position)
{
	position = _position;
}