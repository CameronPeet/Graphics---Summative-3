#version 430 core

in vec3 fragposition;
in vec2 TexCoords;
in vec3 Normal;

out vec4 color;

//texture
uniform sampler2D Texture;
uniform float currentTime;

//lighting
uniform vec3 lightposition;
uniform vec3 lightcolor;
uniform vec3 objectcolor;
uniform vec3 viewposition; //camera position

//uniform vec3 cameraPos; // same as viewposition?
uniform float specularStrength;
uniform float ambientStrength;

void main()
{
    //**ambient light
    //float ambientStrength = 0.2f;
    vec3 ambientValue = ambientStrength * lightcolor;

	//diffuse light
    vec3 norm = normalize (Normal);
    vec3 lightdirection = normalize (lightposition - fragposition);
    float difference = max(dot(norm, lightdirection), 0.0);
    vec3 diffuselight = difference * lightcolor;

	//specular light
    //float specularStrength = 100.0f;
	//float specularStrength = 0.8f;
    vec3 viewDir = normalize(viewposition - fragposition);
	vec3 reflectionDir = reflect(-lightdirection, norm);

	//** Phong Model
	float shininess = 32.0f;
    float spec = pow(max(dot(viewDir, reflectionDir),0.0), shininess);
	//float spec = pow(max(dot(viewDir, reflectionDir),0.0),256);
	vec3 specular = specularStrength * spec * lightcolor;

	 //** blinn Phong Model
    vec3 halfDirection = normalize( lightdirection + viewDir);
//	vec3 specular = pow(max(dot(norm, halfDirection), 0.0), shininess) * lightcolor * specularStrength;
//	vec3 specular = pow(max(dot(norm, halfDirection), 0.0), 256) * lightColor * specularStrength;

//	//** rim lighting
//	float f = 1.0 - dot(norm, viewDir);// Calculate the rim factor 
//	f = smoothstep(0.0, 1.0, f);// Constrain it to the range 0 to 1 using a smoothstep function     
//	f = pow(f, 16);// Raise it to the rim exponent 
//	vec3 rim =  f * vec3(1.0f, 0.0f, 0.0f) * lightColor;// Finally, multiply it by the rim color


    vec3 finalobjectlight = (ambientValue + diffuselight + specular ) * objectcolor;
    color = vec4(finalobjectlight, 1.0f) * texture(Texture, TexCoords);

//	vec3 totalColor = (ambient + diffuse + specular + rim) * objectColor;		
//	color = vec4(totalColor, 1.0f) * texture(Texture, TexCoord);		
//	//color = vec4(objectColor, 1.0f) * texture(Texture, TexCoord);

	//vec4 texel = texture(texture_map, TexCoord);
    //frag_color = vec4(ambient + diffuse + specular, 1.0f) * texel;	

	//color = texture(Texture, TexCoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);

}

/*
void main()
{
    //color = mix(texture(Texture, TexCoord), texture(Texture2, TexCoord), 0.2)  * vec4(outColor, 1.0f) * abs(sin(currentTime));

    //**ambient light
    float ambientStrength = 0.2f;
    vec3 ambientValue = ambientStrength * lightcolor;

    //diffuse light
    vec3 norm = normalize (Normal);
    vec3 lightdirection = normalize (lightposition - fragposition);
    float difference = max(dot(norm, lightdirection), 0.0);
    vec3 diffuselight = difference * lightcolor;

	//specular light
    float specularStrength = 100.0f;
    vec3 viewDir = normalize(viewposition - fragposition);
	vec3 reflectionDir = reflect(-lightdirection, norm);

    //** Phong Model
	float shininess = 32.0f;
    float spec = pow(max(dot(viewDir, reflectionDir),0.0), shininess);
	//float spec = pow(max(dot(viewDir, reflectionDir),0.0),256);
	vec3 specular = specularStrength * spec * lightcolor;

    //** blinn Phong Model
    vec3 halfDirection = normalize( lightdirection + viewDir);
	vec3 specular = pow(max(dot(norm, halfDirection), 0.0), shininess) * lightcolor * specularStrength;
//	vec3 specular = pow(max(dot(norm, halfDirection), 0.0), 256) * lightColor * specularStrength;

//	//** rim lighting
//	float f = 1.0 - dot(norm, viewDir);// Calculate the rim factor 
//	f = smoothstep(0.0, 1.0, f);// Constrain it to the range 0 to 1 using a smoothstep function     
//	f = pow(f, 16);// Raise it to the rim exponent 
//	vec3 rim =  f * vec3(1.0f, 0.0f, 0.0f) * lightColor;// Finally, multiply it by the rim color


    vec3 finalobjectlight = (ambientValue) * objectcolor;
    color = vec4(finalobjectlight, 1.0f) * texture(Texture, TexCoord);

//	vec3 totalColor = (ambient + diffuse + specular + rim) * objectColor;		
//	color = vec4(totalColor, 1.0f) * texture(Texture, TexCoord);		
//	//color = vec4(objectColor, 1.0f) * texture(Texture, TexCoord);

	//vec4 texel = texture(texture_map, TexCoord);
    //frag_color = vec4(ambient + diffuse + specular, 1.0f) * texel;	

	 color = texture(Texture, TexCoord) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	 color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

*/


