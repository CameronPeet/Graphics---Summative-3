#version 430 core

in vec4 WorldPos;

out vec4 color;

uniform vec3 lightColor;

void main()
{
	color = vec4(lightColor, 1.0f);

	//fog
   // float d = distance(WorldPos.xyz, cameraPos);
    //float lerp = (d - 40.0f) / 10.0f;
   // lerp = clamp(lerp, 0.0, 1.0);
   // vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
   // color = mix(color, vFogColor, lerp);
}