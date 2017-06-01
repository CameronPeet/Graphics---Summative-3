#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out vec3 Color;
out vec2 TexCoords;
out vec4 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform vec3 overrideColor;
 
void main(void)
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	//gl_Position = vec4(position, 1.0f);
	TexCoords = texCoords;
	//Color = overrideColor * color;
	vec4 WorldPos = model * vec4(position, 1.0);
}