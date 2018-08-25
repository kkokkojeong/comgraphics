/**
	@ brief
		Bump Mapping - vertex program
*/

varying vec3 lightDir;

void main()
{
	// first, transform Texture Coordinate
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	// second, light position - vertex = Light direction vector
	// why? lighting computation must been.. camera coordinate
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	
	// third, transform Vertex - Clip Space
	gl_Position = ftransform();
}