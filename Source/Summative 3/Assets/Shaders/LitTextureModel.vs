#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out vec3 fragposition;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * view * model * vec4(position, 1.0);

	TexCoords = vec2(texCoords.x, 1.0 - texCoords.y);
	//TexCoord = texCoord;

	fragposition = vec3 (model * vec4 (position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	//Normal = normal;
}