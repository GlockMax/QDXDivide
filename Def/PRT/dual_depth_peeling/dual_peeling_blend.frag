//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//--------------------------------------------------------------------------------------

uniform sampler2DRect TempTex;

void main(void)
{
	gl_FragColor = texture2DRect(TempTex, gl_FragCoord.xy);
	if (gl_FragColor.a == 0)
		discard;
}
