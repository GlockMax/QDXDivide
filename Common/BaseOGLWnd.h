#pragma once
#include "OPENGLRENDER_API.h"
#include "glText.h"

// BaseOGLWnd

class OPENGLRENDER_API BaseOGLWnd : public CWnd
{
	DECLARE_DYNAMIC(BaseOGLWnd)

public:
	BaseOGLWnd();
	virtual ~BaseOGLWnd();
	void Start();
	static void Finish(void);

protected:
	DECLARE_MESSAGE_MAP()
	static BaseOGLWnd * pGlobalOGLWnd;
	void Init();
	bool SetDefaultPixelFormat();
protected:
	CClientDC   *m_pDC;
	HGLRC		m_hrc; 	//OpenGL Rendering Context
	static Text MainText;
private:
	int PixelFormat;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BaseOGLWnd * CreateGlobal(void);
	bool InitOGLGlobal(void);
	bool InitShaders(void);
protected:
	bool LineSmooth;
public:
	afx_msg void OnDestroy();
};


