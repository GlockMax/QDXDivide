#pragma once

#include "SMachState.h"
#include "SCoordsDef.h"
#include "tinyxml.h"
#include <stack>
#include <vector>
#include "MTCollisionArray.h"
#include "direct.h"
#include "MTPalette.h"

//! �������� ����� ��������� �������.
/*! 
  �������� � ���� ��������� �� ������, ������� ���������. � ���� ������ ����������� ��������� ������.
  \sa MTEditPane, MTEditTreePane
  \ingroup Machine */
class AFX_EXT_CLASS MTEdit
{
	struct MTChange
	{
		MTChange() { AfterSelNode == _T(""); }
		TiXmlDocument *Xml;
		char ChangeType; ///< 0 - ��������, 1 - ���������
		CString BeforeSelNode; ///< ��� ���� � ������, ������� ������ ���� �������� ��� ������ ���������
		CString AfterSelNode;  ///< ��� ���� � ������, ������� ������ ���� �������� ����� ���������
	};//struct MTChange
public:
	MTEdit(bool demo = false);
	~MTEdit(void);
public:
	bool Activate(class MTMachineTool &MT, class MTEditTreePane &TreePane, class MTEditPane &PropPane, const CString &path);
	bool Deactivate();
	bool CheckEditing();
	/// ��������� � ������� ����
	void Exit();
	/// ���������� ���� NCManager
	bool ApplyChanges();
	/// ���������� ����������� ������ � ����������� ����
	bool UpdatePicture();
	/// ���������� ����� ��� ������ �������(����������� ��� ��� ������) � ���������� ����
	void OnPick(int PartNumber);
	/// ��� �������� � ����� �������������� ��������� ��������� ������
	void SaveMachState();
	/// ���������� ������� ��������� ������ � ����������
	void ResetMach();
	///����������� ������ �������.
	void ShowPropPane();
	const SMachState & GetEditMachState(void) { return *pMachState;}
	bool ShowAxisProp(const CString& AxisName);
	bool ShowColProp(const CString& Axis1, const CString& Axis2);
	bool ShowColProp(int index);
	void ShowEmptyProp();
	bool RenameTreeNode(const CString &OldName, const CString &NewName);
	void SetCoordValue(const CString &name, double value);

	bool IsDescendant(const CString& BaseAxis, const CString& Axis);

	bool CreateMachState(void);
	bool IsChangingNodeAllowed();///<���������� ��� ��� ����� ���������� ���, ���������� ������ ���������� ���������.

	void GetMachinePath(CString &path, CString &name);

	/// Just save. Function doesn't show any warnings or touch any editor data.
	void Save();
	/// All save actions. Window "are you sure?", cleaning undo stack etc.
	bool ChangeSave(bool warning = true);
	bool ChangeSaveAs(const CString &NewDirName);

	//Edit actions
	//@{
	bool MakeChange(MTChange *change_mt);

	bool ChangeAxisProp(struct SPropChange *Change, int AxisIndex);
	bool ChangeColProp(struct SPropChange *Change, int ColIndex);
	bool ChangeMoveBranch(const CString& AxisName, const CString& DestName);
	bool ChangeMoveTool(const CString& AxisName, const CString& DestName);
	bool ChangeMoveVirtTool(const CString& AxisName, const CString& DestName);
	bool ChangeMoveCollision(CString Axis1, CString Axis2, CString NewAxis1);
	bool ChangeCreateTrans(const CString& ParentName, class MTForwardAxis *Axis = NULL);
	bool ChangeCreateRot(const CString& ParentName, class MTRotaryAxis* Axis = NULL);
	bool ChangeCreateSpin(const CString& ParentName, class MTSpinAxis* Axis);
	bool ChangeCreateSpin(const CString& ParentName);
	bool ChangeCreateSChain(const CString& ParentName, class MTSimpleChainAxis *Axis = NULL);
	bool ChangeCreateArray(const CString& AxisName, int Count, double Step, bool DontRotate);
	bool ChangeLoadBranch(const CString& FileName);
	const class MTRelocateAxis* GetNearestRelAxis(const CString& AxisName) const;
	bool ChangeCreatePart(const CString& ParentName, class MTPartAxis *Axis = NULL);
	bool ChangeCreateParts(const CString& ParentName, std::vector<CString> &list);
	bool ChangeCreateTool(const CString& ParentName, class MTToolAxis *Axis = NULL);
	bool ChangeCreateToolVirt(const CString& ParentName, class MTVirtToolAxis *Axis = NULL);
	bool ChangeCreateStock(const CString& ParentName, class MTStockAxis *Axis = NULL);
	bool ChangeCreateCol(const CString& ParentName, const CString& EnemyName, SCollision *Col = NULL);
	bool ChangeCreateX(const CString& ParentName);
	bool ChangeCreateY(const CString& ParentName);
	bool ChangeCreateZ(const CString& ParentName);
	bool ChangeCreateA(const CString& ParentName);
	bool ChangeCreateB(const CString& ParentName);
	bool ChangeCreateC(const CString& ParentName);
	bool ChangeDeleteBranch(const CString& AxisName);
	bool ChangeDeleteCollision(const CString& Axis1, const CString& Axis2);

	bool IsMachineChanged(void) const { return MachineChanged;}
	void SetMachineChanged(bool Val) { MachineChanged = Val; }
	bool IsActive() const noexcept { return bEditMode; }
	//@}
	//^^^^^^^^^^^^

	///������ � �������� ���������.
	/**������ 2 ���� ���������: � ��������� �-� ��� � � ��������� ������
	 */
	bool Undo();
	bool Redo();
	/// Clear undo and redo history.
	void ClearHistory();
	/// Clear undo history
	void ClearUndoStack();
	/// Clear redo history
	void ClearRedoStack();
	static void ResetSTLConverter();
	static enum StockFileType Conv2STL(CString& IOFileName, enum DocElements DEl, const CString& InitPath = CString());
	static enum StockFileType DetSFT(const CString& FileName);
protected:
	/// ���� ����� �������������� ������� - true
	bool bEditMode;
	/// ��������� ������, ����������� ����� ��� ��������� ������ � ����������� ����
	SMachState *pMachState;
	SCoordsDef *pMachCoordsDef;
	/// ��������� ���������, ��������������� ��� �������� � ����� ��������������
	SMachState *pMachStateOr;
	SCoordsDef *pMachCoordsDefOr;
	MTMachineTool *EditingMT;
	MTEditTreePane *TreePanePtr;
	MTEditPane *EditPanePtr;
	CString MachinePath;
	std::stack <MTChange*> leftStack, rightStack;
	MTPalette palette;
	bool MachineChanged;
public:
	bool DEMO;
};