#pragma once
#include <vector>

// NRibbonBar

class NRibbonBar : public CMFCRibbonBar
{
	DECLARE_DYNAMIC(NRibbonBar)
public:
	NRibbonBar();
	virtual ~NRibbonBar();
protected:
	DECLARE_MESSAGE_MAP()
	CString NCUnitName;
	CString MachName;
	bool IsStartButtonActive;
//	UINT PrevCommandID;
	CMFCRibbonComboBox *pMachCombo;
	CMFCRibbonComboBox *pNCUnitCombo;
	CMFCRibbonComboBox *pToolsCombo;
public:
	bool IsStartButton() const { return IsStartButtonActive; }
	void SetStartStopButton(bool Stop);
//	void SetStartText(CCmdUI *pCmdUI, UINT CommandID);
	BOOL LoadFromResource(UINT uiXMLResID, LPCTSTR lpszResType = RT_RIBBON, HINSTANCE hInstance = NULL) override;
	int MOnToolsCombo(CString &Text);
	bool MOnNCUnitCombo();
	bool MOnMachCombo();
	void LoadToolsCombo(const std::vector<CString> &FullNames);
	void LoadNCUnitCombo();
	void LoadMachCombo();
	void SetNCUnitName(const CString &NewName);
	void SetMachName(const CString &NewName);
	const CString GetToolsSelection();
	const CString GetNCUnitNewName();
	const CString GetMachNewName();
	static bool Add2Group(CMFCRibbonButtonsGroup & Group, CMFCRibbonPanel & Panel, UINT ElementID);
protected:
	void Initialize();
	BOOL LoadState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT)-1) override;
	BOOL SaveState(LPCTSTR lpszProfileName = NULL, int nIndex = -1, UINT uiID = (UINT)-1) override;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool IsMinimized() const;
	void UpdateStartCollcheck(bool CollFound, bool LogIsStale, bool CheckingIsRunning);
	void UpdateStart();
};


