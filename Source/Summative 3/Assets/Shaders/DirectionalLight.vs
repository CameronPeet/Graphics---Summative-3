#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 WorldPos;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

void main(void)
{
	vec4 WorldPos = model * vec4(position, 1.0);

	FragPos = vec3(model * vec4(position, 1.0f)); // vertex position in world space
	Normal = mat3(transpose(inverse(model))) * normal; // normal direction in world space

	TexCoord = texCoord;

	gl_Position = projection * view * model * vec4(position, 1.0);
}