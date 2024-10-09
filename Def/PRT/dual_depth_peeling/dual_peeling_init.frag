//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//--------------------------------------------------------------------------------------

uniform sampler2DRect OpaqueObjectsDepth;

void main(void)
{
	float bias = 0.00001f;
	float opaqueDepth = texture2DRect(OpaqueObjectsDepth, gl_FragCoord.xy).x;

	if (gl_FragCoord.z + bias >= opaqueDepth)
		discard;
		
	gl_FragColor.xy = vec2(-gl_FragCoord.z, gl_FragCoord.z);
}
