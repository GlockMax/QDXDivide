// NCM.h : main header file for the NCM application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "AFXTEMPL.h"
#include "NTiParams.h"
#include "NCMComData.h"
#include "NCMProject.h"
#include "..\guard\Include\GrdAPI.h"
#include "WM_APP_messages.h"

enum ConfigMask
{
	CM_VerifierM = 0x01,
	CM_45 = 0x02,
	CM_Measure = 0x10,
	CM_Compare = 0x20,
	CM_Machine = 0x40,
	CM_ExpDXF = 0x04,
	CM_ExpAPTCL = 0x08,
	CM_VerifierT = 0x0100,
	CM_VerifierMT = 0x0200,
	CM_VerifierW = 0x0400,
	CM_CollisionD = 0x0800,
	CM_MachineEditor = 0x1000,
	CM_AllPosts = 0x2000,
	CM_VerifierM5X = 0x4000,
	CM_MakeDemo = 0x8000,
	CM_ExpSTL = 0x80
}; // Don't forget to change MODULES_NUM if you add/remove ConfigMask element


enum NCMLanguage
{
	NL_UND,
	NL_RUS,
	NL_ENG,
	NL_KOR
};

class NCSingleDocTemplate : public CSingleDocTemplate
{
public:
		NCSingleDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
		:	CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass) {};


	CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL ) { 	return CSingleDocTemplate::OpenDocumentFile(lpszPathName, TRUE, FALSE);}
};

class CNCMApp : public CWinAppEx
{
private:
	class NParams4Coll
	{
	public:
		void Reset();
		bool AnimMode;
		bool CuttingMode;
		bool MSActive;
		bool Defined;
		bool Started;

	} Params4Rest;
protected:
	enum OGLViewStyle
	{
		OG_Stock,
		OG_Mach,
	};
	NCMConfig Configuration;
public:
	CNCMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCMApp)
public:
	void ResetParams4Rest() { Params4Rest.Reset(); }
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void DoWaitCursor(int nCode);
	virtual BOOL OnIdle(LONG lCount) override; // return TRUE if more idle processing
	static long RegCloneBranch(HKEY hkeyDestRoot, HKEY hkeySrcRoot);
	static long RegSaveBranch(HKEY hkeySrcRoot, CArchive &ar);
	static long RegLoadBranch(HKEY hkeyDestRoot, CArchive &ar);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNCMApp)
	afx_msg void OnAppAbout();
	afx_msg void OnSettings();
	afx_msg void OnSettingsAppearance();
	afx_msg void OnSettingsControlPanel();
	afx_msg void OnSettingsGraphics();
	afx_msg void OnGlobalSettingsSave();
	afx_msg void OnSettingsSave();
	afx_msg void OnSettingsLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd) override;
public:
	afx_msg void OnLanguageKorean();
	afx_msg void OnUpdateLanguageKorean(CCmdUI *pCmdUI);
	afx_msg void OnLanguageEnglish();
	afx_msg void OnUpdateLanguageEnglish(CCmdUI *pCmdUI);
	afx_msg void OnLanguageRussian();
	afx_msg void OnUpdateLanguageRussian(CCmdUI *pCmdUI);
public:
	bool IsInCuttingMode(void);
	bool IsInAnimateMode(void);
	bool IsInResultMode(void);
	bool ChangeType(NCMApplicationType NewType);
	afx_msg void OnDebugmode();
	afx_msg void OnCutfastonoff5x();
	afx_msg void OnCutonoff();
	NGlobalState & GetGlobalState(void)
	{
		return NCM_PROJECT.GetGlobalState();
	};
	afx_msg void OnResultMode();
	afx_msg void OnUpdateResultMode(CCmdUI *pCmdUI);
	bool CanResultModeBeEnabled();
	afx_msg void OnSmothnessModeF();
	afx_msg void OnUpdateSmothnessModeF(CCmdUI *pCmdUI);
	bool CanSmothnessModeFBeEnabled();
	afx_msg void OnSmothnessMode();
	afx_msg void OnSmothnessModeRib();
	afx_msg void OnUseFeed();
	afx_msg void OnLayoutProject();
	afx_msg void OnUpdateSmothnessMode(CCmdUI *pCmdUI);
	bool CanSmothnessModeBeEnabled();
	afx_msg void OnUpdateSmothnessModeRib(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUseFeed(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLayoutProject(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
	void StockWarnMessage(void);
	static void ProgErrMessage(void);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenRib();
	int ReadGuard(bool StartCall, bool Clear = false);
	static bool FindDongle(class CGrdDongle *pGrdDongle, TGrdFindInfo &GrdFindInfo, DWORD dwPublic, DWORD dwPrivRD1, int &nRet);
	NCMConfig GetConfig(void);
	bool PrepFileNew(void);
	afx_msg void OnFileNewMill();
	afx_msg void OnFileNewNcmprt();
	afx_msg void OnFileNewTurn();
	afx_msg void OnFileNewMillturnRib();
	afx_msg void OnFileNewMillRib();
	afx_msg void OnFileNewTurnRib();
	afx_msg void OnFileNewNcmprtRib();
	afx_msg void OnFileNewMillturn();
	bool IsVerifEnabled(void);
	static BOOL CALLBACK searcher(HWND hWnd, LPARAM lParam);
	afx_msg void OnFileReopen();
	afx_msg void OnUnitMm();
	afx_msg void OnUnitInch();
	afx_msg void OnUpdateUnitMm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUnitInch(CCmdUI *pCmdUI);
	afx_msg void OnViewAppLook();
	CString GetDemoPath(void);
	afx_msg void OnChangeCutMode();
	afx_msg void OnUpdateChangeCutMode(CCmdUI *pCmdUI);
	afx_msg void OnChangeWin();
	afx_msg void OnUpdateChangeWin(CCmdUI *pCmdUI);
	afx_msg void OnCutMode();
	bool StartSlowCutMode();
	afx_msg void OnFastCutMode5X();
	bool StartFastCutMode();
	afx_msg void OnDontcutMode();
	afx_msg void OnUpdateDontcutMode(CCmdUI *pCmdUI);
	bool CanDontcutModeBeEnabled();
	afx_msg void OnCutModeRib();
	bool StartCutMode();
	afx_msg void OnUpdateCutModeRib(CCmdUI *pCmdUI);
	afx_msg void OnUseDX();
	afx_msg void OnUpdateUseDX(CCmdUI *pCmdUI);
	void SetRemoveState(bool val = true){ RemoveState = val;}
	bool GetRemoveState() const { return RemoveState;}
	bool LoginLicense(ConfigMask Module);
	bool LogoutLicense(ConfigMask Module);
	int GetVerifFlag(void);
	int GetModuleNum(ConfigMask Module);
	bool TruncateLicense(void);
	afx_msg void OnFileNewWeld();
	bool SetEditMachMode(void);
	bool StopEditMachMode(void);
	void SetImportMode(void);
	CString GetEDUPassword();
	void ChangeConfigByRegistry();
	void SetDXParams();
	void SetBSPParams();
	BOOL LoadState(LPCTSTR lpszSectionName = NULL, CFrameImpl* pFrameImpl = NULL) override;
	BOOL SaveState(LPCTSTR lpszSectionName = NULL, CFrameImpl* pFrameImpl = NULL) override;
	bool SaveParams4Rest();
	bool RestoreParams4Rest();
	bool StartFullCollisionCheck();
	bool StopFullCollisionCheck();
	bool StopCuttingWRest();
	bool IsCollChecking() const { return Params4Rest.Started; }


public:
	bool RemoveState;
	bool AllowLoadRibbonState;

};


/////////////////////////////////////////////////////////////////////////////
