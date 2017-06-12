#version 430 core

in float intensity;

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
	//vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
		//color = vec4(0.0, 1.0, 0.0, 1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
		//color = vec4(0.0, 0.0, 1.0, 1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);

	//gl_FragColor = color;
	//color = color * vec4(texture(texture_diffuse1, TexCoords));

}