
// NCEditor2Doc.h : interface of the CNCEditor2Doc class
//


#pragma once
#include "NCUnitDef.h"
#include "NCUnitTreePane.h"
#include "PropertiesWnd.h"
#include "tinyxml.h"
#include <stack>
#include "NCUnitSnapshot.h"
#include "NCUnitHelpPane.h"

using namespace std;

/// ��������
/**
��������, ����������� ��� ���������� � �������� ��������������:
  - ��������� ��������� Definition
  - ��������� ����������� ����������� � ������ (presence)
  - ���������� �������� Word Replacement
  - ��������� srcString �/��� dstString �������� Word Replacment
  - �������� �������� Word Replacement
  - �������������� �������� Word Replacement ����� ������
  - ���������� �����������, �������� ��� � ����� - ���������
*/
class CNCEditor2Doc : public CDocument
{
protected: // create from serialization only
	CNCEditor2Doc();
	DECLARE_DYNCREATE(CNCEditor2Doc)

// Attributes
public:
	CString Content;

// Operations
public:

// Overrides
public:
	/// �������� ������� ���������
	virtual BOOL OnNewDocument();
	/// ��������/����������
	virtual void Serialize(CArchive& ar);

	/// ������������ �����-���������
	void ReloadXML(CString fileName);

	/// ����������� �������� ������
	void ShowSectionProp(CString sectionName);
	void ShowDefinitionProp(CString sectionName, CString defName);
	void ShowDefinitionUnknownProp(CString sectionName, CString defName);
	void ShowWReplProp(CString srcString, CString dstString);
	void ShowGroupProp(CString sectName, CString groupName);

	/// ������������ ��������
	void RefreshTextView();

	/// �������������� ��������� �����������
	void ChangeDef(CString secName, CString defName, Param newValue);
	void ChangeDef(Def *def, Param newValue);
	/// ��������� ����� ����������� � ������ ��� �����������
	void ChangeDefPresence(CString secName, CString defName);
	void ChangeDefPresence(Def *def, bool presence = 1);
	/// ���������� �������� Word Replacement
	void AddWRepl(CString srcString, CString dstString);
	/// ��������� �������� Word Replacement
	void ChangeWRepl(unsigned int wreplIndex, CString srcString, CString dstString);
	void ChangeWRepl(CString srcString, CString dstString, CString newSrcString, CString newDstString);
	/// �������� �������� Word Replacement
	void DelWRepl(CString srcString, CString dstString);
	/// ����������� �������� Word Replacement �� ���� ������� ����� ��� ����
	void MoveWRepl(CString srcString, CString dstString, bool up);
	/// ����������� �������� Word Replacement
	void MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString);
	/// ���������� ����������� � ������, �������� ��� � �����- ���������
	void AddDef(CString secName, Def definition);

	/// ������������ ���������.
	/**
	������� redoStack, ���������� ��������� ��������� � undoStack
	*/
	void CommitChange(CNCUnitSnapshot &snapshot);
	/// �������� ������ ��������� ��������� ��� �������� � ������� undo/redo
	void MakeUnitSnapshot(CNCUnitSnapshot &snapshot);
	/// ��������������� ��������� �� ������
	void ApplyUnitSnapshot(CNCUnitSnapshot &snapshot);
	/// Undo
	/// �������� �������� �� undo stack, ���������� �������� � redo stack
	void Undo();
	/// Redo
	/// �������� �������� �� redo stack, ���������� �������� � undo stack
	void Redo();
	/// ������� ������ ��� undo � redo
	void ClearHistory();
	/// ������� ����� redo
	void ClearRedoStack();
	/// ���� undo �� ������?
	bool isUndoActive();
	/// ���� redo �� ������?
	bool isRedoActive();

	/// ��������� ����� � TreePane, ����� �� �������������� ��������
	void SetInternalSelect(bool IS);

// Implementation
public:
	virtual ~CNCEditor2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	/// ��������� ��������� �� ������ � �������
	CNCUnitTreePane * GetTreeView();
	/// ��������� ��������� �� ���� �������
	CPropertiesWnd * GetPropertiesWnd();
	/// ��������� ��������� �� ���� ������
	CNCUnitHelpPane * GetHelpPane();

	CString GetCurFile();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	
	/// �������� ������, ������� ��������� ������
	CNCUnitDef unitDef;
	/// ����� ������� ���������
	stack <CNCUnitSnapshot> undoStack, redoStack;

	/// ��� �������� �����-���������
	CString curFile;

	bool successLoad;
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


