#version 430 core

in vec3 Color;
in vec2 TexCoords;
in vec4 WorldPos;

out vec4 color;

//uniform sampler2D texKitten;
//uniform sampler2D texPuppy;
uniform sampler2D Texture;

uniform vec3 overrideColor;

void main()
{
	//color = vec4(Color, 1.0) * mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);
	//color = vec4(Color, 1.0) * texture(Texture, TexCoord);
	color = vec4(overrideColor, 1.0f) * texture(Texture, TexCoords);
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	//fog
    //float d = distance(WorldPos.xyz, cameraPos);
   // float lerp = (d - 40.0f) / 10.0f;
    //lerp = clamp(lerp, 0.0, 1.0);
    //vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    //color = mix(color, vFogColor, lerp);
}