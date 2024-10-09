//
// Order Independent Transparency with Dual Depth Peeling
//

void main(void)
{
	vec3 normal, lightDir;
	vec4 diffuse, ambient, globalAmbient;
	float NdotL;
	
	/* first transform normal into eye space and normalize result */
	normal = normalize( gl_NormalMatrix * gl_Normal );
	
	/* now we normalize the light's direction. Note that according to the OpenGL
	specification, the light is stored in eye space. Also since we're talking about
	a directional light, the position field is actually direction */
	lightDir = normalize( vec3(gl_LightSource[0].position) );
	
	/* compute the cos of the angle between normal and light's direction.
	The light is directional so the angle is constant for every vertex.
	Since these two are normalized the cosine is the dot product. We also need
	to clamp the result	to the [0,1] range. */
	NdotL = max( abs( dot(normal, lightDir) ) , 0.0 );
	
	/* compute the diffuse term */
	diffuse = /*gl_FrontMaterial.diffuse*/gl_Color * gl_LightSource[0].diffuse;
	
	/* compute the ambient and the globalAmbient terms */
	ambient = /*gl_FrontMaterial.ambient*/gl_Color * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * /*gl_FrontMaterial.ambient*/gl_Color;
	
	gl_FrontColor = NdotL * diffuse + globalAmbient + ambient;
	gl_FrontColor.a = gl_Color.a;
	
	gl_Position = ftransform();
}
