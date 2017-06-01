#version 430 core


//uniform vec3 lightDir;
varying float intensity;


void main()
{
	vec3 lightDir = normalize(vec3(gl_LightSource[0].position));

	intensity = dot(lightDir, glNormal);
	gl_Position = ftransform();


}



/*
varying vec3 normal;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	gl_Position = ftransform();

}
*/


