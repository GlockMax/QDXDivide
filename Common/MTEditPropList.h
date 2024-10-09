#pragma once
/*!
\file MTEditPropList.h
\brief  онтрол "список свойств" в панели свойств
*/

#include "MTAxis.h"
#include "MTMashineTool.h"

//types:
#define LIST_AXIS             1
#define LIST_FORWARDAXIS      2
#define LIST_ROTARYAXIS       3
#define LIST_PARTAXIS         4
#define LIST_TOOLAXIS         5
#define LIST_TOOLAXISVIRT     6
#define LIST_STOCKAXIS        7
#define LIST_COLLISION        8
#define LIST_EMPTY            9
#define LIST_SCHAINAXIS       10
#define LIST_SPINAXIS		  11

struct CPropChange;

//! –едактируемый список свойств
/** ’ранит ссылку на станок и путь к каталогу со станками.
\sa MTEditPane, MTEdit
*/
class MTEditPropList : public CMFCPropertyGridCtrl
{
public:
	~MTEditPropList();
protected:
	void SetPropValue(const COleVariant &Value, int Index1, int Index2 = -1, int Index3 = -1);
	void SetPropColor(const NColor &Color,      int Index1, int Index2 = -1, int Index3 = -1);
	void SetPropPoint(const BPoint &Point,      int Index1, int Index2 = -1, int Index3 = -1);
	void SetPropBool (bool BValue,              int Index1, int Index2 = -1, int Index3 = -1);
	void SetPropInt  (int iValue,               int Index1, int Index2 = -1, int Index3 = -1);

	const COleVariant & GetPropValue(int Index1, int Index2 = -1, int Index3 = -1);
	int                   GetPropInt(int Index1, int Index2 = -1, int Index3 = -1);
	double             GetPropDouble(int Index1, int Index2 = -1, int Index3 = -1);
	const NColor        GetPropColor(int Index1, int Index2 = -1, int Index3 = -1);
	const BPoint        GetPropPoint(int Index1, int Index2 = -1, int Index3 = -1);
	const BPoint       GetPropVector(int Index1, int Index2 = -1, int Index3 = -1);
	bool				GetPropBool (int Index1, int Index2 = -1, int Index3 = -1);

	BOOL IsValueModified (int Index1, int Index2 = -1, int Index3 = -1) const;
	BOOL IsVectorModified(int Index1, int Index2 = -1, int Index3 = -1) const;
	BOOL IsPointModified (int Index1, int Index2 = -1, int Index3 = -1) const;

	void RemoveHighlightValue (int Index1, int Index2 = -1, int Index3 = -1);
	void RemoveHighlightVector(int Index1, int Index2 = -1, int Index3 = -1);
	void RemoveHighlightPoint (int Index1, int Index2 = -1, int Index3 = -1);

public:
	void SetMachinePath(const CString & path);
	void ShowAxisProp(int index);
	void ShowColProp(int index);
	void ShowEmptyProp();
	/// ѕостроение списка свойств
	bool MakeAxisList();
	bool ChangeSpecPosList();
	/// –азвернуть/свернуть список свойств в зависимости от состо€ни€
	void ExpandCollapse();
protected:
	/// ќтображение только тех свойств, которые соответствуют типу объекта type
	void SetTypeProp(int type);
	void SetCompProp(bool show = TRUE);//!< Show/hide compensation axes
public:
	void SetMachineTool(MTMachineTool *MT);
	bool ReadChanges(struct SPropChange *Change);

	bool IsPropertyGridModified();
	void RevertChanges();
protected:
	bool IsPropertyGridPropertyModified(CMFCPropertyGridProperty *pProp);
	void RevertChanges(CMFCPropertyGridProperty *pProp);
	void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	DECLARE_MESSAGE_MAP()

protected:
	MTMachineTool *EditingMT;
	CString MachinePath;
};//class MTEditPropList : public CMFCPropertyGridCtrl