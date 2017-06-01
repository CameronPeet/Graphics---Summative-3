#pragma once

#include "glew/glew.h"

//#include "glm/gtc/type_ptr.hpp"

#include "freeglut/freeglut.h"


class FrameBuffer
{
public:
	FrameBuffer(GLuint program);
	~FrameBuffer();

	void startframeBuffer();
	void endFrameBuffer();


private:

	GLuint quadVAO;
	GLuint quadVBO;
	GLuint program;

	GLuint frameBuffer;
	GLuint texture;
};



