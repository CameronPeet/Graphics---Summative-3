
#include "GameModel.h"
#include "Camera.h"


GameModel::GameModel(ModelType modelType, Camera* _camera, std::string texFileName, Light * _light, float _ambientStrength, float _specularStrength) {

	camera = _camera;
	light = _light;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;
	speed = 0.05f;
	rotationAxis = glm::bvec3(0.0f, 0.0f, 1.0f);

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);//if enabled, disables writing to depth
	//glDepthFunc(GL_LESS);//passes if the fragment depth value is less than stored depth value


	switch (modelType) {

	case kTriangle: Utils::setTriData(vertices, indices); break;
	case kQuad: Utils::setQuadData(vertices, indices); break;
	case kCube: Utils::setCubeData(vertices, indices); break;
	case kSphere: Utils::setSphereData(vertices, indices); break;
	}

	for (auto item : vertices) {

		//printf("position: %f, %f, %f \n", item.pos.x, item.pos.y, item.pos.z);
	}

	for (auto item : indices) {

		//printf("index: %d, %d, %d \n", item.x, item.y, item.z);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	this->setTexture(texFileName);

	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

GameModel::~GameModel() {

}

void GameModel::update(GLfloat time) {

	//model = glm::rotate(model,glm::radians(45.0f) * time, glm::vec3(0.0, 1.0, 0.0f));		

	//model = glm::translate(model, position);
}


void GameModel::render() {

	glUseProgram(this->program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	glm::mat4 model;
	model = glm::translate(model, position);

	model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	model = glm::scale(model, scale);

	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = camera->getViewMatrix();
	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = camera->getprojectionMatrix();
	GLint projLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	// lighting calculations

	GLint objColorLoc = glGetUniformLocation(program, "objectColor");
	glUniform3f(objColorLoc, color.x, color.y, color.z);

	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	// Spotlight
	glm::vec3 spotlightPos = camera->getCameraPosition();

	// offset the flash light down a little
	spotlightPos.y -= 0.5f;

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);

	GLuint lightDirLoc = glGetUniformLocation(program, "lightDirection");
	glUniform3f(lightDirLoc, this->light->getDirection().x, this->light->getDirection().y, this->light->getDirection().z);

	GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);

	GLuint constantLoc = glGetUniformLocation(program, "constant");
	glUniform1f(constantLoc, 1.0f);

	GLuint linearLoc = glGetUniformLocation(program, "linear");
	glUniform1f(linearLoc, 0.07f);

	GLuint exponentLoc = glGetUniformLocation(program, "exponent");
	glUniform1f(exponentLoc, 0.017f);

	// Spot light
	//lightingShader.setUniform("spotLight.position", spotlightPos);
	//lightingShader.setUniform("lightDirection", camera->getLook());

	GLuint cosInnerConeLoc = glGetUniformLocation(program, "cosInnerCone");
	glUniform1f(cosInnerConeLoc, glm::cos(glm::radians(15.0f)));

	GLuint cosOuterConeLoc = glGetUniformLocation(program, "cosOuterCone");
	glUniform1f(cosOuterConeLoc, glm::cos(glm::radians(20.0f)));

	GLint loc = glGetUniformLocation(program, "on");
	//glUniform1i(loc, gFlashlightOn);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameModel::setAngle(glm::vec3 _angle)
{
	angle = _angle;
}

//movement
void GameModel::moveForward() {

	position -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void GameModel::moveBackward() {

	position += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void GameModel::moveLeft() {

	position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}

void GameModel::moveRight() {

	position += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}

void GameModel::moveUp() {

	position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void GameModel::moveDown() {

	position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void GameModel::rotate(glm::vec3 axis) {

	this->angle.x += axis.x * speed * 20;
	this->angle.y += axis.y * speed * 20;
	this->angle.z += axis.z * speed * 20;
}

void GameModel::setTexture(std::string  texFileName) {

	//** load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texFileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	printf("fileName %s \n", texFileName.c_str());

	//bIsTextureSet = true;
}

// setters and getters

void GameModel::setScale(glm::vec3 _scale) {


	this->scale = _scale;
}

void GameModel::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void GameModel::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void GameModel::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void GameModel::setColor(glm::vec3 _color) {

	this->color = _color;
}

void GameModel::setCamera(Camera * camera) {

}

void GameModel::setSpeed(float _speed) {

	this->speed = _speed;

}

void GameModel::setProgram(GLuint program) {

	this->program = program;
}



void GameModel::setSpecularStrength(float strength) {

	specularStrength = strength;
}

glm::vec3 GameModel::getPosition() {

	return position;
}

glm::vec3 GameModel::getColor() {

	return this->color;
}

glm::vec3 GameModel::getScale() {

	return this->scale;

}

glm::vec3 GameModel::getRotation() {

	return this->angle;
}

glm::vec3 GameModel::getRotationAxis() {

	return rotationAxis;
}