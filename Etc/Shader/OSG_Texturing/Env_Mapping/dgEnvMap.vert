
/**
	@ brief
		Vertex Shader
*/

varying vec3 reflectDir;

void main()
{
	gl_Position = ftransform(); // Transform the vertex position
	gl_TexCoord[0] = gl_MultiTexCoord0;	

	vec3 normal = gl_NormalMatrix * gl_Normal;
	normal = normalize(normal);
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 eyeDir = -vVertex.xyz;
	eyeDir = normalize(eyeDir);
	
	reflectDir = reflect(eyeDir, normal);
}