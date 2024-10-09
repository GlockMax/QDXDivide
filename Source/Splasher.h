/*
Module : Splasher.h
Purpose: A splash screen component for MFC which uses a DIB bitmap instead of a resource. Palette management 
         code is also included so that the bitmap will display using its own optimized palette. Also most
         importantly, the splash screen is shown in its own thread, meaning that it will redraw correctly
         while you app is performing it's startup code and not necessarily pumping the message queue.
Created: PJN / 15-11-1996

Copyright (c) 1996 - 2020 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////// Macros / Defines ////////////////////////////////////

#pragma once

#ifndef __SPLASHER_H__
#define __SPLASHER_H__

#ifndef CSPLASHER_EXT_CLASS
#define CSPLASHER_EXT_CLASS
#endif //#ifndef CSPLASHER_EXT_CLASS

#ifndef CSPLASHER_EXT_API
#define CSPLASHER_EXT_API
#endif //#ifndef CSPLASHER_EXT_API


///////////////////////// Includes ////////////////////////////////////////////

#ifndef __AFXMT_H__
#pragma message("To avoid this message, please put afxmt.h in your pre compiled header (normally stdafx.h)")
#include <afxmt.h>
#endif //#ifndef __AFXMT_H__


#ifndef __ATLIMAGE_H__
#pragma message("To avoid this message, please put atlimage.h in your pre compiled header (normally stdafx.h)")
#include <atlimage.h>
#endif //#ifndef __ATLIMAGE_H__


///////////////////////// Classes /////////////////////////////////////////////

class CSPLASHER_EXT_CLASS CSplashWnd : public CWnd
{
public:
//Constructors / Destructors
  CSplashWnd();

protected:
//Virtual methods
  virtual HRESULT Create(); //NOLINT(clang-diagnostic-overloaded-virtual)
  virtual HRESULT LoadBitmap();

//Misc methods
  void SetBitmapToUse(_In_ const CString& sFilename);
  void SetTransparent(_In_ COLORREF clrTransparent, _In_ DWORD dwFlags, _In_ BYTE bAlpha) noexcept;
  void SetDropShadow(_In_ bool bDropShadow = true) noexcept {  m_bDropShadow = bDropShadow; };
  void SetBitmapToUse(_In_ UINT nResourceID, _In_opt_ HINSTANCE hInstance = AfxGetResourceHandle());
  void SetBitmapToUse(_In_ LPCTSTR pszResourceName, _In_opt_ HINSTANCE hInstance = AfxGetResourceHandle());
  void SetOKToClose() noexcept { m_bOKToClose = true; };
  void SetIcon(_In_ HICON hIcon) noexcept;
  bool SelRelPal(_In_ bool bForceBkgnd);
  bool CreatePaletteFromBitmap();
  void SetDraggable(_In_ bool bDraggable) noexcept { m_bDraggable = bDraggable; };

//Message handlers
  afx_msg int OnCreate(_In_ LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnPaint();
  afx_msg void OnClose();
  afx_msg void OnPaletteChanged(_In_ CWnd* pFocusWnd);
  afx_msg BOOL OnQueryNewPalette();
  afx_msg LRESULT OnNcHitTest(_In_ CPoint point);

  DECLARE_MESSAGE_MAP()

  DECLARE_DYNCREATE(CSplashWnd);
 
//Member variables
  bool m_bOKToClose;         //Ok to respond to WM_CLOSE messages
  ATL::CImage m_Image;       //The image we are displaying
  CPalette m_Palette;        //The palette from the image
  CWnd m_wndOwner;           //Our hidden window parent (causes this window not to have a entry in the task bar)
  bool m_bUseFile;           //Should we use m_sFilename when it comes time to call LoadBitmap 
  LPCTSTR m_pszResourceName; //The resource ID if m_bUseFile is FALSE
  HINSTANCE m_hInstance;     //The instance handle to load from 
  CString m_sFilename;       //The name of the file to load the bitmap from (used if m_bUseFile is true)
  HICON m_hIcon;             //The icon to use for this window
  bool m_bTransparent;       //Should the image be drawn transparent
  COLORREF m_clrTransparent; //The transparent color to use
  bool m_bDraggable;         //Should the splash window be draggable
  bool m_bDropShadow;        //Should the window have a drop shadow effect
  DWORD m_dwSLWAFlags;       //The flags passed to SetLayeredWindowAttributes
  BYTE m_bSLWAAlpha;         //The transparency level passed to SetLayeredWindowAttributes

  friend class CSplashThread;
  friend class CSplashFactory;
};

//GUI thread in which the splash window is run
class CSPLASHER_EXT_CLASS CSplashThread : public CWinThread
{
public:
//Member variables
  CRuntimeClass* m_pRuntimeClassSplashWnd; //The runtime class version of "m_pSplashScreen"
  CEvent m_evtInitInstanceRun;             //Event using to synchronise to our InitInstance method
  volatile LONG  m_bInitOK;                //Was InitInstance successful
  CSplashWnd* m_pSplashScreen;             //Pointer to the CWnd splash screen

protected:
//Constructors / Destructors
  CSplashThread();

//Virtual methods
  BOOL InitInstance() override;
  int  ExitInstance() override;

  DECLARE_MESSAGE_MAP()
  DECLARE_DYNCREATE(CSplashThread)
};

//Class which looks after creating the splash window in the separate thread
class CSPLASHER_EXT_CLASS CSplashFactory : public CWinThread
{
public:
//Constructors / Destructors
  CSplashFactory();
  CSplashFactory(_In_ const CSplashFactory&) = delete;
  CSplashFactory(_In_ CSplashFactory&&) = delete;
  ~CSplashFactory(); //NOLINT(modernize-use-override)

//Methods
  CSplashFactory& operator=(_In_ const CSplashFactory&) = delete;
  CSplashFactory& operator=(_In_ CSplashFactory&&) = delete;
  virtual bool Create(_In_ CRuntimeClass* pRuntimeClassSplashWnd, _In_ int nPriority = THREAD_PRIORITY_NORMAL);
  bool Close(_In_opt_ CWnd* pWndToGainFocus = nullptr);

protected:
//Member variables
  CSplashThread* m_pSplashThread; //The GUI thread we are managing
};


#endif //#ifndef __SPLASHER_H__
