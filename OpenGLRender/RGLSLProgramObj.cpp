#include "windows.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "RGLSLProgramObj.h"
#include "Shader.h"

#include <stdio.h>

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

RGLSLProgramObj::RGLSLProgramObj()
{
	checkError = false;	
	program = 0;
	vertex_shader = 0;
	fragment_shader = 0;
	Disabled = false;
}

RGLSLProgramObj::~RGLSLProgramObj()
{
	DeleteShader();
}

void RGLSLProgramObj::InitShader(char * sourceFragment, char * sourceVertex)
{	
	checkError = CheckingSupportShader();

	const char* VertexSource = NULL;
	const char* FragmentSource = NULL;	
	
	VertexSource = ReadFile(sourceVertex);
	FragmentSource = ReadFile(sourceFragment);
	if (VertexSource == NULL || FragmentSource == NULL)
	{
		checkError = false;		
	}
	else
	{	
		vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

		glShaderSourceARB(vertex_shader, 1, &VertexSource, NULL);							
		glShaderSourceARB(fragment_shader, 1, &FragmentSource, NULL);

		glCompileShaderARB(vertex_shader);
		glCompileShaderARB(fragment_shader);	

		checkError = CheckingCompilingShader();
		
		program = glCreateProgramObjectARB();	

		glAttachObjectARB(program, vertex_shader);
		glAttachObjectARB(program, fragment_shader);

		glLinkProgramARB(program);	
	}
#ifdef _DEBUG
	glValidateProgramARB(program);
	int Status;
	glGetObjectParameterivARB(program, GL_OBJECT_VALIDATE_STATUS_ARB, &Status);
	char buf[1001];
	glGetInfoLogARB(program, 1000, NULL, buf);	
	int k = 0;	
#endif
	delete[] VertexSource;
	delete[] FragmentSource;
}

bool RGLSLProgramObj::CheckingCompilingShader()
{
	GLhandleARB compiled;

	glGetObjectParameterivARB(vertex_shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (compiled == 0)
	   return false;

	glGetObjectParameterivARB(fragment_shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (compiled == 0)
		return false;

	return true;
}

bool RGLSLProgramObj::CheckingSupportShader()
{
	if(!GLEE_ARB_fragment_program)
		return false;

	if(!GLEE_ARB_vertex_program)
		return false;

	return true;
}

void RGLSLProgramObj::ShaderON()
{
	if (checkError && !Disabled)
		glUseProgramObjectARB(program);
}

void RGLSLProgramObj::SetUniform(char* name, float* val, int vector_size) const
{
	GLint loc = glGetUniformLocation(program, name);
	if(loc == -1)
		return;

	switch(vector_size)
	{
		case 1:
			glUniform1fv(loc, 1, val);
			break;
		case 2:
			glUniform2fv(loc, 1, val);
			break;
		case 3:
			glUniform3fv(loc, 1, val);
			break;
		case 4:
			glUniform4fv(loc, 1, val);
			break;
		default:
			break;
	}
}

void RGLSLProgramObj::BindTexture2D(unsigned int target, char* tex_name, unsigned int tex_id, int tex_unit) const
{
	glActiveTextureARB(GL_TEXTURE0_ARB + tex_unit);
	glBindTexture(target, tex_id);

	GLint id = glGetUniformLocation(program, tex_name);
	glUniform1i(id, tex_unit);

	glActiveTextureARB(GL_TEXTURE0_ARB);
}

void RGLSLProgramObj::DeleteShader()
{
	if(fragment_shader > 0)
		glDeleteObjectARB(fragment_shader);// EXIT ERROR
	fragment_shader = 0;
	if(vertex_shader > 0)
		glDeleteObjectARB(vertex_shader);
	vertex_shader = 0;
	if(program > 0)
		glDeleteObjectARB(program);
	program = 0;
}
bool RGLSLProgramObj::GetCheckError() const
{
	return checkError;
}

int RGLSLProgramObj::GetMyProgram() const
{
	return program;
}


char* RGLSLProgramObj::ReadFile(char* fn)
{ 
	char *content = NULL;
	if (fn != NULL)
	{
		FILE *fp = fopen(fn, "rt");
		if (fp != NULL) 
		{
			fseek(fp, 0, SEEK_END);
			size_t count = ftell(fp);
			if (count > 0) 
			{
				rewind(fp);
				content = new char[sizeof(char) * (count + 1)];
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;	
}
