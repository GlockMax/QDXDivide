uniform float R2 = 0.0025;
uniform vec4 Viewport;
uniform vec3 ZDir;
varying vec3 SurfelCenter;
varying vec3 SurfelNormal;
varying vec4 FrontColor;
varying float Ignore;

vec3 UnProject(vec3 WinVec)
{
	vec3 Res;
	Res.xy = (WinVec.xy - Viewport.xy) * Viewport.zw;
	Res.z = WinVec.z;
	Res -= 0.5;
	Res *= 2.;
	return (vec4(Res, 1.) * gl_ModelViewProjectionMatrixInverseTranspose).xyz;
}

vec3 SplatRaster(void)
{
	vec3 Delta = UnProject(gl_FragCoord.xyz);
	Delta -= SurfelCenter; 
	float t = dot(Delta, SurfelNormal) / dot(ZDir, SurfelNormal);
	Delta -= t * ZDir; 
	return Delta;
}

void main(void)
{
	if(Ignore == 1.)
		discard;
	vec3 Delta = SplatRaster();
	float cR2 = dot(Delta, Delta);
	if(cR2 > R2)
		discard;
	float DeltaZ = dot(vec4(Delta, 0.), gl_ModelViewProjectionMatrixTranspose[2]) * 0.5;
	gl_FragDepth = gl_FragCoord.z + DeltaZ;
}

