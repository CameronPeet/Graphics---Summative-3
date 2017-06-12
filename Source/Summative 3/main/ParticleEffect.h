#pragma once

#include <glm\glm.hpp>
#include "glew/glew.h"

class ParticleEffect
{
public:
	ParticleEffect(GLuint compute, GLuint program);
	~ParticleEffect();

	bool Init();
	void Render(int frame);

private:

	GLuint compute_prog;
	GLuint render_prog;
	
};