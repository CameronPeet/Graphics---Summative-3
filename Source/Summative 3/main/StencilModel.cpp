#include "StencilModel.h"

#include "Camera.h"

#include "soil/SOIL.h"
#include "glm/gtc/type_ptr.hpp"

StencilModel::StencilModel()
{

}

StencilModel::~StencilModel()
{

}

StencilModel::StencilModel(GLuint _textureProgram, GLuint _stencilProgram, Camera* _camera)
{
	camera = _camera;
	textureProgram = _textureProgram;
	stencilProgram = _stencilProgram;

	Utils::setCubeData(vertices, indices);

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2); //normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height;
	unsigned char* image = SOIL_load_image("Assets/Images/Werewolf.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texutre wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void StencilModel::render()
{
	//glDepthMask(GL_FALSE);

	// Clear the screen to black
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(textureProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(textureProgram, "Texture"), 0);


	//GLint projLoc = glGetUniformLocation(textureProgram, "projection");
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

	//GLint projview = glGetUniformLocation(textureProgram, "view");
	//glUniformMatrix4fv(projview, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	//glm::mat4 model;
	//GLint modelLoc = glGetUniformLocation(textureProgram, "model");
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//GLint cameraPosLoc = glGetUniformLocation(textureProgram, "cameraPos");
	//glUniform3f(cameraPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(textureProgram, "model");
	model = glm::translate(glm::mat4(), glm::vec3(-4.0f, 40.0f, -2.9f));
	//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set up projection
	glm::mat4 view = glm::lookAt(
		glm::vec3(3.5f, 3.5f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	view = camera->getViewMatrix();
	GLint viewLoc = glGetUniformLocation(textureProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
	projection = camera->getprojectionMatrix();
	GLint projLoc = glGetUniformLocation(textureProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLint Color = glGetUniformLocation(textureProgram, "overrideColor");
	glUniform3f(Color, 1.0f, 1.0f, 1.0f);

	glBindVertexArray(vao);
	// Draw cube
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	//glDepthMask(GL_TRUE);
}

void StencilModel::setPosition(glm::vec3 _position)
{
	position = _position;
}

