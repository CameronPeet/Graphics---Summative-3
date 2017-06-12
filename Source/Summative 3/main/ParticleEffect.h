#include "Camera.h"
#include <vector>
#include <iostream>
#include <random>
#include <glm\glm.hpp>

#pragma warning(disable : 4996)


class ParticleEffect
{
public:
	ParticleEffect(GLuint compute, GLuint program, Camera* camera, GLuint PARTICLE_COUNT = 128);
	~ParticleEffect();

	bool Init();
	void Render();
	void Update(float fDeltaTime)
	{
		deltaTime = fDeltaTime;
	}

	float deltaTime;
	GLuint compute;
	GLuint program;
	GLuint buffers;

	GLuint position_buffer;
	GLuint velocity_buffer;
	GLuint PARTICLE_COUNT;

	Camera* camera;

	//Scale, Position and Rotation Components
	glm::quat m_Rotation;
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_RotationPoint;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;

};

struct Particle
{
	glm::vec4 currPosition;
	glm::vec4 prevPosition;
};
class ParticleBuffer
{
public:
	ParticleBuffer();
	ParticleBuffer(unsigned int, int);
	ParticleBuffer(ParticleBuffer&) = delete;
	ParticleBuffer(ParticleBuffer&&) = delete;
	ParticleBuffer& operator=(ParticleBuffer&) = delete;
	ParticleBuffer& operator=(ParticleBuffer&&) = delete;
	~ParticleBuffer();

	void initializeParticles();

	GLuint getParticleBufferID() const;
	unsigned int getNumParticles() const;


private:
	GLuint       _buffID;
	unsigned int _numParticles;
	int          _initRadius;


	void distributeParticles(Particle*);
	void deleteParticleBuffer() noexcept;
};

class ParticleTexture {
private:
	GLuint _textureID;

	void deleteTexture() noexcept;

public:
	ParticleTexture();
	ParticleTexture(ParticleTexture&) = delete;
	ParticleTexture(ParticleTexture&&) = delete;
	ParticleTexture& operator=(ParticleTexture&) = delete;
	ParticleTexture& operator=(ParticleTexture&&) = delete;
	~ParticleTexture();

	void loadTexture(const std::string& path);
	void useTexture(GLuint shaderProgramID);

};



#ifndef TGA_H
#define TGA_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct {
		long           width;
		long           height;
		unsigned char *data;
		int            alpha;
	} tTGA;

	int    load_TGA(tTGA *tga, const char *filename);
	void   free_TGA(tTGA *tga);

#ifdef __cplusplus
}
#endif

#endif

