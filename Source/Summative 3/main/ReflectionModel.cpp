#include "ReflectionModel.h"

ReflectionModel::ReflectionModel(GLuint program, Camera* _camera, Cubemap* skybox)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_DEPTH_TEST);

	camera = _camera;
	this->program = program;
	this->skybox = skybox;

	Utils::setSphereData(vertices, indices);
	//Utils::setCubeData(vertices, indices);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	//Attributes
	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);

	glEnableVertexAttribArray(1); //texcoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2); //normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ReflectionModel::~ReflectionModel()
{

}

void ReflectionModel::render()
{
	//glDepthMask(GL_FALSE);

	glUseProgram(program);

	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureID());
	
	//glm::mat4 proj = camera->getprojectionMatrix() * camera->getViewMatrix();

	GLint projLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));

	GLint projview = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(projview, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(10.0f, 40.0f, 0.0f));
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	//glDepthMask(GL_TRUE);
}