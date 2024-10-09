#include "windows.h"
#include "GLee.h"
#include "GL/glu.h"
#include "RShaders.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

const int FBOWidth = 1000, FBOHeight = 1000;
typedef GLfloat MemArea[4 * FBOWidth * FBOHeight];
MemArea Areas[8];

RShaders::RShaders(void)
{
}

RShaders::~RShaders(void)
{
}

bool RShaders::Init(void)
{
	GLenum errCode;
	char ErrMsg[500];

	int FBOWidth = 200, FBOHeight = 2000;
	GLint MaxColAtt;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &MaxColAtt);

	//MaxColAtt = 1;

	GLuint FBO;
	glGenFramebuffersEXT(1, &FBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO);
	GLuint *Renderbuffers = new GLuint[MaxColAtt];
	// Renderbuffer case
	glGenRenderbuffersEXT(MaxColAtt, Renderbuffers);
	for(int i = 0; i < MaxColAtt; ++i)
	{
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Renderbuffers[i]);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA32F, FBOWidth, FBOHeight);
		glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_RENDERBUFFER_EXT, Renderbuffers[i]);
	}
	GLenum *Buffers = new GLenum[MaxColAtt];
	for(int k = 0; k < MaxColAtt; ++k)
	{
		Buffers[k] = GL_COLOR_ATTACHMENT0_EXT + k;
	}
//Texture
	//GLuint TexRenderbuffer;
	//glGenRenderbuffersEXT(1, &TexRenderbuffer); 
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, TexRenderbuffer);
	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA32F, FBOWidth, FBOHeight);
	//glFramebufferRenderbufferEXT(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_RENDERBUFFER_EXT, TexRenderbuffer);
	//glReadBuffer(Buffers[1]);

	GLuint Texture;
	glGenTexturesEXT(1, &Texture);
	glBindTextureEXT(GL_TEXTURE_2D, Texture);
	//glCopyTexImage2DEXT(GL_TEXTURE_2D, 0, GL_RGBA32F, 0, 0,FBOWidth, FBOHeight, 0);

	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Renderbuffers[0]);
	//glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_RENDERBUFFER_EXT, Renderbuffers[1]);

	// Texbuffer case
//	glGenTexturesEXT(MaxColAtt, Renderbuffers);
//	for(int i = 0; i < MaxColAtt; ++i)
//	{
//		glBindTextureEXT(GL_TEXTURE_2D, Renderbuffers[i]);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, FBOWidth, FBOHeight, 0, GL_RGBA, GL_FLOAT, Areas[i]);
////		glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_RENDERBUFFER_EXT, Renderbuffers[i]);
//		glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, Renderbuffers[i], 0);
//
//	}

	//GLuint Depthbuffer;
	//glGenRenderbuffersEXT(1, &Depthbuffer);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Depthbuffer);
	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8_EXT, FBOWidth, FBOHeight);
	//glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, Depthbuffer);

	//GLuint Stencilbuffer;
	//glGenRenderbuffersEXT(1, &Stencilbuffer);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Stencilbuffer);
	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8_EXT, FBOWidth, FBOHeight);
	//glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, Stencilbuffer);
	if((errCode = glGetError()) != GL_NO_ERROR)
	{	
		memcpy(ErrMsg, gluErrorString(errCode), 500);
	}
	GLenum FBOStatus = glCheckFramebufferStatusEXT(GL_DRAW_FRAMEBUFFER_EXT);
	if(FBOStatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		return false;
	}


// Shaders
	const GLchar *StringFr = 
//		"varying vec3 normal,lightDir;void main(){float z; z = gl_FragCoord.z; if(gl_FragData[0].z <= 0.)gl_FragData[0].z = z;else gl_FragData[0] = gl_FragData[0] + vec4(z,0.0,0.0,0.0);}";
		"varying vec4 lpos;uniform sampler2D tex,t;void main(){gl_FragData[0] = texture2D(tex, lpos.xy) + vec4(1.0+lpos.x,2.0,3.0,4.0);}";

	const GLchar *StringVe = 
		"varying vec3 normal,lightDir;varying vec4 lpos;void main(){lpos = gl_ModelViewMatrix * gl_Vertex;lightDir = normalize(vec3(gl_LightSource[0].position));normal = normalize(gl_NormalMatrix * gl_Normal);gl_Position = ftransform();}";
// Fragment shader
	GLuint ShFr = glCreateShader(GL_FRAGMENT_SHADER);
	if(ShFr == 0)
		return false;
	glShaderSource(ShFr, 1, &StringFr, NULL);
	glCompileShader(ShFr);
	GLint CompileStatus;
	glGetShaderiv(ShFr, GL_COMPILE_STATUS, &CompileStatus);
	if(CompileStatus != GL_TRUE)
		return false;
// Vertex shader
	GLuint ShVe = glCreateShader(GL_VERTEX_SHADER);
	if(ShVe == 0)
		return false;
	glShaderSource(ShVe, 1, &StringVe, NULL);
	glCompileShader(ShVe);
	glGetShaderiv(ShVe, GL_COMPILE_STATUS, &CompileStatus);
	if(CompileStatus != GL_TRUE)
		return false;

	GLuint Prog = glCreateProgram();
	glAttachShader(Prog, ShFr);
	glAttachShader(Prog, ShVe);
	glLinkProgram(Prog);
	GLint LinkStatus;
	glGetProgramiv(Prog, GL_LINK_STATUS, &LinkStatus);
	if(LinkStatus != GL_TRUE)
		return false;
	glUseProgram(Prog);

	// Render
	glViewport(0, 0, FBOWidth, FBOHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.,(double)FBOWidth,
			 0.,(double)FBOHeight,
			 -1000.,1000.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffers(1/*MaxColAtt*/, Buffers);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	glBegin(GL_QUADS);
	glVertex3d(0., 0., -500.);
	glVertex3d(FBOWidth, 0., -500.);
	glVertex3d(FBOWidth, FBOHeight, -500.);
	glVertex3d(0., FBOHeight, -500.);
	glEnd();
	//glBegin(GL_QUADS);
	//glVertex3d(0., 0., 0.);
	//glVertex3d(FBOWidth, 0., 0.);
	//glVertex3d(FBOWidth, FBOHeight, 0.);
	//glVertex3d(0., FBOHeight, 0.);
	//glEnd();
	//glBegin(GL_QUADS);
	//glVertex3d(0., 0., -500.);
	//glVertex3d(FBOWidth, 0., -500.);
	//glVertex3d(FBOWidth, FBOHeight, -500.);
	//glVertex3d(0., FBOHeight, -500.);
	//glEnd();
	//glBegin(GL_QUADS);
	//glVertex3d(0., 0., 0.);
	//glVertex3d(FBOWidth, 0., 0.);
	//glVertex3d(FBOWidth, FBOHeight, 0.);
	//glVertex3d(0., FBOHeight, 0.);
	//glEnd();
	glPointSize(5.);
	glBegin(GL_POINT);
	glVertex3d(0., 0., 0.);
	glEnd();
	if((errCode = glGetError()) != GL_NO_ERROR)
	{	
		memcpy(ErrMsg, gluErrorString(errCode), 500);
	}
// Read back
	glFinish();
	for(int k = 0; k < MaxColAtt; ++k)
	{
		glReadBuffer(Buffers[k]);
		glReadPixels(0, 0, FBOWidth, FBOHeight, GL_RGBA, GL_FLOAT, Areas[k]);
	}

//	memcpy(Areas[1], Areas[0], sizeof(MemArea));
	glFinish();
	if((errCode = glGetError()) != GL_NO_ERROR)
	{	
		memcpy(ErrMsg, gluErrorString(errCode), 50);
	}
	for(int l = 0; l < 1; ++l)
	{
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Renderbuffers[0]);
	//glFramebufferRenderbufferEXT(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, Renderbuffers[0]);
	glReadBuffer(Buffers[0]);

	//glCopyTexImage2DEXT(GL_TEXTURE_2D, 0, GL_RGBA32F, 0, 0,FBOWidth, FBOHeight, 0);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, TexRenderbuffer);
	//glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_RENDERBUFFER_EXT, TexRenderbuffer);
	//glDrawBuffers(1, &Buffers[1]);

	//glBlitFramebufferEXT(0, 0, 100/*FBOWidth/200*/, 100/*FBOHeight/200*/, 0, 0, 100/*FBOWidth/200*/, 100/*FBOHeight/200*/, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//glReadBuffer(Buffers[1]);
	//glReadPixels(0, 0, FBOWidth, FBOHeight, GL_RGBA, GL_FLOAT, Areas[1]);

	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Renderbuffers[0]);
	//glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, Renderbuffers[0]);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Renderbuffers[1]);
	//glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_RENDERBUFFER_EXT, Renderbuffers[1]);
	}
	glFinish();
	if((errCode = glGetError()) != GL_NO_ERROR)
	{	
		memcpy(ErrMsg, gluErrorString(errCode), 50);
	}
	glReadBuffer(Buffers[0]);
	glReadPixels(0, 0, FBOWidth, FBOHeight, GL_RGBA, GL_FLOAT, Areas[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, FBOWidth, FBOHeight, 0, GL_RGBA, GL_FLOAT, Areas[0]);
	//Texture
	const char *UniformName = "tex";
	int UniformTex = glGetUniformLocationARB(Prog, UniformName); 
	glUniform1iARB(UniformTex, 0); 
	glActiveTextureARB(GL_TEXTURE0_ARB); 

	if((errCode = glGetError()) != GL_NO_ERROR)
	{	
		memcpy(ErrMsg, gluErrorString(errCode), 50);
	}
	glBegin(GL_QUADS);
	glVertex3d(0., 0., 0.);
	glVertex3d(FBOWidth, 0., 0.);
	glVertex3d(FBOWidth, FBOHeight, 0.);
	glVertex3d(0., FBOHeight, 0.);
	glEnd();

	glReadBuffer(Buffers[0]);
	glReadPixels(0, 0, FBOWidth, FBOHeight, GL_RGBA, GL_FLOAT, Areas[0]);


	glDrawBuffer(GL_BACK);


	return true;
}
//varying vec3 normal,lightDir;void main(){float intensity;vec3 n;vec4 color;n = normalize(normal);intensity = max(dot(lightDir,n),0.0);if(intensity > 0.98)color = vec4(0.8,0.8,0.8,1.0);else if (intensity > 0.5)color = vec4(0.4,0.4,0.8,1.0);else if (intensity > 0.25)color = vec4(0.2,0.2,0.4,1.0);else color = vec4(0.1,0.1,0.1,1.0);gl_FragColor = color;}

//varying vec3 normal, lightDir;
//
//void main()
//{
//	float intensity;
//	vec3 n;
//	vec4 color;
//
//	n = normalize(normal);
//	intensity = max(dot(lightDir,n),0.0); 
//
//	if (intensity > 0.98)
//		color = vec4(0.8,0.8,0.8,1.0);
//	else if (intensity > 0.5)
//		color = vec4(0.4,0.4,0.8,1.0);	
//	else if (intensity > 0.25)
//		color = vec4(0.2,0.2,0.4,1.0);
//	else
//		color = vec4(0.1,0.1,0.1,1.0);		
//		
//	gl_FragColor = color;
//}

//varying vec3 normal, lightDir;void main(){lightDir = normalize(vec3(gl_LightSource[0].position));normal = normalize(gl_NormalMatrix * gl_Normal);gl_Position = ftransform();}

//varying vec3 normal, lightDir;
//
//void main()
//{	
//	lightDir = normalize(vec3(gl_LightSource[0].position));
//	normal = normalize(gl_NormalMatrix * gl_Normal);
//		
//	gl_Position = ftransform();
//}
