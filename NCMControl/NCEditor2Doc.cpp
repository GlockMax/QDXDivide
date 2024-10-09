
// NCEditor2Doc.cpp : implementation of the CNCEditor2Doc class
//

#include "stdafx.h"
#include "NCMControl.h"
#include "MainFrm.h"

#include "NCEditor2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNCEditor2Doc

IMPLEMENT_DYNCREATE(CNCEditor2Doc, CDocument)

BEGIN_MESSAGE_MAP(CNCEditor2Doc, CDocument)
END_MESSAGE_MAP()


// CNCEditor2Doc construction/destruction

CNCEditor2Doc::CNCEditor2Doc()
{
	// TODO: add one-time construction code here
	/// Загружаем файл - прототип
	TiXmlDocument *doc = new  TiXmlDocument();
	doc->LoadFile(theApp.workPath + "Settings\\ncunit.xml");
	if(doc->Error())
	{
		CString str; str.LoadString(IDS_ERROR_XML_PROTO);
		AfxMessageBox(str);
		exit(0);
	}//if
	if(!unitDef.LoadXML(doc))
	{
		CString str; str.LoadString(IDS_ERROR_XML_PROTO);
		AfxMessageBox(str);
		exit(0);
	}//if
	delete doc;

	curFile = _T("ncunit.xml");
}

CNCEditor2Doc::~CNCEditor2Doc()
{
}

BOOL CNCEditor2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	ClearHistory();
	// (SDI documents will reuse this document)
	((CEditView*)m_viewList.GetHead())->GetEditCtrl().SetReadOnly();
	unitDef.Reset();
	GetTreeView()->ShowNCUnitDef(&unitDef, NULL);
	((CEditView*)m_viewList.GetHead())->SetWindowText(unitDef.GetString());

	return TRUE;
}

// CNCEditor2Doc serialization

void CNCEditor2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CString s = unitDef.GetString();
		ar.WriteString(s);

		ClearHistory();
	}
	else
	{
		ClearHistory();

		/// Получаем указатель на файл чтобы узнать его длину
		CFile *file = ar.GetFile();
		UINT size = (UINT)file->GetLength();
		/// Выделяем буфер
		char *content = new char[size+1];
		/// Считываем содержимое файла в буфер
		ar.Read(content, size);
		/// Добавляем 0, чтобы CString могло считать строку
		content[size] = '\0';
		/// Создаём CString с содержимым файла
		CString t(content);
		/// Освобождаем буфер
		delete [] content;
		((CEditView*)m_viewList.GetHead())->SetWindowText(t);
		if(!unitDef.SetString(t))
		{
			CString str; str.LoadString(IDS_ERROR_FILE);
			AfxMessageBox(str);
			successLoad = false;
		}//if
		else
			successLoad = true;
		CString str = unitDef.GetString();
		((CEditView*)m_viewList.GetHead())->SetWindowText(str);

		GetTreeView()->ShowNCUnitDef(&unitDef, NULL);
	}
}

void CNCEditor2Doc::ReloadXML(CString fileName)
{
	ClearHistory();

	curFile = fileName;
	CString save = unitDef.GetString();

	unitDef.Clear();

	TiXmlDocument *doc = new TiXmlDocument();
	fileName = theApp.workPath + "Settings\\" + fileName;

	char *ch = fileName.GetBuffer();
	doc->LoadFile(ch);

	if(!unitDef.LoadXML(doc))
	{
		CString str; str.LoadString(IDS_ERROR_XML);
		AfxMessageBox(str+fileName);
		exit(0);
	}//if

	delete doc;

	unitDef.SetString(save);

	GetTreeView()->ShowNCUnitDef(&unitDef);
}//void CNCEditor2Doc::ReloadXML()

void CNCEditor2Doc::ChangeDef(CString secName, CString defName, Param newValue)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(secName != str);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	ClearRedoStack();

	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == secName)
		{
			for(unsigned int j = 0; j < unitDef.sectionList[i].defList.size(); j++)
			{
				ASSERT(unitDef.sectionList[i].defList[j].GetType()==D_COMMON);
				if(unitDef.sectionList[i].defList[j].def.defWord == defName)
				{
					ASSERT(unitDef.sectionList[i].defList[j].def.defParam.GetType() == newValue.GetType());
					
					unitDef.sectionList[i].defList[j].def.param = newValue;

					break; //for
				}//if
			}//for
	
			break;//for
		}//if
	}//for

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::ChangeDef(CString secName, CString defName, Param newValue)

void CNCEditor2Doc::ChangeDef(Def *def, Param newValue)
{
	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	ASSERT(def->defParam.GetType() == newValue.GetType());

	def->param = newValue;

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::ChangeDef(Def *def, Param newValue)

void CNCEditor2Doc::ChangeDefPresence(CString secName, CString defName)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(secName != str);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);
	
	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == secName)
		{
			for(unsigned int j = 0; j < unitDef.sectionList[i].defList.size(); j++)
			{
				ASSERT(unitDef.sectionList[i].defList[j].GetType() == D_COMMON);
				if(unitDef.sectionList[i].defList[j].def.defWord == defName)
				{
					unitDef.sectionList[i].defList[j].def.presence = !unitDef.sectionList[i].defList[j].def.presence;
					break;
				}//if
			}//for
			break;
		}//if
	}//for

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::ChangeDefPresence(CString secName, CString defName, bool presence)

void CNCEditor2Doc::ChangeDefPresence(Def *def, bool presence)
{

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	def->presence = presence;

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::ChangeDefPresence(Def *def, bool presence = 1)

void CNCEditor2Doc::AddWRepl(CString srcString, CString dstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(unitDef.sectionList[1].name == str);

	if(unitDef.FindWRepl(srcString,dstString) != -1)
	{
		CString str;
		str.LoadString(IDS_WREPL_ADD_EXISTS);
		AfxMessageBox(str);
		return;
	}//if

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	WRep wrep;
	wrep.srcString = srcString;
	wrep.destString = dstString;

	Definition def(wrep);

	unitDef.sectionList[1].defList.push_back(def);

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::AddWRepl(CString srcString, CString dstString)

void CNCEditor2Doc::ChangeWRepl(unsigned int wreplIndex, CString srcString, CString dstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(unitDef.sectionList[1].name == str);
	
	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	if(unitDef.sectionList[1].defList.size() <= wreplIndex)
		return;

	unitDef.sectionList[1].defList[wreplIndex].wRep.srcString = srcString;
	unitDef.sectionList[1].defList[wreplIndex].wRep.destString = dstString;

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::ChangeWRepl(unsigned int wreplIndex, CString srcString, CString dstString)

void CNCEditor2Doc::ChangeWRepl(CString srcString, CString dstString, CString newSrcString, CString newDstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(unitDef.sectionList[1].name == str);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);
	
	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);
	
	for(unsigned int i = 0 ; i < unitDef.sectionList[1].defList.size(); i++)
	{
		if(unitDef.sectionList[1].defList[i].wRep.srcString == srcString &&
			unitDef.sectionList[1].defList[i].wRep.destString == dstString)
		{
			unitDef.sectionList[1].defList[i].wRep.srcString = newSrcString;
			unitDef.sectionList[1].defList[i].wRep.destString = newDstString;

			break;
		}//if
	}//for

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::ChangeWRepl(CString srcString, CString dstString, CString newSrcString, CString newDstString)

void CNCEditor2Doc::DelWRepl(CString srcString, CString dstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(unitDef.sectionList[1].name == str);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	for(unsigned int j = 0; j < unitDef.sectionList[1].defList.size(); j++)
	{
		if(unitDef.sectionList[1].defList[j].wRep.srcString == srcString &&
		   unitDef.sectionList[1].defList[j].wRep.destString == dstString)
		{
			unitDef.sectionList[1].defList.erase(unitDef.sectionList[1].defList.begin() + j);

			break;
		}//if
	}//for

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::DelWRepl(CString srcString, CString dstString)

void CNCEditor2Doc::MoveWRepl(CString srcString, CString dstString, bool up)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(unitDef.sectionList[1].name == str);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	for(unsigned int j = 0; j < unitDef.sectionList[1].defList.size(); j++)
	{
		if(unitDef.sectionList[1].defList[j].wRep.srcString == srcString &&
		   unitDef.sectionList[1].defList[j].wRep.destString == dstString)
		{
			Definition a = unitDef.sectionList[1].defList[j];

			unitDef.sectionList[1].defList.erase(unitDef.sectionList[1].defList.begin() + j);

			if(up)
				unitDef.sectionList[1].defList.insert(unitDef.sectionList[1].defList.begin() + j + 1, a);
			else
				unitDef.sectionList[1].defList.insert(unitDef.sectionList[1].defList.begin() + j - 1, a);

			break;
		}//if
	}//for

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, NULL);
	RefreshTextView();
}//void CNCEditor2Doc::MoveWRepl(CString srcString, CString dstString, bool up)

void CNCEditor2Doc::MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString)
{
	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	CNCTreeCtrlSnapshot snapshot;
	GetTreeView()->GetTreeCtrlSnapshot(snapshot);

	if(!unitDef.MoveWRepl(srcsrcString, srcdstString, dstsrcString, dstdstString))
	{
		CString str; str.LoadString(IDS_ERROR_MOVE_WREPL);
		AfxMessageBox(str);
		return;
	}

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot);
	RefreshTextView();
}//void CNCEditor2Doc::MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString)

void CNCEditor2Doc::AddDef(CString secName, Def definition)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(secName != str);

	CNCUnitSnapshot usnapshot;
	MakeUnitSnapshot(usnapshot);

	Definition a(definition);
	str.LoadString(IDS_UNKNOWN_DEFINITION);
	a.def.briefDescr = str;
	a.def.presence = true;
	a.def.defParam = a.def.param;

	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == secName)
		{
			unitDef.sectionList[i].defList.push_back(a);
			break;
		}//if
	}//for

	CommitChange(usnapshot);
	GetTreeView()->ShowNCUnitDef(&unitDef, NULL);
	RefreshTextView();
}//void CNCEditor2Doc::AddDef(CString secName, Def definition)

void CNCEditor2Doc::CommitChange(CNCUnitSnapshot &snapshot)
{
	undoStack.push(snapshot);

	ClearRedoStack();

	SetModifiedFlag(1);
}//void CNCEditor2Doc::CommitChange()

void CNCEditor2Doc::MakeUnitSnapshot(CNCUnitSnapshot &snapshot)
{
	CNCTreeCtrlSnapshot treeSnapshot;
	GetTreeView()->GetTreeCtrlSnapshot(treeSnapshot);

	CNCUnitDef unitSnapshot;
	unitSnapshot = unitDef;

	snapshot.treeSnaphsot = treeSnapshot;
	snapshot.unitDef = unitSnapshot;
}//void CNCEditor2Doc::MakeUnitSnapshot(CNCUnitSnapshot &snapshot)

void CNCEditor2Doc::ApplyUnitSnapshot(CNCUnitSnapshot &snapshot)
{
	unitDef = snapshot.unitDef;
	RefreshTextView();

	GetTreeView()->ShowNCUnitDef(&unitDef, &snapshot.treeSnaphsot);
}//void CNCEditor2Doc::ApplyUnitSnapshot(CNCUnitSnapshot &snapshot)

void CNCEditor2Doc::Undo()
{
	CNCUnitSnapshot redoSnapshot;

	MakeUnitSnapshot(redoSnapshot);
	redoStack.push(redoSnapshot);
	

	CNCUnitSnapshot snapshot;

	snapshot = undoStack.top();
	undoStack.pop();

	ApplyUnitSnapshot(snapshot);
}//void CNCEditor2Doc::Undo()
	
void CNCEditor2Doc::Redo()
{
	CNCUnitSnapshot undoSnapshot;

	MakeUnitSnapshot(undoSnapshot);
	undoStack.push(undoSnapshot);

	CNCUnitSnapshot snapshot;

	snapshot = redoStack.top();
	redoStack.pop();

	ApplyUnitSnapshot(snapshot);
}//void CNCEditor2Doc::Redo()

void CNCEditor2Doc::ClearHistory()
{
	while(!undoStack.empty())
	{
		undoStack.pop();
	}//while
	while(!redoStack.empty())
	{
		redoStack.pop();
	}//while
}//void CNCEditor2Doc::ClearHistory()

void CNCEditor2Doc::ClearRedoStack()
{
	while(!redoStack.empty())
		redoStack.pop();
}//void CNCEditor2Doc::ClearRedoStack()

bool CNCEditor2Doc::isUndoActive()
{
	return !undoStack.empty();
}//bool CNCEditor2Doc::isUndoActive()

bool CNCEditor2Doc::isRedoActive()
{
	return !redoStack.empty();
}//bool CNCEditor2Doc::isRedoActive()

void CNCEditor2Doc::SetInternalSelect(bool IS)
{
	GetTreeView()->SetInternalSelect(IS);
}//void CNCEditor2Doc::SetInternalSelect(bool IS)

void CNCEditor2Doc::ShowSectionProp(CString sectionName)
{
	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == sectionName)
		{
			GetPropertiesWnd()->ShowSection(&unitDef.sectionList[i]);
			GetHelpPane()->ShowHelp(unitDef.sectionList[i].fullDescr.text, unitDef.sectionList[i].fullDescr.picName);
			break;
		}//if
	}//for
}//void CNCEditor2Doc::ShowSectionProp(CString sectioName)

void CNCEditor2Doc::ShowDefinitionProp(CString sectionName, CString defName)
{
	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == sectionName)
		{
			for(unsigned int j = 0; j < unitDef.sectionList[i].defList.size(); j++)
			{
				if(unitDef.sectionList[i].defList[j].def.defWord == defName)
				{
					GetPropertiesWnd()->ShowDefinition(&unitDef.sectionList[i].defList[j].def);
					GetHelpPane()->ShowHelp(unitDef.sectionList[i].defList[j].def.fullDescr.text, unitDef.sectionList[i].defList[j].def.fullDescr.picName);
					break;
				}//if
			}//for
			break;
		}//if
	}//for
}//void CNCEditor2Doc::ShowDefinitionProp(CString defName)

void CNCEditor2Doc::ShowDefinitionUnknownProp(CString sectionName, CString defName)
{
	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == sectionName)
		{
			for(unsigned int j = 0; j < unitDef.sectionList[i].defList.size(); j++)
			{
				if(unitDef.sectionList[i].defList[j].def.defWord == defName)
				{
					GetPropertiesWnd()->ShowDefinitionUnknown(&unitDef.sectionList[i].defList[j].def);
					GetHelpPane()->ShowHelp(unitDef.sectionList[i].defList[j].def.fullDescr.text, unitDef.sectionList[i].defList[j].def.fullDescr.picName);
					break;
				}//if
			}//for
			break;
		}//if
	}//for
}//void CNCEditor2Doc::ShowDefinitionUnknownProp(CString sectionName, CString defName)

void CNCEditor2Doc::ShowWReplProp(CString srcString, CString dstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == str)
		{
			for(unsigned int j = 0; j < unitDef.sectionList[i].defList.size(); j++)
			{
				ASSERT(unitDef.sectionList[i].defList[j].GetType() == D_WREPL);

				if(unitDef.sectionList[i].defList[j].wRep.srcString == srcString && 
					unitDef.sectionList[i].defList[j].wRep.destString == dstString)
				{
					GetPropertiesWnd()->ShowWRepl(&unitDef.sectionList[i].defList[j].wRep);
					CString str2; str2.LoadString(IDS_WREPL_ELEMENT_HELP);
					CString str3; str3.LoadString(IDS_WREPL_ELEMENT_PICTURE);
					GetHelpPane()->ShowHelp(str2, str3);
					break;
				}//if
			}//for
			break;
		}//if
	}//for
}//void CNCEditor2Doc::ShowWReplProp(CString srcString, CString dstString)

void CNCEditor2Doc::ShowGroupProp(CString sectName, CString groupName)
{
	for(unsigned int i = 0; i < unitDef.sectionList.size(); i++)
	{
		if(unitDef.sectionList[i].name == sectName)
		{
			Group group;
			if(!unitDef.GetGroup(sectName, groupName, group))
			{
				CString str; str.LoadString(IDS_ERROR_GROUP_DOESNTEXIST);
				AfxMessageBox(str);
				return;
			}//if
			GetPropertiesWnd()->ShowGroup(&unitDef.sectionList[i], groupName);
			
			GetHelpPane()->ShowHelp(group.fullDescr.text, group.fullDescr.picName);
			for(unsigned j = 0; j < unitDef.sectionList[i].groupList.size(); j++)
			{
				if(unitDef.sectionList[i].groupList[j].name == groupName)
				{
					GetHelpPane()->ShowHelp(unitDef.sectionList[i].groupList[j].fullDescr.text, unitDef.sectionList[i].groupList[j].fullDescr.picName);

					break;
				}//if
			}//for
		}//if
	}//for
}//void CNCEditor2Doc::ShowGroupProp(CString groupName)

void CNCEditor2Doc::RefreshTextView()
{
	CString str = unitDef.GetString();
	((CEditView*)m_viewList.GetHead())->SetWindowText(unitDef.GetString());
}//void CNCEditor2Doc::RefreshTextView()

// CNCEditor2Doc diagnostics

#ifdef _DEBUG
void CNCEditor2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNCEditor2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNCEditor2Doc commands

CNCUnitTreePane * CNCEditor2Doc::GetTreeView()
{
	/// Панель с деревом хранится в MainFrame
	/// доступ к MainFrame можно получить только через вид (NCEditor2View)
	return ((CMainFrame*)(((CEditView*)m_viewList.GetHead())->GetParentFrame()))->GetTreeView();
}//const CNCUnitTreePane * CNCEditor2Doc::GetTreeView()

CPropertiesWnd * CNCEditor2Doc::GetPropertiesWnd()
{
	/// Получаем окно свойств из MainFrame
	return ((CMainFrame*)(((CEditView*)m_viewList.GetHead())->GetParentFrame()))->GetPropertiesWnd();
}//CPropertiesWnd * CNCEditor2Doc::GetPropertiesWnd()

CNCUnitHelpPane * CNCEditor2Doc::GetHelpPane()
{
	return ((CMainFrame*)(((CEditView*)m_viewList.GetHead())->GetParentFrame()))->GetHelpPane();
}//CNCUnitHelpPane * CNCEditor2Doc::GetHelpPane()

CString CNCEditor2Doc::GetCurFile()
{
	return curFile;
}//CString CNCEditor2Doc::GetCurFile()

BOOL CNCEditor2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if(successLoad)
		return TRUE;
	else
		return FALSE;
}
