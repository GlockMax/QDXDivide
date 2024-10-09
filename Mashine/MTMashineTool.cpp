// MTMashineTool.cpp : implementation file
//

#include "stdafx.h"
#include "NCMProject.h"
#include "math.h"
#include "tinyxml.h"
#include "ConstDef.h"
#include "BSpacePos.h"
#include "RPickID.h"
#include "NLine.h"
#include "mtRotaryAxis.h"
#include "mtForwardAxis.h"
#include "mtPartAxis.h"
#include "MTSimpleChainAxis.h"
#include "mtPartArray.h"
#include "mtPart.h"
#include "SMachState.h"
#include "SCoordsDef.h"
#include "NTiltPlane.h"
#include "MTCollisionArray.h"
#include "MTPartVisProp.h"
#include "PropList.h"
#include "NSurfSettings.h"
#include "NCUnitStatePrev.h"
#include "NCadrGeom.h"
#include "SCoordsDef.h"
#include "NCUnit.h"
#include "NMachUnitPair.h"
#include "BScrewMotion.h"
#include "NToolCombined.h"
#include "MTConfig.h"
#include "MTMashineTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

const double EPS = 1.0;
const double EPS2 = EPS*EPS;

// MTMashineTool
IMPLEMENT_SERIAL(MTMachineTool,MTMachine,0)
MTMachineTool::MTMachineTool()
{
	Transparency = 0.5;
	FileName = _T("3X_DEFAULT");
	Comp.CompTool = true;
	Comp.CompLocal = true;
	Comp.CompStock = true;
	ToolLength = 0.;
	StockGabar.Expand(0.,0.,0.);
	DrawAllTools = true;
	RMode = RM_SHADE;
}

MTMachineTool::~MTMachineTool()
{
//	ClearPartsPropList();
}


void MTMachineTool::GetToolAndStockMatr(BMatr &ToolM, BMatr &StockM, int StockPosition, int Tur, int Att, bool UseCompensation)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!UseCompensation)
	{
		GetCurConfig().CurPosIsActual = false;
		GetCurConfig().CompensationIsActual = true;
	}
	MakeActual();
	if(!UseCompensation)
	{
		GetCurConfig().CompensationIsActual = false;
	}
	const MTPartAxis *pToolAxis = GetToolAxis(Tur, Att);
	if(!pToolAxis)
		ToolM.SetE();
	else
		ToolM = pToolAxis->GetMatr();
	const MTPartAxis *pStockAxis = GetStockAxis(StockPosition);
	if(!pStockAxis)
		StockM.SetE();
	else
		StockM = pStockAxis->GetMatr();
}

void MTMachineTool::GetToolAndStockMovedMatr(BMatr &ToolM, BMatr &StockM, int StockPosition, int Tur, int Att, bool UseCompensation)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!UseCompensation)
	{
		GetCurConfig().CurPosIsActual = false;
		GetCurConfig().CompensationIsActual = true;
	}
	MakeActual();
	if(!UseCompensation)
	{
		GetCurConfig().CompensationIsActual = false;
	}
	const MTPartAxis *pToolAxis = GetToolAxis(Tur, Att);
	if(!pToolAxis)
		ToolM.SetE();
	else
		ToolM = pToolAxis->GetMatr();
	const MTPartAxis *pStockAxis = GetStockMovedAxis(StockPosition);
	if(!pStockAxis)
		StockM.SetE();
	else
		StockM = pStockAxis->GetMatr();
}

bool MTMachineTool::HaveStockMoved() const
{
	return GetCurConfig().GetAxisIndex(_T("stock_moved")) >= 0;
}

const BMatr MTMachineTool::GetToolMatr(int Tur, int Att, bool UseCompensation)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!UseCompensation)
	{
		GetCurConfig().CurPosIsActual = false;
		GetCurConfig().CompensationIsActual = true;
	}
	MakeActual();
	if(!UseCompensation)
	{
		GetCurConfig().CompensationIsActual = false;
	}

	BMatr Mb;
	const MTPartAxis *pAxis = GetToolAxis(Tur, Att);
	if(!pAxis)
	{
		Mb.SetE();
		return Mb;
	}
	return pAxis->GetMatr();
}

const BMatr MTMachineTool::GetStockMovedMatr(int StockPosition, bool UseCompensation)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!UseCompensation)
	{
		GetCurConfig().CurPosIsActual = false;
		GetCurConfig().CompensationIsActual = true;
	}
	MakeActual();
	if(!UseCompensation)
	{
		GetCurConfig().CompensationIsActual = false;
	}

	const MTPartAxis *pAxis = GetStockMovedAxis(StockPosition);
	if(!pAxis)
	{
		BMatr Mb;
		Mb.SetE();
		return Mb;
	}
	return pAxis->GetMatr();
}

const BMatr MTMachineTool::GetStockMovedMatrSGr(BStockGrID StockGroup, bool UseCompensation)
{
	return GetStockMovedMatr(Conv2StPos(StockGroup), UseCompensation);
}

const BMatr MTMachineTool::GetStockMatr(int StockPosition, bool UseCompensation)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if (!UseCompensation)
	{
		GetCurConfig().CurPosIsActual = false;
		GetCurConfig().CompensationIsActual = true;
	}
	MakeActual();
	if (!UseCompensation)
	{
		GetCurConfig().CompensationIsActual = false;
	}

	BMatr Mb;
	const MTPartAxis* pAxis = GetStockAxis(StockPosition);
	if (!pAxis)
	{
		Mb.SetE();
		return Mb;
	}
	return pAxis->GetMatr();
}

const BMatr MTMachineTool::GetToolPlaceMatr(int Num, int Tur, int Att, bool *pFound)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	const MTPartAxis *pAxis = GetCurConfig().GetToolPlaceAxis(Num, Tur, Att);
	if(!pAxis)
	{
		BMatr Mb;
		Mb.SetE();
		if(pFound)
			*pFound = false;
		return Mb;
	}

	MakeActual();

	if(pFound)
		*pFound = true;
	return pAxis->GetMatr();
}

const BMatr MTMachineTool::GetStock2ToolMatrWRec(const SMachState& MachState, int StockPosition, int Tur, int Att)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	
	// Save state
	CooArray Buf = Now;

	SetState(BMTLinkage().SetIndW1(), MachState);

	BMatr Stock2ToolMatr = GetStock2ToolMatr(StockPosition, Tur, Att);

	// Restore state
	Now = Buf;

	GetCurConfig().CurPosIsActual = false;
	GetCurConfig().CompensationIsActual = false;

	return Stock2ToolMatr;
}

const BMatr MTMachineTool::GetStockMoved2ToolMatrWRec(const SMachState& MachState, int StockPosition, int Tur, int Att)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	// Save state
	CooArray Buf = Now;

	SMachState::Merge2CooArray(Now, MachState);

	BMatr Stock2ToolMatr = GetStockMoved2ToolMatr(StockPosition, Tur, Att);

	// Restore state
	Now = Buf;

	GetCurConfig().CurPosIsActual = false;
	GetCurConfig().CompensationIsActual = false;

	return Stock2ToolMatr;
}

const BMatr MTMachineTool::GetStock2ToolMatr(int StockPosition, int Tur, int Att)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
 
	// Compensation doesn't affect this matrix but CurPos does
	BMatr ToolM, StockM;
	GetToolAndStockMatr(ToolM, StockM, StockPosition, Tur, Att, false);
	return StockM.Transpose(ToolM);
}

const BMatr MTMachineTool::GetStockMoved2ToolMatr(int StockPosition, int Tur, int Att)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
 
	// Compensation doesn't affect this matrix but CurPos does
	BMatr ToolM, StockM;
	GetToolAndStockMovedMatr(ToolM, StockM, StockPosition, Tur, Att, false);
	return StockM.Transpose(ToolM);
}

const BMatr MTMachineTool::GetStockMoved2ToolMatrCSGr(BStockGrID StockGroup, int Tur, int Att) const
{
	//CSingleLock singleLock(&CritSection);
	//singleLock.Lock();

	BMatr ToolM, StockM;
	const MTPartAxis* pToolAxis = GetToolAxis(Tur, Att);
	if (!pToolAxis)
		ToolM.SetE();
	else
		ToolM = pToolAxis->GetMatr();
	const MTPartAxis* pStockAxis = GetStockMovedAxis(Conv2StPos(StockGroup));
	if (!pStockAxis)
		StockM.SetE();
	else
		StockM = pStockAxis->GetMatr();
	return StockM.Transpose(ToolM);
}

void MTMachineTool::ProcCadrStep(const class NCUnit& Unit, const NCUnitStatePrev & State)
{
	if(State.GetNextGeomActive)
		return;
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	SetCompensations(State.MTComp);
	SMachStateFix1 FixState = State.GetMachStateFixComp();
	SetStatePartial(BMTLinkage().SetIndW1(), SMachState(FixState, &Unit.GetFixedDef()), State.AddCoords);// don't use State.MachLink, i.e. it isn't correct
}

void MTMachineTool::Reset(const SCoordsDef& CDef)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
 
	MTBase::Reset();
	SMachState buf(&CDef);
	buf.GetCooArray(Now);
}

void MTMachineTool::SetFileName(const CString &name)
{
	FileName = name;
}

bool MTMachineTool::LoadXML(const CString &path, const CString &name, bool IgnoreParts)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	TiXmlDocument MachToolXml;

	bool FileNameUnchanged = (FileName == name);
	FileName = name;
	bool loadOkay = MachToolXml.LoadFile(CString(path+name+"\\main.xml").GetString());
	bool res = false;
	if(loadOkay)
		res = ReadXMLObject(&MachToolXml, path, IgnoreParts);

	if (!res)
	{
//		AfxMessageBox(IDS_MT_LOADERR);
		RemoveAll();
		return false;
	}
	if(FileNameUnchanged)
		ApplyPartsPropList();
	else
		ClearPartsPropList();
		
	return true;
}

bool MTMachineTool::LoadXMLBranch(const CString& FileName)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	TiXmlDocument BranchXml;

	bool loadOkay = BranchXml.LoadFile(FileName.GetString());
	bool res = false;
	if (loadOkay)
		res = ReadXMLObjectBranch(&BranchXml);
	return res;
}

bool MTMachineTool::ReadXMLObject(TiXmlDocument *doc, const CString &path, bool IgnoreParts)
{
	if (doc == NULL)   
		return false;

	if(!MTBase::ReadXMLObject(doc))
		return false;

	TiXmlElement* MachTool = doc->FirstChildElement("MachineTool");
	if(MachTool == 0)
	{
//		AfxMessageBox(IDS_MT_LOADERR);
		return false;
	}
	
	TiXmlNode* axes = MachTool->FirstChild("axes");
	if(axes == 0)
	{
//		AfxMessageBox(IDS_MT_LOADERR);
		return false;
	}
	TiXmlNode* axis = axes->FirstChild("axis");
	if(axis == 0)
	{
//		AfxMessageBox(IDS_MT_LOADERR);
		return false;
	}

	//очистка Part
	Part.RemoveAll();

	for(TiXmlNode* curaxis = axis;
		curaxis != 0;
		curaxis = curaxis->NextSibling("axis"))
	{
		MTPart* pPart = ReadXML1Part(curaxis, path, IgnoreParts);
		if (pPart != nullptr)
			Part.AddPart(pPart);
	}
	// Collision pairs
	// For each part axis
	GetCurConfig().Enemies.clear();
	for(TiXmlNode* curaxis = axis;
		curaxis != 0;
		curaxis = curaxis->NextSibling("axis"))
	{
		TiXmlElement* i_el = curaxis->FirstChildElement("collisioncheck");
		if(i_el == 0)
			continue;

		TiXmlElement* p_el = curaxis->FirstChildElement("param");
		if(p_el == 0)
			continue;

		const char *str;
		str = p_el->Attribute("name");
		if(str == NULL) continue;
		int MyNum = GetCurConfig().GetAxisIndex(CString(str));
		if (MyNum < 0)
			continue;

		// For each collision
		for(; i_el != 0; i_el = i_el->NextSiblingElement("collisioncheck"))
		{
			str = i_el->Attribute("axisname");
			if(str == NULL)
				continue;
			CString AxisName(str);
			int AxisNum = GetCurConfig().GetAxisIndex(AxisName);
			if (AxisNum < 0)
				continue;

			bool MyChildren = false;
			str = i_el->Attribute("mychildren");
			if(str != NULL)
			{
				CString Flag(str);
				if(Flag == "Yes")
					MyChildren = true;
			}
			bool AxisChildren = false;
			str = i_el->Attribute("axischildren");
			if(str != NULL)
			{
				CString Flag(str);
				if(Flag == "Yes")
					AxisChildren = true;
			}
			AddColToMT(MyNum, MyChildren, AxisNum, AxisChildren); 
		}
	}
	// END:Collision pairs
	//
	if(!IgnoreParts)
		for(int ip = 0; ip < Part.GetSize(); ++ip)
			Part[ip]->SetGroup(OG_MACHINE);

	MTBase::Reset();

	return true;
}

MTPart* MTMachineTool::ReadXML1Part(TiXmlNode* curaxis, const CString& path, bool IgnoreParts, const MTPartAxis* pPartAxis /*= nullptr*/)
{
	TiXmlElement* p_el = curaxis->FirstChildElement("param");
	if (p_el == 0)
		return nullptr;

	const char* str;
	str = p_el->Attribute("type");
	if (str == NULL)
		return nullptr;
	CString b(str);

	str = p_el->Attribute("name");
	CString curr_name(str);
	if (pPartAxis == nullptr)
	{
		int ind = GetCurConfig().GetAxisIndex(curr_name);
		if (ind < 0)
			return nullptr;
		pPartAxis = (MTPartAxis*)GetCurConfig().Axis[ind]->Axis;
	}
	if (b == "part")
	{
		p_el = curaxis->FirstChildElement("part");
		if (p_el != NULL)
		{
			str = p_el->Attribute("name");
			if (str != NULL)
			{
				MTPart* pPart = new MTPart(pPartAxis);
				if (!IgnoreParts)
				{
					CString FullName(str);
					const bool IsNameFull = (FullName.Find(':') >= 0);
					if (!IsNameFull)
						FullName = path + FileName + "\\" + FullName + ".stl";
					pPart->LoadFromStl(FullName);
				}
				pPart->SetColor(pPartAxis->GetColor());
				pPart->SetEdgesAttrs(EdgesAttrs);
				double marg;
				str = p_el->Attribute("margin", &marg);
				if (str != NULL)
					pPart->SetMargin(marg);
				return pPart;
			}
		}
	}
	return nullptr;
}

bool MTMachineTool::ReadXMLObjectBranch(TiXmlDocument* doc)
{
	if (doc == NULL)
		return false;

	TiXmlNode* MachTool = doc->FirstChild("MachineToolBranch");
	if (MachTool == 0)
	{
		//		AfxMessageBox(IDS_MT_LOADERR);
		return false;
	}

	TiXmlNode* axes = MachTool->FirstChild("axes");
	if (axes == 0)
	{
		//		AfxMessageBox(IDS_MT_LOADERR);
		return false;
	}

	TiXmlNode* axis = axes->FirstChild("axis");
	if (axis == 0)
	{
		//		AfxMessageBox(IDS_MT_LOADERR);
		return false;
	}

	MTLink.Clear();
	int OldPartSize = Part.GetSize();
	// read axes and parts
	for (TiXmlNode* curaxis = axis;
		curaxis != 0;
		curaxis = curaxis->NextSibling("axis"))
	{
		MTAxis* pAx = ReadXML1Axis(curaxis);

		if (pAx == nullptr)
			continue;
		CString NewName = GenNextAxisName(pAx->GetName());
		pAx->SetName(NewName);
		Configs[0].AddAxis(pAx);
		if (pAx->GetType() != MTAxis::AT_PART)
			continue;
		MTPart* pPart = ReadXML1Part(curaxis, _T(""), false, static_cast<MTPartAxis*>(pAx));
		if (pPart == nullptr)
			continue;
		pPart->SetAxisName(NewName);
		Part.AddPart(pPart);
	}
	for (int ip = OldPartSize; ip < Part.GetSize(); ++ip)
		Part[ip]->SetGroup(OG_MACHINE);
	Configs[0].Regularize();

	MTBase::Reset();
	return true;
}

bool MTMachineTool::SaveXML(const CString &path, const CString &name)
{
	TiXmlDocument *doc = new TiXmlDocument();

	WriteXMLObject(doc);

	bool res = doc->SaveFile(path+name +CString("\\main.xml"));
	delete doc;

	return res;
}

bool MTMachineTool::SaveXML4Branch(const CString& path, const CString& name, const CString& AxisName)
{
	TiXmlDocument* doc = new TiXmlDocument();
	auto AxisIndex = GetCurConfig().GetAxisIndex(AxisName);
	WriteXMLObject4Branch(doc, AxisIndex);

	bool res = doc->SaveFile(path + name);
	delete doc;

	return res;
}

void MTMachineTool::WriteXML1Axis(TiXmlElement* parent, const MTAxis* pAxis2Write, bool FullPartNames) const
{
	if (parent == nullptr)
		return;
	TiXmlElement* e_axis = new TiXmlElement("axis");

	TiXmlElement* e_param = new TiXmlElement("param");

	auto CurType = pAxis2Write->GetType();
	switch (CurType)
	{
	case MTAxis::AT_FOR://MTForwardAxis
		e_param->SetAttribute("type", "trans");
		break;
	case MTAxis::AT_ROT://MTRotaryAxis
		e_param->SetAttribute("type", "rotary");
		break;
	case MTAxis::AT_SPIN://MTSpinAxis
		e_param->SetAttribute("type", "spin");
		break;
	case MTAxis::AT_PART://MTPartAxis
		e_param->SetAttribute("type", "part");
		break;
	case MTAxis::AT_TOOL://MTToolAxis
		e_param->SetAttribute("type", "tool");
		break;
	case MTAxis::AT_VTOOL://MTVirtToolAxis
		e_param->SetAttribute("type", "virttool");
		break;
	case MTAxis::AT_STOCK://MTStockAxis
		e_param->SetAttribute("type", "stock");
		break;
	case MTAxis::AT_SCHAIN://MTSimpleChainAxis
		e_param->SetAttribute("type", "simple_chain");
		break;
	}//switch
	e_param->SetAttribute("name", pAxis2Write->GetName());
	e_param->SetAttribute("parent", pAxis2Write->GetParent());
	if (pAxis2Write->HaveExpr())
		e_param->SetAttribute("expression", pAxis2Write->GetExpr());


	e_param->SetDoubleAttribute("length", pAxis2Write->GetLength());
	if (pAxis2Write->GetVisible())
		e_param->SetAttribute("mark", "on");
	if (pAxis2Write->GetIsCS())
		e_param->SetAttribute("coordsys", "Yes");

	e_axis->LinkEndChild(e_param);

	TiXmlElement* e_vector = new TiXmlElement("vector");
	BPoint p_vector = pAxis2Write->GetVectorOr();
	e_vector->SetDoubleAttribute("x", p_vector.GetX());
	e_vector->SetDoubleAttribute("y", p_vector.GetY());
	e_vector->SetDoubleAttribute("z", p_vector.GetZ());

	e_axis->LinkEndChild(e_vector);

	if (CurType != MTAxis::AT_FOR && CurType != MTAxis::AT_ROT && CurType != MTAxis::AT_SPIN)
	{
		TiXmlElement* e_vectorY = new TiXmlElement("vectorY");
		BPoint p_vectorY = pAxis2Write->GetVectorYOr();
		e_vectorY->SetDoubleAttribute("x", p_vectorY.GetX());
		e_vectorY->SetDoubleAttribute("y", p_vectorY.GetY());
		e_vectorY->SetDoubleAttribute("z", p_vectorY.GetZ());

		e_axis->LinkEndChild(e_vectorY);
	}//if

	if (CurType != MTAxis::AT_FOR && CurType != MTAxis::AT_STOCK) //Forward, stock
	{
		TiXmlElement* e_basepoint = new TiXmlElement("basepoint");
		BPoint p_basepoint = pAxis2Write->GetBasePointOr();
		e_basepoint->SetDoubleAttribute("x", p_basepoint.GetX());
		e_basepoint->SetDoubleAttribute("y", p_basepoint.GetY());
		e_basepoint->SetDoubleAttribute("z", p_basepoint.GetZ());

		e_axis->LinkEndChild(e_basepoint);
	}
	if (CurType == 4) //tool
	{
		TiXmlElement* e_basepoint = new TiXmlElement("localdisplvec");
		BPoint p_basepoint = pAxis2Write->GetLocalDisplVec();
		e_basepoint->SetDoubleAttribute("x", p_basepoint.GetX());
		e_basepoint->SetDoubleAttribute("y", p_basepoint.GetY());
		e_basepoint->SetDoubleAttribute("z", p_basepoint.GetZ());

		e_axis->LinkEndChild(e_basepoint);
	}

	{
		TiXmlElement* e_color = new TiXmlElement("color");
		NColor c_color = pAxis2Write->GetColor();
		e_color->SetDoubleAttribute("r", (double)c_color.R());
		e_color->SetDoubleAttribute("g", (double)c_color.G());
		e_color->SetDoubleAttribute("b", (double)c_color.B());
		e_color->SetDoubleAttribute("a", (double)c_color.A());

		e_axis->LinkEndChild(e_color);
	}

	if (CurType == MTAxis::AT_PART)
	{
		MTPart* pPart = GetPart(pAxis2Write->GetName());
		if (pPart != NULL)
		{
			TiXmlElement* e_part = new TiXmlElement("part");

			//Ищем имя stl файла
			CString LocalPath = NCM_PROJECT.GetMachinesPath() + GetFileName() + _T("\\");
			CString PartName = pPart->GetFile();
			if (!FullPartNames)
			{
				bool IsPartLocal = (PartName.Left(LocalPath.GetLength()).CompareNoCase(LocalPath) == 0);// this part is local already
				CString FullName = PartName;
				int pos1 = PartName.ReverseFind('\\');
				PartName.Delete(0, pos1 + 1);
				pos1 = PartName.ReverseFind('.');
				PartName.Delete(pos1, PartName.GetLength());
				if (!IsPartLocal)// PartName is full
				{
					CString NewName = PartName;
					for(int i = 1; 0 == CopyFile(FullName, LocalPath + NewName + _T(".stl"), TRUE); ++i)
					{
						NewName.Format(PartName + _T("_%d"), i);
					}
					PartName = NewName;
					pPart->LoadFromStl(LocalPath + PartName + _T(".stl"));
				}
			}

			e_part->SetAttribute("name", PartName);

			e_part->SetDoubleAttribute("margin", pPart->GetMargin());

			e_axis->LinkEndChild(e_part);
		}
	}

	const MTRelocateAxis* pRelocateAx = dynamic_cast<const MTRelocateAxis*>(pAxis2Write);
	if (pRelocateAx != nullptr)
	{
		{
			TiXmlElement* e_limits = new TiXmlElement("limits");
			e_limits->SetDoubleAttribute("min", pAxis2Write->GetMin());
			e_limits->SetDoubleAttribute("max", pAxis2Write->GetMax());
			e_axis->LinkEndChild(e_limits);
		}
		for (int j = 0; j < pRelocateAx->GetSpecPtsSize(); ++j)
		{
			TiXmlElement* e_spec = new TiXmlElement("specialpt");
			e_spec->SetAttribute("name", j);
			e_spec->SetDoubleAttribute("value", pRelocateAx->GetSpecPtsVal(j));
			e_axis->LinkEndChild(e_spec);
		}
	}

	const MTSimpleChainAxis* pSCAx = dynamic_cast<const MTSimpleChainAxis*>(pAxis2Write);
	if (pSCAx != nullptr)
	{
		TiXmlElement* e_ch = new TiXmlElement("chain_param");
		e_ch->SetDoubleAttribute("cycle_value", pSCAx->GetCycleVal());
		e_ch->SetDoubleAttribute("center_dist", pSCAx->GetCenterDist());
		e_axis->LinkEndChild(e_ch);
	}


	for (int j = 0; j < Collisions.GetSize(); j++)
	{
		if (Collisions[j]->Axis1 == pAxis2Write->GetName())
		{
			TiXmlElement* e_collision = new TiXmlElement("collisioncheck");
			e_collision->SetAttribute("axisname", Collisions[j]->Axis2);
			CString s_flag;
			if (Collisions[j]->Axis1Ch)
				s_flag = "Yes";
			else
				s_flag = "No";
			e_collision->SetAttribute("mychildren", s_flag);

			if (Collisions[j]->Axis2Ch)
				s_flag = "Yes";
			else
				s_flag = "No";
			e_collision->SetAttribute("axischildren", s_flag);


			e_axis->LinkEndChild(e_collision);
		}//if
	}//for

	parent->LinkEndChild(e_axis);
}

bool MTMachineTool::WriteXMLObject(TiXmlDocument * doc) const
{
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "Windows-1251", "" );
	
	doc->LinkEndChild( decl );

	TiXmlElement * e_MT = new TiXmlElement( "MachineTool" );

	TiXmlElement * e_axes = new TiXmlElement( "axes" );


	//создаём элементы axis, прикрепляем их к axes
	auto& ConfAxis = GetCurConfig().Axis;
	for(int i = 0; i < ConfAxis.GetSize(); i++)
	{
		WriteXML1Axis(e_axes, GetCurConfig().Axis[i]->Axis, false);
	}//for

	e_MT->LinkEndChild(e_axes);

	doc->LinkEndChild( e_MT);

	return true;
}

bool MTMachineTool::WriteXMLObject4Branch(TiXmlDocument* doc, int AxisIndex) const
{
	if (Configs.empty())
		return false;
	auto& Conf = Configs[0];
	if (AxisIndex < -1)// internal error
		return false;
	const int level = AxisIndex < 0 ? -1 :Conf.Axis[AxisIndex]->level;

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "Windows-1251", "");

	doc->LinkEndChild(decl);

	TiXmlElement* e_MT = new TiXmlElement("MachineToolBranch");

	TiXmlElement* e_axes = new TiXmlElement("axes");


	//создаём элементы axis, прикрепляем их к axes. All children of AxisIndex
	WriteXML1Axis(e_axes, Conf.Axis.GetAt(AxisIndex)->Axis, true);
	for (int i = AxisIndex + 1; i < Conf.Axis.GetSize(); ++i)
	{
		if (Conf.Axis[i]->level <= level)
			break;
		WriteXML1Axis(e_axes, Conf.Axis.GetAt(i)->Axis, true);
	}

	e_MT->LinkEndChild(e_axes);

	doc->LinkEndChild(e_MT);

	return true;
}

void MTMachineTool::RenameAxis(int index, const CString & NewName)
{
	CString OldName = GetCurConfig().Axis[index]->Axis->GetName();
	//1 - Переименовываем ось в объектах столкновения
	Collisions.RenameAxis(OldName, NewName);

	//2 - если это деталь, переименовываем ось в объекте MTPart
	if(GetCurConfig().Axis[index]->Axis->GetType() == MTAxis::AT_PART)
	{
		for(int i = 0; i < Part.GetSize(); i++)
		{
			if(Part[i]->GetAxisName() == OldName)
				Part[i]->SetAxisName(NewName);
		}//for
	}//if

	//3 - Вызываем метод родительского класса
	MTBase::RenameAxis(index, NewName);
}

int MTMachineTool::SwapAxisNames(const CString& First, const CString& Second)
{
	int Findex = GetCurConfig().GetAxisIndex(First);
	if (Findex < 0)
		return -1;
	int Sindex = GetCurConfig().GetAxisIndex(Second);
	if (Sindex < 0)
		return -2;
	//2 - если это деталь, переименовываем ось в объекте MTPart
	{
		int Fip = -1;
		if (GetCurConfig().Axis[Findex]->Axis->GetType() == MTAxis::AT_PART)
		{
			for (int i = 0; i < Part.GetSize(); i++)
			{
				if (Part[i]->GetAxisName() == First)
				{
					Fip = i;
					break;
				}
			}
		}
		if (Fip >= 0)
			Part[Fip]->SetAxisName(Second);
	}
	{
		int Sip = -1;
		if (GetCurConfig().Axis[Sindex]->Axis->GetType() == MTAxis::AT_PART)
		{
			for (int i = 0; i < Part.GetSize(); i++)
			{
				if (Part[i]->GetAxisName() == Second)
				{
					Sip = i;
					break;
				}
			}
		}
		if (Sip >= 0)
			Part[Sip]->SetAxisName(First);
	}
	MTBase::SwapAxisNames(Findex, Sindex);
	return 0;
}

SCollision* MTMachineTool::GetAxisCol(const CString & AxisName, int index)
{
	return Collisions.GetAxisCol(AxisName, index);
}//SCollision* MTMachineTool::GetAxisCol(CString AxisName, int index)

SCollision* MTMachineTool::GetCollision(int index)
{
	return Collisions[index];
}//SCollision* MTMachineTool::GetCollision(int index)

int MTMachineTool::GetColIndex(const CString & Axis1, const CString & Axis2)
{
	return Collisions.GetColIndex(Axis1, Axis2);
}//int MTMachineTool::GetColIndex(CString Axis1, CString Axis2)

void MTMachineTool::MoveCollision(const CString& Axis1, const CString& Axis2, const CString& NewAxis1)
{
	for(int i =0 ; i < Collisions.GetSize() ; i++)
	{
		if(Collisions[i]->Axis1 == Axis1 && Collisions[i]->Axis2 == Axis2)
		{
			Collisions[i]->Axis1 = NewAxis1;
			break;
		}//if
	}//for
	GetCurConfig().ReGenEnemiesPairs(Collisions);
}

const CString & MTMachineTool::GetPartAxisNameByPartIndex(int PartIndex)
{
	return Part[PartIndex]->GetAxisName();
}
void MTMachineTool::MTPropMap::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		if (m_nCount == 0)
			return;  // nothing more to do

		ASSERT(m_pHashTable != NULL);
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				ar << pAssoc->key;
			}
		}
	}
	else
	{
		DWORD_PTR nNewCount = ar.ReadCount();
		RemoveAll();
		CString newKey;

		while (nNewCount--)
		{
			ar >> newKey;
//			ar >> newValue;
			SetAt(newKey, NULL);
		}
	}
}

void MTMachineTool::MTPropMap::Serialize48(CArchive & ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		SerializeElements(ar, &m_nCount, 1);
		if (m_nCount == 0)
			return;  // nothing more to do

		ASSERT(m_pHashTable != NULL);
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
			{
				SerializeElements(ar, &pAssoc->key, 1);
				SerializeElements(ar, (MTPartVisProp *)(pAssoc->value), 1);
			}
		}
	}
	else
	{
		RemoveAll();
		DWORD_PTR nNewCount;
		SerializeElements(ar, &nNewCount, 1);
		while (nNewCount--)
		{
			CString key;
			SerializeElements(ar, &key, 1);
			MTPartVisProp *pVisProp = new MTPartVisProp;
			SerializeElements(ar, pVisProp, 1);
			SetAt(key, pVisProp);
		}
	}
}

void MTMachineTool::Serialize(CArchive &ar)
{
	SerializeElements(ar, &FileName, 1);
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.8")))
	{
		SerializeElements(ar, &DrawAllTools, 1);
		SerializeElements(ar, &Visible, 1);
	}
	if(!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("3.9")))
	{
		SerializeElements(ar, &DrawSharpEdges, 1);

		int a=0;
		if (ar.IsLoading())
			ar>>a;
		else
			ar<<a;

		EdgesAttrs.Serialize(ar);
		SerializeElements(ar, &Transparency, 1);
		SerializeElements(ar, &DrawSmoothSurf, 1);
		SerializeElements(ar, &RMode, 1);
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.8")))
		{
			if (ar.IsStoring())
				FillPartsPropList();
			PartsPropList.Serialize48(ar);
			if (ar.IsStoring())
				ClearPartsPropList();
		}
		else
		{// Old version of loading
			PartsPropList.Serialize(ar);
			for (POSITION pos = PartsPropList.GetStartPosition(); pos != NULL;)
			{
				CString Key;
				MTPartVisProp *pVisProp = NULL;
				PartsPropList.GetNextAssoc(pos, Key, pVisProp);
				pVisProp = new MTPartVisProp;
				SerializeElements(ar, pVisProp, 1);
				PartsPropList[Key] = pVisProp;
			}
		}
	}
	MTMachine::Serialize(ar);
}

void MTMachineTool::SetVisible(bool vis)
{
	Visible = vis;
}

bool MTMachineTool::IsVisible(void) const
{
	return Visible;
}

const CString & MTMachineTool::GetFileName(void) const
{
	return FileName;
}

void MTMachineTool::SetStateWORecalc(BMTLinkage ConfNum, const SMachState& State)
{
	if (State.IsVector())
		return;

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	MTBase::Reset();
	SetCurConf(ConfNum);
	State.GetCooArray(Now);
	GetCurConfig().Reset();
}

void MTMachineTool::GetState(BPoint& XYZ, BPoint& ABC, const SCoordsDef& FixedDef)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	XYZ.Set(0., 0., 0., 1.);
	ABC.Set(0., 0., 0., 1.);
	for (int Ind = 0; Ind < 6; ++Ind)
	{
		const CString& FixedName = FixedDef.GetName(Ind);
		for (int k = 0; k < 6; ++k)
		{
			if (FixedName == Now[k].AxisName)
			{
				if (Ind < 3)
					XYZ[Ind] = Now[k].Value;
				else
					ABC[Ind - 3] = Now[k].Value;
				break;
			}
		}
	}
}

void MTMachineTool::SetState(BMTLinkage ConfNum, const SMachState & State)
{
	if(State.IsVector())
		return;

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	for (int i = 0; i < Configs.size(); ++i)
		Configs.at(i).Reset();
	SetCurConf(ConfNum);
	State.GetCooArray(Now);
	GetCurConfig().Reset();
	ApplyRCP();
}

void MTMachineTool::GetState(SMachState & State, SCoordsDef &CDef)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	CDef.Define(Now);
	State = SMachState(&CDef);
	for(int i = 0; i < Now.GetSize(); ++i)
		State.SetCoord(i, Now.GetAt(i).Value);
}

void MTMachineTool::GetState(BMTLinkage& Link, SMachState& State, SCoordsDef& CDef)
{
	Link = MTLink;
	GetState(State, CDef);
}

void MTMachineTool::GetSpecState(int i, SMachState & State, SCoordsDef & CDef, int CurChannel)
{
	CDef.Define(Now);
	State = SMachState(&CDef);
	for (int k = 0; k < Now.GetSize(); ++k)
	{
		MTAxis *pAxis = GetCurConfig().GetAxis(Now.GetAt(k).AxisName);
		MTRelocateAxis *pRelocateAx = dynamic_cast<MTRelocateAxis *>(pAxis);
		if (pRelocateAx != nullptr)
			State.SetCoord(k, pRelocateAx->GetSpecPtsVal(i));
	}
	const auto& Shift = ToolShifts.at(CurChannel);
	if (Shift.IsDefined())
	{
		BPoint XYZ = CalcCurToolComp(Shift);
		State.SetCoord(Shift.CoordsDef.GetName(0), State.GetCoord(Shift.CoordsDef.GetName(0)) - XYZ.GetX());
		State.SetCoord(Shift.CoordsDef.GetName(1), State.GetCoord(Shift.CoordsDef.GetName(1)) - XYZ.GetY());
		State.SetCoord(Shift.CoordsDef.GetName(2), State.GetCoord(Shift.CoordsDef.GetName(2)) - XYZ.GetZ());
	}
}

bool MTMachineTool::GetSpecState4Tool(int i, SMachState & State, SCoordsDef & CDef, int CurChannel)
{
	const auto& Shift = ToolShifts.at(CurChannel);
	if (!Shift.IsDefined())
		return false;
	MTPartAxis* pToolAxis = GetToolAxis(Shift.Turr, Shift.Att);
	if (pToolAxis == nullptr)
		return false;
	CDef.Define(Now);
	State = SMachState(&CDef);
	BPoint XYZ = CalcCurToolComp(Shift);
	State.SetCoord(Shift.CoordsDef.GetName(0), State.GetCoord(Shift.CoordsDef.GetName(0)) - XYZ.GetX());
	State.SetCoord(Shift.CoordsDef.GetName(1), State.GetCoord(Shift.CoordsDef.GetName(1)) - XYZ.GetY());
	State.SetCoord(Shift.CoordsDef.GetName(2), State.GetCoord(Shift.CoordsDef.GetName(2)) - XYZ.GetZ());
	for (int k = 0; k < Now.GetSize(); ++k)
	{
		MTAxis *pAxis = GetCurConfig().GetAxis(Now.GetAt(k).AxisName);
		MTRelocateAxis *pRelocateAx = dynamic_cast<MTRelocateAxis *>(pAxis);
		if (pRelocateAx == nullptr)
			continue;
		if (GetCurConfig().IsDescendant(*pRelocateAx, *pToolAxis))
		{
			double Val = pRelocateAx->GetSpecPtsVal(i);
			const CString &AxName = pRelocateAx->GetName();
			if (AxName == Shift.CoordsDef.GetName(0))
				Val -= XYZ.GetX();
			else if (AxName == Shift.CoordsDef.GetName(1))
				Val -= XYZ.GetY();
			else if (AxName == Shift.CoordsDef.GetName(2))
				Val -= XYZ.GetZ();
			State.SetCoord(k, Val);
		}
		else
			State.SetCoord(k, Now.GetAt(k).Value);
	}
	return true;
}

void MTMachineTool::GetCoordsDef(SCoordsDef& CDef) const
{
	CDef.Define(Now);
}

void MTMachineTool::SetState(BMTLinkage ConfNum, const SMachState & State1, const SMachState & State2)
{
	if(State1.IsVector() || State2.IsVector())
		return;

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	MTBase::Reset();
	SetCurConf(ConfNum);

	Now.SetSize(State1.GetSize() + State2.GetSize() - 2);
	int i = 0;
	for(int k = 0; k < State1.GetSize() - 1; ++k, ++i)
		Now[i].Set(State1.GetAName(k), State1.GetCoord(k));
	for(int k = 0; k < State2.GetSize() - 1; ++k, ++i)
		Now[i].Set(State2.GetAName(k), State2.GetCoord(k));
	ApplyRCP();
}

void MTMachineTool::SetStatePartial(BMTLinkage ConfNum, const SMachState& State1, const SMachState& State2)
{
	if (State1.IsVector() || State2.IsVector())
		return;

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	SetCurConf(ConfNum);
	GetCurConfig().Reset();

	SMachState::Merge2CooArray(Now, State1);
	SMachState::Merge2CooArray(Now, State2);
	ApplyRCP();
}

void MTMachineTool::SetStatePartial(BMTLinkage ConfNum, const SMachState& State1)
{
	if (State1.IsVector())
		return;

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	SetCurConf(ConfNum);

	SMachState::Merge2CooArray(Now, State1);
	ApplyRCP();
}

BPoint MTMachineTool::GetVectXYZ(const BPoint& V, const SCoordsDef& FixedDef, int Tur, int Att)
{
	// Определяем на сколько по координатам X, Y, Z нужно пререместить
	// станок, чтобы инструмент сместился на вектор V, заданный
	// в системе координат станка
	// Предполагается, что оси X, Y, Z станка ортогональны
	// Если какие-то из осей отсутствуют, то будет найдена ближайшая позиция
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
 
	MTAxis *pToolAxis = GetToolAxis(Tur, Att);
	if(!pToolAxis)
		return BPoint(0.,0.,0.,-1.);

	int iX = GetCurConfig().GetAxisIndex(FixedDef.GetName(0));
	int iY = GetCurConfig().GetAxisIndex(FixedDef.GetName(1));
	int iZ = GetCurConfig().GetAxisIndex(FixedDef.GetName(2));

	BPoint Vx(0., 0., 0., 0.), Vy(0., 0., 0., 0.), Vz(0., 0., 0., 0.);
	double kx = 0., ky = 0., kz = 0.;

	if(iX >= 0)
	{
		Vx = GetCurConfig().Axis[iX]->Axis->GetVector();
		kx = GetCurConfig().IsDescendant(*(GetCurConfig().Axis[iX]->Axis), *pToolAxis) ? 1. : -1.;
	}
	if(iY >= 0)
	{
		Vy = GetCurConfig().Axis[iY]->Axis->GetVector();
		ky = GetCurConfig().IsDescendant(*(GetCurConfig().Axis[iY]->Axis), *pToolAxis) ? 1. : -1.;
	}
	if(iZ >= 0)
	{
		Vz = GetCurConfig().Axis[iZ]->Axis->GetVector();
		kz = GetCurConfig().IsDescendant(*(GetCurConfig().Axis[iZ]->Axis), *pToolAxis) ? 1. : -1.;
	}
	double Sxy = Vx * Vy;
	double Sxz = Vx * Vz;
	double Syz = Vy * Vz;
	if(Sxy == 0. && Sxz == 0. && Syz == 0.)
		return BPoint(kx * (V * Vx), ky * (V * Vy), kz *(V * Vz), 1.);
	BMatr System(1., Sxy, Sxz, 0.
		, Sxy, 1., Syz, 0.
		, Sxz, Syz, 1., 0.
		, 0., 0., 0., 1.);
	System = System.invr();
	BPoint Coef = BPoint(V * Vx, V * Vy, V * Vz, 0.) * System.T();
	return BPoint(kx * Coef.GetX(), ky * Coef.GetY(), kz * Coef.GetZ(), 1.);
}

BPoint MTMachineTool::GetVectXYZ(const BPoint& V, const ShiftData& Shift)
{
	return GetVectXYZ(V, Shift.CoordsDef, Shift.Turr, Shift.Att);
}

BMatr MTMachineTool::GetMainCoordsMatr(const SCoordsDef& FixedDef)
{
	const int iX = GetCurConfig().GetAxisIndex(FixedDef.GetName(0));
	const int iY = GetCurConfig().GetAxisIndex(FixedDef.GetName(1));
	const int iZ = GetCurConfig().GetAxisIndex(FixedDef.GetName(2));

	BPoint Vx(0., 0., 0., 0.), Vy(0., 0., 0., 0.), Vz(0., 0., 0., 0.);
	double kx = 0., ky = 0., kz = 0.;

	if (iX >= 0)
	{
		Vx = GetCurConfig().Axis[iX]->Axis->GetVector();
		kx = IsAnyToolAncestor(GetCurConfig().Axis[iX]->Axis) ? 1. : -1.;
	}
	if (iY >= 0)
	{
		Vy = GetCurConfig().Axis[iY]->Axis->GetVector();
		ky = IsAnyToolAncestor(GetCurConfig().Axis[iY]->Axis) ? 1. : -1.;
	}
	if (iZ >= 0)
	{
		Vz = GetCurConfig().Axis[iZ]->Axis->GetVector();
		kz = IsAnyToolAncestor(GetCurConfig().Axis[iZ]->Axis) ? 1. : -1.;
	}
	BMatr Res;
	Res.SetRow(0, Vx * kx);
	Res.SetRow(1, Vy * ky);
	Res.SetRow(2, Vz * kz);
	Res.SetRow(3, BPoint(0., 0., 0., 1.));
	return Res.MakeNonDeg();
}

int amax(int a, int b)
{
	return max(a, b); //Needed to prevent double function call when argument is fuction call
}

int MTMachineTool::SetCurComp()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	
	GetCurConfig().CompensationIsActual = true;
	int Result = -1;
	ApplyCurComp(false);
	return Result;
}

int MTMachineTool::ApplyCurComp(bool ForDraw)
{
	int Result = -1;
	double StockHeight = 1.e12;
	double StockX = 0.;
	double StockY = 0.;
	BPoint Vx(0., 0., 0., 0.);
	BPoint Vy(0., 0., 0., 0.);
	BPoint Vz(0., 0., 0., 0.);
	if (Comp.CompStock && StockGabar.IsDefined())
	{
		BPoint stockXYZ(0., 0., 0., 1);
		int ix = GetCurConfig().GetAxisIndex("stock_X");
		if (ix >= 0)
		{
			MTAxis* pAxisX = GetCurConfig().Axis[ix]->Axis;
			Vx = pAxisX->GetVector();
			int iy = GetCurConfig().GetAxisIndex("stock_Y");
			if (iy >= 0)
			{
				MTAxis* pAxisY = GetCurConfig().Axis[iy]->Axis;
				Vy = pAxisY->GetVector();
				int iz = GetCurConfig().GetAxisIndex("stock_Z");
				if (iz >= 0)
				{
					MTAxis* pAxisZ = GetCurConfig().Axis[iz]->Axis;
					Vz = pAxisZ->GetVector();

					int ib = GetStockAxisInd(0);
					if (ib >= 0)
					{
						MTAxis* pAxisB = GetCurConfig().Axis[ib]->Axis;
						BPoint BasePoint = pAxisB->GetBasePoint();

						BBox RotGabar = StockGabar;
						BMatr M(GetStockMatr(0, true));// stock_X ... works for stock positon = 0 only
						RotGabar.Transform(M);

						BMatr MI(M.invr());
						BPoint VzC = Vz * MI;

						BPoint Pmin = StockGabar.GetMinPoint();
						BPoint Pmax = StockGabar.GetMaxPoint();
						BPoint p;
						p.Set(Pmin.GetX(), Pmin.GetY(), Pmax.GetZ(), 1.);
						StockHeight = min(StockHeight, (p - BasePoint) * VzC);
						p.Set(Pmin.GetX(), Pmax.GetY(), Pmax.GetZ(), 1.);
						StockHeight = min(StockHeight, (p - BasePoint) * VzC);
						p.Set(Pmin.GetX(), Pmax.GetY(), Pmin.GetZ(), 1.);
						StockHeight = min(StockHeight, (p - BasePoint) * VzC);
						p.Set(Pmax.GetX(), Pmin.GetY(), Pmin.GetZ(), 1.);
						StockHeight = min(StockHeight, (p - BasePoint) * VzC);
						p.Set(Pmax.GetX(), Pmax.GetY(), Pmin.GetZ(), 1.);
						StockHeight = min(StockHeight, (p - BasePoint) * VzC);
						p.Set(Pmax.GetX(), Pmin.GetY(), Pmax.GetZ(), 1.);
						StockHeight = min(StockHeight, (p - BasePoint) * VzC);
						StockHeight = min(StockHeight, (Pmin - BasePoint) * VzC);
						StockHeight = min(StockHeight, (Pmax - BasePoint) * VzC);

						StockX = (((RotGabar.GetMinPoint() + RotGabar.GetMaxPoint()) * 0.5) - BasePoint) * Vx;
						StockY = (((RotGabar.GetMinPoint() + RotGabar.GetMaxPoint()) * 0.5) - BasePoint) * Vy;

						stockXYZ.Set(-StockX, -StockY, -StockHeight, 1.);
						Result = amax(Result, GetCurConfig().MoveAxis("stock_X", stockXYZ.GetX()));
						Result = amax(Result, GetCurConfig().MoveAxis("stock_Y", stockXYZ.GetY()));
						Result = amax(Result, GetCurConfig().MoveAxis("stock_Z", stockXYZ.GetZ()));
					}
				}
			}
		}
	}
	if (GetCurConfig().HaveExpressions())
	{
		CooArray Buf;
		Buf.Copy(Now);

		bool HaveComp = false;
		for each (auto Shift in ToolShifts)
		{
			BPoint XYZ = CalcCurToolComp(Shift);
			int Result = -1;
			if (Comp.CompStock && StockGabar.IsDefined())
			{
				XYZ += GetVectXYZ(Vz * (-StockHeight) + Vx * (-StockX) + Vy * (-StockY), Shift);
			}
			if (Comp.CompTool || Comp.CompLocal)
			{
				if (XYZ.D2() > MIND * MIND)
				{
					HaveComp = true;
					// recalc Now
					for (int i = 0; i < Now.GetSize(); ++i)
					{
						auto& Co = Now[i];
						if (Co.AxisName == Shift.CoordsDef.GetName(0))
							Co.Value += XYZ.GetX();
						else if (Co.AxisName == Shift.CoordsDef.GetName(1))
							Co.Value += XYZ.GetY();
						else if (Co.AxisName == Shift.CoordsDef.GetName(2))
							Co.Value += XYZ.GetZ();
					}
				}
			}
		}
		if (HaveComp)
		{
			Result = GoToCurPos();
			Now.Copy(Buf);

			for each (auto Shift in ToolShifts)
			{
				// move tool axis to compensate its movement above in this method		
				MTPartAxis* pToolAxis = GetToolAxis(Shift.Turr, Shift.Att);
				if (pToolAxis)
					pToolAxis->Move(BMatr().Trans(BPoint(0., 0., 0., 0.), CalcCurShiftVec(Shift)));// CalcCurShift is calling twice. First call in CalcCurToolComp
			}
		}
	}
	else
	{
		for each (auto Shift in ToolShifts)
		{
			if (!Shift.IsDefined())
				continue;
			BPoint XYZ = CalcCurToolComp(Shift);
			if (Comp.CompStock && StockGabar.IsDefined())
			{
				XYZ += GetVectXYZ(Vz * (-StockHeight) + Vx * (-StockX) + Vy * (-StockY), Shift);
			}
			if (Comp.CompTool || Comp.CompLocal)
			{
				Result = amax(Result, GetCurConfig().MoveAxis(Shift.CoordsDef.GetName(0), XYZ.GetX()));
				Result = amax(Result, GetCurConfig().MoveAxis(Shift.CoordsDef.GetName(1), XYZ.GetY()));
				Result = amax(Result, GetCurConfig().MoveAxis(Shift.CoordsDef.GetName(2), XYZ.GetZ()));
			}
			// move tool axis to compensate its movement above in this method		
			MTPartAxis* pToolAxis = GetToolAxis(Shift.Turr, Shift.Att);
			if (pToolAxis)
				pToolAxis->Move(BMatr().Trans(BPoint(0., 0., 0., 0.), CalcCurShiftVec(Shift)));
		}
	}
	return Result;
}
int MTMachineTool::ApplyRCP()
{
	if (Comp.RCPMask == 0)
		return 0;
	if (ToolShifts.empty())
		return 0;
	CooArray Buf = Now;
	if ((Comp.RCPMask & 0x1) != 0)
		Now[3].Value = 0.;
	if ((Comp.RCPMask & 0x2) != 0)
		Now[4].Value = 0.;
	if ((Comp.RCPMask & 0x4) != 0)
		Now[5].Value = 0.;
	GoToCurPos();
	BPoint ToolStock = BPoint(0., 0., 0., 1) * GetStockMoved2ToolMatr(0, 0, 0);
	Now = Buf;
	GoToCurPos();
	BPoint NewToolMach = BPoint(0., 0., 0., 1) * GetToolMatr(0, 0, false);
	BPoint NewToolStockMach = ToolStock * GetStockMovedMatr(0, false);
	BPoint 	XYZ = GetVectXYZ(NewToolStockMach - NewToolMach, ToolShifts.front());
	Now[0].Value += XYZ.GetX();
	Now[1].Value += XYZ.GetY();
	Now[2].Value += XYZ.GetZ();
	int Res = GoToCurPos();
	return Res;
}
void MTMachineTool::SetComp(const NCUnit& Unit, double TL, const BPoint & ToolShift, int Tur, int Att, const BBox &SG)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	int Chan = Unit.GetChannelInd();

	//if(GetCurConfig().CompensationIsActual && ToolLength == TL
	//		&& StockGabar == SG 
	//		&& (ToolShift - ToolShiftVec).D2() == 0.
	//		&& Tur == ActToolTur
	//		&& Att == ActToolAtt)
	//	return;

	ToolLength = TL;
	StockGabar = SG;
	if (ToolShifts.size() <= Chan)
	{
		ToolShifts.resize(Chan + 1);
	}
	ToolShifts[Chan].Set(Tur, Att, ToolShift, Unit.GetFixedDef());
	ResetComp();
}

BPoint MTMachineTool::CalcCurToolComp(const ShiftData& Shift)
{
	BPoint XYZ(0., 0., 0., 0.);
	if (Comp.CompTool || Comp.CompLocal)
	{
		BPoint CurShiftVec = CalcCurShiftVec(Shift);
		XYZ = GetVectXYZ(CurShiftVec * (-1.), Shift);
	}
	return XYZ;
}

BPoint MTMachineTool::CalcCurShiftVec(const ShiftData& Shift) const
{
	BPoint CurShiftVec(0., 0., 0., 0.);
	MTPartAxis* pToolAxis = GetToolAxis(Shift.Turr, Shift.Att);
	if (pToolAxis)
	{
		if (Comp.CompLocal)
			CurShiftVec = pToolAxis->GetLocalDisplVec();
		if (Comp.CompTool)
			CurShiftVec = CurShiftVec + Shift.ToolShiftVec;
		CurShiftVec = CurShiftVec * pToolAxis->GetMatr();
	}
	return CurShiftVec;
}

int MTMachineTool::GoToCurPos()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	return GetCurConfig().MoveToCurPos(Now);
}

void MTMachineTool::Draw(RenderMode GlobRMode, bool DrawSurf)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(!Visible)
		return;
	glPushName(OG_MACHINE);

	if(DrawAxis && (GlobRMode != RM_TRANSLUCENT))
		GetCurConfig().Draw(DrawPartAxis, SelectedAxis);
	if(DrawPart)
	{
		if(GlobRMode == RM_TRANSLUCENT)
		{
			for(int i = 0; i < Part.GetSize(); ++i)
				Part[i]->GetColor().A() = float(Transparency);
		}
		Part.Draw(GlobRMode, GetRMode(), DrawSharpEdges, DrawSurf, DrawSmoothSurf);
	}

	glPopName();// OG_MACHINE
}
BBox MTMachineTool::GetGabar()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	MakeActual();
	BBox gabar;
//	gabar.Expand(Axis.GetGabar());
	gabar.Expand(Part.GetGabar());
	return gabar;
}

void MTMachineTool::FullCopyWOVis(const MTMachineTool& MT)
{
	MTMachine::FullCopyWOVis(MT);

	Comp = MT.Comp;
	ToolLength = MT.ToolLength;
	StockGabar = MT.StockGabar;
	DrawAllTools = MT.DrawAllTools;
	ToolShifts = MT.ToolShifts;
}

void MTMachineTool::CalcIntState(int RCPMask, double t, const SMachState& Start, const SMachState& End, SMachState& Result, int StockPosition, int Turret, int Attach)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	// Find Result state for the linear movement from Start to End while RCP is active
	Result = Start * (1. - t) + End * t;// Intermediate state with all rotations
	if (RCPMask != 0)
	{
		BPoint BasePoint(0., 0., 0., 1.);
		SetStatePartial(MTLink, Start);
		BPoint StartPosStock = (BasePoint * GetStock2ToolMatr(StockPosition, Turret, Attach));
		// Machine position with rotation
		SMachState CorrEnd(End);
		CorrStateRCP(RCPMask, Start, CorrEnd); // Remove non RCP affected rotations from CorrEnd. Now CorrAnd is an End state with RCP affected rotations only
		SetStatePartial(MTLink, CorrEnd);
		BPoint EndPosStock = (BasePoint * GetStock2ToolMatr(StockPosition, Turret, Attach));
		BPoint IntPosStock = StartPosStock * (1. - t) + EndPosStock * t;
		SMachState Interm(Result.GetDef());
		Interm = Start * (1. - t) + CorrEnd * t;// Intermediate state with RCP affected rotations only
		SetStatePartial(MTLink, Interm);
		BMatr ToolM, StockMatr;
		GetToolAndStockMatr(ToolM, StockMatr, StockPosition, Turret, Attach, false);
		BPoint Src = BasePoint * StockMatr.Transpose(ToolM) * StockMatr;
		BPoint Dst = IntPosStock * StockMatr;
		BPoint V = GetVectXYZ(Dst - Src, *Start.GetDef(), Turret, Attach);
		Interm.AddXYZ(V);
		CorrStateRCP(RCPMask, Result, Interm);// Return non RCP affected rotations to Interm
		Result = Interm;
	}
}

void MTMachineTool::CorrStateRCP(int RCPMask, const SMachState& Start, SMachState& Interm)
{
	if ((RCPMask & 0x1) == 0)
		Interm.SetCoord(3, Start.GetCoord(3));
	if ((RCPMask & 0x2) == 0)
		Interm.SetCoord(4, Start.GetCoord(4));
	if ((RCPMask & 0x4) == 0)
		Interm.SetCoord(5, Start.GetCoord(5));
}

int MTMachineTool::ForceActual()
{
	GetCurConfig().CurPosIsActual = false;
	GetCurConfig().CompensationIsActual = false;
	return MakeActual();
}
int MTMachineTool::MakeActual()
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	int Res = -1;
	if(!GetCurConfig().CurPosIsActual)
		Res = amax(Res, GoToCurPos());

	if(!GetCurConfig().CompensationIsActual)
		Res = amax(Res, SetCurComp());

	return Res;
}

void MTMachineTool::SetCompensations(const MTCompensations &inComp)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();

	if(inComp == Comp)
		return;
	GetCurConfig().CompensationIsActual = false;
	Comp = inComp;
}
const BMatr MTMachineTool::GetRespMatr(const BMatr &CSMatr, const BPoint &ABC, int Tur, int Att, int StockPosition, const SCoordsDef* pFixedState)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	// Save state
	CooArray Buf = Now;

	BPoint P0(1.,0.,0.,1.);
	BPoint P1(0.,1.,0.,1.);
	BPoint P2(0.,0.,1.,1.);
	BPoint P3(0.,0.,0.,1.);
	SetState(BMTLinkage().SetIndW1(), SMachState(P0 * CSMatr, ABC, pFixedState));
	BPoint Q0 = P3 * GetStock2ToolMatr(StockPosition, Tur, Att);
	SetState(BMTLinkage().SetIndW1(), SMachState(P1 * CSMatr, ABC, pFixedState));
	BPoint Q1 = P3 * GetStock2ToolMatr(StockPosition, Tur, Att);
	SetState(BMTLinkage().SetIndW1(), SMachState(P2 * CSMatr, ABC, pFixedState));
	BPoint Q2 = P3 * GetStock2ToolMatr(StockPosition, Tur, Att);
	SetState(BMTLinkage().SetIndW1(), SMachState(P3 * CSMatr, ABC, pFixedState));
	BPoint Q3 = P3 * GetStock2ToolMatr(StockPosition, Tur, Att);

	// Restore state
	Now = Buf;
	GetCurConfig().CurPosIsActual = false;
	GetCurConfig().CompensationIsActual = false;

	BMatr Q;
	Q.SetRow(0, Q0 - Q3);
	Q.SetRow(1, Q1 - Q3);
	Q.SetRow(2, Q2 - Q3);
	Q.SetRow(3, Q3);
	return Q.MakeNonDeg();
}

const CString MTMachineTool::GetNameGeneralAxis(int StockPosition, int Tur, int Att)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
 
	MTPartAxis *AStock = GetStockAxis(StockPosition);
	if(AStock == NULL)
		return "";
	MTPartAxis *ATool = GetToolAxis(Tur, Att);
	if(ATool == NULL)
		return "";

	MTAxis* Ax = AStock;
	while(Ax->GetParent() != "")
	{
		Ax->SetMarked(true);
		Ax = GetCurConfig().GetAxis(Ax->GetParent());//Перешли к родителю
	}
	Ax->SetMarked(true);

	Ax = ATool;
	while(Ax->GetParent() != "")
	{
		Ax = GetCurConfig().GetAxis(Ax->GetParent());//Перешли к родителю
		if(Ax->IsMarked() == true)
			return Ax->GetName();
	}
	return "";
}
int MTMachineTool::NumArray(const CooArray &array, const CString & name)
{
	for(int i = 0; i < array.GetSize(); i++)
		if(array[i].AxisName == name)
			return i;
	return -1;
}
bool MTMachineTool::DataFit(const SMachState& Begin, const SMachState& End)
{
	int kol = (int)Begin.GetSize();
	//Если количество осей в begin и end не соответствует друг другу или передали нулевое количество
	if(kol != End.GetSize() || kol == 0)
		return false;
	for(int i = 0; i < kol - 1; i++)
		if(Begin.GetAName(i) != End.GetAName(i))//Если оси в begin и end не соответствуют друг другу по порядку 
			return false;
	return true;
}
bool MTMachineTool::PartialDerivative(BMatr& M, unsigned int NumAxis, const SMachState& Begin, const SMachState& End, double t, const BMatr& StMatr, int StockPosition, int Tur, int Att)
{
	double valb = Begin.GetCoord(NumAxis);
	double vale = End.GetCoord(NumAxis);
	CString GeneralAxis = GetNameGeneralAxis(StockPosition, Tur, Att), NameAxis = Begin.GetAName(NumAxis);
	BMatr A, B, Temp;

	BMatr S(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	double dval = vale - valb;
	double val = (vale - valb) * t;

	MTAxis* pToolAxis = GetToolAxis(Tur, Att);
	if (pToolAxis == NULL)
		return false;

	int kol = 0;
	bool h = false, f = false;
	int k = 0;
	do//Первое слагаемое у производной найдём надеясь, что есть хоть одна управляемая ось
	{
		h = false;
		f = false;
		k = 0;//Количество пройденных продифференцированных матриц -1

		MTAxis* Ax = pToolAxis;
		CString str = Ax->GetParent();


		while (str != GeneralAxis)//По ветви инструмента
		{
			Ax = GetCurConfig().GetAxis(str);//берем указатель на родителя, т.е. можно взять указатель в случае Name != "";
			if (Ax == nullptr)
				continue;
			if(str == NameAxis)//Если ось управляется xi
			{
				if (f == false)
				{
					if (k == 0)
					{
						Ax->CreateDerivMoveMatrix(Temp, StMatr, val, dval);
						kol++;
						f = true;
					}
					else
					{
						if (k < kol)
						{
							Ax->CreateMoveMatrixSt(Temp, StMatr, val);
							k++;
						}
						else
						{
							Ax->CreateDerivMoveMatrix(Temp, StMatr, val, dval);
							kol++;
							f = true;
						}
					}
				}
				else
				{
					Ax->CreateMoveMatrixSt(Temp, StMatr, val);
					h = true;//Есть управляемая ось, матрицу которой не дифференцировали
				}
				B = Temp * B;
			}
			else
			{
				int num = Begin.GetInd(str);
				if (num >= 0)
				{
					double d = (End.GetCoord(num) - Begin.GetCoord(num)) * t;
					Ax->CreateMoveMatrixSt(Temp, StMatr, d);
					B = Temp * B;
				}
			}
			str = Ax->GetParent();//Взяли имя родителя
		}
		Ax = GetStockAxis(StockPosition);
		if (Ax == nullptr)
			return false;
		str = Ax->GetParent();
		
		while(str != GeneralAxis && Ax != nullptr)//По ветви заготовки. Пока ось != общему узлу двух ветвей
		{
			Ax = GetCurConfig().GetAxis(str);//берем указатель на родителя, т.е. можно взять указатель в случае Name != "";
			if (Ax == nullptr)

				return false;
			if(str == NameAxis)//Если ось управляется xi
			{
				if (f == false)//Если производную в данном слагаемом ещё не брали
				{
					if (k == 0)
					{
						Ax->CreateDerivInvrMoveMatrix(Temp, StMatr, val, dval);
						kol++;
						f = true;
					}
					else
					{
						if (k < kol)
						{
							Ax->CreateInvrMoveMatrix(Temp, StMatr, val);
							k++;
						}
						else
						{
							Ax->CreateDerivInvrMoveMatrix(Temp, StMatr, val, dval);
							kol++;
							f = true;
						}
					}
				}
				else
				{
					Ax->CreateInvrMoveMatrix(Temp, StMatr, val);
					h = true;//Есть управляемая ось, матрицу которой не дифференцировали
				}
				A = A * Temp;
			}
			else
			{
				int num = Begin.GetInd(str);
				if (num >= 0)
				{
					double d = (End.GetCoord(num) - Begin.GetCoord(num)) * t;
					Ax->CreateInvrMoveMatrix(Temp, StMatr, d);
					A = A * Temp;
				}
			}
			str = Ax->GetParent();//Взяли имя родителя
		}
		if (kol == 0)//Если ошибка дерева и нет осей управляемых xi в стойке
			return true;
		S = S + (B * A);//Добавили очередное слагаемое
	} while (h == true);//Пока есть ещё управляемые xi оси, которые не продифференцировали

	M = S; //Если все управляемые xi оси продифференцировали
	return true;
}
bool MTMachineTool::TotalDerivative(BMatr& M, const SMachState& Begin, const SMachState& End, double t, int StPos, int Tur, int Att)
{
	BMatr StMatr = GetStockMatr(StPos, false);
	StMatr = StMatr.invr();
	return TotalDerivative1(M, StMatr, Begin, End, t, StPos, Tur, Att);
}
bool MTMachineTool::TotalDerivative1(BMatr& M, const BMatr& StMatr, const SMachState& Begin, const SMachState& End, double t, int StockPosition, int Tur, int Att)
{
	if (DataFit(Begin, End) != true)
		return false;
	/*Проверим все ли оси, которыми располагает стойка, имеются у станка*/
	//if(MashineVerif(begin) != true)
		//return false;


	bool f = true;
	BMatr S, PD;
	S.Set0();
	int kol = (int)Begin.GetSize() - 1;
	CString key;//Имя переменной, по которой дифференцируем
	for (int i = 0; i < kol; i++)
	{
		key = Begin.GetAName(i);
		if (Begin.GetCoord(i) != End.GetCoord(i))//Если значения координат изменились
		{
			f = PartialDerivative(PD, i, Begin, End, t, StMatr, StockPosition, Tur, Att);
			if (f)//Если частная производная есть
				S = S + PD;
			else
				return false;
		}
	}
	M = S;
	return true;
}
BPoint * MTMachineTool::PointsOfCurve(int RCPMask, const SMachState& Begin, const SMachState& End, const BPoint &Base, const BStockToolInds& STInds)// Base - base point in the tool coordinates system
{
	BPoint *P = new BPoint[4];
	SetState(MTLink, Begin);
	BMatr ToolM, StockM;
	GetToolAndStockMatr(ToolM, StockM, STInds.stPos_, STInds.turret_, STInds.attach_, false);
	BMatr M = StockM.Transpose(ToolM);
	P[0] = Base * M;

	BPoint V0(0., 0., 0., 0.);
	BPoint V1(0., 0., 0., 0.);
	if (RCPMask == 0)
	{
		StockM = StockM.invr();
		BMatr Mb;
		bool f = TotalDerivative1(Mb, StockM, Begin, End, 0.0, STInds.stPos_, STInds.turret_, STInds.attach_);//Выч. полн. произв. матрицы для начала кривой 
		if (!f)//если производная в начале кадра не посчиталась
			return NULL;
		V0 = P[0] * Mb;
	}


	SetState(MTLink, End);
	GetToolAndStockMatr(ToolM, StockM, STInds.stPos_, STInds.turret_, STInds.attach_, false);
	BMatr M1 = StockM.Transpose(ToolM);
	P[3] = Base * M1;

	if (RCPMask != 0)
	{
		SMachState Prev(Begin.GetDef());
		CalcIntState(RCPMask, -0.01, Begin, End, Prev, STInds.stPos_, STInds.turret_, STInds.attach_);
		SetState(MTLink, Prev);
		BMatr Mp = GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
		SMachState Next(Begin.GetDef());
		CalcIntState(RCPMask, 0.01, Begin, End, Next, STInds.stPos_, STInds.turret_, STInds.attach_);
		SetState(MTLink, Next);
		BMatr Mn = GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
		V0 = Base * (Mn - Mp);

		CalcIntState(RCPMask, 1. + 0.01, Begin, End, Prev, STInds.stPos_, STInds.turret_, STInds.attach_);
		SetState(MTLink, Prev);
		Mp = GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
		CalcIntState(RCPMask, 1. - 0.01, Begin, End, Next, STInds.stPos_, STInds.turret_, STInds.attach_);
		SetState(MTLink, Next);
		Mn = GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
		V1 = Base * (Mn - Mp);

	}
	else
	{
		StockM = StockM.invr();
		BMatr Me;
		bool f = TotalDerivative1(Me, StockM, End, Begin, 0.0, STInds.stPos_, STInds.turret_, STInds.attach_);//Выч. полн. произв. матрицы для конца кривой//??????????
		if (!f)//если производная в конце кадра не посчиталась
			return NULL;
		V1 = P[3] * Me * -1.;
	}



	BPoint P03 = P[3] - P[0];
	double t0 = P03 * V0;
	if(fabs(t0) < 1.e-8)
		t0 = 1.;
	else
		t0 = P03.D2() / (2. * t0);
	double t1 = P03 * V1;
	if(fabs(t1) < 1.e-8)
		t1 = 1.;
	else
		t1 = P03.D2() / (2. * t1);

	double h = 2. * (V0.D2() * t0 * t0 + V1.D2() * t1 * t1);
	if(h < 1.e-10)
		h = 1.;
			
	h = sqrt(P03.D2() / h);

	if(h < 1.e-5)
		h = 1.;
	P[1] = (P[0] + V0 * t0) * h;
	P[2] = (P[3] - V1 * t1) * h;
	return P;
}
BPoint* MTMachineTool::PointsOfSegments(int RCPMask, const SMachState& Begin, const SMachState& End, int& kol, const BPoint& Base, const BStockToolInds& STInds)// Base - base point in the tool coordinates system)
{
	BPoint* P = PointsOfCurve(RCPMask, Begin, End, Base, STInds);
	if (P == NULL)
		return NULL;
	BPoint P1 = P[1], P2 = P[2];
	P1.Norm();
	P2.Norm();
	if (P1.LineD2(P[0], P[3]) > EPS2 || P2.LineD2(P[0], P[3]) > EPS2)
	{
		//Найдем точки на настоящей кривой при t=1/3 и t=2/3
		SMachState temp(Begin.GetDef());

		CalcIntState(RCPMask, 1. / 3., Begin, End, temp, STInds.stPos_, STInds.turret_, STInds.attach_);
		SetStatePartial(MTLink, temp);
//		TempState(temp, begin, end, 1. / 3.);
//		Now.Copy(temp);
		GoToCurPos();
		BMatr M = GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
		P1 = BPoint(0., 0., 0., 1.) * M;

		CalcIntState(RCPMask, 2. / 3., Begin, End, temp, STInds.stPos_, STInds.turret_, STInds.attach_);
		SetStatePartial(MTLink, temp);
		//TempState(temp, begin, end, 2. / 3.);
		//Now.Copy(temp);
		GoToCurPos();
		M = GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);
		P2 = BPoint(0., 0., 0., 1.) * M;

		//Найдем точки на приближенной кривой
		NLine L;
		L.Set(P[0], P[1], P[2], P[3]);

		BPoint P11 = L.GetPointFromParam(1. / 3.);
		BPoint P22 = L.GetPointFromParam(2. / 3.);
		P11.Norm();
		P22.Norm();

		if ((P1 - P11).D2() > EPS2 || (P2 - P22).D2() > EPS2)
		{
			CalcIntState(RCPMask, 0.5, Begin, End, temp, STInds.stPos_, STInds.turret_, STInds.attach_);
			//bool f = TempState(temp, begin, end, 0.5);
			//if (!f)
			//	return NULL;
			int kola = 0, kolb = 0;
			BPoint* A, * B;
			A = PointsOfSegments(RCPMask, Begin, temp, kola, Base, STInds);
			B = PointsOfSegments(RCPMask, temp, End, kolb, Base, STInds);

			A = MergeOfPoints(A, B, kola, kolb);
			kol = kola + kolb - 1;

			delete[] B;
			delete[] P;
			return A;
		}
	}
	kol = 4;
	return P;
}
BPoint * MTMachineTool::MergeOfPoints(BPoint *A, BPoint *B, const int kola, const int kolb)
{
	BPoint *C = A;
	A = new BPoint [kola+kolb-1];
	for(int i = 0; i < kola; i++)
		A[i] = C[i];
	for(int i = kola, j = 1; i < kola + kolb - 1; i++, j++)
		A[i] = B[j];
	delete [] C;
	return A;
}
int MTMachineTool::kolRotaryAxis(int StockPosition, const SMachState& Begin, const SMachState& End, double& sumangle)
{	
	if(DataFit(Begin, End))
	{
		sumangle = 0;
		MTAxis *Ax = GetStockAxis(StockPosition);

		if(Ax == NULL)
			return -1;

		Ax->SetMarked(true);

		int kol = 0;
		CString Str;
		Str = Ax->GetParent();
		
		while(Str != "")
		{	
			Ax = GetCurConfig().GetAxis(Str);//Перешли к родителю
			Ax->SetMarked(true);
			if(Ax->GetType() == 2)//Если ось вращательная
			{
				kol++;
				double b = Begin.GetCoord(Ax->GetName());
				double e = Begin.GetCoord(Ax->GetName());
				sumangle += abs(e - b);
			}
			Str = Ax->GetParent();//Узнали имя родителя
		}
		Ax = GetToolAxis(0, 0);

		if(Ax == NULL)
			return -1;

		Ax->SetMarked(true);
		Str = Ax->GetParent();
		while(Str != "")
		{
			Ax = GetCurConfig().GetAxis(Str);//Перешли к родителю

			if(Ax->IsMarked() == true)
			{
				Str = Ax->GetParent();
				while(Str != "")
				{
					Ax = GetCurConfig().GetAxis(Str);//Перешли к родителю
					if(Ax->GetType() == 2)
						kol--;
					Str = Ax->GetParent();
				}
				break;
			}

			if(Ax->GetType() == 2)//Если ось вращательная
			{
				kol++;
				double b = Begin.GetCoord(Ax->GetName());
				double e = Begin.GetCoord(Ax->GetName());
				sumangle += abs(e - b);
			}
			Str = Ax->GetParent();//Узнали имя родителя
		}
		return kol;
	}
	return -1;
}
bool MTMachineTool::TempState(CooArray &temp, const CooArray &begin, const CooArray &end, double t)
{
	int kol = (int)begin.GetSize();
	if(kol != (int)end.GetSize())
		return false;
	for(int i = 0; i < kol; i++)
	{
		temp.GetAt(i).Value = begin.GetAt(i).Value * (1-t) + end.GetAt(i).Value * t;
		temp.GetAt(i).AxisName = begin.GetAt(i).AxisName;
	}
	return true;
}
BCurve * MTMachineTool::Make5xLine(int RCPMask, const NMachUnitPair* pMUPair, const BPoint &XYZ0, const BPoint &ABC0, const BPoint &XYZ1, const BPoint &ABC1, const BPoint &Base, const BStockToolInds& STInds)// Base - base point in the tool coordinates system)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	SMachState Begin = SMachState(XYZ0, ABC0, &pMUPair->GetUnit().GetFixedDef());
	SMachState End = SMachState(XYZ1, ABC1, &pMUPair->GetUnit().GetFixedDef());

	SetState(BMTLinkage().SetIndW1(), Begin);
	BPoint ToolAxisVec = BPoint(0., 0., 1., 0.) * GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);

	auto pRet = Cr5xLine(RCPMask, pMUPair->GetUnit().GetCurStockPosition(), Begin, End, Base, STInds);
	pRet->SetToolAxisVec(ToolAxisVec);
	return pRet;
}
BCurve * MTMachineTool::Make5xLine(int RCPMask, const class NCUnit& Unit, const NCUnitStatePrev & StartSt, const NCUnitStatePrev & EndSt, const BPoint &Base, const BStockToolInds& STInds)// Base - base point in the tool coordinates system)
{
	SMachStateFix1 FixStateSt = StartSt.GetMachStateFixComp();
	SMachState Begin = SMachState(FixStateSt, &Unit.GetFixedDef());

	SetState(BMTLinkage().SetIndW1(), Begin, StartSt.AddCoords);
	BPoint ToolAxisVec = BPoint(0., 0., 1., 0.) * GetStock2ToolMatr(STInds.stPos_, STInds.turret_, STInds.attach_);

	SMachStateFix1 FixStateEn = EndSt.GetMachStateFixComp();
	SMachState End = SMachState(FixStateEn, &Unit.GetFixedDef());
	SetState(BMTLinkage().SetIndW1(), End, EndSt.AddCoords);

	auto pRet = Cr5xLine(RCPMask, Unit.GetCurStockPosition(), Begin, End, Base, STInds);
	if(pRet != nullptr)
		pRet->SetToolAxisVec(ToolAxisVec);
	return pRet;
}

BCurve * MTMachineTool::Cr5xLine(int RCPMask, int StockPosition, const SMachState& Begin, const SMachState& End, const BPoint &Base, const BStockToolInds& STInds)// Base - base point in the tool coordinates system))
{
	bool UseRCP = (RCPMask != 0);
	double sumangle = 0;
	int n = 0;
	const int kol = kolRotaryAxis(STInds.stPos_, Begin, End, sumangle);
	if(kol <= 0 || UseRCP)
		n = 1;
	else
	{
		//Находим требуемое количество сегментов для кадра
		const double a = sumangle / 90.;
		if(a != 0.)
		{
			if((a - (int)a) != 0.)
				n = (int)a + 1;
			else
				n = (int)a;
		}
		else
			n = 1;
	}

	SMachState temp1(Begin.GetDef()), temp2(Begin.GetDef());

	int k1 = 0, k2 = 0;

	CalcIntState(RCPMask, 1. / n, Begin, End, temp1, STInds.stPos_, STInds.turret_, STInds.attach_);
//	TempState(temp1, Begin, End, 1. / n);

	BPoint* A = PointsOfSegments(RCPMask, Begin, temp1, k1, Base, STInds);
	if(A == nullptr)
		return nullptr;

	for(int i = 1; i < n-1; i++)
	{
		CalcIntState(RCPMask, double(i + 1) / n, Begin, End, temp2, STInds.stPos_, STInds.turret_, STInds.attach_);
//		TempState(temp2, Begin, End, double(i + 1) / n);

		BPoint* B = PointsOfSegments(RCPMask, temp1, temp2, k2, Base, STInds);
		if(B == nullptr)
			return nullptr;

		A = MergeOfPoints(A, B, k1, k2);
		k1 = k1 + k2 - 1;
		delete[] B;

		temp1 = temp2;
	}

	if(n > 1)
	{
		BPoint* B = PointsOfSegments(RCPMask, temp1, End, k2, Base, STInds);
		if(B == nullptr)
			return nullptr;
		A = MergeOfPoints(A, B, k1, k2);
		k1 = k1 + k2 - 1;
		delete[] B;
	}

	BCurve *pCurve = BCurve::NewBCurve((k1 - 1) / 3);
	for(int i = 0; i < k1; i++)
		pCurve->SetPoint(i, A[i]);
	delete[] A;
	return pCurve;
}

CCriticalSection & MTMachineTool::GetCriticalSection(void)
{
	return CritSection;
}

void MTMachineTool::GetRealState(SMachState & State, SCoordsDef &CDef)
{
	MakeActual();
	CString InitStr = GetCurConfig().CrInitStr();
	CDef.Define(InitStr);
	State = SMachState(&CDef);
	for(int i = 0; i < State.GetSize() - 1; ++i)
	{
		MTAxis *pAxis = GetCurConfig().GetAxis(State.GetAName(i));
		if(!pAxis)
			continue;
		State.SetCoord(i, pAxis->GetMovement());
	}
}
SMachState MTMachineTool::GetFixState(const SCoordsDef* pFixedNames)
{
	SMachState Result(pFixedNames);
	if (pFixedNames == nullptr)
		return Result;
	MakeActual();
	for (int i = 0; i < pFixedNames->GetSize(); ++i)
	{
		const MTAxis* pAxis = GetCurConfig().GetAxis(pFixedNames->GetName(i));
		if (pAxis != nullptr)
			Result.SetCoord(i, pAxis->GetMovement());
		else
			Result.SetCoord(i, 0.);
	}
	return Result;
}

int MTMachineTool::CalculateTiltPlane(class NTiltPlane &TiltPlane, const SCoordsDef& FixedDef, int StockPosition)
{
	TiltPlane.FixState = GetFixState(&FixedDef);
	TiltPlane.FixState.ClearValues();
	SMachState Original(TiltPlane.MTState);
	Original.ReplaceCoord(TiltPlane.FixState);
	TiltPlane.FixState.ReplaceCoord(TiltPlane.MTState);
	SetState(BMTLinkage().SetIndW1(), TiltPlane.FixState);
	MakeActual();
	bool CanRotate = true;
	// Find first and second axis in the chain from stock to tool
	MTAxis *pAxTool = GetToolAxis(TiltPlane.Turret, TiltPlane.Attach);
	MTAxis *pAxStock = GetStockAxis(StockPosition);
	if(!pAxTool || !pAxStock)
		return -1;
	MTAxis *pAx1 = GetCurConfig().GetAxis(TiltPlane.MTState.GetAName(0));
	MTAxis *pAx2 = GetCurConfig().GetAxis(TiltPlane.MTState.GetAName(1));
	if(!pAx1 || !pAx2)
		CanRotate = false;
	else
	{
		if(pAx1->GetType() != 2 || pAx2->GetType() != 2)
			CanRotate = false;
	}

	int Res = 1;
	double Angles[4] = {0., 0., 0., 0.};
	int DecNum = 0;
	double rA = 0., rB = 0., rC = 0.;// stock rotating coordinates
	if(CanRotate)
	{
		bool Stock1 = GetCurConfig().IsDescendant(*pAx1, *pAxStock);
		bool Stock2 = GetCurConfig().IsDescendant(*pAx2, *pAxStock);
		bool Tool1 = GetCurConfig().IsDescendant(*pAx1, *pAxTool);
		bool Tool2 = GetCurConfig().IsDescendant(*pAx2, *pAxTool);
		if(!(Stock1 || Tool1) && !(Stock2 || Tool2))
			CanRotate = false;
		if(CanRotate)
		{
			BPoint V1(0., 0., 0., -1.);
			BPoint V2(0., 0., 0., -1.);
			int FirstInd = -1;// The index of the axis nearest to the tool
			if(Stock1 && Stock2)
			{
				if(GetCurConfig().IsDescendant(*pAx2, *pAx1))
				{
					FirstInd = 1;
					V1 = pAx1->GetVector() * (-1.);
					V2 = pAx2->GetVector() * (-1.);
				} 
				else
				{
					FirstInd = 0;
					V1 = pAx2->GetVector() * (-1.);
					V2 = pAx1->GetVector() * (-1.);
				}
			}
			else if(Stock1 || Stock2)
			{
				if(Stock1)
				{
					FirstInd = 1;
					V1 = pAx1->GetVector() * (-1.);
					V2 = pAx2->GetVector();
				}
				else
				{
					FirstInd = 0;
					V1 = pAx2->GetVector() * (-1.);
					V2 = pAx1->GetVector();
				}
			}
			else
			{
				if(GetCurConfig().IsDescendant(*pAx2, *pAx1))
				{
					FirstInd = 0;
					V1 = pAx2->GetVector();
					V2 = pAx1->GetVector();
				}
				else
				{
					FirstInd = 1;
					V1 = pAx1->GetVector();
					V2 = pAx2->GetVector();
				}
			}
			BPoint ToolV = pAxTool->GetVector() % pAxTool->GetVectorY();
			BPoint NewToolV = ToolV * TiltPlane.NewSys * GetStockMatr(StockPosition, true);
			Res = NTiltPlane::Calc2AnglesPairs(V1, V2, ToolV, NewToolV, pAxTool->GetVector(), pAxTool->GetVector() * TiltPlane.NewSys * GetStockMatr(0, true), Angles);
			if(Res == 0)
			{
				Res = 1;
			}
			else
			{	
				// Set rotation coords
				if(Res > 1)
				{
					if(TiltPlane.Decision == 2)
						DecNum = 2;
					else if (TiltPlane.Decision == 0 || TiltPlane.Decision == 4)
					{
						double d1 = 0., d2 = 0;
						for(int i = 0; i < 2; ++i)
						{
							double Val = Original.GetCoord((i == 0) ? FirstInd : (FirstInd + 1) % 2);
							d1 += (Angles[i] - Val) * (Angles[i] - Val);
							d2 += (Angles[i + 2] - Val) * (Angles[i + 2] - Val);
						}
						if(TiltPlane.Decision == 0)
							DecNum = d1 > d2 ? 2 : 0;
						else
							DecNum = d1 > d2 ? 0 : 2;
					}
					else if (TiltPlane.Decision == 5 || TiltPlane.Decision == 6)
					{	// 5 - main angle > 0; 6 - main angle < 0 (main angle - angle for the rotation axis nearest to the tool axis)
						bool IsPositive1 = (Angles[FirstInd] >= 0.);
						bool IsPositive2 = (Angles[FirstInd + 2] >= 0.);
						if (TiltPlane.Decision == 5)
						{
							if (IsPositive1)
								DecNum = 0;
							else if (IsPositive2)
								DecNum = 2;
						}
						else // TiltPlane.Decision == 6
						{
							if (IsPositive1)
								DecNum = 2;
							else if (IsPositive2)
								DecNum = 0;
						}
					}
				}

				TiltPlane.MTState.SetCoord(FirstInd, Angles[0 + DecNum]);
				TiltPlane.MTState.SetCoord((FirstInd + 1) % 2, Angles[1 + DecNum]);
			}

		}
		if (Stock1)
		{// First coordinate rotates stock
			if (TiltPlane.MTState.GetAName(0) == _T("A"))
				rA = TiltPlane.MTState.GetCoord(0);
			else if (TiltPlane.MTState.GetAName(0) == _T("B"))
				rB = TiltPlane.MTState.GetCoord(0);
			else if (TiltPlane.MTState.GetAName(0) == _T("C"))
				rC = TiltPlane.MTState.GetCoord(0);
		}
		if (Stock2)
		{// second coordinate rotates stock
			if (TiltPlane.MTState.GetAName(1) == _T("A"))
				rA = TiltPlane.MTState.GetCoord(1);
			else if (TiltPlane.MTState.GetAName(1) == _T("B"))
				rB = TiltPlane.MTState.GetCoord(1);
			else if (TiltPlane.MTState.GetAName(1) == _T("C"))
				rC = TiltPlane.MTState.GetCoord(1);
		}
	}

	TiltPlane.FixState.ReplaceCoord(TiltPlane.MTState);
	TiltPlane.FixState.SetCoord(FixedDef.GetName(0), 0.);
	TiltPlane.FixState.SetCoord(FixedDef.GetName(1), 0.);
	TiltPlane.FixState.SetCoord(FixedDef.GetName(2), 0.);
	SetState(BMTLinkage().SetIndW1(), TiltPlane.FixState);
	BPoint TVect = BPoint(0., 0., 0., 1) - BPoint(0., 0., 0., 1) * GetToolMatr(StockPosition, TiltPlane.Turret, TiltPlane.Attach);
	BPoint XYZ = GetVectXYZ(TVect, FixedDef, TiltPlane.Turret, TiltPlane.Attach);
	TiltPlane.FixState.SetCoord(FixedDef.GetName(0), XYZ.GetX());
	TiltPlane.FixState.SetCoord(FixedDef.GetName(1), XYZ.GetY());
	TiltPlane.FixState.SetCoord(FixedDef.GetName(2), XYZ.GetZ());

	SetState(BMTLinkage().SetIndW1(), SMachState(BPoint(0., 0., 0., 1.), BPoint(rA, rB, rC, 1.), TiltPlane.FixState.GetDef()));
	TiltPlane.CSMatr = GetStock2ToolMatr(StockPosition, TiltPlane.Turret, TiltPlane.Attach);
	TiltPlane.CSMatr = TiltPlane.NewSys * TiltPlane.CSMatr.invr() * BMatr().Trans(XYZ);

	return Res;
}

void MTMachineTool::AddPart(MTPart * pPart)
{
	pPart->SetEdgesAttrs(EdgesAttrs);
	Part.AddPart(pPart);
	MTBase::Reset();
}

void MTMachineTool::RemoveAll()
{
	MTMachine::RemoveAll();
	Collisions.RemoveAll();
}

bool MTMachineTool::RemoveBranch(int AxisIndex)
{
	if (Configs.empty())
		return false;
	auto& Conf = Configs[0];
	if (AxisIndex < 0)
		return false;
	const int level = Conf.Axis[AxisIndex]->level;
	// Check collisions
	// don't remove if present in collisions
	bool PresentInCollisions = false;
	for (int i = AxisIndex + 1; i < Conf.Axis.GetSize() && !PresentInCollisions; i++)
	{
		if (Conf.Axis[i]->level <= level)
			break;
		for (int j = 0; j < Collisions.GetSize() && !PresentInCollisions; j++)
		{
			if (Collisions[j]->Axis1 == Conf.Axis[i]->Axis->GetName() || Collisions[j]->Axis2 == Conf.Axis[i]->Axis->GetName())
				PresentInCollisions = true;
		}//for
	}//for
	for (int j = 0; j < Collisions.GetSize() && !PresentInCollisions; j++)
		if (Collisions[j]->Axis1 == Conf.Axis[AxisIndex]->Axis->GetName() || Collisions[j]->Axis2 == Conf.Axis[AxisIndex]->Axis->GetName())
			PresentInCollisions = true;
	if (PresentInCollisions)
	{
		AfxMessageBox(IDS_MT_NOTCOLLISIONAXIS);
		return false;
	}
	//1 удаляем детали
	for(int i = AxisIndex+1; i < Conf.Axis.GetSize(); i++)
	{
		if(Conf.Axis[i]->level <= level)
			break;
		for(int j = 0; j < Part.GetSize(); j++ )
		{
			if(Part[j]->GetAxisName() == Conf.Axis[i]->Axis->GetName())
			{
				Part.RemovePart(j);
			}//if
		}//for
	}//for
	
	for(int j = 0; j < Part.GetSize(); j++)
		if(Part[j]->GetAxisName() == GetCurConfig().Axis[AxisIndex]->Axis->GetName())
			Part.RemovePart(j);
	MTBase::Reset();

	//3 удаляем компенсацию
	if(Conf.Axis[AxisIndex]->Axis->GetType() == MTAxis::AT_STOCK) //stock
		if ( Conf.IsCompStock(AxisIndex))
		{
			Conf.DeleteComp(AxisIndex);
			return MTBase::RemoveBranch(AxisIndex - 3);
		}

	//4 удаляем оси
	return MTBase::RemoveBranch(AxisIndex);
}

void MTMachineTool::RemoveCollision(const CString & Axis1, const CString & Axis2)
{
	for(int i = 0; i < Collisions.GetSize(); i++ )
		if(Collisions[i]->Axis1 == Axis1 && Collisions[i]->Axis2 == Axis2)
			Collisions.RemoveCol(i);
	GetCurConfig().ReGenEnemiesPairs(Collisions);
}//void RemoveCollision(CString Axis1, CString Axis2);

void MTMachineTool::RenameInCollisions(const CString & OldAxisName, const CString & NewAxisName)
{
	for( int i = 0; i < Collisions.GetSize(); i++)
	{
		if( Collisions[i]->Axis1 == OldAxisName)
			Collisions[i]->Axis1 = NewAxisName;
		if( Collisions[i]->Axis2 == OldAxisName)
			Collisions[i]->Axis2 = NewAxisName;
	}//for
}//void MTMachineTool::RenameInCollisions(CString OldAxisName, CString NewAxisName)
	
#ifdef _DEBUG
void MTMachineTool::DebugPrint(const CString& label, bool parents, bool level)
{
	//CString Name;
	//FILE *fLog = fopen(LOG_FILE,"at");
	//CString indent = "  ";

	//fprintf(fLog,"\n%s\n",label);
	//fprintf(fLog,"================================================\n");
	//fprintf(fLog,"Size: %d    Axis.GetSize(): %d\n",Size, Axis.GetSize());
	//fprintf(fLog,"Collisions.GetSize(): %d \n",Collisions.GetSize());
	//fprintf(fLog,"Part.GetSize(): %d \n",Part.GetSize());

	//for(int i = 0; i < Size; i++)
	//{
	//	Name = GetCurConfig().Axis[i]->Axis->GetName();
	//	for(int j =0; j < GetLevel(i); j++)
	//	{
	//		fprintf(fLog,"%s", indent.GetBuffer());
	//	}//for
	//	fprintf(fLog,"%s",Name.GetBuffer());
	//	if(parents)
	//		fprintf(fLog," (%s)", GetCurConfig().Axis[i]->Axis->GetParent().GetBuffer());
	//	if(level)
	//		fprintf(fLog,"   %d", GetLevel(i));
	//	for( int j = 0; j < Part.GetSize(); j++)
	//		if(Part[j]->GetAxisName() == GetCurConfig().Axis[i]->Axis->GetName())
	//			fprintf(fLog, "    %s", Part[j]->GetFile().GetBuffer());

	//	fprintf(fLog,"\n");
	//}//for
	//fprintf(fLog,"\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

	//fclose(fLog);

}
#endif

//bool MTMachineTool::ChangeToolTurret(int oldTurret, int newTurret)
//{
//	for (int i = 0; i < GetCurConfig().GetSize(); i++)
//	{
//		if (GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_TOOL)
//		{
//			if (GetCurConfig().Axis[i]->Axis->GetTurret() == oldTurret)
//			{
//				// 1 - сохраняем имя для переименовывания объектов столкновения 
//				CString OldName = GetCurConfig().Axis[i]->Axis->GetName();
//
//				// 2 - переименовываем саму ось
//				GetCurConfig().Axis[i]->Axis->SetTurret(newTurret);
//
//				// 3 - переименовываем связанные объекты столкновения
//				Collisions.RenameAxis(OldName, GetCurConfig().Axis[i]->Axis->GetName());
//			}//if
//		}//if
//	}
//	return true;
//}
//
//bool MTMachineTool::ChangeVirtToolTurret(int oldTurret, int newTurret)
//{
//	for(int i = 0; i < GetCurConfig().GetSize(); i++)
//		if(GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL)
//		{
//			if(GetCurConfig().Axis[i]->Axis->GetTurret() == oldTurret)
//			{
//				// 1 - сохраняем имя для переименовывания объектов столкновения 
//				CString OldName = GetCurConfig().Axis[i]->Axis->GetName();
//
//				GetCurConfig().Axis[i]->Axis->SetTurret(newTurret);
//
//				// 3 - переименовываем связанные объекты столкновения
//				Collisions.RenameAxis(OldName, GetCurConfig().Axis[i]->Axis->GetName());
//			}//if
//		}//if
//	//ReGenEnemiesPairs();
//	return true;
//}//bool MTMachineTool::ChangeVirtToolTurret(int oldTurret, int newTurret)

bool MTMachineTool::ChangeToolTurret(int AxisIndex, int newTurret)
{
	ASSERT(GetCurConfig().Axis[AxisIndex]->Axis->GetType() == MTAxis::AT_TOOL);

	CString OldName = GetCurConfig().Axis[AxisIndex]->Axis->GetName();
	GetCurConfig().Axis[AxisIndex]->Axis->SetTurret(newTurret);

	Collisions.RenameAxis(OldName, GetCurConfig().Axis[AxisIndex]->Axis->GetName());
	return true;
}

bool MTMachineTool::ChangeVirtToolTurret(int AxisIndex, int newTurret)
{
	ASSERT(GetCurConfig().Axis[AxisIndex]->Axis->GetType() == MTAxis::AT_VTOOL);

	CString OldName = GetCurConfig().Axis[AxisIndex]->Axis->GetName();
	GetCurConfig().Axis[AxisIndex]->Axis->SetTurret(newTurret);

	Collisions.RenameAxis(OldName, GetCurConfig().Axis[AxisIndex]->Axis->GetName());
	return true;
}

bool MTMachineTool::ExistsToolTurAtt(int Turret, int Attachment)
{
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if(GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_TOOL)
		{
			if( GetCurConfig().Axis[i]->Axis->GetTurret() == Turret && GetCurConfig().Axis[i]->Axis->GetAttachment() == Attachment)
			{
				return true;
			}//if
		}//if
	}//for
	return false;
}

int MTMachineTool::GetToolTurOnLevel(const CString & ParentName)
{
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if(GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_TOOL) //tool
		{
			if( GetCurConfig().Axis[i]->Axis->GetParent() == ParentName)
				return GetCurConfig().Axis[i]->Axis->GetTurret();
		}//if
	}//for
	return -1;
}

bool MTMachineTool::ExistsVirtToolPosTur(int Position, int Turret) 
{
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if(GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL)
		{
			if( GetCurConfig().Axis[i]->Axis->GetPosition() == Position && GetCurConfig().Axis[i]->Axis->GetTurret() == Turret)
			{
				return true;
			}//if
		}//if
	}//for
	return false;
}

int MTMachineTool::GetNextAttachment(int Turret)
{
	int maxattachment = -1;
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if( GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_TOOL) //tool
			if( GetCurConfig().Axis[i]->Axis->GetTurret() == Turret)
			{
				if( GetCurConfig().Axis[i]->Axis->GetAttachment() > maxattachment)
					maxattachment = GetCurConfig().Axis[i]->Axis->GetAttachment();
			}//if
	}//for
	if(maxattachment == -1)
		return 0;
	else
		return maxattachment+1;
}

int MTMachineTool::GetNextTurret()
{
	int maxturret = -1;
	for(int i =0; i < GetCurConfig().GetSize(); i++)
	{
		if( GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_TOOL) //tool
			if( GetCurConfig().Axis[i]->Axis->GetTurret() > maxturret)
				maxturret = GetCurConfig().Axis[i]->Axis->GetTurret();
	}//for
	if(maxturret == -1)
		return 0;
	else
		return maxturret + 1;
}

int MTMachineTool::GetVirtToolTurOnLevel(const CString & ParentName)
{
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if(GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL) //virt tool
		{
			if( GetCurConfig().Axis[i]->Axis->GetParent() == ParentName)
				return GetCurConfig().Axis[i]->Axis->GetTurret();
		}//if
	}//for
	return -1;
}

int MTMachineTool::GetNextPositionVirtTool(int Turret)
{
	int maxpostion = -1;
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if( GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL) //virt tool
			if( GetCurConfig().Axis[i]->Axis->GetTurret() == Turret)
			{
				if( GetCurConfig().Axis[i]->Axis->GetPosition() > maxpostion)
					maxpostion = GetCurConfig().Axis[i]->Axis->GetPosition();
			}//if
	}//for
	if(maxpostion == -1)
		return 1;
	else
		return maxpostion+1;
}

int MTMachineTool::GetNextTurretVirtTool()
{
	int maxturret = -1;
	for(int i =0; i < GetCurConfig().GetSize(); i++)
	{
		if( GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL) //virt tool
			if( GetCurConfig().Axis[i]->Axis->GetTurret() > maxturret)
				maxturret = GetCurConfig().Axis[i]->Axis->GetTurret();
	}//for
	if(maxturret == -1)
		return 0;
	else
		return maxturret + 1;
}

int MTMachineTool::GetTurretToolAmount(int Turret)
{
	int amount = 0;
	for (int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if( GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_TOOL) //tool
		{
			if (GetCurConfig().Axis[i]->Axis->GetTurret() == Turret)
				amount++;
		}//if
	}//for
	return amount;
}

int MTMachineTool::GetTurretVirtToolAmount(int Turret)
{
	int amount = 0;
	for (int i = 0; i < GetCurConfig().Axis.GetSize(); i++)
	{
		if( GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL) //virt tool
		{
			if (GetCurConfig().Axis[i]->Axis->GetTurret() == Turret)
				amount++;
		}//if
	}//for
	return amount;
}

void MTMachineTool::ChangeToolAttachment(int AxisIndex, int newAttachment)
{
	ASSERT(GetCurConfig().Axis[AxisIndex]->Axis->GetType() == MTAxis::AT_TOOL);

	CString OldName = GetCurConfig().Axis[AxisIndex]->Axis->GetName();
	GetCurConfig().Axis[AxisIndex]->Axis->SetAttachment(newAttachment);

	Collisions.RenameAxis(OldName, GetCurConfig().Axis[AxisIndex]->Axis->GetName());

}

void MTMachineTool::ChangeVirtToolPosition(int AxisIndex, int NewPosition)
{
	ASSERT(GetCurConfig().Axis[AxisIndex]->Axis->GetType() == MTAxis::AT_VTOOL);

	CString OldName = GetCurConfig().Axis[AxisIndex]->Axis->GetName();
	GetCurConfig().Axis[AxisIndex]->Axis->SetPosition(NewPosition);

	Collisions.RenameAxis(OldName,GetCurConfig().Axis[AxisIndex]->Axis->GetName()); 

}

void MTMachineTool::ChangeVirtToolAttachment(int AxisIndex, int newAttachment)
{
	ASSERT(GetCurConfig().Axis[AxisIndex]->Axis->GetType() == MTAxis::AT_VTOOL);

	CString OldName = GetCurConfig().Axis[AxisIndex]->Axis->GetName();
	GetCurConfig().Axis[AxisIndex]->Axis->SetAttachment(newAttachment);

	Collisions.RenameAxis(OldName, GetCurConfig().Axis[AxisIndex]->Axis->GetName());

}

const BPoint MTMachineTool::RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N, RPickID& PickID) const
{
	PickID.Set0();
	if(!IsVisible())
		return BPoint(0., 0., 0., -1.);
	return Part.RayCasting(P, V, MinW, N, PickID);
}

bool MTMachineTool::FindSegmentsWithStl(SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr)
{
    if(!IsVisible())
		return false;
	MakeActual();
    return Part.FindSegmentsWithStl(oSegments, oMatrsInvr);
}

void MTMachineTool::SetEdgesColor(const NColor & Col)
{
	EdgesAttrs.SetEdgesColor(Col);
	for(int i = 0; i < Part.GetSize(); ++i)
		Part[i]->SetEdgesAttrs(EdgesAttrs);

}

void MTMachineTool::SetEdgesWidth(double Width)
{
	EdgesAttrs.SetEdgesWidth(Width);
	for(int i = 0; i < Part.GetSize(); ++i)
		Part[i]->SetEdgesAttrs(EdgesAttrs);
}

void MTMachineTool::SetSmoothAngle(double Angle)
{
	EdgesAttrs.SetSmoothAngle(Angle);
	for(int i = 0; i < Part.GetSize(); ++i)
		Part[i]->SetEdgesAttrs(EdgesAttrs);
}

void MTMachineTool::SetTransparency(double Transp)
{
	Transparency = Transp;
}

PropList * MTMachineTool::Create(PropList * List)
{
	List->Clear();
	// Visibility
	PropSingle * h = new PropSingle();
	h->ID=1;
	h->Name=IDS_VISIBILITY;
	h->Type=1;
	h->Comment=IDS_COMM_MACH;
	h->Group=IDS_PROP_MACH;
	h->ST_Value= (Visible ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Edges
	h = new PropSingle();
	h->ID=2;
	h->Name=IDS_EDGE;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_EDGE;
	h->Group=IDS_PROP_MACH;
	h->ST_Value = (DrawSharpEdges ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Edges color
	h = new PropSingle();
	h->ID=3;
	h->Name=IDS_EDGE_COLOR;
	h->Type=3;
	h->Comment=IDS_COMM_MACH_COLOR;
	h->Group=IDS_PROP_MACH;
	h->V_Value=GetEdgesAttrs().GetEdgesColor().GetRGB();
	List->Add(h);
	// Edges thickness
	h = new PropSingle();
	h->ID=4;
	h->Name=IDS_EDGE_THIKNESS;
	h->Type=4;
	h->Comment=IDS_COMM_MACH_THICK;
	h->Group=IDS_PROP_MACH;
	h->Min=1.;
	h->Max=10.;
	h->V_Value = GetEdgesAttrs().GetEdgesWidth();
	List->Add(h);
	// Render mode
	h = new PropSingle();
	h->ID=5;
	h->Name=IDS_DISPLAY_MODE;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_RENDER;
	h->Group=IDS_PROP_MACH;
	h->ST_Value = GetRMode();
	h->For_Enum.Add(IDS_RM_SHADE);
//	h->For_Enum.Add(IDS_RM_WFRAME);
	h->For_Enum.Add(IDS_RM_GLITTER);
	h->For_Enum.Add(IDS_RM_TRANSLUCENT);
	h->For_Enum.Add(IDS_RM_UNDEF);
	h->Color = 1;
	List->Add(h);
	// Transparency
	h = new PropSingle();
	h->ID=6;
	h->Name=IDS_TRANSPARENCY_DEGREE;
	h->Type=5;
	h->Comment=IDS_COMM_MACH_TRANSP;
	h->Group=IDS_PROP_MACH;
	h->Max=100;
	h->Min=0;
	h->V_Value=int((1. - GetTransparency()) * 100. + 0.5);
	List->Add(h);
	// Smoothing
	h = new PropSingle();
	h->ID=7;
	h->Name=IDS_SMOOTHING;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_SMOOTH;
	h->Group=IDS_PROP_MACH;
	h->ST_Value = (DrawSmoothSurf ? IDS_MIX : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_MIX);
	List->Add(h);
	// Smooth angle
	h = new PropSingle();
	h->ID=8;
	h->Name=IDS_SMOOTHING_ANGLE;
	h->Type=4;
	h->Comment=IDS_COMM_MACH_SMOOTHANGL;
	h->Group=IDS_PROP_MACH;
	h->V_Value = GetEdgesAttrs().GetSmoothAngle();
	h->Max=180;
	h->Min=0;
	List->Add(h);
	// Tools
	h = new PropSingle();
	h->ID=9;
	h->Name=IDS_MACH_VTOOLS;
	h->Type=1;
	h->Comment=IDS_COMM_MACH_VTOOLS;
	h->Group=IDS_PROP_MACH;
	h->ST_Value = (GetDrawAllTools() ? IDS_TRUE : IDS_FALSE);
	h->For_Enum.Add(IDS_FALSE);
	h->For_Enum.Add(IDS_TRUE);
	List->Add(h);
	return List;
}
PropList * MTMachineTool::Change(PropList * List,PropSingle * Single, int id, enum UpdateModes &AdditionalModes, int &Group)
{
	AdditionalModes = N_EMPTY;
	Group = OG_GR_EMPTY;
	switch(id)
	{
	case 1: // IDS_VISIBILITY
		Visible = (Single->ST_Value == IDS_MIX);
		break;
	case 2: // Edges
		DrawSharpEdges = (Single->ST_Value == IDS_MIX);
		break;
	case 3: // Color
		{
			NColor Col;
			Col.SetRGB(COLORREF(Single->V_Value));
			SetEdgesColor(Col);
			break;
		}
	case 4: // Edges thickness
		SetEdgesWidth(Single->V_Value);
		break;
	case 5: // Render mode
		SetRMode(RenderMode(Single->ST_Value));
		break;
	case 6: // Transparency
		SetTransparency(1. - Single->V_Value / 100);
		break;
	case 7: // Smoothing
		DrawSmoothSurf = (Single->ST_Value == IDS_MIX);
		break;
	case 8: // Smooth angle
		SetSmoothAngle(Single->V_Value);
		break;
	case 9: // Edges
		SetDrawAllTools(Single->ST_Value == IDS_TRUE);
		break;
	}
	return 	Create(List);
}

void MTMachineTool::FillPartsPropList(void)
{
	for(int ip = 0; ip < Part.GetSize(); ++ip)
	{
		PartsPropList[Part[ip]->GetAxisName()] = Part[ip]->GetPartProp();
	}
}

void MTMachineTool::ApplyPartsPropList(void)
{
	for(int ip = 0; ip < Part.GetSize(); ++ip)
	{
		MTPartVisProp *pVisProp = NULL;
		if(PartsPropList.Lookup(Part[ip]->GetAxisName(), pVisProp))
			Part[ip]->SetPartProp(pVisProp);
	}
	ClearPartsPropList();
}

void MTMachineTool::ClearPartsPropList(void)
{
	for(POSITION pos = PartsPropList.GetStartPosition(); pos != NULL;)
	{
		CString Key;
		MTPartVisProp *pVisProp = NULL;
		PartsPropList.GetNextAssoc(pos, Key, pVisProp);
		delete pVisProp;
	}
	PartsPropList.RemoveAll();
}

bool MTMachineTool::HasTranslucent()
{
	if(RMode == RM_TRANSLUCENT)
		return true;

	if(RMode == RM_UNDEF)
		return Part.HasTranslucent();

	return false;
}

bool MTMachineTool::IsVirtToolUnique(int AxisIndex, int Position, int Attach, int OldTurret, int NewTurret)
{
	for(int i = 0; i < GetCurConfig().GetSize(); i++)
	{
		if(GetCurConfig().Axis[i]->Axis->GetType() == MTAxis::AT_VTOOL && i != AxisIndex)
		{
			//if( OldTurret != NewTurret && GetCurConfig().Axis[i]->Axis->GetTurret() == NewTurret)
			//{
			//	return false;
			//}//if
			if( GetCurConfig().Axis[i]->Axis->GetPosition() == Position && GetCurConfig().Axis[i]->Axis->GetAttachment() == Attach
				&& (GetCurConfig().Axis[i]->Axis->GetTurret() == OldTurret || GetCurConfig().Axis[i]->Axis->GetTurret() == NewTurret))
			{
				return false;
			}//if
		}//if
	}//for
	return true;
}

int MTMachineTool::MakeScrewMotion(int RCPMask, BMTLinkage ConfNum, const BPoint &XYZ0, const BPoint &ABC0, const BPoint &XYZ1, const BPoint &ABC1
	, std::vector <class BScrewMotion> &SMotion, int StockPosition, int Turret, int Attachment, const SCoordsDef& FixedDef)
{
	BPoint Angles = ABC1 - ABC0;
	const SMachState State0(XYZ0, ABC0, &FixedDef);
	const SMachState State1(XYZ1, ABC1, &FixedDef);
	return MakeScrewMotion(RCPMask, ConfNum, State0, State1, SMotion, StockPosition, Turret, Attachment, Angles);
}

int MTMachineTool::MakeScrewMotion(int RCPMask, BMTLinkage ConfNum, const SMachState& State0, const SMachState& State1
	, std::vector<class BScrewMotion>& SMotion, int StockPosition, int Turret, int Attachment, const BPoint& Angles)
{
	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	CooArray Buf = Now;
	double SumAngle = fabs(Angles.GetX()) + fabs(Angles.GetY()) + fabs(Angles.GetZ());
	double Tol = NSurfSettings::GetTolerance();
	SetState(ConfNum, State0);
	BMatr M0 = GetStock2ToolMatr(StockPosition, Turret, Attachment);
	bool IsNotSpecialCase = false;
	BScrewMotion Motion;
	double Tau = 1.;
	for (; Tau > 0.001; Tau *= 0.5)
	{
		SMachState StateEnd(State0);
		CalcIntState(RCPMask, Tau, State0, State1, StateEnd, StockPosition, Turret, Attachment);
		SetState(ConfNum, StateEnd);
		BMatr M1 = GetStock2ToolMatr(StockPosition, Turret, Attachment);
		Motion.Make(M0, M1, ConfNum);
		SMachState StateInt(State0);
		CalcIntState(RCPMask, 0.3, State0, StateEnd, StateInt, StockPosition, Turret, Attachment);
		SetState(ConfNum, StateInt);
		BMatr ToolMatr, StockMatr;
		GetToolAndStockMatr(ToolMatr, StockMatr, StockPosition, Turret, Attachment, false);
		BMatr MMachine = GetStock2ToolMatr(StockPosition, Turret, Attachment);
		double D22 = Motion.GetD2(BPoint(0., 0., 0., 1.) * M0, BPoint(0., 0., 0., 1.) * MMachine);
		if (D22 < Tol * Tol)
			break;
		IsNotSpecialCase = true;
	}
	int PNum = int(SumAngle / 90.);
	PNum = max(PNum, 1);
	PNum = max(PNum, int(1. / Tau));
	if (PNum == 1)
	{
		SMotion.push_back(Motion);
	}
	else
	{
		for (int i = 1; i <= PNum; ++i)
		{
			double t = double(i) / PNum;
			SMachState StateEnd(State0);
			CalcIntState(RCPMask, t, State0, State1, StateEnd, StockPosition, Turret, Attachment);
			SetState(ConfNum, StateEnd);
			BMatr M1 = GetStock2ToolMatr(StockPosition, Turret, Attachment);
			SMotion.resize(SMotion.size() + 1);
			SMotion.back().Make(M0, M1, ConfNum);
			M0 = M1;
		}
	}
	if (!IsNotSpecialCase)
	{
		BPoint ToolAxis = BPoint(0., 0., 1., 0.) * M0;
		for (int i = 0; i < PNum; ++i)
		{
			SMotion[SMotion.size() - PNum + i].RecognizeSpecialCase(M0);
		}

	}
	Now = Buf;
	return PNum;
}

int MTMachineTool::MakeScrewMotion(const NCadrGeom & Geom, std::vector <class BScrewMotion> &SMotion, int Turret, int Attachment, const SCoordsDef& FixedDef)
{
	if (Geom.IsUndef())
		return 0;

	CSingleLock singleLock(&CritSection);
	singleLock.Lock();
	if (Geom.IsCylindric())
	{
		const int MAX_Otrs = 64;
		int NOtrs = min(MAX_Otrs, Geom.GetNumAppr(0.1));
		BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.), XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
		Geom.CalcCurCoord(0., XYZ0, ABC0);
		BCurve *pCurve = NULL;
		int Kp = 0;
		for (int i = 0; i < NOtrs; ++i)
		{
			Geom.CalcCurCoord(double(i + 1) / NOtrs, XYZ1, ABC1);
			Kp += MakeScrewMotion(Geom.GetRCPMask(), Geom.GetMTLink(), XYZ0, ABC0, XYZ1, ABC1, SMotion, Geom.GetStockPos(), Turret, Attachment, FixedDef);
			XYZ0 = XYZ1;
			ABC0 = ABC1;
		}
		return Kp;
	}
	else
	{
		BMatr MCS = GetMCS(&Geom, Turret, Attachment, FixedDef);
		BPoint XYZ0(0., 0., 0., 1.), ABC0(0., 0., 0., 1.);
		Geom.CalcCurCoordMCS(0., MCS, XYZ0, ABC0);
		BPoint XYZ1(0., 0., 0., 1.), ABC1(0., 0., 0., 1.);
		Geom.CalcCurCoordMCS(1., MCS, XYZ1, ABC1);
		return MakeScrewMotion(Geom.GetRCPMask(), Geom.GetMTLink(), XYZ0, ABC0, XYZ1, ABC1, SMotion, Geom.GetStockPos(), Turret, Attachment, FixedDef);
	}
}

INT_PTR MTMachineTool::PositionTool2CS(const BMatr &CS, int StockPosition, int Turret, int Attach, std::vector<SMachState*> &Result, const class SCoordsDef& CDef)
{
	BMatr OrCS;
	
	// Find axes in the chain from stock to tool
	std::vector<int> Chain;
	GetStock2ToolRotChain(StockPosition, Turret, Attach, Chain);
	// Make motion
	BScrewMotion SMotion;
	SMotion.Make(OrCS, CS, GetMTLink());
	// Find solutions
	std::vector<BPoint> Solutions;
	double Delta2;
	switch(Chain.size())
	{
	case 0:
	case 1:
	case 2:
		if(SMotion.GetAngle() == 0.)
		{
			Solutions.push_back(BPoint(0., 0., 0., 1.));
		}
		break;
	case 3:
		SMotion.DecompRot(GetCurConfig().Axis[Chain[1] >= 0 ? Chain[1] : - 1 - Chain[1]]->Axis->GetVector() * ((Chain[1] >= 0) ? 1. : - 1.),
			GetCurConfig().Axis[Chain[1] >= 0 ? Chain[1] : - 1 - Chain[1]]->Axis->GetVector() * ((Chain[1] >= 0) ? 1. : - 1.),
								Solutions, Delta2);
		break;
	case 4:
		SMotion.DecompRot(GetCurConfig().Axis[Chain[1] >= 0 ? Chain[1] : - 1 - Chain[1]]->Axis->GetVector() * ((Chain[1] >= 0) ? 1. : - 1.),
			GetCurConfig().Axis[Chain[2] >= 0 ? Chain[2] : - 1 - Chain[2]]->Axis->GetVector() * ((Chain[2] >= 0) ? 1. : - 1.),
								Solutions, Delta2);
		break;
	case 5:
		SMotion.DecompRot(GetCurConfig().Axis[Chain[1] >= 0 ? Chain[1] : - 1 - Chain[1]]->Axis->GetVector() * ((Chain[1] >= 0) ? 1. : - 1.),
			GetCurConfig().Axis[Chain[2] >= 0 ? Chain[2] : - 1 - Chain[2]]->Axis->GetVector() * ((Chain[2] >= 0) ? 1. : - 1.),
			GetCurConfig().Axis[Chain[3] >= 0 ? Chain[3] : - 1 - Chain[3]]->Axis->GetVector() * ((Chain[3] >= 0) ? 1. : - 1.),
								Solutions);
		break;
	}
	Result.resize(Solutions.size());
	for(unsigned int SolutionInd = 0; SolutionInd < Solutions.size(); ++SolutionInd)
	{
		if (Chain.size() < 3)
		{
			SMachState* pB = new SMachState(&CDef);
			Result[SolutionInd] = pB;
		}
		else
		{
			for (unsigned int i = 1; i < Chain.size() - 1; ++i)
			{
				SMachState* pB = new SMachState(&CDef);
				Result[SolutionInd] = pB;
				MTAxis* pAxis = GetCurConfig().Axis[Chain[i] >= 0 ? Chain[i] : -1 - Chain[i]]->Axis;
				pB->SetCoord(pAxis->GetName(), Solutions[SolutionInd].GetArray()[i - 1]);
			}
		}
		SetState(BMTLinkage().SetIndW1(), *Result[SolutionInd]);
		BPoint DVect = (CS.GetRow(3) - BPoint(0., 0., 0., 1.)) * GetStockMatr(StockPosition, true);
		BPoint XYZ = GetVectXYZ(DVect, CDef, Turret, Attach);
		Result[SolutionInd]->SetCoord(0, XYZ.GetX());
		Result[SolutionInd]->SetCoord(1, XYZ.GetY());
		Result[SolutionInd]->SetCoord(2, XYZ.GetZ());
	}
	return Result.size();
}

void MTMachineTool::GetStock2ToolRotChain(int StockPosition, int Turret, int Attach, std::vector<int> &AxInds)
{
	AxInds.resize(0);
	MTAxis *pAxStock = GetStockAxis(StockPosition);
	if(pAxStock == NULL)
		return;
	MTAxis *pAxTool = GetToolAxis(Turret, Attach);
	if(pAxTool == NULL)
		return;
	AxInds.push_back(- 1 - GetStockAxisInd(0));
	MTAxis *pAxCur = GetCurConfig().GetAxis(pAxStock->GetParent());
	for( ;pAxCur != NULL; pAxCur = GetCurConfig().GetAxis(pAxCur->GetParent()))
	{
		if(GetCurConfig().IsDescendant(*pAxCur, *pAxTool))
			break;
		if(pAxCur->GetType() == MTAxis::AT_ROT) // Rotation axis
			AxInds.push_back(-1 - GetCurConfig().GetAxisIndex(pAxCur->GetName()));
	}

	std::vector<int> AxIndsT;
	for(MTAxis *pAxCur1 = GetCurConfig().GetAxis(pAxTool->GetParent()); pAxCur1 != NULL; pAxCur1 = GetCurConfig().GetAxis(pAxCur1->GetParent()))
	{
		if(pAxCur == pAxCur1)
			break;
		if(pAxCur1->GetType() == MTAxis::AT_ROT) // Rotation axis
			AxIndsT.push_back(GetCurConfig().GetAxisIndex(pAxCur1->GetName()));
	}
	for(unsigned int i = 1; i <= AxIndsT.size(); ++i)
		AxInds.push_back(AxIndsT[AxIndsT.size() - i]);
	int ind = GetCurConfig().GetAxisIndex(MTConfig::GetToolAxisName(Turret, Attach));
	if(ind >= 0)
		AxInds.push_back(ind);
}

void MTMachineTool::FillMachineFile(CStringArray &StringArray)
{
	StringArray.RemoveAll();
	CString Name2Store = FileName;
	if (Name2Store.Right(4) == _T("temp"))
		Name2Store = FileName.Left(FileName.GetLength() - 4);

	StringArray.Add(_T(Name2Store));
	StringArray.Add(_T(""));
	NCMProject::LoadFile2Str(NCM_PROJECT.GetMachinesPath() + FileName + _T("\\Main.xml"), StringArray[1]);
	for(int i = 0; i < Part.GetSize(); ++i)
	{
		CString key = Part[i]->GetFile();
		int index = key.ReverseFind( '\\' );
		key.Delete(0, index + 1);
		StringArray.Add(key);
		StringArray.Add(_T(""));
		NCMProject::LoadFile2Str(Part[i]->GetFile(), StringArray[i * 2 + 3]);
	}

}

CString MTMachineTool::GenNextAxisName(const CString& Name)
{
	CString intString, NewName;

	int index;
	index = GetCurConfig().GetAxisIndex(Name);
	if (index == -1)
	{ // Такого имени нет
		return Name;
	}//if
	else   //Такое имя уже есть
	{
		int i = 1;
		while (index != -1)
		{
			intString.Format("%i", i);
			NewName = Name + CString(_T("_")) + intString;
			index = GetCurConfig().GetAxisIndex(NewName);
			i++;
		}
		return NewName;
	}
}
void MTMachineTool::CalcRCPDisp(const SMachState& Prev, const SMachState& Next, int StockPosition, int Turret, int Attachment, RCPData& RCP, double LengthComp)
{
	if (RCP.GetRCPMask() == 0)
		return;
	// Calculates a shift for XYZ coordinate to compensate rotation for RCP
	// FixedSt contains XYZ for point position and ABC as target rotation
	BPoint BasePoint(0., 0., LengthComp, 1.);
	SMachState N = Next;
	const BPoint& RCPStartABC = RCP.GetStartABC();
	N.SetCoord(3, RCPStartABC.GetX());
	N.SetCoord(4, RCPStartABC.GetY());
	N.SetCoord(5, RCPStartABC.GetZ());
	CorrStateRCP(RCP.GetRCPMask(), Next, N); // Restore non RCP affected rotations in N
	SetStatePartial(MTLink, N);
	// Machine position ignoring rotation
	BPoint OrPosStock = (BasePoint * GetStock2ToolMatr(StockPosition, Turret, Attachment));
	// Machine position with rotation
	SetStatePartial(MTLink, Next);
	BPoint NewPosStock = (BasePoint * GetStock2ToolMatr(StockPosition, Turret, Attachment));
	RCP.SetCoordD(GetVectXYZ((OrPosStock - NewPosStock) * GetStockMatr(StockPosition, false), *Prev.GetDef(), Turret, Attachment));
}

BMatr MTMachineTool::GetMCS(const NCadrGeom* pGeom, const NToolCombined* pTool, const SCoordsDef& FixedDef)
{
	BMatr MCS;
	MCS.SetE();
	if (pGeom == nullptr)
		return MCS;
	int CMatrNum = pGeom->IsPolar() ? 0 : pGeom->GetMatrNum();
	if (pGeom->IsRotate())
		CMatrNum = pGeom->GetInterpNum();
	if (CMatrNum != 0 && !pGeom->IsRCP())
	{
		int Tur = 0;
		int Att = 0;
		if (pTool != nullptr)
		{
			Tur = pTool->Turret;
			Att = pTool->Attach;
		}
		BMatr M = GetRespMatr(BMatr(), BSpacePos::GetABC(CMatrNum), Tur, Att, pGeom->GetStockPos(), &FixedDef);
		BMatr Q = BSpacePos::GetRotMatr(CMatrNum);
		if (M == Q)
			MCS.SetE();
		else
			MCS = Q * M.invr();
	}
	return MCS;
}

BMatr MTMachineTool::GetMCS(const NCadrGeom* pGeom, int Turret, int Attachment, const SCoordsDef& FixedDef)
{
	BMatr MCS;
	MCS.SetE();
	if (pGeom == nullptr)
		return MCS;
	int CMatrNum = pGeom->IsPolar() ? 0 : pGeom->GetMatrNum();
	if (pGeom->IsRotate())
		CMatrNum = pGeom->GetInterpNum();
	if (CMatrNum != 0 && !pGeom->IsRCP())
	{
		BMatr M = GetRespMatr(BMatr(), BSpacePos::GetABC(CMatrNum), Turret, Attachment, pGeom->GetStockPos(), &FixedDef);
		BMatr Q = BSpacePos::GetRotMatr(CMatrNum);
		if (M == Q)
			MCS.SetE();
		else
			MCS = Q * M.invr();
	}
	return MCS;
}

bool MTMachineTool::IsAnyToolAncestor(const MTAxis* pAxis)
{
	const auto& Conf = GetCurConfig();
	return Conf.IsAncestor(pAxis, MTAxis::AT_TOOL);
}

bool MTMachineTool::IsAnyStockAncestor(const MTAxis* pAxis)
{
	const auto& Conf = GetCurConfig();
	return Conf.IsAncestor(pAxis, MTAxis::AT_STOCK);
}

MTMachineTool::ShiftData::ShiftData() : CoordsDef(0)
{
	Defined = false;
}

MTMachineTool::ShiftData::~ShiftData()
{
}

void MTMachineTool::ShiftData::Set(int T, int A, const BPoint& Shift, const SCoordsDef& Def)
{
	Defined = true;
	Att = A;
	Turr = T;
	ToolShiftVec = Shift;
	CoordsDef = Def;
}
