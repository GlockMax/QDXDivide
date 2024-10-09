/*
Module : Splasher.cpp
Purpose: A splash screen component for MFC which uses a DIB bitmap instead of a resource. Palette management 
         code is also included so that the bitmap will display using its own optimized palette. Also most
         importantly, the splash screen is shown in its own thread, meaning that it will redraw correctly
         while you app is performing it's startup code and not necessarily pumping the message queue.
Created: PJN / 15-11-1996
History: PJN / 11-12-1997 1) Incoporation of new DIB code provided by the authors own CDibImage class. A static library 
                          version is used by splasher here. If you want to use CDIbImage for anything other than 
                          CSplashThread, then you need to download CDibImage on its own.
                          2) Can now load from resource or bitmap
                          3) General tidy up of the code.
         PJN / 22-3-1998  1) Palette is now correctly initialised on startup
                          2) Code now protects itself if m_SplashScreen cannot be created
         PJN / 22-12-1998 1) Now no longer dependent on CDibImage.
                          2) Unicode Enabled the code,
                          3) General tidy up of the code
                          4) Removed the unnecessary variable m_bCreated
                          5) Fixed a potential race condition in CSplashThread::HideSplash()
         PJN / 01-03-2000 1) Fixed a problem with bitmaps which do not have a palette
                          2) Fixed a problem in Win 98 and Win2000 when the splash screen is closed but the main window 
                          of your app fails to activate. The code now uses AttachThreadInput to synchronise the UI 
                          activities of the main GUI thread and the splash screen thread.
         PJN / 01-01-2001 1) Now includes copyright message in the source code and documentation.
                          2) Modified the way the sample app waits for the thread to exit. It now synchronises with the 
                          exit of the splash screen thread
                          3) Now allows the icon of the splash screen to be specified
                          4) Now allows the splash screen window to be customized at run time through a virtual function 
                          "CSplashThread::CreateSplash". Thanks to Yury Goltsman for suggesting these enhancements.
         PJN / 27-08-2001 1) Splash screen now by default includes the window style WS_EX_TOPMOST
         PJN / 31-08-2001 1) Fixed an assert which was occurring when you brought up the splash window and closed it 
                          really quickly. Thanks to Wanner Robert for spotting this problem.
                          2) Removed a busy loop in HideSplash and used a Win32 event for synchronisation instead.
                          3) Implemented a class factory design pattern to further simplify the use of the splash screen 
                          class. All client code now needs to do is derive a class from CSplashWnd (making sure it's 
                          declared DYNCREATE) and at a minimum just call some of it's methods in the constructor to 
                          determine what it will display at runtime. Then just use CSplashFactory::Create with the name 
                          of your derived class. When you want to close the splash screen just call CSplashFactory::Close. 
                          What could be simpler!!!
         PJN / 03-10-2002 1) Now allows you to specify a transparent color to draw the splash screen with. This allows you 
                          to realize shapes other than the standard rectangle. Thanks to Gero Gerber for this update. Please 
                          note that this support is only available on Windows 2000 or later.
                          2) Made the destructor of CSplashWnd virtual
         PJN / 11-10-2002 1) Fixed up some coding issues reported by BoundsChecker in CSplashWnd::OnPaint.
                          2) Now supports a drop shadow effect on the splash screen (assuming that the client OS is 
                          Windows XP or later).
         PJN / 28-08-2003 1) Removed a number of level 4 warnings.
         PJN / 06-09-2003 1) Fixed a problem where you get a crash in the close method if the window to gain focus was NULL. 
                          Thanks to Darius Thabit for reporting this problem.
         PJN / 09-05-2004 1) Updated the copyright details
                          2) Included a new sample app in the download to demonstrate the correct usage of the splash screen in 
                          a dialog based app.
                          3) Fixed a bug where sometimes the splash screen component would get stuck while waiting for the splash 
                          screen thread to exit. The fix is to request the parent window to exit in CSplashWnd::OnClose. Thanks 
                          to Frederic Metrich for reporting this issue.
         PJN / 09-07-2006 1. Updated the code to clean compile on VC 2005.
                          2. Updated copyright details.  
                          3. Addition of CSPLASHER_EXT_CLASS macro to allow the classes to be easily added to an extension dll.
                          4. Code now uses newer C++ style casts instead of C style casts.
                          5. Optimized CSplashWnd constructor code
                          6. Optimized CSplashThread constructor code
                          7. Reviewed all TRACE statements for correctness.
                          8. UI thread which is created by the code is now created at idle priority
                          9. Updated the documentation to use the same style as the web site.
         PJN / 22-07-2007 1. Updated copyright details.
                          2. Optimized CSplashFactory constructor code
                          3. CSplashWnd code now uses WM_NCHITTEST logic to support a draggable splash screen
                          4. Made Create and Close methods in CSplashFactory virtual
                          5. CSplashFactory::Create now waits for the worker thread's InitInstance to complete
                          6. Removed unnecessary VERIFY call from CSplashWnd::Create
                          7. CSplashWnd::CreatePaletteFromBitmap now returns a BOOL
                          8. Removed the unnecessary CSplashWnd destructor
                          9. Removed the unnecessary CSplashThread destructor.
                          10. Reviewed all TRACE statements for correctness
                          11. Removed unnecessary use of friend classes
                          12. Updated sample apps to clean compile on VC2005
         PJN / 13-10-2007 1. CSplashFactory::Create now by default creates the splash screen thread at normal priority instead of 
                          idle priority. This helps avoid thread starvation issues when the main thread is busy performing its 
                          startup code. Please be aware of this issue if you modify your applications main thread priority to be 
                          anything other than THREAD_PRIORITY_NORMAL. If you do please make sure that the nPriority value to 
                          CSplashFactory::Create is >= the value you use for your main thread, or otherwise you will starve the 
                          splash screen of CPU cycles on single core processor. Thanks to Ralf Magnussen for reporting this issue.
         PJN / 17-11-2007 1. Fixed a memory leak where the CMySplashWnd instance was not being deleted in the worker thread. 
                          Thanks to Laurent Lherminé for reporting this bug.
         PJN / 17-05-2009 1. Updated copyright details
                          2. Removed VC 6 style class wizard comments from the code.
                          3. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          4. Updated CSplashWnd::SetTransparent to allow the alpha transparency level as well as the flags passed 
                          to SetLayeredWindowAttributes to be completed specified
                          5. Updated code to clean compile on VC 2008.
                          6. Code now compiles cleanly using Code Analysis (/analyze)
                          7. The code now only supports VC 2005 or later.
         PJN / 22-09-2013 1. Updated copyright details
                          2. Updated the code and sample app to clean compile on VC 2012
                          3. Updated CSplashWnd::LoadBitmap to use AfxFindResourceHandle when locating the bitmap. Thanks to Emeric 
                          Maschino for this nice addition.
         PJN / 18-01-2015 1. Updated copyright details.
                          2. Updated the code and sample app to clean compile on VC 2013
         PJN / 10-01-2015 1. Updated copyright details
                          2. Verified the code compiles cleanly on VC 2015.
                          3. Added SAL annotations to all the code.
         PJN / 29-09-2018 1. Updated copyright details
                          2. Fixed a number of C++ core guidelines compiler warnings. These changes mean that the code
                          will now only compile on VC 2017 or later.
                          3. Replaced NULL throughout the codebase with nullptr
                          4. Code no longer uses GetProcAddress to access SetLayeredWindowAttributes API
                          5. Reworked the code to use ATL::CImage for the splash screen bitmap
                          6. Updated the SetBitmapToUse method to include an instance handle parameter to load the image from
         PJN / 02-06-2019 1. Updated copyright details.
                          2. Updated the code to clean compile on VC 2019
         PJN / 21-09-2019 1. Fixed a number of compiler warnings when the code is compiled with VS 2019 Preview
         PJN / 26-12-2019 1. Fixed various Clang-Tidy static code analysis warnings in the code.
                          2. Replaced BOOL with bool throughout the code.
         PJN / 29-03-2020 1. Updated copyright details.
                          2. Fixed more Clang-Tidy static code analysis warnings in the code.

Copyright (c) 1996 - 2020 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


//////////////////// Includes /////////////////////////////////////////////////

#include "stdafx.h"
#include "Splasher.h"


//////////////////// Macros / Defines /////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////// Implementation ///////////////////////////////////////////

#pragma warning(suppress: 26433 26440)
BEGIN_MESSAGE_MAP(CSplashWnd, CWnd) //NOLINT(modernize-avoid-c-arrays)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_CLOSE()
  ON_WM_NCHITTEST()
END_MESSAGE_MAP()

#pragma warning(suppress: 26409 26433 26440 26477)
IMPLEMENT_DYNCREATE(CSplashWnd, CWnd);

#pragma warning(suppress: 26455)
CSplashWnd::CSplashWnd() : m_bOKToClose(false),
                           m_bUseFile(false),
                           m_pszResourceName(nullptr),
                           m_hInstance(nullptr),
                           m_hIcon(nullptr),
                           m_bTransparent(false),
                           m_clrTransparent(RGB(255, 0, 255)),
                           m_bDraggable(true),
                           m_bDropShadow(false),
                           m_dwSLWAFlags(0),
                           m_bSLWAAlpha(0)
{
}

#pragma warning(suppress: 26440)
HRESULT CSplashWnd::LoadBitmap()
{
  HRESULT hr = S_OK;
  if (m_bUseFile)
  {
    hr = m_Image.Load(m_sFilename);
    if (FAILED(hr))
      TRACE(_T("CSplashWnd::LoadBitmap, Failed to load image from disk\n"));
  }
  else
  {
    m_Image.LoadFromResource(m_hInstance, m_pszResourceName);
    if (m_Image.IsNull())
    {
      hr = E_FAIL;
      TRACE(_T("CSplashWnd::LoadBitmap, Failed to load image from resource\n"));
    }
  }
  return hr;
}

void CSplashWnd::SetTransparent(_In_ COLORREF clrTransparent, _In_ DWORD dwFlags, _In_ BYTE bAlpha) noexcept
{
  m_bTransparent = true;
  m_clrTransparent = clrTransparent;
  m_dwSLWAFlags = dwFlags;
  m_bSLWAAlpha = bAlpha;
}

bool CSplashWnd::CreatePaletteFromBitmap()
{
  //What will be the return value from this function (assume the worst)
  bool bSuccess = false;

  //Create a palette from the image if necessary
  if (m_Image.IsIndexed())
  {
    ATL::CHeapPtr<RGBQUAD> rgb;
    constexpr const WORD nColors = 256;
    if (rgb.Allocate(nColors))
    {
#pragma warning(suppress: 26486)
      m_Image.GetColorTable(0, nColors, rgb.m_pData);
      //Create a palette from the color table
      ATL::CHeapPtr<BYTE> palette;
      if (palette.Allocate(sizeof(LOGPALETTE) + (nColors * sizeof(PALETTEENTRY))))
      {
#pragma warning(suppress: 26489 26490)
        auto pLogPal = reinterpret_cast<LOGPALETTE*>(palette.m_pData);
#pragma warning(suppress: 26489)
        pLogPal->palVersion = 0x300;
#pragma warning(suppress: 26489)
        pLogPal->palNumEntries = nColors;
        for (UINT i=0; i<nColors; i++)
        {
#pragma warning(suppress: 26446 26481 26482 26489)
          pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
#pragma warning(suppress: 26446 26481 26482 26489)
          pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
#pragma warning(suppress: 26446 26481 26482 26489)
          pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
#pragma warning(suppress: 26446 26482 26489)
          pLogPal->palPalEntry[i].peFlags = 0;
        }
#pragma warning(suppress: 26486)
        bSuccess = m_Palette.CreatePalette(pLogPal);
        if (!bSuccess)
          TRACE(_T("CSplashWnd::CreatePaletteFromBitmap, Failed to create palette for splash screen\n"));
      }
      else
        TRACE(_T("CSplashWnd::CreatePaletteFromBitmap, Failed to allocate memory for palette\n"));
    }
    else
      TRACE(_T("CSplashWnd::CreatePaletteFromBitmap, Failed to allocate memory for color table\n"));

    if (!bSuccess) //just use a halftone palette as a fallback
    {
      CDC* pRefDC = GetDC();
      if (pRefDC != nullptr)
      {
#pragma warning(suppress: 26486)
        bSuccess = m_Palette.CreateHalftonePalette(pRefDC);
        if (!bSuccess)
          TRACE(_T("CSplashWnd::CreatePaletteFromBitmap, Failed to create halftone palette\n"));
        ReleaseDC(pRefDC);
      }
      else
        TRACE(_T("CSplashWnd::CreatePaletteFromBitmap, Failed to get DC to create halftone palette\n"));
    }
  }
  else
    bSuccess = true;

  return bSuccess;
}

HRESULT CSplashWnd::Create()
{
  //Load up the bitmap from file or from resource
  const HRESULT hr = LoadBitmap();
  if (FAILED(hr))
    return hr;

  //Modify the owner window of the splash screen to be an invisible WS_POPUP window so that the splash screen does not appear in the task bar
  LPCTSTR pszWndClassOwner = AfxRegisterWndClass(0, nullptr, nullptr, m_hIcon);
  if (!m_wndOwner.CreateEx(0, pszWndClassOwner, _T(""), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr))
  {
    TRACE(_T("CSplashWnd::Create, Failed to create owner window\n"));
    return ATL::AtlHresultFromLastError();
  }

  //Create the appropriate window class to use for the splash window
  LPCTSTR pszWndClass = nullptr;
  if (m_bDropShadow)
    pszWndClass = AfxRegisterWndClass(CS_DROPSHADOW, AfxGetApp()->LoadStandardCursor(IDC_ARROW), nullptr, m_hIcon);
  else
    pszWndClass = AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW), nullptr, m_hIcon);

  //Create this window
  if (!CreateEx(WS_EX_TOPMOST, pszWndClass, _T(""), WS_POPUP | WS_VISIBLE, 0, 0, m_Image.GetWidth(), m_Image.GetHeight(), m_wndOwner.GetSafeHwnd(), nullptr))
  {
    TRACE(_T("CSplashWnd::Create, Failed to create the splash screen\n"));
    return ATL::AtlHresultFromLastError();
  }

  //Make the window transparent if required
  if (m_bTransparent)
  {
    //Add the transparent window style
    if (!ModifyStyleEx(0, WS_EX_LAYERED))
    {
      TRACE(_T("CSplashWnd::Create, Failed in make the splash screen layered\n"));
      return ATL::AtlHresultFromLastError();
    }

    //Make the Window transparent using the specifed color
    if (!SetLayeredWindowAttributes(m_clrTransparent, m_bSLWAAlpha, m_dwSLWAFlags))
    {
      TRACE(_T("CSplashWnd::Create, Failed in make the splash screen transparent\n"));
      return ATL::AtlHresultFromLastError();
    }
  }

  //Create the palette, We need to do this after the window is created because
  //we may need to access the DC associated with it
  if (!CreatePaletteFromBitmap())
    return ATL::AtlHresultFromLastError();

  //realize the bitmap's palette into the DC
  OnQueryNewPalette();

  return S_OK;
}

#pragma warning(suppress: 26434)
int CSplashWnd::OnCreate(_In_ LPCREATESTRUCT lpCreateStruct)
{
  //Let the parent class do its thing
  if (__super::OnCreate(lpCreateStruct) == -1)
    return -1;

  //Center the splash window on the screen
  //CenterWindow();
  CRect Rect;
  GetWindowRect(&Rect);
  Rect.MoveToXY(200, 200);
  MoveWindow(Rect);

  return 0;
}

#pragma warning(suppress: 26434)
void CSplashWnd::OnPaint()
{
  CPaintDC dc(this);
  CPalette* pOldPalette = nullptr;
  if (m_Palette.m_hObject != nullptr)
    pOldPalette = dc.SelectPalette(&m_Palette, FALSE);
  dc.RealizePalette();
#pragma warning(suppress: 26486)
  m_Image.BitBlt(dc.m_hDC, 0, 0, SRCCOPY);
  if (pOldPalette != nullptr)
    dc.SelectPalette(pOldPalette, FALSE);
}

#pragma warning(suppress: 26434)
void CSplashWnd::OnClose()
{
  if (m_bOKToClose)
  {
    //Just request the parent window to close which will cause this window to be destroyed
    m_wndOwner.PostMessage(WM_CLOSE);
  }
}

bool CSplashWnd::SelRelPal(_In_ bool bForceBkgnd)
{
  //We are going active, so realize our palette.
  CDC* pDC = GetDC();
  if (pDC == nullptr)
    return false;

  CPalette* pOldPal = pDC->SelectPalette(&m_Palette, bForceBkgnd);
  const UINT nRemappedColors = pDC->RealizePalette();
  pDC->SelectPalette(pOldPal, TRUE);
  pDC->RealizePalette();
#pragma warning(suppress: 26486)
  ReleaseDC(pDC);

  //If any colors have changed or we are in the
  //background, repaint the lot.
  if (nRemappedColors || bForceBkgnd)
    InvalidateRect(nullptr, TRUE);

  return (nRemappedColors != 0); //true if some colors changed
}

#pragma warning(suppress: 26434 26461)
void CSplashWnd::OnPaletteChanged(_In_ CWnd* pFocusWnd)
{
  //See if the change was caused by us and ignore it if not
  if (pFocusWnd != this)
    SelRelPal(true); //Realize in the background
}

#pragma warning(suppress: 26434)
BOOL CSplashWnd::OnQueryNewPalette()
{
  return SelRelPal(false); //Realize in the foreground
}

void CSplashWnd::SetBitmapToUse(_In_ const CString& sFilename)
{
  m_bUseFile = true;
  m_sFilename = sFilename;
}

void CSplashWnd::SetBitmapToUse(_In_ UINT nResourceID, _In_opt_ HINSTANCE hInstance)
{
  m_bUseFile = false;
  m_pszResourceName = MAKEINTRESOURCE(nResourceID);
  m_hInstance = hInstance;
}

void CSplashWnd::SetBitmapToUse(_In_ LPCTSTR pszResourceName, _In_opt_ HINSTANCE hInstance)
{
  m_bUseFile = false;
  m_pszResourceName = pszResourceName;
  m_hInstance = hInstance;
}

#pragma warning(suppress: 26434)
void CSplashWnd::SetIcon(_In_ HICON hIcon) noexcept
{
  //Destroy the icon if we have one already loaded
  if (m_hIcon != nullptr)
    DestroyIcon(m_hIcon);

  m_hIcon = hIcon;
}

#pragma warning(suppress: 26434)
LRESULT CSplashWnd::OnNcHitTest(_In_ CPoint point)
{
  if (m_bDraggable)
    return HTCAPTION;
  else
    return __super::OnNcHitTest(point);
}


#pragma warning(suppress: 26433 26440)
BEGIN_MESSAGE_MAP(CSplashThread, CWinThread) //NOLINT(clang-diagnostic-unused-local-typedef, modernize-avoid-c-arrays)
END_MESSAGE_MAP()

#pragma warning(suppress: 26409 26433 26440 26477)
IMPLEMENT_DYNCREATE(CSplashThread, CWinThread)

#pragma warning(suppress: 26455)
CSplashThread::CSplashThread() : m_pRuntimeClassSplashWnd(nullptr),
                                 m_bInitOK(false),
                                 m_pSplashScreen(nullptr)
{
}

BOOL CSplashThread::InitInstance()
{
  //Validate our parameters
#pragma warning(suppress: 26496)
  AFXASSUME(m_pRuntimeClassSplashWnd != nullptr);

  //Create the splash screen window object
#pragma warning(suppress: 26466)
  m_pSplashScreen = static_cast<CSplashWnd*>(m_pRuntimeClassSplashWnd->CreateObject());
  m_pMainWnd = m_pSplashScreen;
  if (m_pSplashScreen == nullptr)
  {
    TRACE(_T("CSplashThread::InitInstance, Failed to instantiate CSplashWnd object\n"));
    m_evtInitInstanceRun.SetEvent();
    return FALSE;
  }

  //Validate that it is derived from CSplashWnd
  ASSERT(m_pSplashScreen->IsKindOf(RUNTIME_CLASS(CSplashWnd)));

  //Create the HWND of it
  const HRESULT hr = m_pSplashScreen->Create();
  if (FAILED(hr))
  {
    TRACE(_T("CSplashThread::InitInstance, Failed to create CSplashWnd object\n"));

    //Tidy up the heap memory we have allocated
    delete m_pSplashScreen;
    m_pSplashScreen = nullptr;

    m_evtInitInstanceRun.SetEvent();
    return FALSE;
  }

  //Signal the event which indicates that this InitInstance function has run
  m_bInitOK = true;
  m_evtInitInstanceRun.SetEvent();

  return TRUE;
}

int CSplashThread::ExitInstance()
{
  //Free up our splash screen instance if created
  if (m_pSplashScreen != nullptr)
  {
    delete m_pSplashScreen;
    m_pSplashScreen = nullptr;
  }

  //Let the base class do its thing
  return __super::ExitInstance();
}


#pragma warning(suppress: 26455)
CSplashFactory::CSplashFactory() : m_pSplashThread(nullptr)
{
}

CSplashFactory::~CSplashFactory()
{
  //Just incase client code forgot to call Close, we do it anyway
#pragma warning(suppress: 26447)
  Close();
}

bool CSplashFactory::Create(_In_ CRuntimeClass* pRuntimeClassSplashWnd, _In_ int nPriority)
{
  //Validate our parameters
  ASSERT(m_pSplashThread == nullptr);
  ASSERT(pRuntimeClassSplashWnd != nullptr);

  //Create the GUI worker thread
#pragma warning(suppress: 26466)
  m_pSplashThread = static_cast<CSplashThread*>(AfxBeginThread(RUNTIME_CLASS(CSplashThread), nPriority, 0, CREATE_SUSPENDED));
  if (m_pSplashThread == nullptr)
  {
    TRACE(_T("CSplashFactory::Create, Failed to create splash screen thread\n"));
    return false;
  }

  ASSERT(m_pSplashThread->IsKindOf(RUNTIME_CLASS(CSplashThread)));
  m_pSplashThread->m_pRuntimeClassSplashWnd = pRuntimeClassSplashWnd;
  m_pSplashThread->m_bAutoDelete = FALSE; //We are in charge of the deletion of the thread
  m_pSplashThread->ResumeThread(); //Resume the thread now that we have set it up 

  //Wait until the thread's InitInstance code has completed. Note this is important because code in CSplashFactory::Close
  //can execute after Create returns and we want to ensure that "m_pSplashThread->m_pSplashScreen" is not modified from
  //the worker thread while we access it from the main UI thread
  WaitForSingleObject(m_pSplashThread->m_evtInitInstanceRun, INFINITE);
  
  //Get the return value from InitInstance
  return m_pSplashThread->m_bInitOK;
}

bool CSplashFactory::Close(_In_opt_ CWnd* pWndToGainFocus)
{
  //What will be the return value from this function (assume the worst)
  bool bSuccess = false;

  if (m_pSplashThread != nullptr)
  {
    //Attach this threads UI state to the worker threads one, This will ensure that
    //the activation state is managed consistenly across the two threads
    ASSERT(AfxGetApp() != nullptr);
    AttachThreadInput(AfxGetApp()->m_nThreadID, m_pSplashThread->m_nThreadID, TRUE);

    //Set the focus back to the correct window
    CWnd* pWndFocus = pWndToGainFocus;
    if (pWndFocus == nullptr)
      pWndFocus = AfxGetMainWnd();
    if (pWndFocus != nullptr)
    {
      pWndFocus->SetFocus();
      pWndFocus->SetForegroundWindow();
    }

    //If the worker thread has a splash screen then ask it to close before trying to close it. See comment in 
    //CSplashFactory::Create about why can can safely dot into a pointer set in another thread
    if (m_pSplashThread->m_pSplashScreen)
    {
      m_pSplashThread->m_pSplashScreen->SetOKToClose();
      m_pSplashThread->m_pSplashScreen->PostMessage(WM_CLOSE);
    }

    //Wait for the worker thread to exit and tidy up its memory
    WaitForSingleObject(m_pSplashThread->m_hThread, INFINITE);
    delete m_pSplashThread;
    m_pSplashThread = nullptr;

    bSuccess = true;
  }

  return bSuccess;
}
