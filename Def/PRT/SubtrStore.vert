varying vec3 Normal;
varying vec4 FrontColor;


void main()
{
      Normal = normalize(gl_NormalMatrix * gl_Normal);    
      FrontColor = gl_Color;
      gl_Position = ftransform();
      gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
} 
