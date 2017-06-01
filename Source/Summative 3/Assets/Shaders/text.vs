#version 430 core

layout (location = 0) in vec4 position;

out vec2 TexCoords;

uniform mat4 projection;
 
void main(void)
{

	gl_Position = projection * vec4(position.xy, 0.0, 1.0);

	TexCoords = position.zw;

}