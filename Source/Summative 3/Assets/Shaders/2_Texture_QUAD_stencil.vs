#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 WorldPos;
 
void main(void)
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vec4 WorldPos = model * vec4(position, 1.0);
}