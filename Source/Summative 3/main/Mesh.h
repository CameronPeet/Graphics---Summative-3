// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes

#include "Utils.h"
#include "Camera.h"

struct Vertex {
	
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	
	Light* light;

	float ambientStrength  = 0.1f;
	float specularStrength = 0.5f;

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float speed = 0.05f;

	int mFlashlightOn = 1;

	/*  Functions  */
	// Constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
		light = nullptr;
	}

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Light * _light)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
		light = _light;
	}

	glm::vec3 position;

	glm::vec3 getPosition()
	{
		return position;
	}

	void setPosition(glm::vec3 _position)
	{
		position = _position;
	}

	void setScale(glm::vec3 _scale)
	{
		scale = _scale;
	}

	void SetFlashlightOn(bool _bFlashlightOn)
	{
		mFlashlightOn = _bFlashlightOn;
	}

	// Render the mesh
	void Draw(Camera* camera, GLuint program)
	{

		glUseProgram(program);

		
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
											  // Retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();
			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}
		
		glm::mat4 model;
		
		model = glm::translate(model, position);

		model = glm::scale(model, scale);

		glm::mat4 mvp = camera->getprojectionMatrix() *  camera->getViewMatrix() * model;

		GLint mvpLoc = glGetUniformLocation(program, "mvp");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

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
		glUniform3f(objColorLoc, 1.0f, 1.0f, 1.0f);

		GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
		glUniform3f(cameraPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

		// Spotlight
		glm::vec3 spotlightPos = camera->getCameraPosition();

		// offset the flash light down a little
		spotlightPos.y -= 0.5f;

		if (light != nullptr)
		{
			GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
			glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

			GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
			glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);

			GLuint lightDirLoc = glGetUniformLocation(program, "lightDirection");
			glUniform3f(lightDirLoc, this->light->getDirection().x, this->light->getDirection().y, this->light->getDirection().z);
		}
		
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
		GLuint cosInnerConeLoc = glGetUniformLocation(program, "cosInnerCone");
		glUniform1f(cosInnerConeLoc, glm::cos(glm::radians(15.0f)));

		GLuint cosOuterConeLoc = glGetUniformLocation(program, "cosOuterCone");
		glUniform1f(cosOuterConeLoc, glm::cos(glm::radians(20.0f)));

		GLint loc = glGetUniformLocation(program, "on");
		glUniform1i(loc, mFlashlightOn);

		GLuint spotlightPosLoc = glGetUniformLocation(program, "spotlightPos");
		glUniform3f(spotlightPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

		GLuint spotlightDirLoc = glGetUniformLocation(program, "spotDirection");
		glUniform3f(spotlightDirLoc, camera->getLook().x, camera->getLook().y, camera->getLook().z);

		// Draw mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Always good practice to set everything back to defaults once configured.
		for (GLuint i = 0; i < this->textures.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void moveForward() {

		position -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;
	}

	void moveBackward() {

		position += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
	}

	void moveLeft() {

		position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;
	}

	void moveRight() {

		position += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
	}

	void moveUp() {

		position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
	}

	void moveDown() {

		position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;
	}


private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};