#pragma once
#include "OPENGLRENDER_API.h"

class OPENGLRENDER_API RGLSLProgramObj
{
// OpenGL shader program implementation class
public:
	RGLSLProgramObj(void);
	~RGLSLProgramObj(void);

	private:
		int program;
		int vertex_shader;
		int fragment_shader;
		bool Disabled;
		bool checkError;

	public:
		void InitShader (char* sourceFragment, char* sourceVertex);	
		char* ReadFile(char*);

	private:
		bool CheckingCompilingShader(void);
		bool CheckingSupportShader(void);

	public:
		void DeleteShader(void);
		void ShaderON(void);
		void Disable(void);
		void Enable(void);

		void SetUniform(char* name, float* val, int vector_size) const;
		void BindTexture2D(unsigned int target, char* tex_name, unsigned int tex_id, int tex_unit) const;
		bool GetCheckError() const;
		int GetMyProgram(void) const;
};
