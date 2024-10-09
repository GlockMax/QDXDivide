
// Dlg_Encrypt_Project.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CDlgApp:
// Сведения о реализации этого класса: Dlg_Encrypt_Project.cpp
//

class CDlgApp : public CWinApp
{
public:
	CDlgApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CDlgApp theApp;
