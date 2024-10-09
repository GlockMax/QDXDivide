uniform sampler2DRect FlagTex;
uniform sampler2DRect ZfarTex;
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
	int Flag = texture2DRect(FlagTex, gl_FragCoord.xy)[0];
	if(Flag != 0)
		discard;
		
	float Zfar = texture2DRect(ZfarTex, gl_FragCoord.xy)[0];
	if(gl_FragCoord.z <= Zfar)
		discard;
	
	if(gl_FrontFacing)
	{
		gl_FragData[0] = CalcPhColor();
		gl_FragData[0][3] = 1.f;
		gl_FragData[1].rg = vec2(-gl_FragCoord.z, -MAX_DEPTH);

	}
	else
	{
		gl_FragData[0] = vec4(0.,0.,0.,1.);
		gl_FragData[1].rg = vec2(-MAX_DEPTH, -gl_FragCoord.z);
	}
}
