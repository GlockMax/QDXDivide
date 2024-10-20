#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"

#include "glbmp-1.1\glbmp.h"
#include "BaseRender.h"
#include "Shader.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

#define TEX_FIRST 0
#define TEX_SECOND 1

int Shader::currentState = 0;

Shader::Shader()
{
	IsOK = false;	
	program = 0;
	vertex_shader = 0;
	fragment_shader = 0;
	Disabled = false;

	flag = 0;
}

Shader::~Shader(void)
{ 
	DeleteShader();
}


//void Shader::LoadTex(const char* path, int numTex)
//{
//
//	// связываем определенный текстурный объект с определенными праметрами наложения текстуры
//	glBindTexture(GL_TEXTURE_2D, textureNum[numTex]);
//
//	// загружаем текстуру из файла
//	glbmp_t TextureImage[1]; // Выделим место для хранения текстур 	
//	if(!glbmp_LoadBitmap(path, 0, &TextureImage[0]))// читаем из файла текстуру
//		IsOK = false;
//
//	// загружаем текстуру в опен жлp
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[0].width, TextureImage[0].height, 0,
//		GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0].rgb_data);	
//
//	// назначаеи режим фильтрации
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		
//}

bool Shader::LoadTexDDS(int numTexObj,const char* szCubeFace[], char* path)
{	
	GLenum cube[6] = { 
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };	
	

	// связываем определенный текстурный объект 
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureNum[numTexObj]);	

	glTexParameteri ( GL_TEXTURE_CUBE_MAP,  GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri ( GL_TEXTURE_CUBE_MAP,  GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri ( GL_TEXTURE_CUBE_MAP,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP,  GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE );


	char fullName[512];

	for(int i = 0; i < 6; i++)
	{
		glbmp_t TextureImage; // Выделим место для хранения текстур 	
		strcpy(fullName,path);
		 
		if(!glbmp_LoadBitmap(strcat(fullName,szCubeFace[i]), 0, &TextureImage))// читаем из файла текстуру
			IsOK = false;

		// загружаем текстуру в опен жлp
		glTexImage2D(cube[i], 0, GL_RGB, TextureImage.width, TextureImage.height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, TextureImage.rgb_data);	
		glbmp_FreeBitmap(&TextureImage);
	}
	return true;
}


void Shader::InitShader(char* path, char * sourceFragment, char * sourceVertex)
{	
	// проверка поддержки шейдеров ARB
	IsOK = CheckingSupportShader();

	if(!IsOK)
		return;

	const char* VertexSource = NULL;
	const char* FragmentSource = NULL;	
	
	VertexSource = ReadFile(sourceVertex);
	FragmentSource = ReadFile(sourceFragment);

	if (VertexSource == NULL || FragmentSource == NULL)
	{
		delete[] VertexSource;
		delete[] FragmentSource;
		VertexSource = NULL;
		FragmentSource = NULL;
		IsOK = false;
		return;
	}
	// передали указателям нужную область памяти
	vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragment_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);	

	// создание объекта программы, контейнер для вершиннго и фрагментного шейдера
	program = glCreateProgramObjectARB();	

	// присоединение объекта прораммы к шейдерам
	glAttachObjectARB(program, vertex_shader);
	glAttachObjectARB(program, fragment_shader);

	// загрузка кода в шейдерный объект
	glShaderSourceARB(vertex_shader, 1, &VertexSource, NULL);							
	glShaderSourceARB(fragment_shader, 1, &FragmentSource, NULL);

	// компиляция шейдера
	glCompileShaderARB(vertex_shader);
	glCompileShaderARB(fragment_shader);	

	// проверка компиляции шейдеров 
	IsOK = CheckingCompilingShader();		

	// соединение объектной прораммы
	glLinkProgramARB(program);	

	glUseProgramObjectARB(program);			
	LoadUniform();

	
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

// возвращаем текущее значение програмного объекта
int Shader::GetCurrentProgramObj(void)
{
	int tmp = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
	return tmp;
}

// устанавливаем нужное значение програмного объекта
void Shader::SetProgramObj(int tmp)
{
	glUseProgramObjectARB(tmp);		
}

bool Shader::CheckingCompilingShader()
{
	GLhandleARB compiled;
	// проверка компилирования вершиннго шейдера (кода)
	glGetObjectParameterivARB(vertex_shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (compiled == 0)
	{	   
	   return false; // в случае ошибки
	}   

	// проверка компилирования фрагментного шейдера (кода)
	glGetObjectParameterivARB(fragment_shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	if (compiled == 0)
	{
		return false; // в случае ошибки
	} 
	return true;
}

bool Shader::CheckingSupportShader()
{
	// проверка на поддержку фрагментных шейдеров
	if(!GLEE_ARB_fragment_program)
	{
		return false; // в случае ошибки
	}

	// проверка на поддержку вершинных шейдеров
	if(!GLEE_ARB_vertex_program)
	{
		return false; // в случае ошибки
	}	
	return true;
}

void Shader::ShaderON()
{
 	if (IsOK && !Disabled)
 	{  
 		// включение шейдера
 #ifdef _DEBUG
 		glValidateProgramARB(program);
 		int Status;
 		glGetObjectParameterivARB(program, GL_OBJECT_VALIDATE_STATUS_ARB, &Status);
 		char buf[1001];
 		glGetInfoLogARB(program, 1000, NULL, buf);
 #endif
 		glUseProgramObjectARB(program);			
  	}
}


void Shader::ShaderOFF()
{
	// выключение шейдера это значит ставим шейдер по умолчанию 
	glUseProgramObjectARB(0);	
}

void Shader::DeleteShader()
{
	if(!IsOK)
		return;
	
	// отсоединяем шейдерные объекты от програмного
	if(program != 0)
	{
		glDetachObjectARB(program,fragment_shader);
		glDetachObjectARB(program,vertex_shader);
	}

	// тут можно вставить проверку на отсоединение шейдерных объектов, если надо 

	// удаление шейдерных объектов после использования
	if(fragment_shader > 0)
		glDeleteObjectARB(fragment_shader);
	fragment_shader = 0;
	if(vertex_shader > 0)
		glDeleteObjectARB(vertex_shader);
	vertex_shader = 0;
	// освобождение програмных объктов после использования
	if(program > 0)
		glDeleteObjectARB(program);
	program = 0;
}


char* Shader::ReadFile(char* fn)
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

void Shader::Disable(int numShader)
{
	Disabled = true;
	ShaderOFF();
}

void Shader::Enable(int numShader)
{
	Disabled = false;
	ShaderON();
}

void Shader::SaveState(void)
{
	currentState = GetCurrentProgramObj();
}

void Shader::RestoreState(void)
{
	SetProgramObj(currentState);
	currentState = 0;
}

PhongShader::PhongShader() : Shader()
{
}

PhongShader::~PhongShader(void)
{ 
}
GlitShader::GlitShader() : Shader()
{
}

GlitShader::~GlitShader(void)
{ 
}

void GlitShader::InitShader(char* path, char * sourceFragment, char * sourceVertex)
{	
	// делаем рабочей текстуру 2д 
//	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);


	if(flag == 0)
	{
		glGenTextures(TEX_COUNT,textureNum);
		flag = 1;
	}

	const char *szCubeFaceDiffuse[] = {"diffuse_c00.bmp","diffuse_c01.bmp","diffuse_c02.bmp",
		"diffuse_c03.bmp","diffuse_c04.bmp","diffuse_c05.bmp"};

	const char *szCubeFaceSpecular[] = {"specular_c00.bmp","specular_c01.bmp","specular_c02.bmp",
		"specular_c03.bmp","specular_c04.bmp","specular_c05.bmp"};
	
	// загружаем текстуры в ожл
	LoadTexDDS(TEX_FIRST,szCubeFaceDiffuse, path);
	LoadTexDDS(TEX_SECOND,szCubeFaceSpecular, path);	

	Shader::InitShader(path, sourceFragment, sourceVertex);


}

bool PhongShader::LoadUniform(void)
{
	// узнаем текущий програмный объект
	int currentState = GetCurrentProgramObj();

	// проверяем на ошибки и выставляем нужные значения переменным
	if (IsOK != true)
		return true;
	int cprog = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &cprog);
	
	if(cprog <= 0)
		return false;
			
	GLhandleARB fvLightPosition = glGetUniformLocation(currentState, "fvLightPosition");
	if (fvLightPosition >= 0)
	{		
		glUniform3fARB(fvLightPosition,0.0,0.0,1000.0);
	}

	GLhandleARB fvEyePosition = glGetUniformLocation(currentState, "fvEyePosition");
	if (fvEyePosition >= 0)
	{		
		glUniform3fARB(fvEyePosition,0.0,0.0,1000.0 );
	}
	return true;
}

bool GlitShader::LoadUniform(void)
{
	int currentState = GetCurrentProgramObj();

	// проверяем на ошибки и выставляем нужные значения переменным
	if (IsOK != true)
		return true;
	int cprog = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &cprog);
	
	if(cprog <= 0)
		return false;

	// изменяем положение источника света для текстуры
	GLhandleARB move = glGetUniformLocation(currentState, "move");
	if (move >= 0)
	{		
		glUniform3fARB(move,50.0,50.0,50.0);
	}


	GLhandleARB fvSpecular = glGetUniformLocation(currentState, "fvSpecular");
	if (fvSpecular >= 0)
	{		
		glUniform4fARB(fvSpecular,1.0,1.0,1.0,1.0);
	}

	GLhandleARB fvDiffuse = glGetUniformLocation(currentState, "fvDiffuse");
	if (fvDiffuse >= 0)
	{		
		glUniform4fARB(fvDiffuse,0.8f,0.8f,0.8f,1.0f);
	}

	GLhandleARB BaseColor = glGetUniformLocation(currentState, "BaseColor");
	if (BaseColor >= 0)
	{		
		glUniform3fARB(BaseColor,0.08f,0.08f,0.08f );
	}

	GLhandleARB SpecularPercent = glGetUniformLocation(currentState, "SpecularPercent");
	if (SpecularPercent >= 0)
	{		
		glUniform1fARB(SpecularPercent,1.0);
	}

	GLhandleARB roughness_value = glGetUniformLocation(currentState, "roughness_value");
	if (roughness_value >= 0)
	{		
		glUniform1fARB(roughness_value,0.1000f);
	}

	GLhandleARB DiffusePercent = glGetUniformLocation(currentState, "DiffusePercent");
	if (DiffusePercent >= 0)
	{		
		glUniform1fARB(DiffusePercent,1.01f);
	}

	
	GLhandleARB SpecularEnvMap = glGetUniformLocationARB(currentState, "SpecularEnvMap");
	if (SpecularEnvMap >= 0)
	{		
		glUniform1i(SpecularEnvMap, 1);
	}

	GLhandleARB DiffuseEnvMap = glGetUniformLocationARB(currentState, "DiffuseEnvMap");
	if (DiffuseEnvMap >= 0)
	{		
		glUniform1i(DiffuseEnvMap, 0);
	}			

	return true;
}
void GlitShader::ShaderON()
{
 	if (IsOK && !Disabled)
 	{  
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureNum[TEX_SECOND]);

 		glActiveTexture(GL_TEXTURE0);// GL_TEXTURE0 must be the last active texture for the proper Map2d work on ATI	
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureNum[TEX_FIRST]);

		Shader::ShaderON();
 	}
}

