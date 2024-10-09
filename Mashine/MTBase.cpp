/*LoadXML использует библиотеку tinyxml*/

#include "stdafx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "ResLocal.h"
#include "tinyxml.h"
#include "BMatr.h"
#include "MTForwardAxis.h"
#include "MTRotaryAxis.h"
#include "MTSpinAxis.h"
#include "MTPartAxis.h"
#include "MTToolAxis.h"
#include "MTVirtToolAxis.h"
#include "MTStockAxis.h"
#include "MTSimpleChainAxis.h"
#include "SMachState.h"
#include "SCoordsDef.h"
#include "ctype.h"
#include "MNEvalExp.h"
#include "BStockGrID.h"
#include "MTConfig.h"
#include "MTBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(MTBase, CObject, 0)

MTBase::MTBase()
{
	Configs.resize(1);
	Regularize();
}

MTBase::MTBase(const MTBase &MT)
{
	MTLink = MT.MTLink;
	Configs = MT.Configs;
}

bool MTBase::RemoveBranch(int AxisIndex)
{	
	if (Configs.empty())
		return false;
	return GetCurConfig().RemoveBranch(AxisIndex);
}

bool MTBase::SwapAxisNames(int First, int Second)
{
	return GetCurConfig().SwapAxisNames(First, Second, Collisions);
}

MTConfig& MTBase::GetCurConfig()
{
	return Configs[MTLink.GetConfInd()];
}

const MTConfig& MTBase::GetCurConfig() const
{
	return Configs[MTLink.GetConfInd()];
}

void MTBase::Regularize()
{
	StPositionsSet.clear();
	for (int i = 0; i < Configs.size(); ++i)
	{
		Configs.at(i).Regularize();
		Configs.at(i).ReGenEnemiesPairs(Collisions);
		Configs.at(i).GetStockPos(StPositionsSet);
	}
}

void MTBase::Reset()
{
	MTLink.Clear();
	for (int i = 0; i < Configs.size(); ++i)
		Configs.at(i).Reset();
}

void MTBase::ResetConfigs()
{
	MTLink.Clear();
	Configs.resize(1);
}

void MTBase::ResetComp()
{
	for (int i = 0; i < Configs.size(); ++i)
		Configs.at(i).ResetComp();
}

void MTBase::RemoveAll()
{
	MTLink.Clear();

	for (int i = 0; i < Configs.size(); ++i)
		Configs.at(i).RemoveAll();
	Configs.resize(1);
	Regularize();

	Now.RemoveAll();
}

bool MTBase::ReadXMLObject(TiXmlDocument *doc)
{
	if(doc == NULL)
		return false;

	TiXmlNode* MachTool = doc->FirstChild("MachineTool");
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
	
	/***********Освобождение ресурсов класса****************/
	RemoveAll();
	Configs.resize(1);
	MTLink.Clear();
	/*******************************************************/

	for(TiXmlNode* curaxis = axis;
		curaxis != 0;
		curaxis = curaxis->NextSibling("axis"))
	{
		MTAxis* pAx = ReadXML1Axis(curaxis);
		if(pAx != nullptr)
			Configs[0].AddAxis(pAx);
	}//for
	Regularize();

	return true;
}

MTAxis* MTBase::ReadXML1Axis(TiXmlNode* curaxis)
{
	MTAxis* pAx = nullptr;

	TiXmlElement* p_el = curaxis->FirstChildElement("param");
	if (p_el == 0) return nullptr;

	const char* str;
	const char* name;

	name = p_el->Attribute("name");
	CString n(name);

	str = p_el->Attribute("type");
	if (str == NULL) return nullptr;
	CString b(str);
	if (b == "trans")
		pAx = new MTForwardAxis;
	else if (b == "rotary")
		pAx = new MTRotaryAxis;
	else if (b == "spin")
		pAx = new MTSpinAxis;
	else if (b == "part")   //Для обратной совместимости, тип tool, virttool или stock можно задать типом
							// part и особым именем
	{
		if (n == "stock" || n == "stock_moved")
			pAx = new MTStockAxis;
		else if (n.Find("tool") == 4 && isdigit(n[0]) && isdigit(n[1]) && isdigit(n[2]) && isdigit(n[3]))
			pAx = new MTVirtToolAxis;
		else if (n.Find("tool") == 0)
			pAx = new MTToolAxis;
		else
			pAx = new MTPartAxis;
	}
	else if (b == "tool")
		pAx = new MTToolAxis;
	else if (b == "virttool")
		pAx = new MTVirtToolAxis;
	else if (b == "stock" || b == "stock_moved")
		pAx = new MTStockAxis;
	else if (b == _T("simple_chain"))
		pAx = new MTSimpleChainAxis;
	else
		return nullptr;

	str = p_el->Attribute("name");
	if (str == NULL) return nullptr;
	pAx->SetName(CString(str));

	str = p_el->Attribute("parent");
	pAx->SetParent(CString(str));

	str = p_el->Attribute("expression");
	if (str != nullptr)
		pAx->SetExpr(CString(str));

	double d;
	str = p_el->Attribute("length", &d);
	if (str != NULL)
		pAx->SetLength(d);

	bool IsCS = false;
	str = p_el->Attribute("coordsys");
	if (str != NULL)
	{
		CString Flag(str);
		pAx->SetIsCS(Flag == "Yes");
	}


	str = p_el->Attribute("mark");
	if (str != NULL)
	{
		CString b(str);
		if (b == "on")
			pAx->SetVisible(true);
	}//if

	double x, y, z;
	p_el = curaxis->FirstChildElement("vector");
	if (p_el == 0) return nullptr;
	str = p_el->Attribute("x", &x);
	if (str == NULL) return nullptr;
	str = p_el->Attribute("y", &y);
	if (str == NULL) return nullptr;
	str = p_el->Attribute("z", &z);
	if (str == NULL) return nullptr;
	pAx->InitVector(BPoint(x, y, z, 0.));

	double dmin, dmax;
	p_el = curaxis->FirstChildElement("limits");
	if (p_el != 0)
	{
		str = p_el->Attribute("min", &dmin);
		if (str != NULL)
		{
			str = p_el->Attribute("max", &dmax);
			if (str != NULL)
			{
				pAx->SetMin(dmin);
				pAx->SetMax(dmax);
			}//if
			else
			{
				pAx->SetMin(0);
				pAx->SetMax(0);
			}//else
		}//if
		else
		{
			pAx->SetMin(0);
			pAx->SetMax(0);
		}//else
	}//if
	else
	{
		pAx->SetMin(0);
		pAx->SetMax(0);
	}//else
	pAx->SetMovement(0);

	p_el = curaxis->FirstChildElement("color");
	if (p_el != 0)
	{
		double r, g, b, a;
		str = p_el->Attribute("r", &r);
		if (str != NULL)
		{
			str = p_el->Attribute("g", &g);
			if (str != NULL)
			{
				str = p_el->Attribute("b", &b);
				if (str != NULL)
				{
					str = p_el->Attribute("a", &a);
					if (str != NULL)
						pAx->SetColor(NColor((float)r, (float)g, (float)b, (float)a));
				}//if
			}//if
		}//if
	}//if
	else // Задаём цвет по умолчанию.
	{
		pAx->SetColor(NColor(0, 0, 0, 1));
	}//else

	{
		p_el = curaxis->FirstChildElement("vectorY");
		if (p_el != nullptr)
		{
			str = p_el->Attribute("x", &x);
			if (str == nullptr) return nullptr;
			str = p_el->Attribute("y", &y);
			if (str == nullptr) return nullptr;
			str = p_el->Attribute("z", &z);
			if (str == nullptr) return nullptr;
			pAx->InitVectorY(BPoint(x, y, z, 0.));
		}
	}//if
	{
		p_el = curaxis->FirstChildElement("basepoint");
		if (p_el != nullptr)
		{
			str = p_el->Attribute("x", &x);
			if (str == NULL) return nullptr;
			str = p_el->Attribute("y", &y);
			if (str == NULL) return nullptr;
			str = p_el->Attribute("z", &z);
			if (str == NULL) return nullptr;
			pAx->InitBasePoint(BPoint(x, y, z, 1.));
		}
	}//if

	p_el = curaxis->FirstChildElement("localdisplvec");
	if (p_el != 0)
	{
		str = p_el->Attribute("x", &x);
		if (str == NULL) return nullptr;
		str = p_el->Attribute("y", &y);
		if (str == NULL) return nullptr;
		str = p_el->Attribute("z", &z);
		if (str == NULL) return nullptr;
		pAx->SetLocalDisplVec(BPoint(x, y, z, 0.));
	}//if
	for (TiXmlElement* p_specialpt = curaxis->FirstChildElement("specialpt");
		p_specialpt != nullptr;
		p_specialpt = p_specialpt->NextSiblingElement("specialpt"))
	{
		int ind;
		str = p_specialpt->Attribute("name", &ind);
		double val;
		str = p_specialpt->Attribute("value", &val);
		MTRelocateAxis* pRelocateAx = dynamic_cast<MTRelocateAxis*>(pAx);
		if (pRelocateAx != nullptr)
			pRelocateAx->SetSpecPtsVal(ind, val);
	}
	if (b == _T("simple_chain"))
	{
		MTSimpleChainAxis* pSCAxis = dynamic_cast<MTSimpleChainAxis*>(pAx);
		if (pSCAxis != nullptr)
		{
			p_el = curaxis->FirstChildElement("chain_param");
			if (p_el != nullptr)
			{
				double d;
				str = p_el->Attribute("cycle_value", &d);
				if (str != nullptr)
					pSCAxis->SetCycleVal(d);
				str = p_el->Attribute("center_dist", &d);
				if (str != nullptr)
					pSCAxis->SetCenterDist(d);
			}
		}
	}
	return pAx;
}

MTBase::~MTBase()
{
	RemoveAll();
}

const MTBase& MTBase::operator = (const MTBase &B)
{
	//1 Проверка на самоприсваивание
	if(&B == this) return *this;

	//2 Удаление данных
	RemoveAll();

	//3 Копирование значений
	Configs = B.Configs;
	//4 Возвращение указателя *this
	return *this;
}

void MTBase::CopyKinematics(const MTBase &Source)
{
	*this = Source;
}

void MTBase::RenameAxis(const CString& OldName, const CString& NewName)
{
	int index = GetCurConfig().GetAxisIndex(OldName);
	if (index >= 0)
		RenameAxis(index, NewName);
}//void MTBase::RenameAxis(CString OldName, CString NewName)

void MTBase::RenameAxis(int index, const CString& NewName)
{
	CString OldName = GetCurConfig().Axis[index]->Axis->GetName();

	//1 - переименовываем родителя у непосредственных потомков
	for(int i = 0; i < GetCurConfig().Axis.GetSize(); i++)
	{
		if(GetCurConfig().Axis[i]->Axis->GetParent() == OldName)
			GetCurConfig().Axis[i]->Axis->SetParent(NewName);
	}//for

	//2 - переименовываем саму ось
	GetCurConfig().Axis[index]->Axis->SetName(NewName);
}

#ifdef _DEBUG
void MTBase::DebugPrint(const CString& label, BOOL level, BOOL parents)
{
	//CString Name;
	//FILE *fLog = fopen(LOG_FILE,"at");
	//CString indent = "  ";

	//fprintf(fLog,"\n%s\n",label);
	//fprintf(fLog,"================================================\n");
	//fprintf(fLog,"Size: %d    Axis.GetSize(): %d\n",Size, Axis.GetSize());

	//for(int i = 0; i < Size; i++)
	//{
	//	Name = (*this)[i]->GetName();
	//	for(int j =0; j < GetLevel(i); j++)
	//	{
	//		fprintf(fLog,"%s", indent.GetBuffer());
	//	}//for
	//	fprintf(fLog,"%s",Name.GetBuffer());
	//	if(parents)
	//		fprintf(fLog," (%s)", (*this)[i]->GetParent().GetBuffer());
	//	if(level)
	//		fprintf(fLog,"   %d", GetLevel(i));

	//	fprintf(fLog,"\n");
	//}//for
	//fprintf(fLog,"\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

	//fclose(fLog);

}//void MTBase::DebugPrint(int DetLevel)
#endif
void MTBase::FullCopyWOVis(const MTBase& MT)
{
	*this = MT;
}

void MTBase::DelFullCopyWOVis()
{
	delete this;
}

void MTBase::SetCurConf(BMTLinkage iMTLink)
{
	if(!iMTLink.IsIndW1())
		MTLink = iMTLink;
}

MTConfig& MTBase::AddNewConfig()
{
	Configs.push_back(GetCurConfig());
	GetMTLink().SetConfInd(int(Configs.size()) - 1);
	return Configs.back();
}

int MTBase::GetAxisColNum(const CString& AxisName)
{
	return Collisions.GetAxisColNum(AxisName);
}

void MTBase::AddColToMT(SCollision& Col)
{
	Collisions.AddCol(this, Col);
	GetCurConfig().ReGenEnemiesPairs(Collisions);
}

void MTBase::AddColToMT(int MyNum, BOOL MyChildren, int AxisNum, BOOL AxisChildren)
{
	SCollision Col;// = new SCollision();
	Col.Axis1 = GetCurConfig().Axis[MyNum]->Axis->GetName();
	Col.Axis2 = GetCurConfig().Axis[AxisNum]->Axis->GetName();
	Col.Axis1Ch = MyChildren;
	Col.Axis2Ch = AxisChildren;
	AddColToMT(Col);
}

const MTAxis* MTBase::GetParentAxis(const MTAxis* pAxis) const
{
	return GetCurConfig().GetAxis(pAxis->GetParent());
}

const MTAxis* MTBase::GetNearestMoveRotAxis(const MTAxis* pAxisInp) const
{
	return GetCurConfig().GetNearestMoveRotAxis(pAxisInp);
}

int MTBase::GetNearestMoveRotAxisInd(int AxisInd) const
{
	return GetCurConfig().GetNearestMoveRotAxisInd(AxisInd);
}

void MTBase::ClearNow()
{
	Now.RemoveAll();
}

int MTBase::Conv2StPos(BStockGrID StGr) const
{
	return MTLink.GetStGr2StPos().Get(StGr);
}

const std::set<std::pair<int, int>>& MTBase::GetEnemies(void) const
{
	return GetCurConfig().Enemies;
}
