#include "stdafx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "Shader.h"
#include "RTranslucentProg.h"

const float RTranslucentProg::MAX_DEPTH = 1.0f;

RTranslucentProg::RTranslucentProg()
{
	quadDisplayList = 0;
	initProgComplete = true;

	for(int i = 0; i < MAX_COLOR_ATTACHMENTS; ++i)
		RenderTargets[i] = GL_COLOR_ATTACHMENT0 + i;
}
RTranslucentProg::~RTranslucentProg()
{
	if(quadDisplayList != 0)
		glDeleteLists(quadDisplayList, 1);
}
// ===================================================================================
void RTranslucentProg::deleteProgram()
{
	initProg.DeleteShader();
	peelProg.DeleteShader();
	blendProg.DeleteShader();
	finalProg.DeleteShader();
}
void RTranslucentProg::initProgram(char* frag_src, char* vertex_src, unsigned int program_num)
{
	switch(program_num)
	{
		case INIT_PROGRAM:
			initProg.InitShader(frag_src, vertex_src);
			break;
		case PEEL_PROGRAM:
			peelProg.InitShader(frag_src, vertex_src);
			break;
		case BLEND_PROGRAM:
			blendProg.InitShader(frag_src, vertex_src);
			break;
		case FINAL_PROGRAM:
			finalProg.InitShader(frag_src, vertex_src);
			break;
		default:
			break;
	}

	switch(program_num) {

		case INIT_PROGRAM:
			if( !initProg.GetCheckError() ) 
				initProgComplete = false;
			break;
		case PEEL_PROGRAM:
			if( !peelProg.GetCheckError() )	
				initProgComplete = false;
			break;
		case BLEND_PROGRAM:
			if( !blendProg.GetCheckError() )	
				initProgComplete = false;
			break;
		case FINAL_PROGRAM:
			if( !finalProg.GetCheckError() )	
				initProgComplete = false;
			break;
		default:
			break;
	}
}

void RTranslucentProg::setAlpha(float alpha_value)
{
	alpha = alpha_value;
}

// ===================================================================================
bool RTranslucentProg::getOQFlag() const
{
	return USE_OQ;
}

bool RTranslucentProg::isProgramComplete()
{
	if(!initProgComplete)
		return false;

	if (!GLEE_ARB_draw_buffers)
		initProgComplete = false;
	else if (!GLEE_ARB_texture_float)
		initProgComplete = false;
	else if (!GLEE_ARB_texture_rectangle)
		initProgComplete = false;
	else if (!GLEE_EXT_blend_minmax)
		initProgComplete = false;
	else if (!GLEE_ARB_shader_objects)
		initProgComplete = false;
	else if (!GLEE_ARB_vertex_shader)
		initProgComplete = false;
	else if (!GLEE_ARB_fragment_shader)
		initProgComplete = false;
	else if (!GLEE_ARB_vertex_program)
		initProgComplete = false;
	else if (!GLEE_ARB_fragment_program)
		initProgComplete = false;

	if(!initProgComplete)
		return false;

	if(initProgComplete)
	{
		int maxColorAttachments[1];
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, maxColorAttachments);

		if (maxColorAttachments[0] < MAX_COLOR_ATTACHMENTS)
			initProgComplete = false;
	}

	if(!initProgComplete)
		return false;

	if( !initProg.GetCheckError() ) 
		initProgComplete = false;
	if( !peelProg.GetCheckError() )	
		initProgComplete = false;
	if( !blendProg.GetCheckError() )	
		initProgComplete = false;
	if( !finalProg.GetCheckError() )	
		initProgComplete = false;

	return initProgComplete;
}

// ===================================================================================
void RTranslucentProg::makeFullScreenQuad()
{
	if(quadDisplayList == 0)
		quadDisplayList = glGenLists(1);
	glNewList(quadDisplayList, GL_COMPILE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, 1.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		{
			glVertex2f(0.0, 0.0); 
			glVertex2f(1.0, 0.0);
			glVertex2f(1.0, 1.0);
			glVertex2f(0.0, 1.0);
		}
		glEnd();
	glPopMatrix();

	glEndList();
}

void RTranslucentProg::initFBO() 
{
	glGenTextures(2, dualDepthTexId);
	glGenTextures(2, dualFrontBlenderTexId);
	glGenTextures(2, dualBackTempTexId);
	glGenFramebuffersEXT(2, dualFboId);

	int viewportParams[4];
	glGetIntegerv(GL_VIEWPORT, viewportParams);

	for (int i = 0; i < 2; i++) {

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, dualDepthTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB32F_ARB, viewportParams[2], viewportParams[3], 0, GL_RGB, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, dualFrontBlenderTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, viewportParams[2], viewportParams[3], 0, GL_RGBA, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, dualBackTempTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, viewportParams[2], viewportParams[3],0, GL_RGBA, GL_FLOAT, 0);
	}

	glGenTextures(1, &dualBackBlenderTexId);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, dualBackBlenderTexId);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, viewportParams[2], viewportParams[3], 0, GL_RGB, GL_FLOAT, 0);

	glGenTextures(1, &dualOpaqueObjectsDepthTexId);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, dualOpaqueObjectsDepthTexId);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_DEPTH_COMPONENT32_ARB, viewportParams[2], viewportParams[3], 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	int j = 0;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dualFboId[j]);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, dualDepthTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_RECTANGLE_ARB, dualFrontBlenderTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_RECTANGLE_ARB, dualBackTempTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_RECTANGLE_ARB, dualBackBlenderTexId, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_RECTANGLE_ARB, dualOpaqueObjectsDepthTexId, 0);

	j = 1;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dualFboId[j]);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_ARB, dualDepthTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_RECTANGLE_ARB, dualFrontBlenderTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_TEXTURE_RECTANGLE_ARB, dualBackTempTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_TEXTURE_RECTANGLE_ARB, dualBackBlenderTexId, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_RECTANGLE_ARB, dualOpaqueObjectsDepthTexId, 0);
}

void RTranslucentProg::destroyFBO()
{
	glDeleteFramebuffersEXT(1, &dualFboId[0]);
	glDeleteFramebuffersEXT(1, &dualFboId[1]);
	glDeleteTextures(2, dualDepthTexId);
	glDeleteTextures(2, dualFrontBlenderTexId);
	glDeleteTextures(2, dualBackTempTexId);
	glDeleteTextures(1, &dualBackBlenderTexId);
	glDeleteTextures(1, &dualOpaqueObjectsDepthTexId);
}

// ===================================================================================
bool RTranslucentProg::prepareI()
{
	glGenQueries(1, &queryId);
	makeFullScreenQuad();
	initFBO();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dualFboId[0]);

	// //////////////////////////////////////////////////////////////////////////////////////////////
	// Draw opaque objects to the back blender texture as a background. Using depth texture attached
	// //////////////////////////////////////////////////////////////////////////////////////////////
	glDrawBuffers(1, &RenderTargets[3]);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
		return false;
	return true;
}

bool RTranslucentProg::prepareII(PutCode put_code)
{
	switch(put_code) 
	{
	case PUT_BEFORE: 
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDrawBuffers(2, &RenderTargets[1]);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffers(1, &RenderTargets[0]);
		glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlendEquationEXT(GL_MAX_EXT);

		initProg.ShaderON();
		initProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "OpaqueObjectsDepth", dualOpaqueObjectsDepthTexId, 0);

		if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
			return false;
		break;

	case PUT_AFTER:
		if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
			return false;

		Shader::ShaderOFF();
		break;
	}

	return true;
}

bool RTranslucentProg::prepareIII(PutCode put_code, int curr_id, int prev_id, bool* are_samples_remained)
{
	*are_samples_remained = true;

	switch(put_code)
	{
	case PUT_BEFORE:
		// Switch FBO
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, dualFboId[curr_id]);
		// 

		if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
			return false;

		glDrawBuffers(2, &RenderTargets[1]);			// BACK&FRONT COLORS
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffers(1, &RenderTargets[0]);			// DEPTH
		glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawBuffers(3, &RenderTargets[0]);			// 0,1,2
		glBlendEquationEXT(GL_MAX_EXT);


		peelProg.ShaderON();
		peelProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "OpaqueObjectsDepth", dualOpaqueObjectsDepthTexId, 0);
		peelProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "DepthBlenderTex", dualDepthTexId[prev_id], 1);
		peelProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "FrontBlenderTex", dualFrontBlenderTexId[prev_id], 2);

		if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
			return false;

		return true;

	case PUT_AFTER:

		if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
			return false;

		Shader::ShaderOFF();

		glDrawBuffers(1, &RenderTargets[3]);
		glBlendEquationEXT(GL_MAX_EXT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (USE_OQ)
			glBeginQuery(GL_SAMPLES_PASSED_ARB, queryId);

		blendProg.ShaderON();
		blendProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "TempTex", dualBackTempTexId[curr_id], 0);
		glCallList(quadDisplayList);
		Shader::ShaderOFF();

		if (USE_OQ)
		{
			glEndQuery(GL_SAMPLES_PASSED_ARB);
			GLuint sample_count;
			glGetQueryObjectuiv(queryId, GL_QUERY_RESULT_ARB, &sample_count);
			if (sample_count == 0)
			{
				*are_samples_remained = false;
			}
		}			// end if
		return true;
	}					// end switch
	return false;
}

bool RTranslucentProg::prepareIV(int curr_id, int prev_id)
{
	if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT))
				return false;

	glDisable(GL_BLEND);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);

	finalProg.ShaderON();
	finalProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "FrontBlenderTex", dualFrontBlenderTexId[curr_id], 0);
	finalProg.BindTexture2D(GL_TEXTURE_RECTANGLE_ARB, "BackBlenderTex", dualBackBlenderTexId, 1);
	glCallList(quadDisplayList);
	Shader::ShaderOFF();

	destroyFBO();

	return true;
}