
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "NCMControl.h"
#include "NCEditor2Doc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


void CPropertiesWnd::ShowSection(Section *section)
{
	/// Очищаем список свойств
	m_wndPropList.RemoveAll();

	editingObjectType = PO_SECTION;
	editingObject = section;

	/// Отображаем нередактируемое свойство "Название секции" 
	CString str; str.LoadString(IDS_SECTION_NAME);
	CMFCPropertyGridProperty *pName = new CMFCPropertyGridProperty(str, (_variant_t)"", section->briefDescr);
	pName->SetValue(section->name);
	pName->SetOriginalValue(section->name);
	pName->AllowEdit(false);
	m_wndPropList.AddProperty(pName);

	/// Создаём группу элементов для отображения всех определений секции
	str.LoadString(IDS_DEFINITION_LIST);
	CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty(str);
	for(unsigned int i = 0; i < section->defList.size(); i++)
	{
		switch(section->defList[i].GetType())
		{
		case D_COMMON:
			{
				if(!section->defList[i].def.presence)
					break;

				CMFCPropertyGridProperty *pDef;

				switch(section->defList[i].def.param.GetType())
				{
				case P_B:
					pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)false, section->defList[i].def.briefDescr);
					pDef->SetValue((_variant_t)section->defList[i].def.param.b);
					pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.b);
					break;
				case P_I:
					{
					pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)0l, section->defList[i].def.briefDescr);
					pDef->SetValue((_variant_t)(long)section->defList[i].def.param.i);
					pDef->SetOriginalValue((_variant_t)(long)section->defList[i].def.param.i);
					}
					break;
				case P_D:
					pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)0.f, section->defList[i].def.briefDescr);
					pDef->SetValue((_variant_t)(float)section->defList[i].def.param.d);
					pDef->SetOriginalValue((_variant_t)(float)section->defList[i].def.param.d);
					break;
				case P_S:
					pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)"", section->defList[i].def.briefDescr);
					pDef->SetValue((_variant_t)section->defList[i].def.param.s);
					pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.s);
					break;
				case P_SE:
					pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)"", section->defList[i].def.briefDescr);
					pDef->SetValue((_variant_t)section->defList[i].def.param.s);
					pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.s);
					pDef->AllowEdit(0);
					for(unsigned int k = 0; k < section->defList[i].def.enumerate.size(); k++)
					{
						pDef->AddOption(section->defList[i].def.enumerate[k]);
					}//for
					break;
				case P_EE:
					pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)"", section->defList[i].def.briefDescr);
					pDef->SetValue((_variant_t)section->defList[i].def.param.s);
					pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.s);
					for(unsigned int k = 0; k < section->defList[i].def.enumerate.size(); k++)
					{
						pDef->AddOption(section->defList[i].def.enumerate[k]);
					}//for
					break;
				case P_EMPTY:
					ASSERT(false);
					break;
				}//switch
				pGroup->AddSubItem(pDef);
			}
			break;
		case D_WREPL:
			{
				CString str; str.LoadString(IDS_REPLACE);
				CMFCPropertyGridProperty *pWRepl = new CMFCPropertyGridProperty(str);

				/// Номер wrepl в секции Word Replacement и src/dst кодируется переменной
				/// unsigned long
				
				/// Для каждого элемента секции берем его номер, увеличиваем на единицу,
				/// Для srcString записываем в Data отрицательное значение,
				/// для dstString записываем в Data положительное значение
				long a = i+1;

				str.LoadString(IDS_SOURCE_STRING);
				CMFCPropertyGridProperty *pSrcS = new CMFCPropertyGridProperty(str, (_variant_t)"", _T(""));
				pSrcS->SetValue(section->defList[i].wRep.srcString);
				pSrcS->SetOriginalValue(section->defList[i].wRep.srcString);
				pSrcS->SetData((DWORD_PTR)(-a));
				str.LoadString(IDS_DESTINATION_STRING);
				CMFCPropertyGridProperty *pDstS = new CMFCPropertyGridProperty(str, (_variant_t)"", _T(""));
				pDstS->SetValue(section->defList[i].wRep.destString);
				pDstS->SetOriginalValue(section->defList[i].wRep.destString);
				pDstS->SetData((DWORD_PTR)a);
				pWRepl->AddSubItem(pSrcS);
				pWRepl->AddSubItem(pDstS);
				pGroup->AddSubItem(pWRepl);
			}
			break;
		}//switch
	}//for
	m_wndPropList.AddProperty(pGroup);
}//void CPropertiesWnd::ShowSection(Section *section)

void CPropertiesWnd::ShowGroup(Section *section, CString group)
{
	/// Очищаем список свойств
	m_wndPropList.RemoveAll();

	editingObjectType = PO_GROUP;
	editingObject = section;

	/// Отображаем свойство "Название группы" 
	CString str; str.LoadString(IDS_GROUP_NAME);
	CMFCPropertyGridProperty *pName = new CMFCPropertyGridProperty(str, (variant_t)"",_T(""));
	pName->SetValue(group);
	pName->SetOriginalValue(group);
	pName->AllowEdit(0);
	m_wndPropList.AddProperty(pName);

	/// Создаём группу элементов для отображения всех определений группы
	/// \todo отображение группы
	str.LoadString(IDS_DEFINITION_LIST);
	CMFCPropertyGridProperty *pGroup = new CMFCPropertyGridProperty(str);
	
	for(unsigned int i = 0; i < section->defList.size(); i++)
	{
		if(!section->defList[i].def.presence)
			continue;

		if(section->defList[i].def.group != group)
			continue;

		CMFCPropertyGridProperty *pDef;

		switch(section->defList[i].def.param.GetType())
		{
		case P_B:
			pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)false, section->defList[i].def.briefDescr);
			pDef->SetValue((_variant_t)section->defList[i].def.param.b);
			pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.b);
			break;
		case P_I:
			pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)0l, section->defList[i].def.briefDescr);
			pDef->SetValue((_variant_t)(long)section->defList[i].def.param.i);
			pDef->SetOriginalValue((_variant_t)(long)section->defList[i].def.param.i);
			break;
		case P_D:
			pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)0.f, section->defList[i].def.briefDescr);
			pDef->SetValue((_variant_t)(float)section->defList[i].def.param.d);
			pDef->SetOriginalValue((_variant_t)(float)section->defList[i].def.param.d);
			break;
		case P_S:
			pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)"", section->defList[i].def.briefDescr);
			pDef->SetValue((_variant_t)section->defList[i].def.param.s);
			pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.s);
			break;
		case P_SE:
			pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)"", section->defList[i].def.briefDescr);
			pDef->SetValue((_variant_t)section->defList[i].def.param.s);
			pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.s);
			pDef->AllowEdit(0);
			for(unsigned int k = 0; k < section->defList[i].def.enumerate.size(); k++)
			{
				pDef->AddOption(section->defList[i].def.enumerate[k]);
			}//for
			break;
		case P_EE:
			pDef = new CMFCPropertyGridProperty(section->defList[i].def.defWord, (_variant_t)"", section->defList[i].def.briefDescr);
			pDef->SetValue((_variant_t)section->defList[i].def.param.s);
			pDef->SetOriginalValue((_variant_t)section->defList[i].def.param.s);
			for(unsigned int k = 0; k < section->defList[i].def.enumerate.size(); k++)
			{
				pDef->AddOption(section->defList[i].def.enumerate[k]);
			}//for
			break;
		case P_EMPTY:
			ASSERT(false);
			break;
		}//switch
		pGroup->AddSubItem(pDef);
	}//for

	m_wndPropList.AddProperty(pGroup);

}//void CPropertiesWnd::ShowGroup(Section *section, CString group)

void CPropertiesWnd::ShowDefinition(Def *def)
{
	/// Очищаем список свойств
	m_wndPropList.RemoveAll();

	editingObjectType = PO_DEFINITION;
	editingObject = def;

	/// Отображаем свойство "Слово определения" 
	CString str; str.LoadString(IDS_DEFINITION_WORD);
	CMFCPropertyGridProperty *pName = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
	pName->AllowEdit(0);
	pName->SetData(0);// Номер элемента в списке свойств
	pName->SetValue(def->defWord);
	pName->SetOriginalValue(def->defWord);
	m_wndPropList.AddProperty(pName);

	CMFCPropertyGridProperty *pParam;

	CString sType;
	switch(def->param.GetType())
	{
	case P_B:
		sType.LoadString(IDS_TYPEW_BOOL);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)false, def->briefDescr);
		pParam->SetValue((_variant_t)def->param.b);
		pParam->SetOriginalValue((_variant_t)def->param.b);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false, def->briefDescr);
		pParam->SetValue((_variant_t)def->defParam.b);
		pParam->SetOriginalValue((_variant_t)def->defParam.b);
		//pParam->AllowEdit(0); // Для логических свойств нельзя так делать
		pParam->Enable(0);
		pParam->SetData(2);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_I:
		sType.LoadString(IDS_TYPEW_INT);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)0l, def->briefDescr);
		pParam->SetValue((_variant_t)(long)def->param.i);
		pParam->SetOriginalValue((_variant_t)(long)def->param.i);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)0l, def->briefDescr);
		pParam->SetValue((_variant_t)(long)def->defParam.i);
		pParam->SetOriginalValue((_variant_t)(long)def->defParam.i);
		pParam->AllowEdit(0);
		pParam->SetData(2);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_D:
		sType.LoadString(IDS_TYPEW_DOUBLE);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)0.f, def->briefDescr);
		pParam->SetValue((_variant_t)(float)def->param.d);
		pParam->SetOriginalValue((_variant_t)(float)def->param.d);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)0.f, def->briefDescr);
		pParam->SetValue((_variant_t)(float)def->defParam.d);
		pParam->SetOriginalValue((_variant_t)(float)def->defParam.d);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_S:
		sType.LoadString(IDS_TYPEW_STRING);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->param.s);
		pParam->SetOriginalValue(def->param.s);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->defParam.s);
		pParam->SetOriginalValue(def->defParam.s);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_SE:
		sType.LoadString(IDS_TYPEW_SENUM);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->param.s);
		pParam->SetOriginalValue(def->param.s);
		pParam->SetData(1);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		for(unsigned int k = 0; k < def->enumerate.size(); k++)
		{
			pParam->AddOption(def->enumerate[k]);
		}//for
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->defParam.s);
		pParam->SetOriginalValue(def->defParam.s);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_EE:
		sType.LoadString(IDS_TYPEW_EENUM);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->param.s);
		pParam->SetOriginalValue(def->param.s);
		pParam->SetData(1);// Номер элемента в списке свойств
		for(unsigned int k = 0; k < def->enumerate.size(); k++)
		{
			pParam->AddOption(def->enumerate[k]);
		}//for
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->defParam.s);
		pParam->SetOriginalValue(def->defParam.s);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_EMPTY:
		ASSERT(FALSE);
		break;
	}//switch
}//void CPropertiesWnd::ShowDefinition(Definition *definition)

void CPropertiesWnd::ShowDefinitionUnknown(Def *def)
{
	/// Очищаем список свойств
	m_wndPropList.RemoveAll();

	editingObjectType = PO_DEFINITION;
	editingObject = def;

	/// Отображаем свойство "Слово определения" 
	CString str; str.LoadString(IDS_DEFINITION_WORD);
	CMFCPropertyGridProperty *pName = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
	pName->AllowEdit(0);
	pName->SetData(0);// Номер элемента в списке свойств
	pName->SetValue(def->defWord);
	pName->SetOriginalValue(def->defWord);
	m_wndPropList.AddProperty(pName);

	CMFCPropertyGridProperty *pParam;

	CString sType;
	switch(def->param.GetType())
	{
	case P_B:
		sType.LoadString(IDS_TYPEW_BOOL);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)false, def->briefDescr);
		pParam->SetValue((_variant_t)def->param.b);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue((_variant_t)def->param.b);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false, def->briefDescr);
		pParam->SetValue((_variant_t)def->defParam.b);
		pParam->SetOriginalValue((_variant_t)def->defParam.b);
		//pParam->AllowEdit(0); // Для логических свойств нельзя так делать
		pParam->Enable(0);
		pParam->SetData(2);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->Enable(0);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_I:
		sType.LoadString(IDS_TYPEW_INT);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)0l, def->briefDescr);
		pParam->SetValue((_variant_t)(long)def->param.i);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue((_variant_t)(long)def->param.i);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)0l, def->briefDescr);
		pParam->SetValue((_variant_t)(long)def->defParam.i);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue((_variant_t)(long)def->defParam.i);
		pParam->AllowEdit(0);
		pParam->SetData(2);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->Enable(0);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_D:
		sType.LoadString(IDS_TYPEW_DOUBLE);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)0.f, def->briefDescr);
		pParam->SetValue((_variant_t)(float)def->param.d);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue((_variant_t)(float)def->param.d);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)0.f, def->briefDescr);
		pParam->SetValue((_variant_t)(float)def->defParam.d);
		pParam->SetOriginalValue((_variant_t)(float)def->defParam.d);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->Enable(0);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_S:
		sType.LoadString(IDS_TYPEW_STRING);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->param.s);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue(def->param.s);
		pParam->SetData(1);// Номер элемента в списке свойств
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->defParam.s);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue(def->defParam.s);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);
		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->Enable(0);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_SE:
		sType.LoadString(IDS_TYPEW_SENUM);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->param.s);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue(def->param.s);
		pParam->SetData(1);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		for(unsigned int k = 0; k < def->enumerate.size(); k++)
		{
			pParam->AddOption(def->enumerate[k]);
		}//for
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->defParam.s);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue(def->defParam.s);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->Enable(0);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_EE:
		sType.LoadString(IDS_TYPEW_EENUM);
		pParam = new CMFCPropertyGridProperty(sType, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->param.s);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue(def->param.s);
		pParam->SetData(1);// Номер элемента в списке свойств
		for(unsigned int k = 0; k < def->enumerate.size(); k++)
		{
			pParam->AddOption(def->enumerate[k]);
		}//for
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_DEFAULT);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)"", def->briefDescr);
		pParam->SetValue(def->defParam.s);
		pParam->AllowEdit(0);
		pParam->SetOriginalValue(def->defParam.s);
		pParam->SetData(2);// Номер элемента в списке свойств
		pParam->AllowEdit(0);
		m_wndPropList.AddProperty(pParam);

		str.LoadString(IDS_PRESENCE);
		pParam = new CMFCPropertyGridProperty(str, (variant_t)false , _T(""));
		pParam->SetValue((_variant_t)def->presence);
		pParam->Enable(0);
		pParam->SetOriginalValue((_variant_t)def->presence);
		pParam->SetData(3);// Номер элемента в списке свойств

		m_wndPropList.AddProperty(pParam);
		break;
	case P_EMPTY:
		ASSERT(FALSE);
		break;
	}//switch
}//void CPropertiesWnd::ShowDefinitionUnknown(Def *def)

void CPropertiesWnd::ShowWRepl(WRep *wrep)
{
	/// Очищаем список свойств
	m_wndPropList.RemoveAll();

	editingObjectType = PO_WREPL;
	editingObject = wrep;

	CString str; str.LoadString(IDS_SOURCE_STRING);
	CMFCPropertyGridProperty *pSrcS = new CMFCPropertyGridProperty(str, (variant_t)"", _T(""));
	pSrcS->SetValue(wrep->srcString);
	pSrcS->SetOriginalValue(wrep->srcString);
	pSrcS->SetData(0);
	m_wndPropList.AddProperty(pSrcS);
	str.LoadString(IDS_DESTINATION_STRING);
	CMFCPropertyGridProperty *pDstS = new CMFCPropertyGridProperty(str, (variant_t)"", _T(""));
	pDstS->SetValue(wrep->destString);
	pDstS->SetOriginalValue(wrep->destString);
	pDstS->SetData(1);
	m_wndPropList.AddProperty(pDstS);
}//void CPropertiesWnd::ShowWRepl(WRep *wrep)

CNCEditor2Doc * CPropertiesWnd::GetDocument()
{
	CFrameWnd *Parent = GetParentFrame();
	return (CNCEditor2Doc*)((CMainFrame*)Parent)->GetActiveDocument();
}//CNCEditor2Doc * CPropertiesWnd::GetDocument()

void CPropertiesWnd::OnPropertyChanged(CMFCPropertyGridProperty *pProp)
{
	switch(editingObjectType)
	{
	case PO_SECTION:
		{
			/// Определяем вид секции
			CString sectionName = ((Section*)editingObject)->name;

			CString str; str.LoadString(IDS_WORD_REPLACEMENT);
			if(sectionName == str)
				/// wrepl
			{
				CString defName = pProp->GetName();
				/// Получаем знаковое число из беззнакового
				unsigned long asdf = pProp->GetData();
				long lPos = asdf;
				unsigned long wreplPos = abs(lPos)-1;

				CString newsrcString;
				CString newdstString;

				/// Если lPos > 0, то 
				/// поменялась dstString, если <0, то srcString
				if(lPos > 0)
				{
					newdstString = pProp->GetValue();
					newsrcString = ((Section*)editingObject)->defList[wreplPos].wRep.srcString;
				}//if
				else
				{
					newsrcString = pProp->GetValue();
					newdstString = ((Section*)editingObject)->defList[wreplPos].wRep.destString;
				}//else

				GetDocument()->ChangeWRepl(wreplPos, newsrcString, newdstString);
			}//if
			else
				/// не wrepl
			{
				CString defName = pProp->GetName();
				
				for(unsigned int j = 0; j < ((Section*)editingObject)->defList.size(); j++)
				{
					if(((Section*)editingObject)->defList[j].def.defWord == defName)
					{
						switch(((Section*)editingObject)->defList[j].def.param.GetType())
						{
						case P_B:
							{
								COleVariant tVariant;
								tVariant = pProp->GetValue();
								Param p((bool)tVariant.boolVal);
								GetDocument()->ChangeDef(sectionName, defName, p);
							}
							break;
						case P_I:
							{
								COleVariant tVariant;
								tVariant = pProp->GetValue();
								Param p(tVariant.intVal);
								GetDocument()->ChangeDef(sectionName, defName, p);
							}
							break;
						case P_D:
							{
								COleVariant tVariant;
								tVariant = pProp->GetValue();
								Param p((double)tVariant.fltVal);
								GetDocument()->ChangeDef(sectionName, defName, p);
							}
							break;
						case P_S:
							{
								CString s = pProp->GetValue();
								Param p(s);
								GetDocument()->ChangeDef(sectionName, defName, p);
							}
							break;
						case P_SE:
							{
								CString s = pProp->GetValue();
								Param e(s, true);
								GetDocument()->ChangeDef(sectionName,defName, e);
							}
							break;
						case P_EE:
							{
								CString s = pProp->GetValue();
								Param e(s, false);
								GetDocument()->ChangeDef(sectionName,defName, e);
							}
							break;
						case P_EMPTY:
							ASSERT(false);
							break;
						}//switch
						break;//for
					}//if
				}//for
			}//else
		}
		break;
	case PO_GROUP:
		{
			/// Определяем вид секции
			CString sectionName = ((Section*)editingObject)->name;

			CString defName = pProp->GetName();
			
			for(unsigned int j = 0; j < ((Section*)editingObject)->defList.size(); j++)
			{
				if(((Section*)editingObject)->defList[j].def.defWord == defName)
				{
					switch(((Section*)editingObject)->defList[j].def.param.GetType())
					{
					case P_B:
						{
							COleVariant tVariant;
							tVariant = pProp->GetValue();
							Param p((bool)tVariant.boolVal);
							GetDocument()->ChangeDef(sectionName, defName, p);
						}
						break;
					case P_I:
						{
							COleVariant tVariant;
							tVariant = pProp->GetValue();
							Param p(tVariant.intVal);
							GetDocument()->ChangeDef(sectionName, defName, p);
						}
						break;
					case P_D:
						{
							COleVariant tVariant;
							tVariant = pProp->GetValue();
							Param p((double)tVariant.fltVal);
							GetDocument()->ChangeDef(sectionName, defName, p);
						}
						break;
					case P_S:
						{
							CString s = pProp->GetValue();
							Param p(s);
							GetDocument()->ChangeDef(sectionName, defName, p);
						}
						break;
					case P_SE:
						{
							CString s = pProp->GetValue();
							Param e(s, true);
							GetDocument()->ChangeDef(sectionName,defName, e);
						}
						break;
					case P_EE:
						{
							CString s = pProp->GetValue();
							Param e(s, false);
							GetDocument()->ChangeDef(sectionName,defName, e);
						}
						break;
					case P_EMPTY:
						ASSERT(false);
						break;
					}//switch
					break;//for
				}//if
			}//for
		}
		break;
	case PO_DEFINITION:
		{
			/// Считываем все параметры Definition и записываем в стойку
			switch(((Def*)editingObject)->defParam.GetType())
			{
			case P_B:
				{
					int num = (int)pProp->GetData();
					if(num == 1)
					{
						COleVariant tVariant = pProp->GetValue();
						Param p((bool)tVariant.boolVal);

						GetDocument()->ChangeDef( ((Def*)editingObject), p);
					}//if
					else
					if(num == 3)
					{
						COleVariant tVariant;
						tVariant = pProp->GetValue();

						GetDocument()->ChangeDefPresence( (Def*)editingObject, (bool)tVariant.boolVal);
					}//if
					else
						ASSERT(FALSE);
				}
				break;
			case P_I:
				{
					int num = (int)pProp->GetData();
					if(num == 1)
					{
						COleVariant tVariant = pProp->GetValue();

						int i = tVariant.intVal;

						Param p(i);

						GetDocument()->ChangeDef( ((Def*)editingObject), p);
					}//if
					else
					if(num == 3)
					{
						COleVariant tVariant;
						tVariant = pProp->GetValue();

						GetDocument()->ChangeDefPresence( (Def*)editingObject, (bool)tVariant.boolVal);
					}//if
					else
						ASSERT(FALSE);
				}
				break;
			case P_D:
				{
					int num = (int)pProp->GetData();
					if(num == 1)
					{
						COleVariant tVariant = pProp->GetValue();

						double d = tVariant.fltVal;

						Param p(d);
						GetDocument()->ChangeDef( (Def*)editingObject, p);
					}//if
					else
					if(num == 3)
					{
						COleVariant tVariant;
						tVariant = pProp->GetValue();

						GetDocument()->ChangeDefPresence( (Def*)editingObject, (bool)tVariant.boolVal);
					}//if
					else
						ASSERT(FALSE);
				}
				break;
			case P_S:
				{
					int num = (int)pProp->GetData();
					if(num == 1)
					{
						CString str = pProp->GetValue();

						Param p(str);
						GetDocument()->ChangeDef( (Def*)editingObject, p);
					}//if
					else
					if(num == 3)
					{
						COleVariant tVariant = pProp->GetValue();

						GetDocument()->ChangeDefPresence( (Def*)editingObject, (bool)tVariant.boolVal);
					}//if
					else
						ASSERT(FALSE);
				}
				break;
			case P_SE:
				{
					int num = (int)pProp->GetData();
					if(num == 1)
					{
						CString str = pProp->GetValue();

						Param p(str, true);
						GetDocument()->ChangeDef( (Def*)editingObject, p);
					}//if

					else
					if(num == 3)
					{
						COleVariant tVariant = pProp->GetValue();

						GetDocument()->ChangeDefPresence( (Def*)editingObject, (bool)tVariant.boolVal);
					}//if
					else
						ASSERT(FALSE);
				}
				break;
			case P_EE:
				{
					int num = (int)pProp->GetData();
					if(num == 1)
					{
						CString str = pProp->GetValue();

						Param p(str, false);
						GetDocument()->ChangeDef( (Def*)editingObject, p);
					}//if
					else
					if(num == 3)
					{
						COleVariant tVariant = pProp->GetValue();

						GetDocument()->ChangeDefPresence( (Def*)editingObject, (bool)tVariant.boolVal);
					}//if
					else
						ASSERT(FALSE);
				}
				break;
			case P_EMPTY:
				ASSERT(false);
				break;
			}//switch
		}
		break;
	case PO_WREPL:
		{
			CString srcString;
			CString dstString;
			switch(pProp->GetData())
			{
			case 0:
				srcString = pProp->GetValue();
				dstString = ((WRep*)editingObject)->destString;
				break;
			case 1:
				srcString = ((WRep*)editingObject)->srcString;
				dstString = pProp->GetValue();
				break;
			default:
				ASSERT(FALSE);
				break;
			}//switch

			GetDocument()->ChangeWRepl(((WRep*)editingObject)->srcString, 
									   ((WRep*)editingObject)->destString,
									   srcString, 
									   dstString );
		}
		break;
	}//switch
	pProp->SetOriginalValue(pProp->GetValue());

}//void CPropertiesWnd::OnPropertyChanged(CMFCPropertyGridProperty *pProp)

void CPropertiesWnd::OnChangeSelection(CMFCPropertyGridProperty *pNewSel)
{
	/// Задача функции - отображать помощь по выбираемым элементам в панели помощи программы
	/// только для элементов, для которых предусмотрено описание
	switch(editingObjectType)
	{
	case PO_SECTION:
		{
			/// Определяем вид секции
			CString sectionName = ((Section*)editingObject)->name;

			CString str; str.LoadString(IDS_WORD_REPLACEMENT);
			if(sectionName != str)
			{
				/// Обрабатываем только не word replacement

				/// Мы должны показывать помощь только по элементам секции.
				/// Пропускаем название секции
				if( pNewSel == m_wndPropList.GetProperty(0))
					return;
				/// Пропускаем группирующий элемент для элементов секции
				if( pNewSel == m_wndPropList.GetProperty(1))
					return;

				CString defName = pNewSel->GetName();

				for(unsigned int j = 0; j < ((Section*)editingObject)->defList.size(); j++)
				{
					if(((Section*)editingObject)->defList[j].def.defWord == defName)
					{
						GetDocument()->GetHelpPane()->ShowHelp(
							///text
							((Section*)editingObject)->defList[j].def.fullDescr.text,
							///picname
							((Section*)editingObject)->defList[j].def.fullDescr.picName);
					}//if
				}//for

			}//if
		}//case
	}//switch
}//void CPropertiesWnd::OnChangeSelection(CMFCPropertyGridProperty *pNewSel)

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() -cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, rectDummy, this, (UINT)-1))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.EnableHeaderCtrl(0);
	m_wndPropList.SetBoolLabels(_T("Да"),_T("Нет"));

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), (_variant_t) false, _T("Specifies the window's font will be non-bold and controls will have a 3D border")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl();
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), NULL, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static TCHAR BASED_CODE szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CPropertiesWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	return;
	// TODO: Add your message handler code here
}
