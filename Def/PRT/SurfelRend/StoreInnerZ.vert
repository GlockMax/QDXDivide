varying float Ignore;
varying vec3 SurfelCenter;
varying vec3 SurfelNormal;


void main()
{
    vec3 Normal = normalize(gl_NormalMatrix * gl_Normal);
	if(Normal.z < 0.)
		Ignore = 1.;
	else
	{
		gl_PointSize = 0.;
		Ignore = 0.;
	}
    gl_Position = ftransform();
    gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
	SurfelCenter = gl_Vertex.xyz;
	SurfelNormal = gl_Normal;
} 
