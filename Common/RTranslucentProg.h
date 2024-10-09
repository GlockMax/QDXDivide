#pragma once
#include "RGLSLProgramObj.h"
#include "OPENGLRENDER_API.h"


class OPENGLRENDER_API RTranslucentProg
{
public:
	enum {	INIT_PROGRAM = 0,
			PEEL_PROGRAM = 1,
			BLEND_PROGRAM = 2,
			FINAL_PROGRAM = 3};

	enum PutCode
	{
		PUT_BEFORE,
		PUT_AFTER
	};
// members
private:
	static const int	MAX_COLOR_ATTACHMENTS = 4;
	static const float	MAX_DEPTH;
	static const bool	USE_OQ = true;

	RGLSLProgramObj initProg;
	RGLSLProgramObj peelProg;
	RGLSLProgramObj blendProg;
	RGLSLProgramObj finalProg;

	unsigned int dualFboId[2];
	unsigned int dualDepthTexId[2];
	unsigned int dualFrontBlenderTexId[2];
	unsigned int dualBackTempTexId[2];
	unsigned int dualBackBlenderTexId;
	unsigned int dualOpaqueObjectsDepthTexId;

	unsigned int RenderTargets[MAX_COLOR_ATTACHMENTS];
	unsigned int queryId;
	unsigned int quadDisplayList;

	float alpha;

	// if smth goes wrong with shaders
	bool initProgComplete;

// **
public:
	RTranslucentProg();
	~RTranslucentProg();

// initialization
public:
	void initProgram(char* frag_src, char* vertex_src, unsigned int program_num);
	void deleteProgram(void);
private:
	void initFBO();
	void makeFullScreenQuad();	

// **
public:
	void setAlpha(float alpha_value);
	bool getOQFlag() const;
	bool isProgramComplete();
	
	void destroyFBO();

// draw with translucent
public:
	bool prepareI(void);
	bool prepareII(PutCode put_code);
	bool prepareIII(PutCode put_code, int curr_id, int prev_id, bool* are_samples_remained);
	bool prepareIV(int curr_id, int prev_id);
};