#pragma once

#include "Utils.h"

//class Utils;
class Camera;
class Light;

class StencilMirror
{
public:
	StencilMirror();
	StencilMirror(GLuint _program, Camera* _camera, Light* _light);
	~StencilMirror();

	void setPosition(glm::vec3 _position);

	void render();

private:

	Camera* camera;
	//ModelType type;
	GLuint program;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	//GLfloat speed;
	Light* light;
	char* filename;

	std::vector<VertexFormat> quadVertices;
	std::vector<GLuint> quadIndices;

	std::vector<VertexFormat> cubeVertices;
	std::vector<GLuint> cubeIndices;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	glm::mat4 model;
	//GLfloat deltaTime;

	GLuint vaoCube;
	GLuint vboCube;
	GLuint eboCube;
	GLuint textureCube;
};


