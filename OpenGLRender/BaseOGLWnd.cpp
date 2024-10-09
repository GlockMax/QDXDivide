// BaseOGLWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ResInv.h"
#include "NCMVersion.h"
#include "NCMProject.h"
#include "BaseRender.h"
#include "BaseOGLWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
Text BaseOGLWnd::MainText(1256);
BaseOGLWnd *BaseOGLWnd::pGlobalOGLWnd = NULL;
// BaseOGLWnd

IMPLEMENT_DYNAMIC(BaseOGLWnd, CWnd)

BaseOGLWnd::BaseOGLWnd()
{
	m_pDC = NULL;
	m_hrc = NULL;
	PixelFormat = 0;
	LineSmooth = true;
}

BaseOGLWnd::~BaseOGLWnd()
{
}


BEGIN_MESSAGE_MAP(BaseOGLWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// BaseOGLWnd message handlers



int BaseOGLWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(pGlobalOGLWnd == NULL)
	{
		pGlobalOGLWnd = CreateGlobal();
		if(pGlobalOGLWnd == NULL)
			return -1;
		MainText.Init(wglGetCurrentDC(), USEFONTBITMAPS, "arial.ttf");
	}
	return 0;
}

BaseOGLWnd * BaseOGLWnd::CreateGlobal(void)
{
	Start();
	if(!pGlobalOGLWnd)
		return NULL;

	if(!pGlobalOGLWnd->InitOGLGlobal())
		return NULL;
	// Multisample support
	bool multisampleOK = true;
	if(NCM_PROJECT.Defaults.GetBool("Defaults/Render/Multisample@Enable", false))
	{
		int NewPixelFormat = 0;
		bool arbMultisampleSupported = BaseRender::IsMultisampleSupported(pGlobalOGLWnd->m_pDC->GetSafeHdc(), &NewPixelFormat);
		multisampleOK = arbMultisampleSupported;
		if(arbMultisampleSupported && NewPixelFormat > 0)
		{// Recreation needed
			wglMakeCurrent(pGlobalOGLWnd->m_pDC->GetSafeHdc(), 0);
			Finish();
			Start();
			if(!pGlobalOGLWnd)
				return nullptr;
			pGlobalOGLWnd->PixelFormat = NewPixelFormat;
			pGlobalOGLWnd->Init();
		}
	}
	bool shadersOK = pGlobalOGLWnd->InitShaders();
	bool vboOK = BaseRender::IsVBOEnabled();
	BaseRender::SetOGLDiag(multisampleOK, shadersOK, vboOK);

	return pGlobalOGLWnd;
}

bool BaseOGLWnd::InitShaders()
{
#ifndef SHARED_HANDLERS
	NTiParams &Par = NCM_PROJECT.Defaults;
	if(Par.GetBool("Defaults/Render/Shaders@Enable", false))
	{
		BaseRender::PhShader.InitShader(NCM_PROJECT.GetPrototypesPath().GetBuffer(), (NCM_PROJECT.GetPrototypesPath() + _T("Phong.frag")).GetBuffer(), (NCM_PROJECT.GetPrototypesPath() + _T("Phong.vert")).GetBuffer());
		BaseRender::GlShader.InitShader(NCM_PROJECT.GetPrototypesPath().GetBuffer(), (NCM_PROJECT.GetPrototypesPath() + _T("Glit.frag")).GetBuffer(), (NCM_PROJECT.GetPrototypesPath() + _T("Glit.vert")).GetBuffer());

		BaseRender::TranslucentPr.initProgram((NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_init.frag")).GetBuffer(),
													(NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_init.vert")).GetBuffer(), 0);
		BaseRender::TranslucentPr.initProgram((NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_peel.frag")).GetBuffer(),
													(NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_peel.vert")).GetBuffer(), 1);
		BaseRender::TranslucentPr.initProgram((NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_blend.frag")).GetBuffer(),
													(NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_blend.vert")).GetBuffer(), 2);
		BaseRender::TranslucentPr.initProgram((NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_final.frag")).GetBuffer(),
													(NCM_PROJECT.GetPrototypesPath() + _T("dual_depth_peeling/dual_peeling_final.vert")).GetBuffer(), 3);
	}
#endif
	return true;
}

bool BaseOGLWnd::InitOGLGlobal()
{
	delete m_pDC;
	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);
	m_pDC->SetMapMode(MM_TEXT);

	static PIXELFORMATDESCRIPTOR pfd;
	int MaxPixelFormat = ::DescribePixelFormat(m_pDC->GetSafeHdc(), 1, sizeof(pfd), &pfd);

	if( MaxPixelFormat < 1)
	{
		if(!SetDefaultPixelFormat())
			exit(1);
	}
	else
	{
		int *Allowable = new int[MaxPixelFormat];
		int AllN = 0;
		for(int i = 1 ; i <= MaxPixelFormat; ++i)
		{
			::DescribePixelFormat(m_pDC->GetSafeHdc(), i, sizeof(pfd), &pfd);
			//if(! (pfd.dwFlags & PFD_GENERIC_ACCELERATED ))
			//	continue;
			if(! (pfd.dwFlags & PFD_DRAW_TO_WINDOW ))
				continue;
			//if(! (pfd.dwFlags & PFD_SUPPORT_COMPOSITION ))
			//	continue;
			if(! (pfd.dwFlags & PFD_SWAP_COPY))
				continue;
			if(! (pfd.dwFlags & PFD_DOUBLEBUFFER ))
				continue;
			if(! (pfd.iPixelType == PFD_TYPE_RGBA))
				continue;
			if( pfd.cColorBits < 32 )
				continue;
			if( pfd.cDepthBits < 24 )
				continue;
			if( pfd.cAccumBits < 32 )
				continue;
			if( pfd.cStencilBits < 8 )
				continue;

			Allowable[AllN++] = i;
		}

		int StoredPixelFormat = -1;
#ifndef SHARED_HANDLERS
		StoredPixelFormat = AfxGetApp()->GetProfileInt(NCMVersion, _T("PixelFormat"), 0);
#endif

		PixelFormat = Allowable[0];
		for( int k = AllN - 1; k >= 0; --k)
		{
			::DescribePixelFormat(m_pDC->GetSafeHdc(), Allowable[k], sizeof(pfd), &pfd);
			if( pfd.dwFlags & PFD_SUPPORT_COMPOSITION )
			{
				PixelFormat = Allowable[k];
			}
			if(Allowable[k] == StoredPixelFormat)
			{
				PixelFormat = StoredPixelFormat;
				break;
			}
		}

		delete[] Allowable;

		if(AllN < 1 || PixelFormat < 1)
		{
			if(!SetDefaultPixelFormat())
				exit(1);
		}
		if (SetPixelFormat(m_pDC->GetSafeHdc(), PixelFormat, &pfd) == FALSE)
		{
			MessageBox(_T("InitOGL: SetPixelFormat failed"));
			exit(1);
		}
		
	}
	m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
	BOOL res = wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

	return true;
}

void BaseOGLWnd::Init()
{
	delete m_pDC;
	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);
	m_pDC->SetMapMode(MM_TEXT);

	PIXELFORMATDESCRIPTOR pfd;
	PixelFormat = pGlobalOGLWnd->PixelFormat;
	if (SetPixelFormat(m_pDC->GetSafeHdc(), PixelFormat, &pfd) == FALSE)
	{
		MessageBox(_T("Init: SetPixelFormat failed"));
		exit(1);
	}

	m_hrc = wglCreateContext(m_pDC->GetSafeHdc());
	BOOL res = wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);
	NTiParams &Par = NCM_PROJECT.Defaults;
	if(this != pGlobalOGLWnd)
	{
		wglShareLists(pGlobalOGLWnd->m_hrc, m_hrc);
		// Next code is placed here to prevent the light source
		// position changing while view point changes
		//float PS[4] = {-0.58f, 0.58f, 0.58f, 0.f};
		float PS[4] = {-0.f, 0.f, 1.0f, 0.f};
		glLightfv(GL_LIGHT0, GL_POSITION, PS);

		if(!Par.GetBool("Defaults/Render/LineSmooth@Enable", true))
			LineSmooth = false;
	}
}

bool BaseOGLWnd::SetDefaultPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_DOUBLEBUFFER |            // double buffered
		  PFD_SUPPORT_OPENGL|           // support OpenGL
		  PFD_GENERIC_FORMAT |
		  PFD_SWAP_COPY,					//copy back buf to front(hint)
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		32,                              // 32-bit accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		8,                              // 8-bit stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	if ( (PixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), PixelFormat, &pfd) == FALSE)
	{
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}

void BaseOGLWnd::Finish()
{
	if(pGlobalOGLWnd)
	{
		pGlobalOGLWnd->DestroyWindow();
		delete pGlobalOGLWnd;
	}
}

void BaseOGLWnd::Start()
{
	pGlobalOGLWnd = new BaseOGLWnd;
	if(pGlobalOGLWnd == NULL)
		return;

	if(!pGlobalOGLWnd->CreateEx( WS_EX_CLIENTEDGE, NULL,   //CWnd default
				NULL,   //has no name
				WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				CRect(0, 0, 1, 1),
				GetParent(),   
				ID_GLOBALOGLWND)) 
	{
		delete pGlobalOGLWnd;
		pGlobalOGLWnd = NULL;
	}
}

void BaseOGLWnd::OnDestroy()
{
	CWnd::OnDestroy();

	HGLRC   hrc;
	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL,  NULL);

	if (m_hrc) 
		::wglDeleteContext(m_hrc); //Crashes on ATI after using GlShader

	m_hrc = NULL;
	delete m_pDC;
	m_pDC = NULL;

}

bool Text::Init(HDC hDC, int pmode, char* filename)
{
	mode = pmode;
	if (mode == USEFONTBITMAPS)
	{
		AddFontResourceA(filename);
		HFONT font =
			CreateFontA(-height,		// logical height of font
				width,					// logical average character width
				0,						// angle of escapement
				0,						// base-line orientation angle
				FW_BOLD,				// font weight
				false,					// italic attribute flag
				false,					// underline attribute flag
				false,					// strikeout attribute flag
				RUSSIAN_CHARSET,		// character set identifier
				OUT_TT_PRECIS,			// output precision
				CLIP_DEFAULT_PRECIS,		// clipping precision
				ANTIALIASED_QUALITY,		// output quality
				FF_DONTCARE | DEFAULT_PITCH, // pitch and family 
				"OGL Font"			// pointer to typeface name string
			);
		SelectObject(hDC, font);
		wglUseFontBitmaps(hDC, 0, 256, listBase);
		RemoveFontResourceA(filename);
		DeleteObject(font);
		m_hDC = hDC;
	}
	return true;
}

void Text::Draw3D(double x, double y, double z, const char* string) const
{
	glListBase(listBase);
	glRasterPos3d(x, y, z);
	glCallLists(GLsizei(strlen(string)), GL_UNSIGNED_BYTE, string);
}

void Text::Draw2D(double x, double y, int halign, double wndWidth,
	double wndHeight, char* string)
{
	align = halign;
	glPushMatrix();
	glLoadIdentity();
	int LengthOfStrinf = int(strlen(string));
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	if (mode == USEFONTBITMAPS)
	{
		GetTextExtentPoint32A(m_hDC, string, LengthOfStrinf, &strwidth);
		switch (align)
		{
		case TEXTALIGNRIGHT:
			glRasterPos2d(x, y);
			break;
		case TEXTALIGNLEFT:
			glRasterPos2d(x - strwidth.cx / wndWidth, y);
			break;
		case TEXTALIGNCENTER:
			glRasterPos2d(x - (strwidth.cx / 2.0) / wndWidth, y);
			break;
		default:
			glRasterPos2d(x, y);
			break;
		}
		glCallLists(LengthOfStrinf, GL_UNSIGNED_BYTE, string);
	}
	glPopMatrix();
}

