varying vec3 Normal;
varying vec4 FrontColor;
const float MAX_DEPTH = 1.f;
vec4 CalcPhColor()
{
	vec3 lightDir, normal;
	normal = normalize(Normal);
	vec4 diffuse, ambient, specular;
	float NdotL , NdotHV;

	lightDir = normalize(gl_LightSource[0].position.xyz);	
	NdotL = abs(dot(normal, lightDir));
	NdotHV = max(dot(normal, normalize(gl_LightSource[0].halfVector.xyz)), 0.0);   
	specular = gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess * 2);
	diffuse = 0.3 * FrontColor + NdotL * FrontColor * gl_LightSource[0].diffuse;

	return diffuse + specular;
}
void main()
{
	gl_FragData[1].a = 1.;
	if(gl_FrontFacing)
	{
		gl_FragData[1].rgb = vec3(0.f,0.f,0.f);
		gl_FragData[0].rgba = vec4(-gl_FragCoord.z, -MAX_DEPTH, gl_FragCoord.z, -MAX_DEPTH);
	}
	else
	{
		gl_FragData[1].rgb = CalcPhColor().xyz;
		gl_FragData[0].rgba = vec4(-MAX_DEPTH, -gl_FragCoord.z, -MAX_DEPTH, gl_FragCoord.z);
	}
}
