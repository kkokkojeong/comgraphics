/**
	@ brief
		Bump mapping - fragment shader
*/

varying vec3 lightDir;
uniform sampler2D baseTex;
uniform sampler2D normalTex;

// Expand a range-compressed vector
vec3 expand(vec3 v)
{
	return (v - 0.5) * 2.0;
}

void main()
{
 	vec3 L = normalize(lightDir);
	
	// [0, 1] -> [-1, 1]
	vec3 vNormal = 2.0 * (texture2D(normalTex, gl_TexCoord[0].st).xyz - 0.5);
	
	//The diffuse component is defined as: I = Dl * Dm * clamp(0, 1)
	vec3 color = texture2D(baseTex, gl_TexCoord[0].st) * clamp(dot(L, vNormal), 0.0, 1.0);
	
	// out final color
	gl_FragColor = vec4(color, 1.0);
}