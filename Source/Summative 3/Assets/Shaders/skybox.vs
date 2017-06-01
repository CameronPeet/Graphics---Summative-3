#version 430 core

layout (location = 0) in vec3 position; 

out vec3 TexCoords;

uniform mat4 mvp;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main()
{
	//gl_Position =  projection * view * vec4(position, 1.0);  

	gl_Position = mvp * vec4(position, 1.0);

	TexCoords = position;
}

