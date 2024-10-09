varying vec3 Normal;
varying vec4 FrontColor;
void main()
{
	vec3 lightDir, normal;
	vec4 diffuse, ambient, specular;
	float NdotL , NdotHV;

	lightDir = normalize(gl_LightSource[0].position.xyz);	
	normal = normalize(Normal);
	NdotL = abs(dot(normal, lightDir));
	NdotHV = max(abs(dot(normal, normalize(gl_LightSource[0].halfVector.xyz))), 0.0);   
	specular = gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess * 2);
	 
	diffuse = 0.3 * FrontColor + NdotL * FrontColor * gl_LightSource[0].diffuse;

	gl_FragColor = diffuse + specular;
	gl_FragColor[3] = 1.;
}
