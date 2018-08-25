
/**
	@ brief
		Fragment Shader
*/

uniform samplerCube EnvTex;
varying vec3 reflectDir;

void main()
{
	vec3 R = normalize(reflectDir);
	vec3 envColor = vec3(textureCube(EnvTex, R));
	
	gl_FragColor = vec4(envColor, 1.0);
}