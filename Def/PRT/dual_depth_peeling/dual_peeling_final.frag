//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//--------------------------------------------------------------------------------------

uniform sampler2DRect FrontBlenderTex;
uniform sampler2DRect BackBlenderTex;

void main(void)
{
	vec4 frontColor = texture2DRect(FrontBlenderTex, gl_FragCoord.xy);
	vec3 backColor = texture2DRect(BackBlenderTex, gl_FragCoord.xy).rgb;

	// front + back
	gl_FragColor.rgb = frontColor.rgb + backColor * (1.0 - frontColor.w);
}
