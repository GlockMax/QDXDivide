varying vec4 FrontColor;
varying vec3 SurfelCenter;
varying vec3 SurfelNormal;
varying float Ignore;

void main()
{
    vec3 Normal = normalize(gl_NormalMatrix * gl_Normal);
	if(Normal.z < 0.)
	{
		gl_PointSize = 0.;
		Ignore = 1.;
	}
	else
		Ignore = 0.;
	vec3 lightDir, normal;
	vec4 diffuse, ambient, specular;
	float NdotL , NdotHV;

	lightDir = normalize(gl_LightSource[0].position.xyz);	
	normal = normalize(Normal);
	NdotL = abs(dot(normal, lightDir));
	NdotHV = max(dot(normal, normalize(gl_LightSource[0].halfVector.xyz)), 0.0);   
	specular = gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess * 2);
	 
	diffuse = gl_Color * (0.3 + NdotL * gl_LightSource[0].diffuse);

	FrontColor = diffuse + specular;
	FrontColor.w = 1.;
    gl_Position = ftransform();
    gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	SurfelCenter = gl_Vertex.xyz;
	SurfelNormal = gl_Normal;
} 
