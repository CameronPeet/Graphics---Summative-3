#include "FrameBuffer.h"
#include "Utils.h"

#include <iostream>

FrameBuffer::FrameBuffer(GLuint _program)
{
	program = _program;

	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
								 // Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	//// Quad vertices
	//GLfloat quadVertices[] = {
	//	-1.0f,  1.0f,  0.0f, 1.0f,
	//	1.0f,  1.0f,  1.0f, 1.0f,
	//	1.0f, -1.0f,  1.0f, 0.0f,

	//	1.0f, -1.0f,  1.0f, 0.0f,
	//	-1.0f, -1.0f,  0.0f, 0.0f,
	//	-1.0f,  1.0f,  0.0f, 1.0f
	//};

	//GLfloat quadVertices[] = {   // Vertex attributes for a quad that in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
	//							 // Positions   // TexCoords
	//	-2.3f,  1.0f,  0.0f, 1.0f,
	//	-2.3f,  0.7f,  0.0f, 0.0f,
	//	0.3f,  0.7f,  1.0f, 0.0f,

	//	-2.3f,  1.0f,  0.0f, 1.0f,
	//	0.3f,  0.7f,  1.0f, 0.0f,
	//	0.3f,  1.0f,  1.0f, 1.0f
	//};

	// Setup screen VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	
	glBindVertexArray(quadVAO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
	glUseProgram(program);

	//// Setup screen VAO
	//glGenVertexArrays(1, &quadVAO);
	//glGenBuffers(1, &quadVBO);
	//glBindVertexArray(quadVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	//glBindVertexArray(0);

	glUniform1i(glGetUniformLocation(program, "texFramebuffer"), 0);

	// Create framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// Generate texture ID to hold color buffer and load texture data 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Utils::WIDTH, Utils::HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glBindTexture(GL_TEXTURE_2D, 0);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// Create Renderbuffer Object to hold depth and stencil buffers (won't be sampling these)
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Utils::WIDTH, Utils::HEIGHT); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it


	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &frameBuffer);
}

void FrameBuffer::startframeBuffer()
{
	/////////////////////////////////////////////////////
	// Bind to framebuffer and draw to color texture 
	// as we normally would.
	// //////////////////////////////////////////////////

	// First pass
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now

	glEnable(GL_DEPTH_TEST);
	//DrawScene();
}

void FrameBuffer::endFrameBuffer()
{
	//// Bind default framebuffer and draw contents of our framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glUseProgram(program);
	//glBindVertexArray(quadVAO);
	//glBindTexture(GL_TEXTURE_2D, texture);	// Use the color attachment texture as the texture of the quad plane
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindVertexArray(0);

	//{
	// Bind default framebuffer and draw contents of our framebuffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(quadVAO);

	glDisable(GL_DEPTH_TEST);
	glUseProgram(program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//}

	//{
	//	// Second pass
	//	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	//glClear(GL_COLOR_BUFFER_BIT);

	//	//screenShader.Use();
	//	//glBindVertexArray(quadVAO);
	//	//glDisable(GL_DEPTH_TEST);
	//	//glBindTexture(GL_TEXTURE_2D, frameBuffer);
	//	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//	//glBindVertexArray(0);
	//}
}

//
//{
//	// Bind default framebuffer and draw contents of our framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glBindVertexArray(vaoQuad);
//	glDisable(GL_DEPTH_TEST);
//	glUseProgram(screenShaderProgram);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
//
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//{
//	/////////////////////////////////////////////////////
//	// Bind to default framebuffer again and draw the 
//	// quad plane with attched screen texture.
//	// //////////////////////////////////////////////////
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	// Clear all relevant buffers
//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
//	glClear(GL_COLOR_BUFFER_BIT);
//	glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad
//
//							  // Draw Screen
//	screenShader.Use();
//	glBindVertexArray(quadVAO);
//	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//	glBindVertexArray(0);
//}