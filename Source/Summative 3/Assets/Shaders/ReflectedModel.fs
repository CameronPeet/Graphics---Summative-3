#version 430 core

in vec3 Normal;
in vec3 Position;
in vec4 WorldPos;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - cameraPos);

    vec3 R = reflect(I, normalize(Normal));

    color = texture(skybox, R);

	//fog
    float d = distance(WorldPos.xyz, cameraPos);
    float lerp = (d - 40.0f) / 10.0f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);
}