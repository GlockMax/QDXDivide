#pragma once
#include "OPENGLRENDER_API.h"


typedef unsigned char GLubyte;
typedef signed int GLint;
typedef unsigned int GLuint;


#define TEX_COUNT 4


class OPENGLRENDER_API Shader
{
public:
	Shader(void);
	virtual ~Shader(void);		
	
private:
	 int program;
	 int vertex_shader;
	 int fragment_shader;	

protected:
	 bool Disabled;
	 bool IsOK;

public:
	static int GetCurrentProgramObj(void);
	static void SetProgramObj(int);
	void InitShader (char* path, char * sourceFragment, char * sourceVertex);	
	char* ReadFile(char*);	 
	
private:
	 bool CheckingCompilingShader(void);
	 bool CheckingSupportShader(void);

public:
	 void DeleteShader(void);
	 void ShaderON();
	 static void ShaderOFF();
	 void Disable(int numShader = 0);
	 void Enable(int numShader = 0);
	 static void SaveState(void);
	 static void RestoreState(void);

public: 
	virtual bool LoadUniform(void) = 0;
	bool LoadTexDDS(int numTexObj,const char* szCubeFace[], char* path);
//	void LoadTex(const char* path, int numTex);
	
	GLuint textureNum[TEX_COUNT];
	bool flag;
	float pos[3];
protected:
	static int currentState;

};

class OPENGLRENDER_API PhongShader : public Shader
{
public:
	PhongShader(void);
	~PhongShader(void);
	bool LoadUniform(void);
};

class OPENGLRENDER_API GlitShader : public Shader
{
public:
	GlitShader(void);
	~GlitShader(void);		
	void InitShader (char* path, char * sourceFragment, char * sourceVertex);	
	bool LoadUniform(void);
	void ShaderON();
};
