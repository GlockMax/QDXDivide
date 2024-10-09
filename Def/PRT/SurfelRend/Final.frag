uniform sampler2DRect BlendTex;
uniform sampler2DRect OuterDepthTex;
void Phong(const vec3 Normal)
{
	vec4 FrontColor = vec4(0.5,0.5,0.5,1.);
	vec3 lightDir, normal;
	vec4 diffuse, ambient, specular;
	float NdotL , NdotHV;

	lightDir = normalize(gl_LightSource[0].position.xyz);	
	normal = normalize(Normal);
	NdotL = abs(dot(normal, lightDir));
	NdotHV = max(dot(normal, normalize(gl_LightSource[0].halfVector.xyz)), 0.0);   
	specular = gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess * 2);
	 
	diffuse = FrontColor * (0.3 + NdotL * gl_LightSource[0].diffuse);

	gl_FragColor = diffuse + specular;
}


void main(void)
{
	vec4 Color = texture2DRect(BlendTex, gl_FragCoord.xy);
	if(Color.w == 0.)
		discard;
	//Phong(Normal.xyz / Normal.w);
	gl_FragColor = Color / Color.w;
	gl_FragDepth = texture2DRect(OuterDepthTex, gl_FragCoord.xy).x;
}


