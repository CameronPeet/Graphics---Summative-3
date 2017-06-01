#pragma once

#include "Utils.h"

//class Utils;
class Camera;
class Light;

class StencilModel
{
public:
	StencilModel();
	StencilModel(GLuint _textureProgram, GLuint _stencilProgram, Camera* _camera);
	~StencilModel();

	void setPosition(glm::vec3 _position);

	void render();

private:

	Camera* camera;
	//ModelType type;
	GLuint textureProgram;
	GLuint stencilProgram;
	//glm::vec3 color;
	glm::vec3 position;
	//glm::vec3 scale;
	//glm::vec3 rotation;
	//GLfloat speed;
	//Light* light;
	//char* filename;

	std::vector<VertexFormat> vertices;
	std::vector<GLuint> indices;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	glm::mat4 model;
	//GLfloat deltaTime;
};


