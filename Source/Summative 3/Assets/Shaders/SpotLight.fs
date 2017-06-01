#version 430 core

in vec2 TexCoord;

in vec3 Normal;
in vec3 FragPos;
in vec4 WorldPos;

out vec4 color;

// texture
uniform sampler2D Texture;

//lighting
uniform vec3 objectColor;

uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDirection;

uniform float specularStrength;
uniform float ambientStrength;

uniform float constant;
uniform float linear;
uniform float exponent;

uniform float cosInnerCone;
uniform float cosOuterCone;
uniform int on;
uniform vec3 spotlightPos;
uniform vec3 spotDirection;

vec3 calcSpotLight();

void main()
{		
	//**ambient
	vec3 ambient = ambientStrength * lightColor;

	vec3 spotColor = vec3(0.0f);

	// If the light isn't on then just return 0 for diffuse and specular colors
	if (on == 1)
		spotColor = calcSpotLight();

	vec3 totalColor = (ambient + spotColor) * objectColor;
		
	color = vec4(totalColor, 1.0f) * texture(Texture, TexCoord);

	//fog
    float d = distance(WorldPos.xyz, cameraPos);
    float lerp = (d - 100.0f) / 10.0f;
    lerp = clamp(lerp, 0.0, 1.0);
    vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    color = mix(color, vFogColor, lerp);
}

//--------------------------------------------------------------
// Calculate the spotlight effect and return the resulting 
// diffuse and specular color summation
//--------------------------------------------------------------
vec3 calcSpotLight()
{
	vec3 lightDir = normalize(spotlightPos - FragPos);
	vec3 spotDir  = normalize(spotDirection);

	float cosDir = dot(-lightDir, spotDir);  // angle between the lights direction vector and spotlights direction vector
	float spotIntensity = smoothstep(cosOuterCone, cosInnerCone, cosDir);

	//**diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
		
	//**specular 
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectionDir = reflect(-lightDir, norm);
		
	//** Phong Model
	//float spec = pow(max(dot(viewDir, reflectionDir),0.0),256);
	//vec3 specular = specularStrength * spec * lightColor;
		
	//** blinn Phong Model
	vec3 H = normalize( lightDir + viewDir);
	vec3 specular = pow(max(dot(norm, H), 0.0), 256) * lightColor * specularStrength;

	//** rim lighting
	float f = 1.0 - dot(norm, viewDir);// Calculate the rim factor 
	f = smoothstep(0.0, 1.0, f);// Constrain it to the range 0 to 1 using a smoothstep function     
	f = pow(f, 16);// Raise it to the rim exponent 
	vec3 rim =  f * vec3(0.0f, 0.0f, 0.0f) * lightColor;// Finally, multiply it by the rim color

	// Attenuation using Kc, Kl, Kq ---------------------------------------------------------
	float fdistance = length(spotlightPos - FragPos);  // distance to light
	float attenuation = 1.0f / (constant + linear * fdistance + exponent * (fdistance * fdistance));

	diffuse *= spotIntensity;
	//specular *= attenuation * spotIntensity;
	
	return (diffuse + specular + rim);
}



