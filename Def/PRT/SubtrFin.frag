uniform sampler2DRect ToolDepthTex;
uniform sampler2DRect ToolColorTex0;
uniform sampler2DRect ToolColorTex1;
uniform sampler2DRect DepthTex;

void main()
{	
	vec4 Dexels = texture2DRect(ToolDepthTex, gl_FragCoord.xy);
	float StockDepth = texture2DRect(DepthTex, gl_FragCoord.xy);
	int k = -1;
	float min = 0.;
	float max = 0.;
	for(int i = 0; i < 2; ++i)
	{
		min = Dexels[2 * i];
		max = Dexels[2 * i + 1];
		if(i == 0)
		{
			min = -min;
			max = -max;
		}
		if(StockDepth > min && StockDepth < max)
		{
			k = i;
			break;
		}
	}
	if(k == -1)
	{
		gl_FragColor = texture2DRect(ToolColorTex1, gl_FragCoord.xy);
		gl_FragDepth = StockDepth;
	}
	else
	{
		gl_FragColor = texture2DRect(ToolColorTex0, gl_FragCoord.xy);
		gl_FragDepth = max;
	}
}
