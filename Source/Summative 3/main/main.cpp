//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// File Name	: main.cpp
// Description	: main file
// Author		: Charmaine Lim 
// Mail			: charmaine.lim6440@mediadesign.school.nz
//


#include "glew/glew.h"
#include "freeglut/freeglut.h"
#include "soil/SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "fmod/fmod.hpp"

#include <iostream>

#include "ShaderLoader.h"
#include "Camera.h"
#include "TextLabel.h"
#include "Light.h"
#include "GameModel.h"
#include "Cubemap.h"
#include "ReflectionModel.h"
#include "StencilModel.h"
#include "StencilMirror.h"
#include "Model.h"
#include "Terrain.h"
#include "GeometryModel.h"
#include "TessModel.h"
#include "FrameBuffer.h"

float currentTime;
float prevTime;
float deltaTime;

unsigned char keyState[255];

#define BUTTON_UP		0
#define BUTTON_DOWN		1

//int mouseX = Utils::WIDTH / 2;
//int mouseY = Utils::HEIGHT / 2;

#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT 2
unsigned char mouseState[3];
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
/*GLfloat*/ int lastX = Utils::WIDTH / 2.0;
/*GLfloat*/ int lastY = Utils::HEIGHT / 2.0;
//bool firstMouse = true;
glm::vec3 rayDirection;
int mouseX;
int mouseY;

TextLabel* label;

FMOD::System* audioMgr;
FMOD::Sound* hitSound;
FMOD::Sound* bgMusic;

Camera* camera;
Light* light;
GameModel* triangle;
GameModel* ground;
GameModel* cube;
GameModel* sphere;
GameModel* quad;
Cubemap* skybox;
ReflectionModel* reflectModel;
StencilModel* stencilModel;
StencilMirror* stencilMirror;
Model* my3DModel;
Terrain* terrain;
GeometryModel* geomModel;
TessModel* tessModel;
FrameBuffer *frameBuffer;

bool bMultipleLights = false;
bool firstMouse = true;
int gFlashlightOn = 1;
Model* barrel;
Model* bowling;
Model* rabbit;
Model* crate;
Model* lampPost;
Model* robot;
Model* woodcrate;
Light* pointlight;

GLuint DirectionalLightMeshProgram;
GLuint PointLightProgram;
GLuint SpotLightProgram;
GLuint MultipleLightsProgram;

bool bWireFrame = false;
int iCull = 0;
bool wireButtonDown = false;
bool cullButtonDown = false;
bool bGreyScale = false;

bool InitFmod()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&audioMgr);
	if (result != FMOD_OK) return false;

	result = audioMgr->init(50, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) return false;

	return true;
}

const bool LoadAudio()
{
	FMOD_RESULT result;

	//FMOD_DEFAULT - FMOD_LOOP_OFF, FMOD_2D and FMOD_HARDWARE
	result = audioMgr->createSound("Assets/audio/pop.mp3", FMOD_DEFAULT, 0, &hitSound);
	result = audioMgr->createSound("Assets/audio/theme.mp3", FMOD_DEFAULT, 0, &bgMusic);

	bgMusic->setMode(FMOD_LOOP_NORMAL);

	return true;
}

void init() 
{
	//InitFmod();
	//LoadAudio();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE for wireframe mode
	glEnable(GL_DEPTH_TEST); // enalbe the depth testing
	glDepthFunc(GL_LESS);

	//enable multisampling
	//glutSetOption(GLUT_MULTISAMPLE, 8);
	//glEnable(GL_MULTISAMPLE);

	//glStencilFunc(GL_NOTEQUAL, //func
	// 1, //c-ref - value is compared with the value in stencil buffer
	// 0xFF), //mask - each bit is writted to stencil buffer

	////**how data is modified when a fragment passes or fails stencil test
	//glStencilOp(GL_KEEP, //fail - fragment fails stencil test
	// GL_KEEP, //zfail - stencil pass an depth fails
	// GL_KEEP); //zpass - depth passes or no depth test is performed

	//FMOD::Channel* channel;
	//audioMgr->playSound(bgMusic, 0, false, &channel);

	ShaderLoader shader;
	GLuint flatShaderProgram = shader.CreateProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	//GLuint litTexturedShaderProgram = shader.CreateProgram("Assets/Shaders/LitTextureModel.vs", "Assets/Shaders/LitTextureModel.fs");
	GLuint litTexturedShaderProgram = shader.CreateProgram("Assets/Shaders/LitTexturedModel.vs", "Assets/Shaders/LitTexturedModel.fs");
	GLuint DirectionalLightProgram = shader.CreateProgram("Assets/Shaders/DirectionalLight.vs", "Assets/Shaders/DirectionalLight.fs");
	DirectionalLightMeshProgram = shader.CreateProgram("Assets/Shaders/DirectionalLightMesh.vs", "Assets/Shaders/DirectionalLightMesh.fs");
	PointLightProgram = shader.CreateProgram("Assets/Shaders/PointLight.vs", "Assets/Shaders/PointLight.fs");
	SpotLightProgram = shader.CreateProgram("Assets/Shaders/SpotLight.vs", "Assets/Shaders/SpotLight.fs");
	MultipleLightsProgram = shader.CreateProgram("Assets/Shaders/MultipleLights.vs", "Assets/Shaders/MultipleLights.fs");

	//GLuint texturedShaderProgram = shader.CreateProgram("Assets/shaders/TexturedModel.vs", "Assets/shaders/TexturedModel.fs");
	GLuint cubemapProgram = shader.CreateProgram("Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.fs");
	GLuint reflectionProgram = shader.CreateProgram("Assets/Shaders/ReflectedModel.vs", "Assets/Shaders/ReflectedModel.fs");

	camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 10000.0f);
	//camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 100.0f);
	camera->setCameraSpeed(15.0f);

	light = new Light(ModelType::kSphere, camera);
	//light = new Light(ModelType::kCube, camera);
	light->setProgram(flatShaderProgram);
	light->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	//light->setPosition(glm::vec3(0.0f, 100.0f, 0.0f));
	light->setPosition(glm::vec3(0.0f, 40.0f, -22.0f));
	//light->setScale(glm::vec3(20.0f, 20.0f, 20.0f));
	light->setSpeed(10.0f);

	triangle = new GameModel(ModelType::kTriangle, camera, "Assets/Images/wall.jpg", light, 0.1f, 0.5f);
	triangle->setProgram(litTexturedShaderProgram);
	//triangle->setPosition(glm::vec3(2.0f, -2.0f, 0.0f));
	triangle->setPosition(glm::vec3(2.0f, 38.0f, -24.0f));
	triangle->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	triangle->setScale(glm::vec3(3.0f, 3.0f, 3.0f));

	ground = new GameModel(ModelType::kQuad, camera, "Assets/images/container.jpg", light, 0.1f, 0.5f);
	ground->setProgram(litTexturedShaderProgram);
	ground->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//ground->setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	ground->setPosition(glm::vec3(-5.0f, 40.0f, -25.0f));
	ground->setScale(glm::vec3(2.0f, 2.0f, 1.0f));

	cube = new GameModel(ModelType::kCube, camera, "Assets/images/Rayman.jpg", light, 0.1f, 0.5f);
	cube->setProgram(litTexturedShaderProgram);
	cube->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//cube->setPosition(glm::vec3(-2.0f, 2.0f, 0.0f));
	cube->setPosition(glm::vec3(10.0f, 40.0f, -26.0f));
	//cube->setRotation(0.0f);
	cube->setScale(glm::vec3(2.0f, 2.0f, 1.0f));

	sphere = new GameModel(ModelType::kSphere, camera, "Assets/images/earth3.jpg", light, 0.1f, 0.5f);
	sphere->setProgram(DirectionalLightProgram);
	sphere->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//sphere->setPosition(glm::vec3(-2.0f, -2.0f, 0.0f));
	sphere->setPosition(glm::vec3(-20.0f, 38.0f, 20.0f));
	sphere->setScale(glm::vec3(2.0f, 2.0f, 2.0f));

	quad = new GameModel(ModelType::kQuad, camera, "Assets/images/container.jpg", light, 0.1f, 0.5f);
	quad->setProgram(litTexturedShaderProgram);
	quad->setColor(glm::vec3(0.0f, 0.0f, 0.0f));
	quad->setPosition(glm::vec3(-4.0f, 40.0f, -24.1f));
	quad->setScale(glm::vec3(1.5f, 1.5f, 0.0f));

	//////text label
	////label = new TextLabel("Basic Game", "Assets/fonts/waltographUI.ttf");
	////label->setPosition(glm::vec2(720.0f, 650.0f));
	////label->setColor(glm::vec3(1.0f, 1.0f, 0.0f));

	//cubemap
	skybox = new Cubemap(cubemapProgram, camera);

	////reflection
	//reflectModel = new ReflectionModel(reflectionProgram, camera, skybox);

	////geom shader
	//GLuint geomProgram = shader.CreateProgram("Assets/shaders/geomModel.vs",
	//											"Assets/shaders/geomModel.fs",
	//											"Assets/shaders/geomModel.gs");

	//geomModel = new GeometryModel(geomProgram, camera);
	//geomModel->setPosition(glm::vec3(-5.0f, -60.0f, -40.0f));

	////tesselation
	//GLuint tessProgram = shader.CreateProgram("Assets/shaders/tessTriModel.vs",
	//	"Assets/shaders/tessTriModel.fs",
	//	"Assets/shaders/tessQuadModel.tcs",
	//	"Assets/shaders/tessQuadModel.tes");

	//tessModel = new TessModel(tessProgram, camera);
	//tessModel->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));

	//stencil
	//GLuint twotextureprogram = shader.CreateProgram("Assets/shaders/2_Texture_QUAD.vs",
	//	"Assets/shaders/2_Texture_QUAD.fs");

	//GLuint twoStencilProgram = shader.CreateProgram("Assets/shaders/2_Texture_QUAD_stencil.vs",
	//	"Assets/shaders/2_Texture_QUAD_stencil.fs");

	//stencilModel = new StencilModel(twotextureprogram, twoStencilProgram, camera);
	//stencilModel->setPosition(glm::vec3(10.0f, 40.0f, 0.0f));

	////stencil Mirror
	////stencilMirror = new StencilMirror(litTexturedShaderProgram, camera, light);
	//stencilMirror = new StencilMirror(twotextureprogram, camera, light);

	//framebuffer
	GLuint framebufferProgram = shader.CreateProgram("Assets/shaders/framebuffer.vs",
		"Assets/shaders/framebuffer.fs");

	frameBuffer = new FrameBuffer(framebufferProgram);

	////mesh import
	//GLuint modelProgram = shader.CreateProgram("Assets/Shaders/mesh.vs", "Assets/Shaders/mesh.fs");
	////my3DModel = new Model("Assets/Models/castle/Castle OBJ.obj", camera, modelProgram);

	//pointlight = new Light(ModelType::kSphere, camera);
	//pointlight->setProgram(flatShaderProgram);
	//pointlight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//pointlight->setPosition(glm::vec3(2.0f, 43.8f, 14.0f));
	//
	//lampPost = new Model("Assets/Models/lampPost.obj", camera, PointLightProgram, pointlight);

	//robot = new Model("Assets/Models/robot.obj", camera, PointLightProgram, pointlight);
	//crate = new Model("Assets/Models/crate.obj", camera, PointLightProgram, pointlight);

	//barrel = new Model("Assets/Models/barrel.obj", camera, SpotLightProgram, pointlight);
	//bowling = new Model("Assets/Models/bowling_pin.obj", camera, SpotLightProgram, pointlight);
	//rabbit = new Model("Assets/Models/bunny.obj", camera, SpotLightProgram, pointlight);

	//woodcrate = new Model("Assets/Models/woodcrate.obj", camera, DirectionalLightMeshProgram, pointlight);

	//float xShift = 20.0f;
	//float zShift = -10.0f;

	//barrel->setPosition(glm::vec3(9.0f, 39.0f, -6.0f));
	//rabbit->setPosition(glm::vec3(9.0f, 39.0f, -1.0f));
	////glm::vec3(0.7f, 0.7f, 0.7f),	// bunny
	////bowling->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
	//bowling->setPosition(glm::vec3(20.0f, 37.0f, 2.0f));
	//
	//lampPost->setPosition(glm::vec3(2.0f, 40.0f, 14.0f));
	//robot->setScale(glm::vec3(1.0f, 1.0f, -1.0f));
	//robot->setPosition(glm::vec3(0.0f, 39.0f, 15.0f));
	//crate->setPosition(glm::vec3(4.0f, 40.0f, 15.0f));

	//woodcrate->setPosition(glm::vec3(-15.0f, 39.0f, 10.0f));


	////terrain
	//GLuint terrainProgram = shader.CreateProgram("Assets/shaders/heightmap.vs", "Assets/shaders/heightmap.fs");
	//terrain = new Terrain(L"Assets/heightmap/grandcanyon2.raw",
	//	"Assets/heightmap/sand.jpg",
	//	"Assets/heightmap/grass.jpg",	
	//	"Assets/heightmap/rock.jpg",
	//	terrainProgram,
	//	camera,
	//	light);

	////blending
	//GLuint blendProgram = shader.CreateProgram("Assets/shaders/blending.vs", "Assets/shaders/blending.fs");
	//blendModelFence = new BlendModel(ModelType:kQuad, blendProgram, camera, "Assets/images/WireFence.png");
	//blendModelFence->setPosition(glm::vec3(0.0f, 8.0f, 8.0f));

	//blendModelWater = new BlendModel(ModelType:kQuad, blendProgram, camera, "Assets/images/water.png");
	//blendModelWater->setPosition(glm::vec3(6.4f, 7.5f, 10.45f));
	//blendModelWater->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
	//blendModelWater->setScale(glm::vec3(1.3f, 1.3f, 1.3f));

	GLuint toonShaderProgram = shader.CreateProgram("Assets/shaders/toon.vs", "Assets/shaders/toon.fs");
	bowling = new Model("Assets/Models/bowling_pin.obj", camera, toonShaderProgram, pointlight);

}

void updateControls()
{
	//camera controls
	if (keyState[(unsigned char) 'w'] == BUTTON_DOWN) {
		//camera->moveForward();
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}if (keyState[(unsigned char) 's'] == BUTTON_DOWN) {
		//camera->moveBack();
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}if (keyState[(unsigned char) 'a'] == BUTTON_DOWN) {
		//camera->moveLeft();
		camera->ProcessKeyboard(LEFT, deltaTime);
	}if (keyState[(unsigned char) 'd'] == BUTTON_DOWN) {
		//camera->moveRight();
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}if (keyState[(unsigned char) 'q'] == BUTTON_DOWN) {
		//camera->moveUp();
		camera->ProcessKeyboard(UPWARD, deltaTime);
	}if (keyState[(unsigned char) 'e'] == BUTTON_DOWN) {
		//camera->moveDown();
		camera->ProcessKeyboard(DOWNWARD, deltaTime);
	}

	//light controls
	if (keyState[(unsigned char) 'i'] == BUTTON_DOWN) {
		light->moveForward();
	}if (keyState[(unsigned char) 'k'] == BUTTON_DOWN) {
		light->moveBackward();
	}if (keyState[(unsigned char) 'j'] == BUTTON_DOWN) {
		light->moveLeft();
	}if (keyState[(unsigned char) 'l'] == BUTTON_DOWN) {
		light->moveRight();
	}if (keyState[(unsigned char) 'u'] == BUTTON_DOWN) {
		light->moveUp();
	}if (keyState[(unsigned char) 'o'] == BUTTON_DOWN) {
		light->moveDown();
	}

	//move cube
	if (keyState[(unsigned char) 't'] == BUTTON_DOWN) {
		//cube->moveForward();
		lampPost->moveBackward();
		pointlight->setPosition(lampPost->getPosition() + glm::vec3(0.0f, 3.8f, 0.0f));
	}if (keyState[(unsigned char) 'g'] == BUTTON_DOWN) {
		//cube->moveBackward();
		lampPost->moveForward();
		pointlight->setPosition(lampPost->getPosition() + glm::vec3(0.0f, 3.8f, 0.0f));
	}if (keyState[(unsigned char) 'f'] == BUTTON_DOWN) {
		//cube->moveLeft();
		lampPost->moveRight();
		pointlight->setPosition(lampPost->getPosition() + glm::vec3(0.0f, 3.8f, 0.0f));
	}if (keyState[(unsigned char) 'h'] == BUTTON_DOWN) {
		//cube->moveRight();
		lampPost->moveLeft();
		pointlight->setPosition(lampPost->getPosition() + glm::vec3(0.0f, 3.8f, 0.0f));
	}if (keyState[(unsigned char) 'r'] == BUTTON_DOWN) {
		//cube->moveUp();
		lampPost->moveUp();
		pointlight->setPosition(lampPost->getPosition() + glm::vec3(0.0f, 3.8f, 0.0f));
	}if (keyState[(unsigned char) 'y'] == BUTTON_DOWN) {
		//cube->moveDown();
		lampPost->moveDown();
		pointlight->setPosition(lampPost->getPosition() + glm::vec3(0.0f, 3.8f, 0.0f));
	}

	if (keyState[(unsigned char) 'z'] == BUTTON_DOWN) {
		cube->rotate(glm::vec3(1.0f, 0.0f, 0.0f));
	}if (keyState[(unsigned char) 'x'] == BUTTON_DOWN) {
		cube->rotate(glm::vec3(-1.0f, 0.0f, 0.0f));
	}if (keyState[(unsigned char) 'n'] == BUTTON_DOWN) {
		cube->rotate(glm::vec3(0.0f, 0.0f, 1.0f));
	}if (keyState[(unsigned char) 'm'] == BUTTON_DOWN) {
		cube->rotate(glm::vec3(0.0f, 0.0f, -1.0f));
	}

	if (keyState[(unsigned char) 'c'] == BUTTON_DOWN)
	{
		if (!cullButtonDown)
		{
			cullButtonDown = true;
			iCull++;
			if (iCull == 3)
			{
				iCull = 0;
			}
		}
	}
	if (keyState[(unsigned char) 'c'] == BUTTON_UP)
	{
		cullButtonDown = false;

	}

	if (keyState[(unsigned char) 'v'] == BUTTON_DOWN)
	{
		if (!wireButtonDown)
		{
			wireButtonDown = true;
			bWireFrame = !bWireFrame;
		}
	}
	
	if (keyState[(unsigned char) 'v'] == BUTTON_UP)
	{
		wireButtonDown = false;
	}
	//reset
	if (keyState[(unsigned char) 'b'] == BUTTON_DOWN) {
		light->setPosition(glm::vec3(0.0f, -4.0f, -2.0f));
		cube->setPosition(glm::vec3(10.0f, -4.0f, -6.0f));
		cube->setAngle(glm::vec3(0.0f, 0.0f, 0.0f));
		camera->Reset();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		bWireFrame = false;
		iCull = 0;
	}

	if (keyState[(unsigned char) '1'] == BUTTON_DOWN) {

		gFlashlightOn = 1;

		barrel->SetFlashlightOn(gFlashlightOn);
		bowling->SetFlashlightOn(gFlashlightOn);
		rabbit->SetFlashlightOn(gFlashlightOn);
		crate->SetFlashlightOn(gFlashlightOn);
		lampPost->SetFlashlightOn(gFlashlightOn);
		robot->SetFlashlightOn(gFlashlightOn);
		woodcrate->SetFlashlightOn(gFlashlightOn);
	}
	if (keyState[(unsigned char) '2'] == BUTTON_DOWN) {

		gFlashlightOn = 0;

		barrel->SetFlashlightOn(gFlashlightOn);
		bowling->SetFlashlightOn(gFlashlightOn);
		rabbit->SetFlashlightOn(gFlashlightOn);
		crate->SetFlashlightOn(gFlashlightOn);
		lampPost->SetFlashlightOn(gFlashlightOn);
		robot->SetFlashlightOn(gFlashlightOn);
		woodcrate->SetFlashlightOn(gFlashlightOn);
	}

	if (keyState[(unsigned char) '5'] == BUTTON_DOWN) {
		light->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keyState[(unsigned char) '5'] == BUTTON_UP) {
		if (!bMultipleLights)
		{
			light->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		}		
	}

	if (keyState[(unsigned char) '3'] == BUTTON_DOWN) {
		bMultipleLights = true;

		//light->setColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//bMultipleLights = !bMultipleLights;
		//if (bMultipleLights)
		//{
			pointlight->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
			barrel->SetProgram(MultipleLightsProgram);
			bowling->SetProgram(MultipleLightsProgram);
			rabbit->SetProgram(MultipleLightsProgram);
			crate->SetProgram(MultipleLightsProgram);
			lampPost->SetProgram(MultipleLightsProgram);
			robot->SetProgram(MultipleLightsProgram);
			woodcrate->SetProgram(MultipleLightsProgram);
	}
	if (keyState[(unsigned char) '4'] == BUTTON_DOWN) {
		bMultipleLights = false;
		//light->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//{
			pointlight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
			barrel->SetProgram(SpotLightProgram);
			bowling->SetProgram(SpotLightProgram);
			rabbit->SetProgram(SpotLightProgram);
			crate->SetProgram(PointLightProgram);
			lampPost->SetProgram(PointLightProgram);
			robot->SetProgram(PointLightProgram);
			woodcrate->SetProgram(DirectionalLightMeshProgram);
		//}
	}

	if (keyState[(unsigned char) '6'] == BUTTON_DOWN) {
		bGreyScale = true;
	}
}

bool checkCollision(GameModel* obj1, GameModel* obj2)
{
	bool collisionX = obj1->getPosition().x + obj1->getScale().x >= obj2->getPosition().x &&
		obj2->getPosition().x + obj2->getScale().x >= obj1->getPosition().x;
	bool collisionY = obj1->getPosition().y + obj1->getScale().y >= obj2->getPosition().y &&
		obj2->getPosition().y + obj2->getScale().y >= obj1->getPosition().y;
	bool collisionZ = obj1->getPosition().y + obj1->getScale().y >= obj2->getPosition().y &&
		obj2->getPosition().y + obj2->getScale().y >= obj1->getPosition().y;

	if (collisionX && collisionY && collisionZ)
	{
		printf("Colliding \n");
		return true;
	}
	else {
		//printf("Not - Colliding \n");
		return false;
	}
}

void updateCollisions()
{
	if (checkCollision(cube, sphere))
	{
		cube->setPosition(glm::vec3(-1.0f, 1.0f, 0.0f));

		//play sound
		FMOD::Channel* channel;
		audioMgr->playSound(hitSound, 0, false, &channel);
	}
}

bool UpdateMousePicking()
{
	//http://antongerdelan.net/opengl/raycasting.html

	// Camera needs to be stationary for picking to work!!

	//glm::vec4 normalizedScreenPos = glm::vec4(mouseX, mouseY, 1.0f, 1.0f);
	//glm::mat4 invViewProjMatrix = glm::inverse(camera->getViewMatrix() * camera->getProjectionMatrix());
	//glm::vec4 rayWorld = invViewProjMatrix * normalizedScreenPos;
	//rayDirection = glm::normalize(glm::vec3(rayWorld));

	//screen pos
	glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);

	//screenpos to clip coords
	//screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);

	//clip space to eye space
	//Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(camera->getprojectionMatrix());
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 invViewMat = glm::inverse(camera->getViewMatrix());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	rayDirection = glm::normalize(glm::vec3(rayWorld));

	//add code to check
	//intersection with other objects
	//checks intersection of a ray with a sphere radius of 1.0f
	float radius = 1.0f;
	glm::vec3 v = sphere->getPosition() - camera->getCameraPosition();

	float a = glm::dot(rayDirection, rayDirection);
	float b = 2 * glm::dot(v, rayDirection);
	float c = glm::dot(v, v) - radius * radius;
	float d = b * b - 4 * a * c;

	if (d > 0)
	{
		float x1 = (-b - sqrt(d)) / 2;
		float x2 = (-b + sqrt(d)) / 2;
		if (x1 >= 0 && x2 >= 0)	return true; // intersects
		if (x1 < 0 && x2 >= 0)	return true; // intersects
	}
	else if (d <= 0)
	{
		return false; // no intersection
	}

	return true;
}

void update()
{
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME); // get current time
	currentTime = currentTime / 1000; // convert millisecond to seconds

	GLfloat dt = currentTime - prevTime;
	deltaTime = currentTime - prevTime;

	updateControls();

	camera->update();
	/*light->update(dt);
	triangle->update(dt);
	ground->update(dt);
	cube->update(dt);
	sphere->update(dt);*/


	//label->setText(std::to_string(dt).c_str());

	//blendModelFence->update(dt);
	//blendModelWater->update(dt);

	//updateCollisions();

	prevTime = currentTime;

	UpdateMousePicking();

	glutPostRedisplay(); // the render function is called
}

void render() 
{
	if (bWireFrame == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (bGreyScale)
	{
		frameBuffer->startframeBuffer();
	}	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//reflectModel->render();

	skybox->render();

	//glEnable(GL_SCISSOR_TEST);
	//glScissor(200, 200, 400, 200);

	//** for fog disable skybox
	//render skybox first
	//light->render();
	//ground->render();
	//triangle->render();

	/*if (iCull == 1)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}
	if (iCull == 2)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}*/

	//cube->render();
	//sphere->render();
	//quad->render();

	//glDisable(GL_CULL_FACE);

	//reflectModel->render();
	//geomModel->render();
	//tessModel->render();
	//stencilModel->render();
	//stencilMirror->render();

	//glDisable(GL_SCISSOR_TEST);

	//my3DModel->Draw();

	//terrain->draw();

	//blendModelFence->render();

	//blendModelWater->render();

	////render at the end
	//label->Render();

	//barrel->Draw();
	bowling->Draw();
	//rabbit->Draw();
	//crate->Draw();
	//lampPost->Draw();
	//robot->Draw();
	//woodcrate->Draw();

	if (bGreyScale)
	{
		frameBuffer->endFrameBuffer();
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_DOWN;
}

void keyboard_up(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_UP;
}

void mouseMove(int xpos, int ypos)
{
	if (firstMouse)
	{
		mouseX = xpos;
		mouseY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - mouseX;
	GLfloat yoffset = mouseY - ypos;

	mouseX = xpos;
	mouseY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void mouseScroll(int button, int state, int x, int y)
{
	// Used for wheels, has to be up
	if (state == GLUT_UP)
	{
		if (button == 3 || button == 4)
		{
			int dir;
			if (button == 3)
			{
				dir = 1;
			}
			else if (button == 4)
			{
				dir = -1;
			}

			const double ZOOM_SENSITIVITY = -3.0;
			//double fov = camera.getFOV() + dir * ZOOM_SENSITIVITY;

			camera->ProcessMouseScroll(dir);
		}
	}
}

void mouse(int button, int button_state, int x, int y)
{
#define state ((button_state == GLUT_DOWN) ? BUTTON_DOWN : BUTTON_UP)

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mouseState[MOUSE_LEFT] = state;
		break;
	case GLUT_MIDDLE_BUTTON:
		mouseState[MOUSE_LEFT] = state;
		break;
	case GLUT_RIGHT_BUTTON:
		mouseState[MOUSE_LEFT] = state;
		break;
	default:
		break;
	}
}

void mousePassiveMove(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y; // Reversed since y-coordinates go from bottom to left
	lastX = x;
	lastY = y;

	GLfloat sensitivity = 0.5;//
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera->setCameraFront(glm::normalize(front));

	mouseX = (2.0f * x) / (float)Utils::WIDTH - 1.0f;
	mouseY = 1.0f - (2.0f * y) / (float)Utils::HEIGHT;
}

int main(int argc, char **argv)
{
	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Utils::WIDTH, Utils::HEIGHT);
	glutCreateWindow("Graphics Summative 3");

	//init GLEW
	glewInit();

	init();

	//clear
	//glClearColor(1.0, 1.0, 1.0, 1.0);//clear red

	glutIdleFunc(update);

	// register callbacks
	glutDisplayFunc(render);

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);

	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseScroll);

	//glutMouseFunc(mouse);
	//glutPassiveMotionFunc(mousePassiveMove);

	glutMainLoop();

	return 0;

}