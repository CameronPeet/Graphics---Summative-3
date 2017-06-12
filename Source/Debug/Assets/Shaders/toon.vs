#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 mvp;

//uniform vec3 lightDirection;
out float intensity;

void main()
{
	//vec3 lightDir = normalize(vec3(gl_LightSource[0].position));

	vec3 lightDirection = vec3(0, 0, 1);

	intensity = dot(lightDirection, normal);
	gl_Position = mvp * vec4(position, 1.0f);
	TexCoords = texCoords;

}


