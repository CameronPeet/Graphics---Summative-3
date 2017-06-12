#version 430 core

in vec4 vert;
uniform mat4 mvp;

out float intenstity;


void main(void)
{
	//vert.w is the life expectancy of the particle
	intenstity = vert.w;
	gl_Position = mvp * vec4(vert.xyz, 1.0);
}