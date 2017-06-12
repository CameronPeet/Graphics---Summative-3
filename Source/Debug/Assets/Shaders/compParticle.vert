#version 430 core

in vec4 vert;
uniform mat4 mvp;

//This is derived from the age of the particle read by the vertex shader
out float intensity;

void main(void)
{
	//vert.w is the life expectancy of the particle
	intenstity = vert.w;
	gl_Position = mvp * vec4(vert.xyz, 1.0);
}