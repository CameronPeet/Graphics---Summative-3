#include "ParticleEffect.h"

#include <glm\ext.hpp>
#include <glm\gtc\random.hpp>

#define NUM_PARTICLES 1024*1024
#define WORK_GROUP_SIZE 128

ParticleEffect::ParticleEffect(GLuint compute, GLuint program, Camera* camera, GLuint PARTICLE_COUNT)
{
	this->compute = compute;
	this->program = program;
	this->camera = camera;
	this->PARTICLE_COUNT = PARTICLE_COUNT;
}

ParticleEffect::~ParticleEffect()
{
	
}


bool ParticleEffect::Init()
{

	glm::vec4* positions;
	positions = new glm::vec4[NUM_PARTICLES];
	glm::vec4* velocities;
	velocities = new glm::vec4[NUM_PARTICLES];


	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		float xyz[3];
		for (int i = 0; i < 3; i++) xyz[i] = static_cast<float>((rand() % 20) - 10.0f);

		positions[i] = glm::vec4(xyz[0], xyz[1], xyz[2], static_cast<float>((rand() % 100) / 100.0f));
	}


	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		float xyz[3];
		for (int i = 0; i < 3; i++) xyz[i] = static_cast<float>((rand() % 20) - 10.0f);

		velocities[i] = glm::vec4(xyz[0], xyz[1], xyz[2], static_cast<float>((rand() % 100) / 100.0f));
	}

	glGenBuffers(2, &position_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(glm::vec4), positions, GL_DYNAMIC_COPY);

	//Map the position buffer and fill it with random vectors

	////Map the position buffer and fill it with random vectors
	//positions = (glm::vec4 *)glMapBufferRange(
	//	GL_SHADER_STORAGE_BUFFER,
	//	0,
	//	NUM_PARTICLES * sizeof(glm::vec4),
	//	GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);


	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glGenBuffers(2, &velocity_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocity_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES * sizeof(glm::vec4), velocities, GL_DYNAMIC_COPY);

	////Map the position buffer and fill it with random vectorsaa
	//velocities = (glm::vec4 *)glMapBufferRange(
	//	GL_SHADER_STORAGE_BUFFER,
	//	0,
	//	NUM_PARTICLES * sizeof(glm::vec4),
	//	GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);


	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	return true;
}


void ParticleEffect::Render()
{
	glUseProgram(compute);

	glUniform1f(glGetUniformLocation(compute, "dt"), deltaTime);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocity_buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, position_buffer);

	glDispatchCompute(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);


	glUseProgram(program);
	
	//set MVP matrix
	glm::mat4 model;
	//Translate the model according to its position and apply the rotation
	model = glm::translate(model, m_Position) * glm::mat4(m_Rotation);

	//Rotation points

	//Scale the model according to private member m_Scale
	glm::mat4 normalMatrix;
	normalMatrix = glm::transpose(glm::inverse(model * camera->GetViewMatrix()));

	glm::mat4 vp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * model;

	GLuint vpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	glPointSize(5.0f);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glVertexPointer(4, GL_FLOAT, 0, (void*)0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*glBindVertexArray(vao);
	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	glBindVertexArray(0);*/
	//active render program

	//bind vertex array

	//draw vertex array

}



ParticleBuffer::ParticleBuffer() : 
	_buffID(0),
	_numParticles(1000),
	_initRadius(15) 
{

}
ParticleBuffer::ParticleBuffer(unsigned int numParticles, int initRadius): 
	_buffID(0),
	_numParticles(numParticles),
	_initRadius(initRadius)
{


}

ParticleBuffer::~ParticleBuffer() {
	deleteParticleBuffer();
}


void ParticleBuffer::initializeParticles()
{
	Particle* particles = new Particle[_numParticles];
	distributeParticles(particles);

	glGetError();

	glGenBuffers(1, &_buffID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, _numParticles * sizeof(Particle), particles, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _buffID);

	delete particles;

	if (glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not generate or bind shader storage buffer for particles!");
	}
}


void ParticleBuffer::distributeParticles(Particle* particles)
{
	float rndX, rndY, rndZ;
	auto fInitRadius = static_cast<float>(_initRadius);
	std::mt19937 eng;
	std::uniform_real_distribution<float> dist(fInitRadius*(-1.f), fInitRadius);

	for (unsigned int i = 0; i < _numParticles; ++i)
	{
		rndX = dist(eng);
		rndY = dist(eng);
		rndZ = dist(eng);
		particles[i].currPosition = glm::vec4(rndX, rndY, rndZ, 1.f);
		particles[i].prevPosition = glm::vec4(rndX, rndY, rndZ, 1.f);
	}
}


GLuint ParticleBuffer::getParticleBufferID() const {
	return _buffID;
}

unsigned int ParticleBuffer::getNumParticles() const {
	return _numParticles;
}

void ParticleBuffer::deleteParticleBuffer() noexcept {
	if (!_buffID) return;
	glGetError();
	glDeleteBuffers(1, &_buffID);
	if (glGetError() != GL_NO_ERROR) std::cerr << "Could not properly delete particle Buffer!" << std::endl;
}



ParticleTexture::ParticleTexture() : _textureID(0) {}
ParticleTexture::~ParticleTexture() {
	deleteTexture();
}

void ParticleTexture::loadTexture(const std::string& path) {
	tTGA  tga;
	if (!load_TGA(&tga, path.c_str())) {
		throw std::runtime_error("ERROR: Could not load particle texture!");
	}

	glGetError();

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		(tga.alpha) ? (GL_RGB) : (GL_RGB8),
		tga.width,
		tga.height,
		0,
		(tga.alpha) ? (GL_RGBA) : (GL_RGB),
		GL_UNSIGNED_BYTE,
		tga.data
	);

	if (glGetError() != GL_NO_ERROR) {
		glDeleteTextures(1, &_textureID);
		free_TGA(&tga);
		throw std::runtime_error("ERROR: Could not initialize particle texture!");
	}

}

void ParticleTexture::useTexture(GLuint shaderProgramID) {
	glGetError();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	auto uniLocation = glGetUniformLocation(shaderProgramID, "texture");

	if (glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("ERROR: Could not use particle texture!");
	}
	glUniform1i(uniLocation, 0);
}

void ParticleTexture::deleteTexture() noexcept {
	if (_textureID != 0) glDeleteTextures(1, &_textureID);
}




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define then
#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

/*--------------------------------------------------------------------------+/
load_TGA
/+--------------------------------------------------------------------------*/
int load_TGA(tTGA *tga, const char *filename) {

#define SIZEOF_TGA_HEADER 18

	unsigned char   buffer[256];

	int             size_of_image_id;
	int             is_colormap;
	int             targa_type;
	int             colormap_origin;
	unsigned int    colormap_length;
	int             colormap_entry_size;
	int             image_pixel_size;
	int             image_descriptor;
	int             is_inverted;

	int             image_width;
	int             image_height;

	unsigned char  *colormap;
	FILE           *f;
	unsigned char  *data;
	int             x, y, i, j;
	int             raster_width;

	if ((f = fopen(filename, "rb")) == NULL) then return FALSE;

	/* header info */
	if (fread(buffer, 1, SIZEOF_TGA_HEADER, f) != SIZEOF_TGA_HEADER) then return FALSE;

	size_of_image_id = buffer[0];
	is_colormap = buffer[1];
	targa_type = buffer[2];

	colormap_origin = buffer[3] + ((int)(buffer[4]) << 8);
	colormap_length = buffer[5] + ((int)(buffer[6]) << 8);
	colormap_entry_size = buffer[7];

	image_width = buffer[12] + ((unsigned)(buffer[13]) << 8);
	image_height = buffer[14] + ((unsigned)(buffer[15]) << 8);
	image_pixel_size = buffer[16];
	image_descriptor = buffer[17];

	/* valid type? */
	if ((targa_type != 1) && (targa_type != 2)) then return FALSE;

	/* colormap required but missing? */
	if ((targa_type == 1) && !is_colormap) then return FALSE;

	/* cannot load direct-color images */
	if ((targa_type == 2) && is_colormap) then return FALSE;

	/* image id */
	if (size_of_image_id) then
		if ((int)fread(buffer, 1, size_of_image_id, f) != size_of_image_id) then return FALSE;

	is_inverted = (image_descriptor & 0x10) != 0;

	/* cannot handle interlacing */
	if ((image_descriptor & 0xC0)) then return FALSE;

	/* assume that targa 32 contains alpha (image_descriptor bits 0..3) */

	/* load colormap, if any */
	if (is_colormap)
		then{
		/* must be targa 24 or targa 32 */
		if ((colormap_entry_size != 24) && (colormap_entry_size != 32)) then return FALSE;

	/* convert to number of bytes/color entry */
	colormap_entry_size >>= 3;

	colormap = (unsigned char*)malloc(colormap_length *colormap_entry_size);
	if (colormap == NULL) then return FALSE;

	if (fread(colormap, colormap_entry_size, colormap_length, f) != colormap_length)
		then{
	lerror:
		free(colormap);
		return FALSE;
	}

		/* initializations */
	image_pixel_size = (image_pixel_size + 7) >> 3;
	raster_width = image_width *colormap_entry_size;
	}
	else {
		/* must be targa 24 or targa 32 */
		if ((image_pixel_size != 24) && (image_pixel_size != 32)) then return FALSE;
		image_pixel_size >>= 3;
		raster_width = image_width *image_pixel_size;
	}

	data = (unsigned char*)malloc(raster_width *image_height);
	if (data == NULL) then goto lerror;

	/* load image data */
	for (y = (is_inverted ? (image_height - 1) : 0);
		(is_inverted ? (y >= 0) : (y < (int)image_height));
		(is_inverted ? (--y) : (++y)))
		for (x = 0; x < image_width; x++) {

			/* get the next pixel */
			if ((int)fread(buffer, 1, image_pixel_size, f) != image_pixel_size) then goto lerror;

			/* store it */
			if (is_colormap)
				then{
				/* colormapped */
				i = ((buffer[0] + ((unsigned)(buffer[1]) << 8)) - colormap_origin)
				*colormap_entry_size;
			j = (y *raster_width) + (x *colormap_entry_size);

			data[j] = colormap[i + 2];
			data[j + 1] = colormap[i + 1];
			data[j + 2] = colormap[i];

			if (colormap_entry_size > 3) then data[j + 3] = colormap[i + 3];
			}
			else {
				/* non-colormapped */
				j = (y *raster_width) + (x *image_pixel_size);

				data[j] = buffer[2];
				data[j + 1] = buffer[1];
				data[j + 2] = buffer[0];

				if (image_pixel_size > 3) then data[j + 3] = buffer[3];
			}
		}

	/* free the colormap if we had loaded it */
	if (is_colormap) then free(colormap);

	/* store the result */
	tga->width = image_width;
	tga->height = image_height;
	tga->data = data;
	tga->alpha = (is_colormap ? (colormap_entry_size > 3) : (image_pixel_size > 3));

#undef SIZEOF_TGA_HEADER

	return TRUE;
}

/*--------------------------------------------------------------------------+/
free_TGA
/+--------------------------------------------------------------------------*/
void free_TGA(tTGA *tga) {

	if (tga->data) then free(tga->data);

	tga->data = NULL;
	tga->height =
		tga->width = 0;
	tga->alpha = 0;
}

/* end tga.c */