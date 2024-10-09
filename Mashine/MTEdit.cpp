#include "StdAfx.h"
#include <filesystem>
#include "ConstDef.h"
#include "MTForwardAxis.h"
#include "MTRotaryAxis.h"
#include "MTSpinAxis.h"
#include "MTPartAxis.h"
#include "MTSimpleChainAxis.h"
#include "MTToolAxis.h"
#include "MTVirtToolAxis.h"
#include "MTStockAxis.h"
#include "WM_APP_messages.h"
#include "MTMashineTool.h"
#include "MTEditPane.h"
#include "MTEditTreePane.h"
#include "MTPart.h"
#include "SPropChange.h"
#include "NCMProject.h"
#include <sys/stat.h>
#include "NOCImport.h"
#include "NStlConverterDlg.h"
#include "io.h"
#include "MTConfig.h"
#include "MTEdit.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MTEdit::MTEdit(bool demo)
{
	pMachCoordsDef = NULL;
	pMachState = NULL;
	MachinePath = "";
	bEditMode = false;
	DEMO = demo;
	EditingMT = nullptr;
	MachineChanged = false;
}

MTEdit::~MTEdit(void)
{
	delete pMachState;
	delete pMachCoordsDef;
	ClearHistory();
}

bool MTEdit::Activate(MTMachineTool &MT, MTEditTreePane &TreePane, MTEditPane &PropPane, const CString &path)
{
	MT.SetCurConf(BMTLinkage());

	MachineChanged = false;
	bEditMode = true;

	EditPanePtr = &PropPane;
	TreePanePtr = &TreePane;

	EditingMT = &MT;
	
	TreePanePtr->TBEnableRedo(false);
	TreePanePtr->TBEnableUndo(false);
	TreePanePtr->TBEnableSave(false);

	SaveMachState();
	ResetMach();

	PropPane.SetMachineTool(EditingMT);
	TreePane.SetMachineTool(EditingMT);
	
	MachinePath = path;
	TreePane.SetMTEdit(this,path);
	PropPane.SetMTEdit(this,path);
	

	TreePane.ShowMachineTool(EditingMT);

	PropPane.ShowPane(TRUE, FALSE, FALSE);
	TreePane.ShowPane(TRUE, FALSE, TRUE);

	TreePane.TBEnableUndo(false);

	return true;
}

bool MTEdit::Deactivate()
{
	//1 - проверка на сохранение
	if(!EditPanePtr->IsClosingAllowed())
		return false;
	if(!leftStack.empty())
	{
		if(DEMO)
		{
			/// Не выполняем сохранение
		}//if
		else
		{
			switch( AfxMessageBox(IDS_EDITOR_EXIT_YES_NO_CANCEL, MB_YESNOCANCEL) )
			{
				case IDYES:
					Save();
				case IDNO:
					TreePanePtr->TBEnableSave(false);
					break;
				case IDCANCEL:
					return false;
					break;
				default:
					ASSERT(FALSE);
					break;	
			}//switch
		}//else
	}//if

	//2 - освобождение памяти
	ClearHistory();
	TreePanePtr->TBEnableRedo(false);
	TreePanePtr->TBEnableUndo(false);
	delete pMachStateOr;
	delete pMachCoordsDefOr;

	//3 - скрытие панелей редактора
	EditPanePtr->Deactivate();
	EditPanePtr->ShowPane(FALSE, FALSE, FALSE);
	TreePanePtr->ShowPane(FALSE, FALSE, FALSE);
	EditPanePtr = NULL;
	TreePanePtr = NULL;

	// 4 - Перезагрузка станка из каталога.
	if (IsMachineChanged())
		EditingMT->LoadXML(MachinePath, EditingMT->GetFileName());
	else
		EditingMT->ClearNow();

	EditingMT->SelectAxis(CString());
	
	bEditMode = false;
	return true;
}

bool MTEdit::CheckEditing()
{
	if (bEditMode)
	{
		CString sError; sError.LoadString(IDS_ERROR_EDIT_MODE);
		AfxMessageBox(sError);
		return false;
	}//if
	else
		return true;
}//bool MTEdit::CheckEditing()

void MTEdit::Exit()
{
	SendMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_COMMAND, ID_PA_MACH_EDIT_END, NULL);
}

bool MTEdit::ApplyChanges()
{
	/// Вторым параметром список окон, которые нужно обновить.
	/// Рядом с определением N_OGL полный список возможных параметров.
	AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL | N_LEFT);
	return true;
}

bool MTEdit::UpdatePicture()
{
	AfxGetMainWnd()->SendMessage(WM_APP_UPDATE_PANES, N_OGL);
	return true;
}//bool MTEdit::UpdatePicture()

void MTEdit::OnPick(int PartNumber)
{
	//если номер чётный - номер оси, если нечётный - номер детали
	if(PartNumber % 2)
	{
		//нечётное
		CString AxisName = EditingMT->GetPartAxisNameByPartIndex( (PartNumber-1) /2);
		TreePanePtr->SelectAxis(AxisName);
	}//if
	else
	{
		//чётное
		CString AxisName = EditingMT->GetCurConfig()[PartNumber/2]->GetName();
		TreePanePtr->SelectAxis(AxisName);
	}//else
	
	return;
}

void MTEdit::SaveMachState()
{
	// Create init string
	CString InitStr = EditingMT->GetCurConfig().CrInitStr();

	SCoordsDef *pNewDef = new SCoordsDef(InitStr);
	SMachState *pNewState = new SMachState(pNewDef);

	for(int i =0; i < pNewDef->GetSize(); i++)
	{
		pNewState->SetCoord(i, EditingMT->GetCurConfig()[(EditingMT->GetCurConfig().GetAxisIndex(pNewState->GetAName(i)))]->GetMovement());
	}//for

	pMachStateOr = pNewState;
	pMachCoordsDefOr = pNewDef;
}//void MTEdit::SaveMachState()

void MTEdit::ResetMach()
{
	/// Если мы не в режиме редактирования, ничего не делаем
	if(!bEditMode)
		return;

	CString InitStr = EditingMT->GetCurConfig().CrInitStr();

	SCoordsDef *pNewDef = new SCoordsDef(InitStr);
	SMachState *pNewState = new SMachState(pNewDef);
	if(pMachStateOr)
		for(int k = 0; k < pMachStateOr->GetSize() - 1; ++k)
			pNewState->SetCoord(pMachStateOr->GetAName(k), pMachStateOr->GetCoord(k));

	delete pMachState;
	delete pMachCoordsDef;
	pMachState = pNewState;
	pMachCoordsDef = pNewDef;

}// void MTEdit::ResetMach()

void MTEdit::ShowPropPane()
{
	EditPanePtr->ShowPane(true,false,true);
}//void MTEdit::ShowPropPane()

bool MTEdit::ShowAxisProp(const CString& AxisName)
{
	int index = EditingMT->GetCurConfig().GetAxisIndex(AxisName);

	EditingMT->SelectAxis(AxisName);
	ApplyChanges();

	EditPanePtr->ShowAxisProp(index);

	return TRUE;
}//bool MTEdit::ShowAxisProp(CString AxisName)

bool MTEdit::ShowColProp(const CString& Axis1, const CString& Axis2)
{
	int index = EditingMT->GetColIndex(Axis1, Axis2);
	if (index < 0)
		return false;
	return ShowColProp(index);
}//bool MTEdit::ShowColProp(CString Axis1, CString Axis2)

bool MTEdit::ShowColProp(int index)
{
	EditPanePtr->ShowColProp(index);

	return TRUE;
}//bool MTEdit::ShowColProp(int index)

void MTEdit::ShowEmptyProp()
{
	EditPanePtr->ShowEmptyProp();
}//void MTEdit::ShowEmptyProp()

bool MTEdit::RenameTreeNode(const CString &OldName, const CString &NewName)
{
	return TreePanePtr->RenameTreeNode(OldName, NewName);
}//bool MTEdit::RenameTreeNode(const CString &OldName, const CString &NewName)

void MTEdit::SetCoordValue(const CString &name, double value)
{
	if(pMachState)
		pMachState->SetCoord(name, value);

}//void MTEdit::MoveAxis(int index, double value)

bool MTEdit::IsDescendant(const CString& BaseAxis, const CString& Axis)
{
	return EditingMT->GetCurConfig().IsDescendant(BaseAxis, Axis);
}//bool MTEdit::IsDescendant(CString &BaseAxis, CString &Axis)

bool MTEdit::MakeChange(MTChange *change_mt)
{
	SetMachineChanged(true);
	TreePanePtr->TBEnableUndo();
	TreePanePtr->TBEnableSave();
	//Сохраняем состояние в левый стек
	leftStack.push(change_mt);

	MTChange *c;
	//очищаем правый стек
	while(!rightStack.empty())
	{	
		c = rightStack.top();
		c->Xml->Clear();
		delete c->Xml;
		delete c;
		rightStack.pop();	
	}//while
	TreePanePtr->TBEnableRedo(false);
	return true;
}//bool MTEdit::MakeChange(MTChange *change_mt)

/*
Каждая функция, начинающаяся с Change выполняет определённые изменения в станке.
Ни станок, ни его отображение в редакторе не модифицируются без этих функций.

Набор функций Change... определяет все возможные изменения, которые пользователь может
совершить над станком в редакторе.

Правила создания этих функций:

1) Проверяем возможность внесения изменений.
Если изменение произвести невозможно, показываем пользователю сообщение об ошибке 
и возвращаем false.

2) Сохраняем состояние станка до изменения в историю изменений:
Создаём объект изменения MTChange. 
Создаём объект TiXmlDocument, сохраняем в него текущее состояние станка.
Записываем объект TiXmlDocument с сохранённым станком в объект MTChange.
Определяем имена узлов в отображаемом дереве, которые должны быть выделены при
отмене изменения (BeforeSelNode), и после redo этого изменения(AfterSelNode)

3) Вносим изменения в объект станка MTMachinetTool. На редактируемый экземпляр ссылается
ссылка *EditingMT

4) Вносим изменения в изображении станка в редакторе.
Если произошло изменение в структуре станка или в имени узлов, вызываем функцию перерисовки
дерева.
Вызываем функцию выделения изменённого узла и отображения его свойств в панели свойств(SelectAndShowPropNode)

5) Вносим изменения в объект редактора, если это необходимо

6) Возвращаем true.

*/

bool MTEdit::ChangeAxisProp(SPropChange *Change, int AxisIndex)
{
	// 1 - Проверяем возможность внесения изменений
	// 2 - Сохраняем состояние редактора в историю изменений
	// 3 - Производим изменения в станке
	// 4 - Производим изменения в GUI
	// 5 - Обновляем параметры редактора
	
	// 1 - Проверяем возможность внесения изменений
	// a - Не существует ли уже такого имени?
	if(Change->NameChanged && EditingMT->GetCurConfig().GetAxisIndex(Change->Name) != -1)
	{
		AfxMessageBox(IDS_ERROR_NAME_EXISTS);
		return false;
	}//if

	if(Change->PositionChanged && Change->Position >9999)
	{
		AfxMessageBox(IDS_ERROR_POSITION_BIG);
		return false;
	}//if

	// c - проверка параметров tool, virtual tool
	switch (EditingMT->GetCurConfig()[AxisIndex]->GetType())
	{
	case MTAxis::AT_TOOL: //tool, проверяем уникальность пары turret - attachment
			if(Change->AttachmentChanged && Change->TurretChanged)
			{
				if(EditingMT->ExistsToolTurAtt(Change->Turret, Change->Attachment))
				{
					CString sError;
					sError.LoadString(IDS_ERROR_TURRET_ATTACHMENT_EXISTS);
					AfxMessageBox(sError);
					return false;
				}//if
			}//if
			if(Change->AttachmentChanged && !Change->TurretChanged)
			{
				if(EditingMT->ExistsToolTurAtt( EditingMT->GetCurConfig()[AxisIndex]->GetTurret(), Change->Attachment))
				{
					CString sError;
					sError.LoadString(IDS_ERROR_TURRET_ATTACHMENT_EXISTS);
					AfxMessageBox(sError);
					return false;
				}//if
			}//if
			if(Change->TurretChanged && !Change->AttachmentChanged)
			{
				if(EditingMT->ExistsToolTurAtt( Change->Turret, EditingMT->GetCurConfig()[AxisIndex]->GetAttachment()))
				{
					CString sError;
					sError.LoadString(IDS_ERROR_TURRET_ATTACHMENT_EXISTS);
					AfxMessageBox(sError);
					return false;
				}//if
			}//if
			break;
		case MTAxis::AT_VTOOL:  //virttool, проверяем уникальность тройки position - attach - turret
			{
				int OldTurret = EditingMT->GetCurConfig()[AxisIndex]->GetTurret();
				int NewTurret = OldTurret;
				if(Change->TurretChanged)
					NewTurret = Change->Turret;
				int NewAttachment = EditingMT->GetCurConfig()[AxisIndex]->GetAttachment();
				if(Change->AttachmentChanged)
					NewAttachment = Change->Attachment;
				int NewPosition = EditingMT->GetCurConfig()[AxisIndex]->GetPosition();
				if(Change->PositionChanged)
					NewPosition = Change->Position;
				if(!EditingMT->IsVirtToolUnique(AxisIndex, NewPosition, NewAttachment, OldTurret, NewTurret))
				{
					AfxMessageBox(IDS_ERROR_POSITION_TURRET_EXISTS);
					return false;
				}
			}
			break;
	}//switch

	// 2 - Сохраняем состояние редактора в историю изменений
	TiXmlDocument *xml_mt = new TiXmlDocument();
	EditingMT->WriteXMLObject(xml_mt);

	MTChange *change_mt = new MTChange();
	change_mt->Xml = xml_mt;
	change_mt->BeforeSelNode = EditingMT->GetCurConfig()[AxisIndex]->GetName();
	if(Change->NameChanged)
	{
		change_mt->AfterSelNode = Change->Name;
		change_mt->ChangeType = 1;
	}//if
	else
	{
		change_mt->AfterSelNode = EditingMT->GetCurConfig()[AxisIndex]->GetName();
		change_mt->ChangeType = 0;
	}//else

	if(Change->TurretChanged || Change->PositionChanged || Change->AttachmentChanged)
		change_mt->ChangeType = 1;

	//MakeChange(change_mt);

	// 3 - Производим изменения в станке
	CString OldName = EditingMT->GetCurConfig()[AxisIndex]->GetName();
	
	switch (EditingMT->GetCurConfig()[AxisIndex]->GetType())
	{
	case MTAxis::AT_FOR://FORWARD
		if(Change->NameChanged)
			EditingMT->RenameAxis(AxisIndex, Change->Name);
		if(Change->ExprChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetExpr(Change->Expr);
		if(Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if(Change->LimitMinChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMin(Change->LimitMin);
		if(Change->LimitMaxChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMax(Change->LimitMax);
		if(Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if(Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if(Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		//{
		//	auto Tst = EditingMT->GetCurConfig()[AxisIndex];
		//	auto Tst1 = static_cast<MTRelocateAxis*>(Tst);
		//	auto& Tst2 = Tst1->SpecialPts;
		//	Tst2 = Change->SpecialPts;
		//}
		static_cast<MTRelocateAxis *>(EditingMT->GetCurConfig()[AxisIndex])->SpecialPts = Change->SpecialPts;
		break;
	case MTAxis::AT_ROT://ROTARY
		if (Change->NameChanged)
			EditingMT->RenameAxis(AxisIndex, Change->Name);
		if (Change->ExprChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetExpr(Change->Expr);
		if (Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if (Change->BasePointChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitBasePoint(Change->BasePoint);
		if (Change->LimitMinChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMin(Change->LimitMin);
		if (Change->LimitMaxChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMax(Change->LimitMax);
		if (Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if (Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if (Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		static_cast<MTRelocateAxis*>(EditingMT->GetCurConfig()[AxisIndex])->SpecialPts = Change->SpecialPts;
		break;
	case MTAxis::AT_SPIN:
		if (Change->NameChanged)
			EditingMT->RenameAxis(AxisIndex, Change->Name);
		if (Change->ExprChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetExpr(Change->Expr);
		if (Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if (Change->BasePointChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitBasePoint(Change->BasePoint);
		if (Change->LimitMinChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMin(Change->LimitMin);
		if (Change->LimitMaxChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMax(Change->LimitMax);
		if (Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if (Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if (Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		static_cast<MTRelocateAxis*>(EditingMT->GetCurConfig()[AxisIndex])->SpecialPts = Change->SpecialPts;
		break;
	case MTAxis::AT_SCHAIN:
		if(Change->NameChanged)
			EditingMT->RenameAxis(AxisIndex, Change->Name);
		if (Change->ExprChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetExpr(Change->Expr);
		if(Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if(Change->BasePointChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitBasePoint(Change->BasePoint);
		if (Change->VectorYChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVectorY(Change->VectorY);
		if(Change->LimitMinChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMin(Change->LimitMin);
		if(Change->LimitMaxChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetMax(Change->LimitMax);
		if(Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if(Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if(Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		if (Change->CycleValChanged)
			static_cast<MTSimpleChainAxis*>(EditingMT->GetCurConfig()[AxisIndex])->SetCycleVal(Change->CycleVal);
		if (Change->CenterDistChanged)
			static_cast<MTSimpleChainAxis*>(EditingMT->GetCurConfig()[AxisIndex])->SetCenterDist(Change->CenterDist);
		static_cast<MTRelocateAxis *>(EditingMT->GetCurConfig()[AxisIndex])->SpecialPts = Change->SpecialPts;
		break;
	case MTAxis::AT_PART:
		{
		int PartIndex = EditingMT->GetPartIndex(AxisIndex);
		MTPart *pPart = EditingMT->GetPart(EditingMT->GetCurConfig()[AxisIndex]->GetName());

		if(Change->NameChanged)
			EditingMT->RenameAxis(AxisIndex, Change->Name);
		if(Change->FileNameChanged)
		{
			if(pPart != nullptr)
			{
				if (Change->FileName.IsEmpty())
				{
					EditingMT->SelectAxis(CString());// clear selection
					EditingMT->RemovePart(PartIndex);
				}
				else
				{
					CString PartName = Change->FileName;
					CString PartPath = MachinePath + EditingMT->GetFileName() + "\\" + PartName;
					pPart->LoadFromStl(PartPath);
				}
			}//if
			else if (!Change->FileName.IsEmpty())
			{
				MTPart *pPart = new MTPart((MTPartAxis *)EditingMT->GetCurConfig()[AxisIndex]);
				CString PartName = Change->FileName;
				CString PartPath = MachinePath + EditingMT->GetFileName() + "\\" + PartName;
				pPart->LoadFromStl(PartPath);
				pPart->SetColor(EditingMT->GetCurConfig()[AxisIndex]->GetColor());
				pPart->SetEdgesAttrs(EditingMT->GetEdgesAttrs());
				EditingMT->AddPart(pPart);
			}//else
		}//if
		if(Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if(Change->BasePointChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitBasePoint(Change->BasePoint);	
		if(Change->VectorYChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVectorY(Change->VectorY);
		if(Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if(Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		if (Change->IsCSChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetIsCS(Change->IsCS);
		if (Change->MarginChanged)
		{
			if (pPart != nullptr)
				pPart->SetMargin(Change->Margin);
		}
		if(Change->MiscColorChanged)
		{
			if(pPart != nullptr)
				pPart->SetColor(Change->MiscColor);
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->MiscColor);
		}//if
		break;
		}
	case MTAxis::AT_TOOL: //tool
		//сначала выполняем операции с turret, att
		if(Change->TurretChanged)
			EditingMT->ChangeToolTurret(AxisIndex, Change->Turret);  
		if(Change->AttachmentChanged)
		{
			EditingMT->ChangeToolAttachment(AxisIndex, Change->Attachment);
		}//if
		change_mt->AfterSelNode = EditingMT->GetCurConfig()[AxisIndex]->GetName();

		if(Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if(Change->BasePointChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitBasePoint(Change->BasePoint);
		if(Change->VectorYChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVectorY(Change->VectorY);
		if(Change->LocalDVChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLocalDisplVec(Change->LocalDV);
		if(Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if(Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if(Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		if (Change->IsCSChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetIsCS(Change->IsCS);
		break;
	case MTAxis::AT_VTOOL:  //virtual tool
		if(Change->TurretChanged)
			EditingMT->ChangeVirtToolTurret( AxisIndex, Change->Turret); 
		if(Change->AttachmentChanged)
			EditingMT->ChangeVirtToolAttachment(AxisIndex, Change->Attachment);
		if(Change->PositionChanged)
			EditingMT->ChangeVirtToolPosition(AxisIndex, Change->Position);
		change_mt->AfterSelNode = EditingMT->GetCurConfig()[AxisIndex]->GetName();

		if(Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if(Change->BasePointChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitBasePoint(Change->BasePoint);
		if(Change->VectorYChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVectorY(Change->VectorY);
		if(Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if(Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if(Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
#ifdef _DEBUG
		//EditingMT->DebugPrint(CString(_T("virtool changes completed")));
#endif
		break;
	case MTAxis::AT_STOCK:
		if(Change->NameChanged)
			EditingMT->RenameAxis(AxisIndex, Change->Name);
		if(Change->VectorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVector(Change->Vector);
		if(Change->VectorYChanged)
			EditingMT->GetCurConfig()[AxisIndex]->InitVectorY(Change->VectorY);
		if(Change->ColorChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetColor(Change->Color);
		if(Change->VisibilityChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetVisible(Change->Visibility);
		if(Change->LengthChanged)
			EditingMT->GetCurConfig()[AxisIndex]->SetLength(Change->Length);
		if(Change->CompAxisXChanged)
		{
			int AxIndex = EditingMT->GetCurConfig().GetAxisIndex(_T("stock_X"));
			EditingMT->GetCurConfig()[AxIndex]->InitVector(Change->CompAxisX);
		}
		if(Change->CompAxisYChanged)
		{
			int AxIndex = EditingMT->GetCurConfig().GetAxisIndex(_T("stock_Y"));
			EditingMT->GetCurConfig()[AxIndex]->InitVector(Change->CompAxisY);
		}
		if(Change->CompAxisZChanged)
		{
			int AxIndex = EditingMT->GetCurConfig().GetAxisIndex(_T("stock_Z"));
			EditingMT->GetCurConfig()[AxIndex]->InitVector(Change->CompAxisZ);
		}
		if(Change->CompensationChanged)
		{
			//Компенсацию обрабатываем в последнюю очередь, т.к. после изменений может потеряться ссылка Axis
			if(EditingMT->GetCurConfig().IsCompStock(AxisIndex) )
			{
				BOOL comp;
				comp = Change->Compensation;
				if(comp)
				{
					//считываем новые вектора компенсации
					EditingMT->GetCurConfig().SetCompXVect(Change->CompAxisX, AxisIndex);
					EditingMT->GetCurConfig().SetCompYVect(Change->CompAxisY, AxisIndex);
					EditingMT->GetCurConfig().SetCompZVect(Change->CompAxisZ, AxisIndex);
				}//if
				else
				{
					EditingMT->GetCurConfig().DeleteComp(AxisIndex);
				}//else
			}//if
			else
			{
				BOOL comp;
				comp = Change->Compensation;
				if(comp)
				{
					EditingMT->GetCurConfig().CreateComp(AxisIndex);
				}//if
			}//else
		}
		break;
	}//switch

	//Может измениться имя, после этого необходимо пресоздать хэшмап
	EditingMT->Regularize();

	MakeChange(change_mt);
	

	// 4 - Производим изменения в GUI
	if(Change->NameChanged)  //Переименовываем узел в дереве MTTreeCtrl
	{
		TreePanePtr->RenameTreeNode(OldName, Change->Name);
	}//if
	if(Change->TurretChanged || Change->AttachmentChanged || Change->PositionChanged)
	{
		TreePanePtr->ShowMachineTool(EditingMT,AxisIndex);
//		TreePanePtr->SelectAndShowPropNode(EditingMT->GetCurConfig()[AxisIndex]->GetName());
	}//if
	else
	{
		//Обновляем панель свойств
		int SelNode = EditingMT->GetCurConfig().GetAxisIndex(change_mt->AfterSelNode);
		EditPanePtr->ShowAxisProp(SelNode);
	}//else

	// 5 - Обновляем параметры редактора
	CreateMachState();
	ApplyChanges();

	return true;
}//bool MTEdit::ChangeAxisProp(SPropChange *Change)

bool MTEdit::ChangeColProp(SPropChange *Change, int ColIndex)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора

	// 1 - Проверяем возможность внесения изменений

	SCollision* Col = EditingMT->GetCollision(ColIndex);
	if(Col == NULL)
		return FALSE;
	if(EditingMT->GetColIndex(Col->Axis1, Change->EnemyAxisName) != ColIndex && EditingMT->GetColIndex(Col->Axis1, Change->EnemyAxisName) != -1)
	{
		CString sError;
		sError.LoadString(IDS_ERROR_COLLISION_EXISTS);
		AfxMessageBox(sError);
		return FALSE;
	}

	CString NewName, OldName, ColString;
	ColString.LoadString(IDS_COLLISION_STRING);
	OldName = ColString + Col->Axis2;
	NewName = ColString + Change->EnemyAxisName;

	// 2 - сохраняем состояние редактора в историю изменений
	TiXmlDocument *xml_mt = new TiXmlDocument();
	EditingMT->WriteXMLObject(xml_mt);

	MTChange *change_mt = new MTChange();
	change_mt->Xml = xml_mt;
	change_mt->BeforeSelNode = OldName;
	if(Change->EnemyAxisNameChanged)
	{
		change_mt->AfterSelNode = NewName;
		change_mt->ChangeType = 1;
	}//if
	else
	{
		change_mt->AfterSelNode = OldName;
		change_mt->ChangeType = 0;
	}//else

	MakeChange(change_mt);


	// 3 -  Изменения в станке
	if(Change->EnemyAxisNameChanged)
	{
		Col->Axis2 = Change->EnemyAxisName;
	}//if
	if(Change->EnemyAxisChildrenChanged)
	{
		Col->Axis2Ch = Change->EnemyAxisChildren;
	}//if
	if(Change->MyChildrenChanged)
	{
		Col->Axis1Ch = Change->MyChildren;
	}//if

	EditingMT->GetCurConfig().ReGenEnemiesPairs(EditingMT->GetCollisions());

	// 4 - изменения в GUI

	if(Change->EnemyAxisNameChanged)
	{
		TreePanePtr->RenameTreeNode(OldName, NewName);
	}//if

	// 5 - Обновляем параметры редактора
	EditPanePtr->ShowColProp(ColIndex);
	TreePanePtr->SelectNode(NewName);

	return true;
}//bool MTEdit::ChangeColProp(SPropChange *Change, int ColIndex)

bool MTEdit::ChangeMoveBranch(const CString& AxisName, const CString& DestName)
{
	/// У инструментов специальные правила перемещения
	int index = EditingMT->GetCurConfig().GetAxisIndex(AxisName);
	if( EditingMT->GetCurConfig()[index]->GetType() == MTAxis::AT_TOOL)
		return ChangeMoveTool(AxisName, DestName);
	if( EditingMT->GetCurConfig()[index]->GetType() == MTAxis::AT_VTOOL)
		return ChangeMoveVirtTool(AxisName, DestName);
	
	// 1 - Проверяем возможность внесения изменений
	// 2 - Сохраняем состояние редактора в историю изменений
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	change_mt->Xml = xml_mt;
	change_mt->ChangeType =1;
	change_mt->BeforeSelNode = AxisName;
	change_mt->AfterSelNode = AxisName;
	MakeChange(change_mt);
	// 3 - Производим изменения в станке
	EditingMT->GetCurConfig().MoveBranch(AxisName, DestName);
	// 4 - Производим изменения в GUI
	TreePanePtr->MoveBranchCtrl(AxisName, DestName);
	TreePanePtr->SelectAndShowPropNode(AxisName);
	// 5 - Обновляем параметры редактора

	return true;
}//bool MTEdit::ChangeMoveBranch(CString AxisName, CString DestName)

bool MTEdit::ChangeMoveTool(const CString& AxisName, const CString& DestName)
{
	// 2 - сохраняем состояние в историю
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	change_mt->Xml = xml_mt;
	change_mt->ChangeType =1;
	change_mt->BeforeSelNode = AxisName;

	// 3 - изменения в станке
	CString NewName = AxisName;
	int index = EditingMT->GetCurConfig().GetAxisIndex(AxisName);
	int destTur = EditingMT->GetToolTurOnLevel(DestName);
	//Проверяем, не перемещаем ли мы инструмент в другую турету
	if(destTur == -1)
	{
		//Проверяем, сколько инструментов было на текущей турете
		if( EditingMT->GetTurretToolAmount( EditingMT->GetCurConfig()[index]->GetTurret()) == 1)
		{
			EditingMT->GetCurConfig().MoveBranch(AxisName, DestName);
		}//if
		else
		{
			EditingMT->GetCurConfig()[index]->SetTurret( EditingMT->GetNextTurret());
			NewName = EditingMT->GetCurConfig()[index]->GetName();
			EditingMT->RenameInCollisions(AxisName, NewName);
			EditingMT->Regularize();
			EditingMT->GetCurConfig().MoveBranch( NewName, DestName);
		}//else
	}//if
	else
	{
		//Проверяем, есть ли в новой турете инструмент с таким же attachment
		if( EditingMT->ExistsToolTurAtt(destTur, EditingMT->GetCurConfig()[index]->GetAttachment()))
		{
			int att = EditingMT->GetNextAttachment(destTur);
			EditingMT->GetCurConfig()[index]->SetAttachment(att);
		}//if
		EditingMT->GetCurConfig()[index]->SetTurret(destTur);
		NewName = EditingMT->GetCurConfig()[index]->GetName();
		EditingMT->RenameInCollisions(AxisName, NewName);
		EditingMT->Regularize();
		EditingMT->GetCurConfig().MoveBranch(NewName, DestName);
	}//else

	change_mt->AfterSelNode = NewName;
	MakeChange(change_mt);
	// 4 - изменения в GUI
	TreePanePtr->RenameTreeNode(AxisName, NewName);
	TreePanePtr->MoveBranchCtrl(NewName, DestName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}//bool MTEdit::ChangeMoveTool(CString AxisName, CString DestName)
bool MTEdit::ChangeMoveVirtTool(const CString& AxisName, const CString& DestName)
{
	CString NewName = AxisName;
	int index = EditingMT->GetCurConfig().GetAxisIndex(AxisName);
	// 2 - сохраняем состояние в историю
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	change_mt->Xml = xml_mt;
	change_mt->ChangeType =1;
	change_mt->BeforeSelNode = AxisName;

	// 3 - изменения в станке
	int destTur = EditingMT->GetVirtToolTurOnLevel(DestName);
	//Проверяем, не перемещаем ли мы инструмент в другую турету
	if(destTur == -1)
	{
		//Проверяем, сколько инструментов было на текущей турете
		if( EditingMT->GetTurretVirtToolAmount( EditingMT->GetCurConfig()[index]->GetTurret()) == 1)
		{
			EditingMT->GetCurConfig().MoveBranch(AxisName, DestName);
		}//if
		else
		{
			EditingMT->GetCurConfig()[index]->SetTurret( EditingMT->GetNextTurretVirtTool());
			NewName = EditingMT->GetCurConfig()[index]->GetName();
			EditingMT->RenameInCollisions(AxisName, NewName);
			EditingMT->Regularize();
			EditingMT->GetCurConfig().MoveBranch( NewName, DestName);
		}//else
	}//if
	else
	{
		//Проверяем, есть ли в новой турете инструмент с таким же attachment
		if( EditingMT->ExistsVirtToolPosTur(EditingMT->GetCurConfig()[index]->GetPosition(),destTur ))
		{
			int pos = EditingMT->GetNextPositionVirtTool(destTur);
			EditingMT->GetCurConfig()[index]->SetTurret(destTur);
			EditingMT->GetCurConfig()[index]->SetPosition(pos);
			NewName = EditingMT->GetCurConfig()[index]->GetName();
			EditingMT->RenameInCollisions(AxisName, NewName);
			EditingMT->Regularize();
			EditingMT->GetCurConfig().MoveBranch( NewName, DestName);
		}//if
		else
		{
			EditingMT->GetCurConfig()[index]->SetTurret(destTur);
			NewName = EditingMT->GetCurConfig()[index]->GetName();
			EditingMT->RenameInCollisions(AxisName, NewName);
			EditingMT->Regularize();
			EditingMT->GetCurConfig().MoveBranch( NewName, DestName);
		}//else
	}//else

	change_mt->AfterSelNode = NewName;
	MakeChange(change_mt);
	// 4 - изменения в GUI
	TreePanePtr->RenameTreeNode(AxisName, NewName);
	TreePanePtr->MoveBranchCtrl(NewName, DestName);
	TreePanePtr->SelectAndShowPropNode(NewName);
	return true;
}//bool MTEdit::ChangeMoveVirtTool(CString AxisName, CString DestName)
	
bool MTEdit::ChangeMoveCollision(CString Axis1, CString Axis2, CString NewAxis1)
{
	// 1 - Проверяем возможность внесения изменений
	//    проверка была произведена на этапе перетаскивания в элементе управления MTEditTreeCtrl
	// 2 - Сохраняем состояние редактора в историю изменений
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType =1;

	CString ColString; ColString.LoadString(IDS_COLLISION_STRING);
	CString ColNode = ColString + Axis2;
	
	change_mt->BeforeSelNode = ColNode;
	change_mt->AfterSelNode = ColNode;
	MakeChange(change_mt);
	// 3 - Производим изменения в станке
	EditingMT->MoveCollision(Axis1, Axis2, NewAxis1);
	// 4 - Производим изменения в GUI
	TreePanePtr->MoveCollisionCtrl(Axis1, Axis2, NewAxis1);
	TreePanePtr->SelectAndShowPropNode(ColNode);
	// 5 - Обновляем параметры редактора
	return true;
}//bool MTEdit::ChangeMoveCollision(CString Axis1, CString Axis2, CString NewAxis1)

bool MTEdit::ChangeCreateTrans(const CString& ParentName, MTForwardAxis *Axis )
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	MTForwardAxis *FAxis;

	CString NewName;
	if(Axis == NULL)
	{
		CString AxisString;
		AxisString.LoadString(IDS_EDITOR_TREE_AXIS);
		NewName = EditingMT->GenNextAxisName(AxisString);
		FAxis = new MTForwardAxis();
		FAxis->SetName(NewName);
	}//if
	else
	{
		NewName = EditingMT->GenNextAxisName(Axis->GetName());
		FAxis = Axis;
		FAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(FAxis, ParentName);
	
	// 4
	TreePanePtr->CreateTransCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}//bool MTEdit::ChangeCreateTrans(CString ParentName, MTForwardAxis *Axis = NULL)

bool MTEdit::ChangeCreateRot(const CString& ParentName, MTRotaryAxis* Axis)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	MTRotaryAxis* RAxis;

	CString NewName;
	if (Axis == NULL)
	{
		CString AxisString;
		AxisString.LoadString(IDS_EDITOR_TREE_AXIS);
		NewName = EditingMT->GenNextAxisName(AxisString);
		RAxis = new MTRotaryAxis();
		RAxis->SetName(NewName);
	}//if
	else
	{
		NewName = EditingMT->GenNextAxisName(Axis->GetName());
		RAxis = Axis;
		RAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange* change_mt = new MTChange;
	TiXmlDocument* xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(RAxis, ParentName);

	// 4
	TreePanePtr->CreateRotCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;

}

bool MTEdit::ChangeCreateSpin(const CString& ParentName, MTSpinAxis* Axis)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	MTSpinAxis* RAxis;

	CString NewName;
	if (Axis == NULL)
	{
		CString AxisString;
		AxisString.LoadString(IDS_EDITOR_TREE_AXIS);
		NewName = EditingMT->GenNextAxisName(AxisString);
		RAxis = new MTSpinAxis();
		RAxis->SetName(NewName);
	}//if
	else
	{
		NewName = EditingMT->GenNextAxisName(Axis->GetName());
		RAxis = Axis;
		RAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange* change_mt = new MTChange;
	TiXmlDocument* xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(RAxis, ParentName);

	// 4
	TreePanePtr->CreateSpinCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;

}

bool MTEdit::ChangeCreateSpin(const CString& ParentName)
{
	MTSpinAxis* pC = new MTSpinAxis();

	pC->SetName(CString(_T("spin")));
	pC->InitBasePoint(0, 0, 0);
	pC->InitVector(BPoint(0., 0., 1., 0.));

	return ChangeCreateSpin(ParentName, pC);
}

bool MTEdit::ChangeCreateSChain(const CString& ParentName, MTSimpleChainAxis* Axis)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	MTSimpleChainAxis* SAxis;

	CString NewName;
	if (Axis == nullptr)
	{
		CString AxisString;
		AxisString.LoadString(IDS_EDITOR_TREE_AXIS);//TOCHANGE
		NewName = EditingMT->GenNextAxisName(AxisString);
		SAxis = new MTSimpleChainAxis();
		SAxis->SetName(NewName);
	}//if
	else
	{
		NewName = EditingMT->GenNextAxisName(Axis->GetName());
		SAxis = Axis;
		SAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange* change_mt = new MTChange;
	TiXmlDocument* xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(SAxis, ParentName);

	// 4
	TreePanePtr->CreateAxisCtrl(*SAxis, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}
//bool MTEdit::ChangeCreateRot(CString ParentName, MTRotaryAxis *Axis )

bool MTEdit::ChangeCreateArray(const CString& AxisName, int Count, double Step, bool DontRotate)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	// 1
	MTAxis* pAxis = EditingMT->GetCurConfig().GetAxis(AxisName);
	if (pAxis == nullptr)
		return false;
	const MTRelocateAxis* pRelAxis = GetNearestRelAxis(AxisName);
	if (pRelAxis == nullptr)
		return false;
	// 2
	MTChange* change_mt = new MTChange;
	TiXmlDocument* xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = AxisName;
	change_mt->AfterSelNode = AxisName;

	MakeChange(change_mt);

	// 3
	vector<MTAxis*> DescAxes;
	EditingMT->GetCurConfig().FindAllDescendant(pAxis, DescAxes);
	DescAxes.push_back(pAxis);
	vector<int> Parents;
	Parents.resize(DescAxes.size());
	for (int k = 0; k < DescAxes.size(); ++k)
	{
		const CString& PName = DescAxes[k]->GetParent();
		Parents[k] = -1;
		for (int l = 0; l < DescAxes.size(); ++l)
		{
			if (l == k)
				continue;
			if (PName == DescAxes[l]->GetName())
			{
				Parents[k] = l;
				break;
			}
		}
	}

	CString ParentName = pAxis->GetParent();
	MTAxis* pBufAx = pRelAxis->Clone();
	for (int i = 1; i < Count; ++i)
	{
		// Calculate i'th matrix (Mi)
		pBufAx->SetMovement(0);
		double Param = 0.;
		switch (pRelAxis->GetType())
		{
		case MTAxis::AT_FOR:
			Param = i * Step;
			break;
		case MTAxis::AT_ROT:
			Param = 360. * i / Count;
			break;
		case MTAxis::AT_SCHAIN:
		{
			MTSimpleChainAxis* pSimpAx = dynamic_cast<MTSimpleChainAxis*>(pBufAx);
			if (pSimpAx != nullptr)
				Param = pSimpAx->GetCycleVal() * i / Count;
		}
		break;
		}
		CooArray Coords;
		Coords.Add(MTCoordinate(pRelAxis->GetName(), Param));
		EditingMT->GetCurConfig().MoveToCurPos(Coords, true);
		vector<MTAxis*> NewDescAxes;
		// for each axis descendant to pAxis (including itself)
		for (int k = 0; k < DescAxes.size(); ++k)
		{
			const MTAxis* pCurAxis = DescAxes[k];
			CString Suffix = pCurAxis->GetName();
			if (Suffix.Left(4) == _T("0001"))
				Suffix = Suffix.Right(AxisName.GetLength() - 4);
			// Create new axis (pNewAx)
			CString NewName;
			NewName.Format(_T("%04u"), i + 1);
			NewName += Suffix;
			NewName = EditingMT->GenNextAxisName(NewName);
			const MTPartAxis* pPartAx = dynamic_cast<const MTPartAxis*>(pCurAxis);
			MTPart* pPart = nullptr;
			if (pPartAx != nullptr)
				pPart = EditingMT->GetPart(pCurAxis->GetName());
			if (pPart != nullptr)
			{
				MTPart* pNewPart = new MTPart();
				pNewPart->SetColor(pPart->GetColor());
				pNewPart->SetMargin(pPart->GetMargin());
				pNewPart->LoadFromStl(pPart->GetFile());
				pNewPart->SetAxisName(NewName);
				pNewPart->SetEdgesAttrs(EditingMT->GetEdgesAttrs());
				EditingMT->GetParts().AddPart(pNewPart);
			}
			MTAxis* pNewAx = pCurAxis->Clone();
			pNewAx->SetName(NewName);
//			pNewAx->GoTo(Mi);
			pNewAx->InitBasePoint(pCurAxis->GetBasePoint());
			if (!DontRotate)
			{
				pNewAx->InitVector(pCurAxis->GetVector());
				pNewAx->InitVectorY(pCurAxis->GetVectorY());
			}
			NewDescAxes.push_back(pNewAx);
		}
		for (int k = 0; k < DescAxes.size(); ++k)
		{
			CString LocPName;
			if (Parents[k] == -1)
				LocPName = ParentName;
			else
				LocPName = NewDescAxes[Parents[k]]->GetName();

			// Add new axis (pNewAx) to machine tool
			NewDescAxes[k]->SetParent(LocPName);
			EditingMT->GetCurConfig().AddAxis(NewDescAxes[k]);
		}
		EditingMT->GetCurConfig().Regularize();
		bool Stop = false;
		bool* Used = new bool[DescAxes.size()];
		for (int k = 0; k < DescAxes.size(); ++k)
			Used[k] = false;
		while (!Stop)
		{
			Stop = true;
			for (int k = 0; k < DescAxes.size(); ++k)
			{
				if (!Used[k] && TreePanePtr->GetmtTreeTree().GetItemByName(nullptr, NewDescAxes[k]->GetParent()) != NULL)
				{
					Stop = false;
					Used[k] = true;
					TreePanePtr->CreateAxisCtrl(*NewDescAxes[k], NewDescAxes[k]->GetParent());
				}
			}
			
		}
	}
	delete pBufAx;
	

	// 4
	TreePanePtr->SelectAndShowPropNode(pAxis->GetParent());

	return true;

}

bool MTEdit::ChangeLoadBranch(const CString& FileName)
{
	// 1 - Проверяем возможность внесения изменений
	// 2 - Сохраняем состояние редактора в историю изменений
	MTChange* change_mt = new MTChange;
	TiXmlDocument* xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = _T("");
	change_mt->AfterSelNode = _T("");
	MakeChange(change_mt);
	// 3 - Производим изменения в станке
	EditingMT->LoadXMLBranch(FileName);
	// 4 - Производим изменения в GUI
	TreePanePtr->ShowMachineTool(EditingMT);
	TreePanePtr->SelectAndShowPropNode(_T(""));
	// 5 - Обновляем параметры редактора

	return true;
}

const MTRelocateAxis* MTEdit::GetNearestRelAxis(const CString& AxisName) const
{
	const MTAxis* pAxis = EditingMT->GetCurConfig().GetAxis(AxisName);
	if (pAxis == nullptr)
		return nullptr;
	const MTRelocateAxis* pRelAxis = dynamic_cast<const MTRelocateAxis*>(EditingMT->GetNearestMoveRotAxis(pAxis));
	return pRelAxis;
}
//bool MTEdit::ChangeCreateRot(CString ParentName, MTRotaryAxis *Axis )

bool MTEdit::ChangeCreatePart(const CString& ParentName, MTPartAxis *Axis)
{
	/// 1 - проверяем возможность внесения изменений
	/// 2 - сохраняем состояние редактора в историю изменений
	/// 3 - производим изменения в станке
	/// 4 - производим изменения в gui
	/// 5 - обновляем параметры редактора
	MTPartAxis *PAxis;

	CString NewName;
	if(Axis == NULL)
	{
		CString AxisString;
		AxisString.LoadString(IDS_EDITOR_TREE_AXIS);
		NewName = EditingMT->GenNextAxisName(AxisString);
		PAxis = new MTPartAxis();
		PAxis->SetName(NewName);
	}//if
	else
	{
		NewName = EditingMT->GenNextAxisName(Axis->GetName());
		PAxis = Axis;
		PAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(PAxis, ParentName);
	
	// 4
	TreePanePtr->CreatePartCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}//bool MTEdit::ChangeCreatePart(CString ParentName, MTPartAxis *Axis )

bool MTEdit::ChangeCreateParts(const CString& ParentName, vector<CString> &list)
{
	/// 1 - проверяем возможность внесения изменений
	/// 2 - сохраняем состояние редактора в историю изменений
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = ParentName;

	MakeChange(change_mt);
	/// 3 - производим изменения в станке
	CString str;
	str.LoadString(IDS_NO_STL_FILE);
	for(unsigned int i = 0; i < list.size(); i++)
	{
		if (list.at(i).IsEmpty())
			continue;
		/// 0 - проверяем уникальность имени оси
		CString AxisName = list[i].Mid(0, list[i].GetLength()-4);
		bool IsEmpty = (list[i] == str);
		if (IsEmpty)
			AxisName = list[i];
		AxisName = EditingMT->GenNextAxisName(AxisName);
		/// a - создаём и добавляем в станок объект MTPartAxis с именем stl
		MTPartAxis *pPartAxis = new MTPartAxis();
		pPartAxis->SetName(AxisName);
		pPartAxis->SetParent(ParentName);
		
		/// Загружаем цвет из палитры
		COLORREF col = palette.GetRecurColor(0); 
		pPartAxis->SetColor(col);

		NColor ncol;
		ncol.SetRGB(col);

		EditingMT->GetCurConfig().AddAxis(pPartAxis);
		
		/// b - создаём и добавляем в станок объект MTPart, содержащий stl
		if (!IsEmpty)
		{
			MTPart* pPart = new MTPart();
			pPart->SetColor(ncol);
			pPart->LoadFromStl(MachinePath + EditingMT->GetFileName() + "\\" + list[i]);
			pPart->SetAxisName(AxisName);
			pPart->SetEdgesAttrs(EditingMT->GetEdgesAttrs());
			EditingMT->AddPart(pPart);
		}
		EditingMT->Regularize();
	}//for
	/// 4 - производим изменения в gui
	TreePanePtr->ShowMachineTool(EditingMT);
	TreePanePtr->SelectAndShowPropNode(ParentName);
	/// 5 - обновляем параметры редактора
	return true;

}//bool MTEdit::ChangeCreateParts(CString ParentName, vector<CString> &list)

bool MTEdit::ChangeCreateTool(const CString& ParentName, MTToolAxis *Axis)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	MTToolAxis *TAxis;

	//необходимо обеспечить уникальность имён и изоляцию турретов
	CString NewName;
	int turret = EditingMT->GetToolTurOnLevel(ParentName);	
	if(turret != -1)
	{
		int attachment = EditingMT->GetNextAttachment(turret);
		NewName = MTConfig::GetToolAxisName(turret, attachment);
	}//if
	else
	{
		turret = EditingMT->GetNextTurret();
		int attachment = EditingMT->GetNextAttachment(turret);
		NewName = MTConfig::GetToolAxisName(turret, attachment);
	}//else
	if(Axis == NULL)
	{
		TAxis = new MTToolAxis();
		TAxis->SetName(NewName);
	}//if
	else
	{
		TAxis = Axis;
		TAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(TAxis, ParentName);
	
	// 4
	TreePanePtr->CreateToolCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}//bool MTEdit::ChangeCreateTool(CString ParentName, MTToolAxis *Axis )

bool MTEdit::ChangeCreateToolVirt(const CString& ParentName, MTVirtToolAxis *Axis )
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	MTVirtToolAxis *VTAxis;

	CString NewName;
	int turret = EditingMT->GetVirtToolTurOnLevel(ParentName);	
	if(turret != -1)
	{
		int position = EditingMT->GetNextPositionVirtTool(turret);
		NewName = MTConfig::GetToolPlaceAxisName(position, turret, 0);
	}//if
	else
	{
		turret = EditingMT->GetNextTurretVirtTool();
		int position = EditingMT->GetNextPositionVirtTool(turret);
		NewName = MTConfig::GetToolPlaceAxisName(position, turret, 0);
	}//else

	if(Axis == NULL)
	{
		VTAxis = new MTVirtToolAxis();
		VTAxis->SetName(NewName);
	}//if
	else
	{
		VTAxis = Axis;
		VTAxis->SetName(NewName);
	}//
	// 1

	// 2
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(VTAxis, ParentName);
	
	// 4
	TreePanePtr->CreateToolVirtCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}//bool MTEdit::ChangeCreateVirtTool(CString ParentName, MTToolAxis *Axis )

bool MTEdit::ChangeCreateStock(const CString& ParentName, MTStockAxis *Axis )
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора

	// 1

	MTStockAxis *SAxis;

	CString NewName;
	for (int i = 0; ; ++i)
	{
		NewName = MTMachine::GetStockAxisName(i);
		if (EditingMT->GetCurConfig().GetAxisIndex(NewName) < 0)
			break;
	}
	if(Axis == NULL)
	{
		SAxis = new MTStockAxis();
		SAxis->SetName(NewName);
	}//if
	else
	{
		SAxis = Axis;
		SAxis->SetName(NewName);
	}//

	// 2
	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = NewName;

	MakeChange(change_mt);

	// 3
	EditingMT->GetCurConfig().CreateChildAxis(SAxis, ParentName);
	
	// 4
	TreePanePtr->CreateStockCtrl(NewName, ParentName);
	TreePanePtr->SelectAndShowPropNode(NewName);

	return true;
}//bool MTEdit::ChangeCreateStock(CString ParentName, MTToolAxis *Axis )
bool MTEdit::ChangeCreateCol(const CString& ParentName, const CString& EnemyName, SCollision *Col )
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора
	SCollision Collision;

	CString NewName;
	if(Col == NULL)
	{
		Collision.Axis1 = ParentName;
		Collision.Axis2 = EnemyName;
	}//if
	else
	{
		Collision = *Col;
	}//
	// 1
	if(EditingMT->GetColIndex(ParentName, EnemyName) != -1 || EditingMT->GetColIndex(EnemyName, ParentName) != -1)
	{
		CString sError;
		sError.LoadString(IDS_ERROR_COLLISION_EXISTS);
		AfxMessageBox(sError);
		return FALSE;
	}//if

	// 2
	CString ColString;ColString.LoadString(IDS_COLLISION_STRING);
	CString ColNodeString = ColString + Collision.Axis2;

	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ParentName;
	change_mt->AfterSelNode = ColNodeString;

	MakeChange(change_mt);

	// 3
	EditingMT->AddColToMT(Collision);	
	
	// 4
	TreePanePtr->CreateColCtrl(ColNodeString, ParentName);
	TreePanePtr->SelectAndShowPropNode(ColNodeString);

	return true;
}//bool MTEdit::ChangeCreateStock(CString ParentName, MTToolAxis *Axis )

bool MTEdit::ChangeCreateX(const CString& ParentName)
{
	MTForwardAxis *pX = new MTForwardAxis();

	pX->SetName(CString(_T("X")));
	pX->InitBasePoint(0, 0, 0);
	pX->InitVector(BPoint(1., 0., 0., 0.));

	return ChangeCreateTrans(ParentName, pX);
}//bool ChangeCreateX(CString ParentName);

bool MTEdit::ChangeCreateY(const CString& ParentName)
{
	MTForwardAxis *pY = new MTForwardAxis();

	pY->SetName(CString(_T("Y")));
	pY->InitBasePoint(0, 0, 0);
	pY->InitVector(BPoint(0., 1., 0., 0.));

	return ChangeCreateTrans(ParentName, pY);
}//bool ChangeCreateY(CString ParentName);

bool MTEdit::ChangeCreateZ(const CString& ParentName)
{
	MTForwardAxis *pZ = new MTForwardAxis();

	pZ->SetName(CString(_T("Z")));
	pZ->InitBasePoint(0, 0, 0);
	pZ->InitVector(BPoint(0., 0., 1., 0.));

	return ChangeCreateTrans(ParentName, pZ);
}//bool ChangeCreateZ(CString ParentName);

bool MTEdit::ChangeCreateA(const CString& ParentName)
{
	MTRotaryAxis *pA = new MTRotaryAxis();

	pA->SetName(CString(_T("A")));
	pA->InitBasePoint(0, 0, 0);
	pA->InitVector(BPoint(1., 0., 0., 0.));

	return ChangeCreateRot(ParentName, pA);
}//bool ChangeCreateA(CString ParentName);

bool MTEdit::ChangeCreateB(const CString& ParentName)
{
	MTRotaryAxis *pB = new MTRotaryAxis();

	pB->SetName(CString(_T("B")));
	pB->InitBasePoint(0, 0, 0);
	pB->InitVector(BPoint(0., 1., 0., 0.));

	return ChangeCreateRot(ParentName, pB);
}//bool ChangeCreateB(CString ParentName);

bool MTEdit::ChangeCreateC(const CString& ParentName)
{
	MTRotaryAxis *pC = new MTRotaryAxis();

	pC->SetName(CString(_T("C")));
	pC->InitBasePoint(0, 0, 0);
	pC->InitVector(BPoint(0., 0., 1., 0.));

	return ChangeCreateRot(ParentName, pC);
}//bool ChangeCreateC(CString ParentName);

bool MTEdit::ChangeDeleteBranch(const CString& AxisName)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора

	int index = EditingMT->GetCurConfig().GetAxisIndex(AxisName);
	if (index == -1)
		return false;
	CString ParentName = EditingMT->GetCurConfig()[index]->GetParent();

	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	if (!EditingMT->RemoveBranch(index))
	{// removing is impossible
		delete xml_mt;// undo not needed
		return false;
	}

	MTChange *change_mt = new MTChange;
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = AxisName;
	change_mt->AfterSelNode = ParentName;

	MakeChange(change_mt);

	TreePanePtr->DeleteBranchCtrl(AxisName);
	TreePanePtr->SelectAndShowPropNode(ParentName);

	ApplyChanges();

	return true;

}//bool MTEdit::ChangeDeleteBranch(CString AxisName)

bool MTEdit::ChangeDeleteCollision(const CString& Axis1, const CString& Axis2)
{
	// 1 - проверяем возможность внесения изменений
	// 2 - сохраняем состояние редактора в историю изменений
	// 3 - производим изменения в станке
	// 4 - производим изменения в gui
	// 5 - обновляем параметры редактора

	// 1
	int index;
	if( (index = EditingMT->GetColIndex(Axis1, Axis2)) == -1)
		return false;
	// 2
	CString ParentName = Axis1; 
	CString ColString; ColString.LoadString(IDS_COLLISION_STRING);
	CString ColName = ColString + Axis2;

	MTChange *change_mt = new MTChange;
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);
	change_mt->Xml = xml_mt;
	change_mt->ChangeType = 1;
	change_mt->BeforeSelNode = ColName;
	change_mt->AfterSelNode = ParentName;

	MakeChange(change_mt);

	// 3
	EditingMT->RemoveCollision(Axis1, Axis2);
	// 4
	TreePanePtr->DeleteBranchCtrl(ColName);
	TreePanePtr->SelectAndShowPropNode(ParentName);

	//5
	ApplyChanges();

	return true;
}//bool MTEdit::ChangeDeleteCollision(CString ColName)

bool MTEdit::CreateMachState(void)
{
	// Create init string
	CString InitStr = EditingMT->GetCurConfig().CrInitStr();

	SCoordsDef *pNewDef = new SCoordsDef(InitStr);
	SMachState *pNewState = new SMachState(pNewDef);
	if(pMachState)
		for(int k = 0; k < pMachState->GetSize() - 1; ++k)
			pNewState->SetCoord(pMachState->GetAName(k), pMachState->GetCoord(k));

	delete pMachState;
	delete pMachCoordsDef;
	pMachState = pNewState;
	pMachCoordsDef = pNewDef;

	return false;
}

bool MTEdit::IsChangingNodeAllowed()
{
	return EditPanePtr->IsChangingNodeAllowed();
}//bool MTEdit::IsChangingNodeAllowed()

void MTEdit::GetMachinePath(CString &path, CString &name)
{
	path = MachinePath;
	name = EditingMT->GetFileName();
}//void MTEdit::GetMachinePath(CString &path, CString &name)

void MTEdit::Save()
{
	bool MachineSaved = EditingMT->SaveXML(MachinePath, EditingMT->GetFileName());
	if(!MachineSaved)
	{
		CString Mes;
		Mes.Format(IDS_MES_MACHFILENOTACCESS, MachinePath + EditingMT->GetFileName() + CString("\\main.xml"));
		AfxMessageBox(Mes);
	}
}//void MTEdit::Save()

bool MTEdit::ChangeSave(bool warning)
{
	if(warning)
	{
		CString sReplace; sReplace.LoadString(IDS_EDITOR_REPLACE);
		if (AfxMessageBox(IDS_EDITOR_REPLACE,MB_OKCANCEL | MB_ICONEXCLAMATION) != IDOK)
			return false;
	}//if

	Save();
	ClearUndoStack();

	TreePanePtr->TBEnableUndo(false);
	TreePanePtr->TBEnableSave(false);
	return true;
}//bool MTEdit::ChangeSave()


bool MTEdit::ChangeSaveAs(const CString &NewDirName)
{
	//Если имя не изменилось, просто сохраняем xml
	if(NewDirName == EditingMT->GetFileName())
		ChangeSave(false);
	else
	{
		// 1 - Создаём каталог, если уже есть - пересоздаём
		int Res = _mkdir(MachinePath + NewDirName);
		if(errno == EEXIST)
		{
			NCMProject::DeleteDirectory(MachinePath + NewDirName);
			Res = _mkdir(MachinePath+NewDirName);
		}//if

		// 2 - копируем из старого каталога все stl файлы
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile(MachinePath + EditingMT->GetFileName() + "\\*.stl", &findFileData);
         
        if(hFind  != INVALID_HANDLE_VALUE && Res == 0) 
		{
			//копирование файла
			CString stl = findFileData.cFileName;
			CopyFile(MachinePath+EditingMT->GetFileName()+ "\\" +stl, MachinePath+NewDirName + "\\" +stl,1);
			while(FindNextFile(hFind, &findFileData))
			{
				stl = findFileData.cFileName;
				CopyFile(MachinePath+EditingMT->GetFileName()+ "\\" +stl, MachinePath+NewDirName + "\\" +stl,1);
			}//while

			FindClose(hFind);
		}//if

		// 3 - сохраняем в новый каталог xml, перезаписываем FileName в станке

		bool MachineSaved = EditingMT->SaveXML(MachinePath, NewDirName);
		if(MachineSaved)
		{
			EditingMT->SetFileName(NewDirName);
			EditingMT->LoadXML(MachinePath, EditingMT->GetFileName());
		}
		else
		{
			CString Mes;
			Mes.Format(IDS_MES_MACHFILENOTACCESS, MachinePath + NewDirName + CString("\\main.xml"));
			AfxMessageBox(Mes);
		}
	}//else
	ClearUndoStack();

	TreePanePtr->TBEnableUndo(false);
	TreePanePtr->TBEnableSave(false);

	return true;
}//bool MTEdit::ChangeSaveAs()

bool MTEdit::Undo()
{
	MTChange *change_mt;
	if(leftStack.empty())
		return false;
	change_mt = leftStack.top();
	if(change_mt == NULL)
		return false;

	// 1 - сохранение текущего состояния редактора в rightStack
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	MTChange *old_change_mt = new MTChange;
	old_change_mt->ChangeType = change_mt->ChangeType;
	old_change_mt->BeforeSelNode = change_mt->AfterSelNode;
	old_change_mt->AfterSelNode = change_mt->BeforeSelNode;
	old_change_mt->Xml = xml_mt;

	rightStack.push(old_change_mt);

	TreePanePtr->TBEnableRedo(true);
	// 2 - считывание состояния из leftStack
	
	leftStack.pop();
	
	if(leftStack.empty())
	{
		TreePanePtr->TBEnableUndo(false);
		TreePanePtr->TBEnableSave(false);
	}//if
	// 3 - откат изменений в станке
	EditingMT->ReadXMLObject(change_mt->Xml, MachinePath);

	// 4 - откат изменений в GUI редактора
	switch(change_mt->ChangeType)
	{
	case 0: //Изменения в свойствах
		TreePanePtr->SelectAndShowPropNode(change_mt->BeforeSelNode);
		break;
	case 1: //Изменения в структуре дерева
		TreePanePtr->ShowMachineTool(EditingMT);
		TreePanePtr->SelectAndShowPropNode(change_mt->BeforeSelNode);
		break;
	}//switch

	ApplyChanges();

	// освобождение памяти из под элемента стека
	change_mt->Xml->Clear();
	delete change_mt->Xml;
	delete change_mt;
	return true;
}//bool MTEdit::Undo()

bool MTEdit::Redo()
{
	MTChange *change_mt;
	change_mt = rightStack.top();
	if(change_mt == NULL)
		return false;

	// 1 - сохранение текущего состояния редактора в leftStack
	TiXmlDocument *xml_mt = new TiXmlDocument;
	EditingMT->WriteXMLObject(xml_mt);

	MTChange *old_change_mt = new MTChange;
	old_change_mt->ChangeType = change_mt->ChangeType;
	old_change_mt->BeforeSelNode = change_mt->AfterSelNode;
	old_change_mt->AfterSelNode = change_mt->BeforeSelNode;
	old_change_mt->Xml = xml_mt;

	leftStack.push(old_change_mt);

	SetMachineChanged(true);
	TreePanePtr->TBEnableUndo(true);
	TreePanePtr->TBEnableSave(true);
	// 2 - считывание состояния из rightStack
	
	rightStack.pop();

	if(rightStack.empty())
		TreePanePtr->TBEnableRedo(false);
	// 3 - откат изменений в станке
	EditingMT->ReadXMLObject(change_mt->Xml, MachinePath);
	// 4 - откат изменений в GUI редактора

	switch(change_mt->ChangeType)
	{
	case 0: //Изменения в свойствах
		TreePanePtr->SelectAndShowPropNode(change_mt->BeforeSelNode);
		break;
	case 1: //Изменения в структуре дерева
		TreePanePtr->ShowMachineTool(EditingMT);
		TreePanePtr->SelectAndShowPropNode(change_mt->BeforeSelNode);
		break;
	}//switch

	ApplyChanges();

	// освобождение памяти из под элемента стека
	change_mt->Xml->Clear();
	delete change_mt->Xml;
	delete change_mt;
	return true;
}//bool MTEdit::Redo()

void MTEdit::ClearHistory()
{
	ClearUndoStack();
	ClearRedoStack();
}//void MTEdit::ClearHistory()

void MTEdit::ClearUndoStack()
{
	MTChange *c;
	//очищаем правый стек
	while(!leftStack.empty())
	{	
		c = leftStack.top();
		c->Xml->Clear();
		delete c->Xml;
		delete c;
		leftStack.pop();	
	}//while
}//void MTEdit::ClearUndoStack()

void MTEdit::ClearRedoStack()
{
	MTChange *c;
	//очищаем правый стек
	while(!rightStack.empty())
	{	
		c = rightStack.top();
		c->Xml->Clear();
		delete c->Xml;
		delete c;
		rightStack.pop();	
	}//while
}
void MTEdit::ResetSTLConverter()
{
	NStlConverterDlg::ResetAccValues();
}
//void MTEdit::ClearRedoStack()

StockFileType MTEdit::Conv2STL(CString& IOFileName, enum DocElements DEl, const CString& InitPath /*= CString()*/)
{
	CString OFileName = IOFileName;
	StockFileType StockType = DetSFT(IOFileName);
	if (StockType != SFT_STL)
	{
		int index = IOFileName.ReverseFind('.');
		if (index >= 0)
			OFileName.Delete(index, IOFileName.GetLength());
		OFileName += _T(".stl");
	}
	if (!InitPath.IsEmpty())
	{
		// change path
		OFileName.Delete(0, OFileName.ReverseFind('\\') + 1);
		OFileName = InitPath + OFileName;
	}
	bool res = false;
	switch (StockType)
	{
	case SFT_STL:
	{
		CString b1 = OFileName;
		CString b2 = IOFileName;
		if (b1.MakeUpper() == b2.MakeUpper())
			res = true;
		else
			res = std::filesystem::copy_file(IOFileName.GetBuffer(), OFileName.GetBuffer(), std::filesystem::copy_options::overwrite_existing);
	}
		break;
	case SFT_STEP:
	case SFT_IGES:
	{
		HMODULE m = LoadLibrary("OCImport.dll");
		if (!m)
		{
			AfxMessageBox(IDS_ERROR_LOADING_OCIMPORT_DLL);
			return SFT_UNKNOWN;
		}
		NStlConverterDlg ConvDlg(DEl);
		ConvDlg.SrcFileName = IOFileName;
		ConvDlg.DestFileName = OFileName;
		ConvDlg.Format = 1;
		if (ConvDlg.DoModal() != IDOK)
			return SFT_UNKNOWN;
		CWaitCursor w;
		res = NOCImport::ConvExt2STL(StockType, IOFileName.GetBuffer(), OFileName.GetBuffer()
			, ConvDlg.Format == 0, ConvDlg.GetAccuracy(), ConvDlg.GetMinAngle() * PI / 180.);
	}
		break;
	default:
		return StockType;
	}

	if (res)
	{
		StockType = SFT_STL;
		if (!InitPath.IsEmpty())
		{
			// remove path
			OFileName.Delete(0, OFileName.ReverseFind('\\') + 1);
		}
		IOFileName = OFileName;
	}
	else
		StockType = SFT_UNKNOWN;
	return StockType;
}

StockFileType MTEdit::DetSFT(const CString& FileName)
{
	int index = FileName.ReverseFind('.');
	if (index <= 0)
		return SFT_UNKNOWN;
	CString RecExt = FileName.Right(FileName.GetLength() - index - 1).MakeUpper();

	if (RecExt == _T("STL"))
		return SFT_STL;
	else if (RecExt == _T("F2C") || RecExt == _T("STC") || RecExt == _T("DXF"))
		return SFT_CONT;
	else if (RecExt == _T("MFR"))
		return SFT_MFR;
	else if (RecExt == _T("STP") || RecExt == _T("STEP"))
		return SFT_STEP;
	else if (RecExt == _T("IGS") || RecExt == _T("IGES"))
		return SFT_IGES;
	else
		return SFT_UNKNOWN;
}

