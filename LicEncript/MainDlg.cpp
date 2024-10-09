
// MainDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Dlg_Encrypt_Project.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include <iostream> 
#include <string> 
#include <windows.h> 
#include <atlstr.h> 
#include "license.h"
#include "Encrypt.h"


#using <System.Xml.dll>

using namespace System;
using namespace System::Xml;
using namespace std;
using namespace System::Text;

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()

// Диалоговое окно CMainDlg

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_ENCRYPT_PROJECT_DIALOG, pParent)
	, clientName(_T(""))
	, clientID(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Client_Name, clientName);
	DDX_Text(pDX, IDC_EDIT_Client_ID, clientID);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_License, &CMainDlg::OnBnClickedBtnLicense)
	ON_BN_CLICKED(IDC_BTN_Get_PPFile, &CMainDlg::OnBnClickedBtnGetPpfile)
	ON_BN_CLICKED(IDC_BTN_Get_License, &CMainDlg::OnBnClickedBtnGetLicense)
	ON_BN_CLICKED(IDC_BUTTON_Delete, &CMainDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()

// Обработчики сообщений CMainDlg

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	clientName = "Имя клиента";
	UpdateData(FALSE);
	clientID = "Идентификатор клиента";
	UpdateData(FALSE);

	//add strings to listbox
	CListBox* lbox = (CListBox*)GetDlgItem(IDC_LIST1);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnBnClickedBtnGetLicense()
{
	UpdateData(TRUE);
	vector_ppfiles.clear();
	CListBox* lbox = (CListBox*)GetDlgItem(IDC_LIST1);
	lbox->SendMessage(LB_RESETCONTENT);
	CString CMsgName;
	CString CMsgID;
	CMsgName = "Имя клиента: ";
	CMsgID = "Идентификатор клиента: ";

	CFolderPickerDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();
		CListBox* lbox = (CListBox*)GetDlgItem(IDC_LIST1);
		license li;
		GetDlgItem(IDC_BTN_Get_PPFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_Delete)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_License)->EnableWindow(TRUE);
		if (!li.check_license(lbox, vector_ppfiles, pathName, clientName, clientID))
		{
			MessageBox("Ошибка в файле лицензии", "Ошибка", MB_OK | MB_ICONERROR);
		}
		
		
	};
}

void CMainDlg::OnBnClickedBtnGetPpfile()
{
	UpdateData(TRUE);

	CFileDialog dlg1(TRUE, "*.sm2; *.sm3", NULL, OFN_ALLOWMULTISELECT, "Postprocessor file (*.sm2, *.sm3) | *.sm2; *.sm3||");
	
	if (dlg1.DoModal() != IDOK)
		return;

	POSITION pos(dlg1.GetStartPosition());
	CListBox* lbox = (CListBox*)GetDlgItem(IDC_LIST1);

	while (pos)
	{
		PathPPFileName = dlg1.GetNextPathName(pos);

		string pp_filename = PathPPFileName;
		auto dotPosition = pp_filename.rfind('\\', pp_filename.length());
		pp_filename = pp_filename.substr(dotPosition+1, pp_filename.length());
		lbox->AddString((CString)pp_filename.c_str());

		vector_ppfiles.push_back(PathPPFileName);
	};
	
}

void CMainDlg::OnBnClickedBtnLicense()
{
	CListBox* lbox = (CListBox*)GetDlgItem(IDC_LIST1);
	license li;
	lbox->SendMessage(LB_RESETCONTENT);

	/*if (vector_ppfiles.size() == 0)
	{
		MessageBox("Нет выбранных файлов постпроцессора для добавления в лицензию", "Ошибка", MB_OK | MB_ICONERROR);
		vector_ppfiles.clear();
		GetDlgItem(IDC_BTN_Get_PPFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_Delete)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_License)->EnableWindow(FALSE);
		return;
	}*/

	

	for (int i = 0; i < vector_ppfiles.size(); i++)
	{ 

		std::string str1 = (std::string)vector_ppfiles[i];
		str1 = str1.substr(str1.rfind("\\", str1.length()) + 1, str1.length());

		auto dotPos = str1.rfind('.', str1.length());
		str1 = str1.substr(dotPos, str1.length() - dotPos);

		if (str1 != ".smx")
		{
			str1 = (std::string)vector_ppfiles[i];
			str1 = str1.substr(str1.rfind("\\", str1.length()) + 1, str1.length());

			auto dotPos = str1.rfind('.', str1.length());
			str1 = str1.substr(0, dotPos);
			str1 += ".smx";
			str1 = (std::string)pathName + "\\" + str1;
			LPCTSTR outputFileName = str1.data();

			Encrypt encr;
			if (!encr.CreateNewFile(vector_ppfiles[i], outputFileName))
			{
				MessageBox("Ошибка при шифровании файла постпроцессора или при подписывании файла лицензии", "Ошибка", MB_OK | MB_ICONERROR);
				vector_ppfiles.clear();
				GetDlgItem(IDC_BTN_Get_PPFile)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_Delete)->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_License)->EnableWindow(FALSE);
				return;
			}
			
			str1 = str1.substr(str1.rfind("\\", str1.length()) + 1, str1.length());
			vector_ppfiles[i] = str1.c_str();
		}

	}

	string file_name = pathName + "\\" + clientName + ".xml";
	String^ xmlName = gcnew String(file_name.c_str());
	li.deleteAll(xmlName);

	for (int i = 0; i < vector_ppfiles.size(); i++)
	{	
		String^ xmlName = gcnew String(file_name.c_str());
		li.addXML(xmlName, vector_ppfiles[i]);
	}

	li.check_license(lbox, vector_ppfiles, pathName, clientName, clientID);
	string sign_file_name = pathName + "\\" + clientName + "_Signed.xml";
	Encrypt en;
	en.SignXmlFile(gcnew System::String(file_name.c_str()), (gcnew System::String(sign_file_name.c_str())));
	//vector_ppfiles.clear();
	
	GetDlgItem(IDC_BTN_Get_PPFile)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_Delete)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_License)->EnableWindow(FALSE);

	MessageBox("Процесс шифрования и подписывания закончен ", "Сообщение", MB_OK | MB_ICONINFORMATION);
	lbox->SendMessage(LB_RESETCONTENT);
}

void CMainDlg::OnBnClickedButtonDelete()
{
	CListBox* lbox = (CListBox*)GetDlgItem(IDC_LIST1);
	CString strstring;
	int nIndex = lbox->GetCurSel();
	lbox->GetText(nIndex, strstring);
	for (int j = 0; j < vector_ppfiles.size(); j++)
	{
		std::string str = (std::string)vector_ppfiles[j];
		str = str.substr(str.rfind("\\", str.length()) + 1, str.length());
		if (str.c_str() == strstring)
		{
			auto iter = vector_ppfiles.cbegin();
			vector_ppfiles.erase(iter + j);
		}
	};
	lbox->DeleteString(nIndex);
}
