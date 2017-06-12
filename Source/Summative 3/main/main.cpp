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


#include <iostream>

#include "ShaderLoader.h"
#include "Camera.h"
#include "Light.h"
#include "Cubemap.h"
#include "Model.h"
#include "TessModel.h"
#include "FrameBuffer.h"
#include "ProceduralTerrain.h"
#include "ParticleEffect.h"

//float currentTime;
float prevTime;
float deltaTime;

unsigned char keyState[255];

#define BUTTON_UP		0
#define BUTTON_DOWN		1

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
int lastX = Utils::WIDTH / 2.0;
int lastY = Utils::HEIGHT / 2.0;
bool firstMouse = true;
int mouseX;
int mouseY;

std::vector<Light*> vecBullets;

Camera* camera;
Light* light;
Cubemap* skybox;
Model* castle;
ProceduralTerrain* pterrain;
TessModel* tessModel;
FrameBuffer *frameBuffer;
ParticleEffect* m_ParticleEffect;

bool bWireFrame = false;
bool wireButtonDown = false;
bool bGreyScale = false;
bool bComputeShader = true;

int frame = 0;

void init() 
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE for wireframe mode
	glEnable(GL_DEPTH_TEST); // enable the depth testing
	glDepthFunc(GL_LESS);

	ShaderLoader shader;
	GLuint flatShaderProgram = shader.CreateProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	GLuint cubemapProgram = shader.CreateProgram("Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.fs");

	camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 10000.0f);
	camera->setCameraSpeed(25.0f);

	light = new Light(ModelType::kSphere, camera);
	light->setProgram(flatShaderProgram);
	light->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	light->setPosition(camera->getCameraPosition());
	light->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	for (int i = 0; i < 10; i++)
	{
		Light* bullet = new Light(ModelType::kSphere, camera);
		bullet->setProgram(flatShaderProgram);
		bullet->setColor(glm::vec3(0.0f, 1.0f, 1.0f));
		bullet->setPosition(camera->getCameraPosition() + camera->getLook());
		bullet->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
		bullet->setSpeed(10.0f);
		vecBullets.push_back(bullet);
	}

	//cubemap
	skybox = new Cubemap(cubemapProgram, camera);

	//tesselation
	GLuint tessProgram = shader.CreateProgram("Assets/shaders/tessTriModel.vs",
		"Assets/shaders/tessTriModel.fs",
		"Assets/shaders/tessQuadModel.tcs",
		"Assets/shaders/tessQuadModel.tes");

	tessModel = new TessModel(tessProgram, camera);
	tessModel->setPosition(glm::vec3(-3.0f, 175.0f, -5.0f));

	//framebuffer
	GLuint framebufferProgram = shader.CreateProgram("Assets/shaders/framebuffer.vs",
		"Assets/shaders/framebuffer.fs");

	frameBuffer = new FrameBuffer(framebufferProgram);

	//terrain
	GLuint terrainProgram = shader.CreateProgram("Assets/shaders/heightmap.vs", "Assets/shaders/heightmap.fs");

	pterrain = new ProceduralTerrain(L"noise.ppm",
		"assets/heightmap/sand.jpg",
		"assets/heightmap/rock.jpg",
		"assets/heightmap/crack.png",
		terrainProgram,
		camera,
		light);

	pterrain->m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	pterrain->ObjectColor = glm::vec3(0.5f, 0.5f, 0.5f);

	GLuint toonShaderProgram = shader.CreateProgram("Assets/shaders/toon.vs", "Assets/shaders/toon.fs");
	castle = new Model("Assets/Models/castle/Castle OBJ.obj", camera, toonShaderProgram, light);
	castle->setPosition(glm::vec3(4.0f, 150.0f, -250.0f));

	GLuint computeProgram = shader.CreateProgram("Assets/shaders/compParticle.comp");
	GLuint particleProgram = shader.CreateProgram("Assets/shaders/compParticle.vert", "Assets/shaders/compParticle.frag");
	
	m_ParticleEffect = new ParticleEffect(computeProgram, particleProgram);
	m_ParticleEffect->Init();
}

void updateControls()
{
	//camera controls
	if (keyState[(unsigned char) 'w'] == BUTTON_DOWN) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}if (keyState[(unsigned char) 's'] == BUTTON_DOWN) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}if (keyState[(unsigned char) 'a'] == BUTTON_DOWN) {
		camera->ProcessKeyboard(LEFT, deltaTime);
	}if (keyState[(unsigned char) 'd'] == BUTTON_DOWN) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}if (keyState[(unsigned char) 'q'] == BUTTON_DOWN) {
		camera->ProcessKeyboard(UPWARD, deltaTime);
	}if (keyState[(unsigned char) 'e'] == BUTTON_DOWN) {
		camera->ProcessKeyboard(DOWNWARD, deltaTime);
	}

	if (keyState[(unsigned char) 'v'] == BUTTON_DOWN)
	{
		bWireFrame = true;
	}	
	if (keyState[(unsigned char) 'v'] == BUTTON_UP)
	{
		bWireFrame = false;
	}

	if (keyState[(unsigned char) 'g'] == BUTTON_DOWN) {
		bGreyScale = true;
	}
	if (keyState[(unsigned char) 'g'] == BUTTON_UP)
	{
		glEnable(GL_DEPTH_TEST);
		bGreyScale = false;
	}

	if (keyState[(unsigned char) 'c'] == BUTTON_DOWN) {
		bComputeShader = true;
		m_ParticleEffect->Init();
	}
	if (keyState[(unsigned char) 'c'] == BUTTON_UP)
	{
		bComputeShader = false;
	}

	//space key
	if (keyState[32] == BUTTON_DOWN) 
	{
		light->setPosition(camera->getCameraPosition() + camera->getLook());
		float fScalar = 100.0f;
		glm::vec3 speed = glm::vec3(camera->getLook().x * fScalar, camera->getLook().y * fScalar, camera->getLook().z * fScalar);
		light->setVelocity(speed);
	}
}

void update()
{
	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME); // get current time
	currentTime = currentTime / 1000; // convert millisecond to seconds
	deltaTime = currentTime - prevTime;

	updateControls();

	glm::vec3 pos = camera->getCameraPosition();
	pos.y = pterrain->GetYPoint(*camera) + 1.0f;
	camera->setCameraPosition(pos);

	camera->update();
	light->update(deltaTime);

	prevTime = currentTime;

	glutPostRedisplay(); // the render function is called
}

void render() 
{
	if (bGreyScale)
	{
		frameBuffer->startframeBuffer();
	}

	if (bWireFrame == false)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	if (!bComputeShader)
	{
		skybox->render();

		light->render();

		//vecBullets[0]->render();

		tessModel->render();

		castle->Draw();

		pterrain->Render();
	}
	else
	{
		m_ParticleEffect->Render(frame);
	}
	

	if (frame < 1024)
	{
		frame++;
	}
	else
	{
		frame = 0;
	}
		
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

	glutIdleFunc(update);

	// register callbacks
	glutDisplayFunc(render);

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);

	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseScroll);

	glutMainLoop();

	return 0;
}