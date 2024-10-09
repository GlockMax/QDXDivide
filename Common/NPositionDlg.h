#pragma once
#include "CMapStringToNStock.h"
#include "NExprEdit.h"

// NPositionDlg dialog

class NPositionDlg : public CDialog
{
	DECLARE_DYNAMIC(NPositionDlg)

public:
	NPositionDlg(CWnd* pParent, CNCMDoc *ipDoc);   // standard constructor
	virtual ~NPositionDlg();

// Dialog Data
	enum { IDD = IDD_PART_POSITION1 };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	void PostNcDestroy() override;
	BOOL OnInitDialog() override;
	void OnCancel() override;

	DECLARE_MESSAGE_MAP()
protected:
	double m_move_x;
	double m_move_y;
	double m_move_z;
	double m_pos_x;
	double m_pos_y;
	double m_pos_z;
	int m_rel;
	int m_move_copy;
	double m_abs_x;
	double m_abs_y;
	double m_abs_z;
	double m_abs_sx;
	double m_abs_sy;
	double m_abs_sz;
	double m_abs_a;
	double m_abs_b;
	double m_abs_c;
	BOOL m_abs_simm;
	BOOL m_abs_2sol;

	class CMapStringToNStock Buffer;

	class CNCMDoc *pDoc;
	NExprEdit m_abs_x_c;
	NExprEdit m_abs_y_c;
	NExprEdit m_abs_z_c;
	NExprEdit m_abs_a_c;
	NExprEdit m_abs_b_c;
	NExprEdit m_abs_c_c;
	NExprEdit m_move_x_c;
	NExprEdit m_move_y_c;
	NExprEdit m_move_z_c;
	NExprEdit m_pos_x_c;
	NExprEdit m_pos_y_c;
	NExprEdit m_pos_z_c;
	NExprEdit m_ang_x_c;
	NExprEdit m_ang_y_c;
	NExprEdit m_ang_z_c;
	NExprEdit m_abs_sx_c;
	NExprEdit m_abs_sy_c;
	NExprEdit m_abs_sz_c;
	CButton m_abs_simm_c;
	CButton m_abs_2sol_c;
	double m_ang_x;
	double m_ang_y;
	double m_ang_z;
	NExprEdit m_scale_x_c;
	NExprEdit m_scale_y_c;
	NExprEdit m_scale_z_c;
	double m_scale_x;
	double m_scale_y;
	double m_scale_z;
	CButton m_redo_c;
	CButton m_undo_c;
	CButton m_apply_c;
	CButton m_apply_abs_c;
	CStatic m_Title_Delta;
	CStatic m_Title_Rotang;
	CStatic m_Title_Scale;
	CButton m_CheckBox;
	BOOL m_MovePrg;
	bool Active;
	CString m_ObjName;
	CEdit m_ObjName_c;
	CToolTipCtrl m_ToolTip;
public:
	bool IsActive() const { return Active; }
	void StartPosDlg();
	void FillAbsFields();
	void Draw();
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedRedo();
protected:
	void FillBasePos();
	void StoreBasePos();
	bool GetLastObject(class NStock*& pStock, CString& Name);
	void RecalcBasePos();
	bool Copy(const BMatr &M);
	void EnableFields();
	void Transform(const BMatr &M, bool Abs = false);
	void SetMoving(bool val);
	const BBox GetGabar(void);
	void FillAbsFields(const BMatr& M);
	afx_msg void OnBnClickedFields();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedApplyAbs();
	afx_msg void OnBnClickedAbs2Sol();
	afx_msg void OnKillfocusPos();
	bool CanPosUndo() const;
	bool CanPosRedo() const;
public:
	BOOL PreTranslateMessage(MSG* pMsg) override;
};
