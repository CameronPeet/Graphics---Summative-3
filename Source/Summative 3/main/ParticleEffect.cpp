#include "ParticleEffect.h"

ParticleEffect::ParticleEffect(GLuint compute, GLuint program)
{
	compute_prog = compute;
	render_prog = program;
}

ParticleEffect::~ParticleEffect()
{

}

bool ParticleEffect::Init()
{
	GLuint texHandle;
	glGenTextures(1, &texHandle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 512, 512, 0, GL_RED, GL_FLOAT, NULL);

	// Because we're also using this tex as an image (in order to write to it),
	// we bind it to an image unit as well
	glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

	glUseProgram(render_prog);
	glUniform1i(glGetUniformLocation(render_prog, "srcTex"), 0);

	GLuint vertArray;
	glGenVertexArrays(1, &vertArray);
	glBindVertexArray(vertArray);

	GLuint posBuf;
	glGenBuffers(1, &posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	float data[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, data, GL_STREAM_DRAW);
	GLint posPtr = glGetAttribLocation(render_prog, "pos");
	glVertexAttribPointer(posPtr, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posPtr);

	glUniform1i(glGetUniformLocation(compute_prog, "destTex"), 0);

	return true;
}

void ParticleEffect::Render(int frame)
{
	glUseProgram(compute_prog);
	glUniform1f(glGetUniformLocation(compute_prog, "roll"), (float)frame*0.01f);
	glDispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2

	glUseProgram(render_prog);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}