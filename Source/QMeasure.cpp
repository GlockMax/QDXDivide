#include "stdafx.h"
#include "ResInv.h"
#include "BConst.h"
#include "ConstDef.h"
#include "MQuadTree.h"
#include "MFace.h"
#include "MRay.h"
#include "math.h"
#include "QMeasureDlg.h"
#include "MBSPNode.h"
#include "SOrigSurf.h"
#include "SOCyl.h"
#include "SOSphere.h"
#include "SOTorus.h"
#include "SOCone.h"
#include "SOECyl.h"
#include "GQuadRenderRot.h"
#include "BaseRender.h"
#include "NStock.h"
#include "GQuadRenderDX5X.h"
#include "DXSubModel5X.h"
#include "NCMDoc.h"
#include "QComparison.h"
#include "QMeasure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

typedef MBSPNode *NodePtr;
//////////////////////////////////////////////////////////////////////////
// base method's
//////////////////////////////////////////////////////////////////////////
QMeasure::QMeasure(CNCMDoc& iDoc) : Doc(iDoc)
{
	IsActiv = FALSE;
	Draw_Type = NONE_M;
	Type_Measure = NONE_M;
	ModelMode = NGlobalState::BSP;
}

QMeasure::~QMeasure(void)
{
}

BOOL QMeasure::IsActivated(void) const
{
	return IsActiv;
}

void QMeasure::Activate(NGlobalState::ModelModes Mode, CWnd * parent, class NStock* pSt)
{
	if(pSt == nullptr)
		return;
	pStock = pSt;
	MBody* pBody = pStock->GetBodyMill();
	if (pBody == nullptr)
		return;
	const BMatrPair& BodyRot = pStock->GetBodyMill()->GetBodyRot();
	if (NCMComData::IsTurnEnabled())
		CMatr = BodyRot.Or() * BMatr().RotX(0., 0., 0., 90.);
	else
		CMatr = BodyRot;
	// Change stock section view parameters
	GQuadRenderRot::SetFull();
	
	IsActiv = TRUE;
	DialogBuild(parent);
	SetSurfInformation(BPoint(0.,0.,0.,1.), 0, cadrID(-1)); //Clear surf information
	ModelMode = Mode;
}

void QMeasure::DeActivate(void)
{
	if(!IsActiv)
		return;

	// Change stock section view parameters
	GQuadRenderRot::ResetStored();

	IsActiv = FALSE;
	delete MeasureDlg;
	pStock = nullptr;
	MFOREST.Compress();
}
void QMeasure::DialogBuild(CWnd * p)
{
	UINT id = IDD_MEASURE;
	MeasureDlg = new QMeasureDlg(p, id);
	MeasureDlg->Create(id);
	MeasureDlg->ShowWindow(SW_SHOW);
	MeasureDlg->SetQMeasure(this);
	MeasureDlg->SetPoint(BPoint(0., 0., 0., 1.));
	MeasureDlg->SetVector(BPoint(0., 0., 1., 0.));
}
//////////////////////////////////////////////////////////////////////////
// in - out method's
//////////////////////////////////////////////////////////////////////////
void QMeasure::SetPoint(CPoint point,  QMeasureType MesType)
{
	if(!IsActivated())	//проверка включен ли режим измерений
		return;
	GLdouble x = 0.,y = 0.,z = 0.;
	GLint viewport[4];
	GLdouble projMatrix[16],modelMatrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX ,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX ,projMatrix);
	glGetIntegerv(GL_VIEWPORT ,viewport);
	point.y = viewport[3] - point.y;
	gluUnProject(point.x+0.5,point.y+0.5,0,modelMatrix, projMatrix,viewport,&x,&y,&z);
	BPoint p1(x,y,z,1);
	gluUnProject(point.x+0.5,point.y+0.5,1,modelMatrix, projMatrix,viewport,&x,&y,&z);
	BPoint p2(x,y,z,1);
	BPoint nv = p2-p1;
	BPoint resP(0., 0., 0., 1.), resV(0., 0., 0., 1.);
	ElemID FoundID;
	cadrID FoundCadrID;
	Find_Node_Point_Vector(p1, nv, TRUE, resP, resV, FoundID, FoundCadrID);
	switch(MesType) 
	{
	case FIND_POINT:
		SetSurfInformation(resP, FoundID, FoundCadrID, resV); //information about type of surface for dialog
		break;
	case AUTO_M:
		Draw_Type = NONE_M;
		int SurfNum = MFOREST.GetIDProc()->GetSurfID(FoundID);
		if(SurfNum < 0 || SurfNum == ID_EMPTY)
			MeasureDlg->SetNotDefine(1);
		else
		{
			bp = resP;
			vn = resV;
			CalcAuto();
		}
		break;
	}
}
void QMeasure::SetPoint(const BPoint &Ps, const BPoint &View, QMeasureType MesType)
{
	if(!IsActivated())	//проверка включен ли режим измерений
		return;
	BPoint resP(0., 0., 0., 1.), resV(0., 0., 0., 1.);
	ElemID FoundID;
	cadrID FoundCadrID;
	Type_Measure = FIND_POINT;
	bool IntersectionFound = Find_Node_Point_Vector(Ps - View * 0.5, View, TRUE, resP, resV, FoundID, FoundCadrID);
	Type_Measure = NONE_M;
	switch(MesType) 
	{
	case FIND_POINT:
		SetSurfInformation(resP, FoundID, FoundCadrID, resV); //information about type of surface for dialog
		break;
	case AUTO_M:
		Draw_Type = NONE_M;
		if(!IntersectionFound)
			MeasureDlg->SetNotDefine(1);
		else
		{
			bp = resP;
			vn = resV;
			CalcAuto();
			// set current line info to the dialog
			CString Chan, Prog, Line, Step;
			Doc.GetCadrIDStrings(FoundCadrID, Chan, Prog, Line, Step);
			MeasureDlg->ProgName = "Name= " + Prog;
			MeasureDlg->Cadr = "N= " + Line + " St= " + Step;
			cadrID ID0, ID1;
			FoundCadrID.SplitMix(ID0, ID1);
			MeasureDlg->CadrID = ID0;
			MeasureDlg->m_Goto.EnableWindow(MeasureDlg->CadrID.IsEmpty() ? FALSE : TRUE);
			MeasureDlg->UpdateData(FALSE);
		}
		break;
	}
}

void QMeasure::Draw(void)
{
	if(IsActiv && Draw_Type != NONE_M)
	{
		glPushMatrix();
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glPointSize(7.0);
		glEnable(GL_POINT_SMOOTH);
		if (bp == ep)
		{
			glBegin(GL_POINTS);
			glColor3d(0., 1., 0.);
			glVertex3d(bp.GetX(), bp.GetY(), bp.GetZ());
			glEnd();
		}
		else
		{
			glColor3d(1., 1., 0.);
			glBegin(GL_LINES);
			glVertex3d(bp.GetX(), bp.GetY(), bp.GetZ());
			glVertex3d(ep.GetX(), ep.GetY(), ep.GetZ());
			if (Draw_Type == MANUAL_M)
			{
				glVertex3d(bp.GetX(), bp.GetY(), bp.GetZ());
				glVertex3d(outp.GetX(), outp.GetY(), outp.GetZ());
			}
			glEnd();

			glBegin(GL_POINTS);
			glColor3d(1., 0., 0.);
			glVertex3d(ep.GetX(), ep.GetY(), ep.GetZ());
			if (Draw_Type == MANUAL_M)
				glVertex3d(outp.GetX(), outp.GetY(), outp.GetZ());
			glColor3d(0., 1., 0.);
			glVertex3d(bp.GetX(), bp.GetY(), bp.GetZ());
			glEnd();
		}

		glPopAttrib();
		glPopMatrix();
	}
}


//////////////////////////////////////////////////////////////////////////
// measure method's
//////////////////////////////////////////////////////////////////////////
bool QMeasure::RayShooting(const BPoint &p, const BPoint &v, bool INOUT, BPoint &FoundP, BPoint &FoundN, ElemID &FoundID, cadrID& FoundCadrID) const
{
	FoundID = MElemIDProc::ID_NULL;
	switch(ModelMode)
	{
		case NGlobalState::BSP:
		{
				auto pTree = static_cast<MQuadTree*>(pStock->GetBodyMill());
				if (pTree == nullptr)
					return false;
				if(!pTree->RayShooting(p, v, INOUT, FoundP, FoundN, FoundID, FoundCadrID))
					return false;
				return true;
		}
			break;
		case NGlobalState::DX:
		{
			auto pBody = pStock->GetBodyMill();
			auto pBodyRender = pStock->GetRender();
			if (pBodyRender == nullptr)
				return false;
			const GQuadRenderDX5X* pRender5X = dynamic_cast<const GQuadRenderDX5X*>(pBodyRender);
			if (pRender5X == nullptr)
				return false;
			BPoint q[3];// vertexes of the triangle containing nearest point
			FoundP = pRender5X->RayCasting(p, v, 1.e-6, FoundN, q[0], q[1], q[2]);
			if (!FoundP.IsPoint())
				return false;
			DXSubModel5X* pBodyDX = dynamic_cast<DXSubModel5X*>(pBody);
			if (pBodyDX != nullptr)
			{
				const double IdentDist = 0.0001 * (pBodyDX->GetXstep() * pBodyDX->GetXstep() + pBodyDX->GetYstep() * pBodyDX->GetYstep() + pBodyDX->GetZstep() * pBodyDX->GetZstep());
				MBodyPointInf Inf[3];
				double D2[3];
				for (int i = 0; i < 3; ++i)
				{
					D2[i] = pBodyDX->GetNearestPoint(q[i], Inf[i]);// distance from dexel vertex
					if (D2[i] < IdentDist)
					{
						FoundID = Inf[i].ElemID_;
						FoundN = Inf[i].Nr_ * CMatr.Or();
						FoundCadrID = Inf[i].HistCadrID_;
					}
				}
			}
			return true;
		}
			break;
		default:
			return false;
	}
}

bool QMeasure::Find_Node_Point_Vector(const BPoint &point, const BPoint &vector, bool IN_OUT, BPoint &outpoint, BPoint &outvector, ElemID &FoundID, cadrID& FoundCadrID)
{
	BPoint temp(0., 0., 0., 1.);			//временная точка
	bool IntersectionFound = false;
	FoundID = MElemIDProc::ID_NULL;

	//////////////////////////////////////////////////////////////////////////
//Смещение точки 
	temp = point + (vector * (0.1));		//делаем простое смещение
	if(Type_Measure == AUTO_M || Type_Measure == MANUAL_M)
	{
		BPoint FP, FN;
		BOOL in_ptr = Contains(temp);
		if(IN_OUT && in_ptr) //ЕсЛИ ИЩЕМ ВХОД и находимся в теле, то найти точку выхода
			IntersectionFound = QComparison::RayShootingStatic(pStock, point, vector, false, FP, FN, FoundID, FoundCadrID);

		if(!IN_OUT && !in_ptr) // ищем выход и находимся снаружи, то найти точку входа
			IntersectionFound = QComparison::RayShootingStatic(pStock, point, vector, true, FP, FN, FoundID, FoundCadrID);

		if(IntersectionFound)
			temp = FP;
		else
			temp = point;
	}
	temp = temp + (vector * (0.1));		//делаем простое смещение

//////////////////////////////////////////////////////////////////////////
	BOOL in_ptr = Contains(temp);
	bool FindEntry = IN_OUT;
	if(Type_Measure == FIND_POINT)
		FindEntry = !in_ptr;	
	BPoint FoundP, FoundN;// in global CS
	IntersectionFound = QComparison::RayShootingStatic(pStock, temp, vector, FindEntry, FoundP, FoundN, FoundID, FoundCadrID);
	if(IntersectionFound)		//проверяем найдена ли такая вершина в дереве
	{
		outpoint = FoundP * CMatr.Inv(); //ищем приближенную точку пересечения
		outvector = FoundN * CMatr.Inv();	//ищем нормаль для FaceNum-грани
//////////////////////////////////////////////////////////////////////////
// refining point
//////////////////////////////////////////////////////////////////////////
		int BFaceN = MFOREST.GetIDProc()->GetSurfID(FoundID);
		if(BFaceN >= 0 && BFaceN != ID_EMPTY)	//если у грани есть изначальная поверхность то пытаемся уточнить
		{
			MFOREST.GetOrigSurf(BFaceN)->RefiningPoint(outpoint, vector * CMatr.Inv());
			outvector = MFOREST.GetOrigSurf(BFaceN)->GetNormal(outpoint);
		}
//////////////////////////////////////////////////////////////////////////
		outpoint = outpoint * CMatr.Or();
		outvector = outvector * CMatr.Or();
		if(vector * outvector < 0.)
			outvector = outvector * (-1.);
		if(Type_Measure == FIND_POINT && !FindEntry)
			outvector = outvector * (-1.);

	}
	return IntersectionFound;		
}


void QMeasure::CalcAuto(void)
{
//////////////////////////////////////////////////////////////////////////
	Type_Measure = AUTO_M; // Set type of measure
	Draw_Type = AUTO_M;		//Set type of drawing
	ep = bp; // to draw one point if second point is absent
//////////////////////////////////////////////////////////////////////////
	MeasureDlg->SetPoint(bp);			//передаем точку в Диалог для показа координат
	if(!MeasureDlg->m_Bind.GetState())	
	{
		if(MeasureDlg->m_Back.GetState())
			vn = vn * (-1.);
		MeasureDlg->SetVector(vn);
	}
	else
		vn = MeasureDlg->GetVector();
	
	BPoint tmp(0., 0., 0., 1.);
	ElemID FoundID;
	cadrID FoundCadrID;
	bool IntersectionFound = Find_Node_Point_Vector(bp, vn, MeasureDlg->m_Back.GetState() != 0, ep, tmp, FoundID, FoundCadrID);

	if(!IntersectionFound)
	{
		MeasureDlg->SetNotDefine(2);
		Type_Measure = NONE_M;
		return;
	}
//Output result
	double Result;
	CString Str;
	Str.Format((MM_INCH == M_MM) ? IDS_1ENDPOINT : IDS_1ENDPOINT_INCH, ep.GetX(), ep.GetY(), ep.GetZ());  //фомируем строку вывода координат
	MeasureDlg->m_EndPoint.SetWindowText(Str);			//пишем в диалоге
	Result=sqrt(bp.Dist2(ep, bp));						//Расчет расстояния между точками
	Str.Format((MM_INCH == M_MM) ? IDS_1RESULT : IDS_1RESULT_INCH,Result);						//Формируем строку вывода результата
	MeasureDlg->MeasureResult.SetWindowText(Str);		//пишем в диалог
	//////////////////////////////////////////////////////////////////////////
	Type_Measure = NONE_M;	//Set type of measure
//////////////////////////////////////////////////////////////////////////
}
void QMeasure::CalcManual(const BPoint &P, const BPoint &V)
{
	MeasureDlg->SetPoint(P);
	MeasureDlg->SetVector(V);
	CalcManual();
	MeasureDlg->ReDraw();
}

bool QMeasure::Contains(const BPoint &bp) const
{
	switch(ModelMode)
	{
		default:
			return false;
		case NGlobalState::BSP:
		{
			auto pTree = static_cast<MQuadTree*>(pStock->GetBodyMill());
			if (pTree == nullptr)
				return false;
			return pTree->Contains(bp * CMatr.Inv());
		}
		case NGlobalState::DX:
		{
			auto pBodyRender = pStock->GetRender();
			if (pBodyRender == nullptr)
				return false;
			BPoint FoundN;
			int Count = 0;
			for (BPoint FoundP = pBodyRender->RayCasting(bp, BPoint(1., 1., 1., 0.), 1.e-6, FoundN);
				FoundP.IsPoint();
				FoundP = pBodyRender->RayCasting(FoundP + BPoint(1., 1., 1., 0.) * 0.01, BPoint(1., 1., 1., 0.), 1.e-6, FoundN))
			{
				++Count;
			}
			return (Count % 2) != 0;
		}
	}
}


void QMeasure::CalcManual(void)
{
	if(!IsActivated())	//проверка вкдючен ли режим измерений
		return;
//////////////////////////////////////////////////////////////////////////
	Type_Measure = MANUAL_M;// Set type of measure
//////////////////////////////////////////////////////////////////////////
	CString Str1, Str2;
	BPoint tmp(0., 0., 0., 1.);
	BOOL TWO_POINTS = FALSE;
	bp = MeasureDlg->GetPoint();
	vn = MeasureDlg->GetVector();
	BPoint back_vn = vn * (-1.);
	bool IntFound1 = false;
	bool IntFound2 = false;
	ElemID FoundID;
	cadrID FoundCadrID(-1);
//////////////////////////////////////////////////////////////////////////
	if(Contains(bp)) //if point is inside the body
	{
		IntFound1 = Find_Node_Point_Vector(bp, vn, FALSE, ep, tmp, FoundID, FoundCadrID);
		IntFound2 = Find_Node_Point_Vector(bp, back_vn, FALSE, outp, tmp, FoundID, FoundCadrID);
		if(MeasureDlg->m_Back.GetState())
		{
			if (IntFound1)
				IntFound1 = Find_Node_Point_Vector(ep, vn, TRUE, ep, tmp, FoundID, FoundCadrID);
			if (IntFound2)
				IntFound2 = Find_Node_Point_Vector(outp, back_vn, TRUE, outp, tmp, FoundID, FoundCadrID);
		}
		if(!IntFound1 && !IntFound2)
		{
			MeasureDlg->SetNotDefine(2);
			Draw_Type = NONE_M;
			Type_Measure = NONE_M;
			return;
		}
		if(IntFound1)
			TWO_POINTS = TRUE;
		else
			if(IntFound2)
				ep = outp;
	}
	else
	{
		IntFound1 = Find_Node_Point_Vector(bp, vn, TRUE, ep, tmp, FoundID, FoundCadrID);
		IntFound2 = Find_Node_Point_Vector(bp, back_vn, TRUE, outp, tmp, FoundID, FoundCadrID);
		if(MeasureDlg->m_Back.GetState())
		{
			if (IntFound1)
				IntFound1 = Find_Node_Point_Vector(ep, vn, FALSE, ep, tmp, FoundID, FoundCadrID);
			if (IntFound2)
				IntFound2 = Find_Node_Point_Vector(outp, back_vn, FALSE, outp, tmp, FoundID, FoundCadrID);
		}
		if(!IntFound1 && !IntFound2)
		{
			MeasureDlg->SetNotDefine(2);
			Draw_Type = NONE_M;
			Type_Measure = NONE_M;
			return;
		}
		if(IntFound1 && IntFound2)
			TWO_POINTS = TRUE;
		else
			if(IntFound2)
				ep = outp;
	}
//////////////////////////////////////////////////////////////////////////
//out data in dialog
	double Result1 = 0.;
	if(TWO_POINTS)
	{
		//считаем расстояния, формируем строки вывода 
		Result1 = sqrt(vn.Dist2(ep, bp));
		double Result2 = sqrt(vn.Dist2(outp, bp));
		Str1.Format((MM_INCH == M_MM) ? IDS_2ENDPOINT : IDS_2ENDPOINT_INCH, ep.GetX(), ep.GetY(), ep.GetZ(), outp.GetX(), outp.GetY(), outp.GetZ());  
		Str2.Format((MM_INCH == M_MM) ? IDS_2RESULT : IDS_2RESULT_INCH,Result1,Result2,Result1+Result2);
		Draw_Type = MANUAL_M;
	}
	else
	{
		//считаем расстояния, формируем строки вывода 
		Result1=sqrt(vn.Dist2(ep, bp));
		Str1.Format((MM_INCH == M_MM) ? IDS_1ENDPOINT : IDS_1ENDPOINT_INCH, ep.GetX(), ep.GetY(), ep.GetZ());  
		Str2.Format((MM_INCH == M_MM) ? IDS_1RESULT : IDS_1RESULT,Result1);
		Draw_Type = AUTO_M;
	}
	MeasureDlg->m_EndPoint.SetWindowText(Str1);
	MeasureDlg->MeasureResult.SetWindowText(Str2);
//////////////////////////////////////////////////////////////////////////
	Type_Measure = NONE_M;	//Set type on measure
//////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////
// методы для информационной закладки
//////////////////////////////////////////////////////////////////////////

SufrInformation QMeasure::GetSufrInfo(void)
{
	return SOrSurfInfo;
}
void QMeasure::SetSurfInformation(BPoint SurfPoint, ElemID ElID, cadrID CaID, const BPoint& N/* = BPoint(0., 0., 0., 0.)*/)
{	
	SOrSurfInfo.BeginPoint.Set(0.,0.,0.,1.);
	SOrSurfInfo.Normal.Set(0.,0.,0.,0.);
	SOrSurfInfo.Axis.Set(0.,0.,0.,0.);
	SOrSurfInfo.Angle = 0.;
	SOrSurfInfo.ExtR = 0.;
	SOrSurfInfo.IntR = 0.;
	// SurfPoint & N is in global CS
	CString Chan, Prog, Line, Step;
	Doc.GetCadrIDStrings(CaID, Chan, Prog, Line, Step);
	if (Prog.IsEmpty())
		SOrSurfInfo.LineID.LoadString(IDS_TYPE_NDEF);
	else
	{
		if (Chan.IsEmpty())
			SOrSurfInfo.LineID.Format("%s, %s : %s", Prog, Line, Step);
		else
			SOrSurfInfo.LineID.Format("%s.%s, %s : %s", Chan, Prog, Line, Step);
	}
	if(ElID == MElemIDProc::ID_NULL)
	{
		if(N.D2() < MIND)
			SOrSurfInfo.Type.LoadString(IDS_TYPE_NDEF);
		else
		{
			SOrSurfInfo.Type.LoadString(IDS_TYPE_PLANE);
			SOrSurfInfo.Normal = N;
			SOrSurfInfo.BeginPoint = SurfPoint;
		}
		return;
	}
	
	int BFaceN = MFOREST.GetIDProc()->GetSurfID(ElID);
	if(BFaceN == -1)
	{
		SOrSurfInfo.Type.LoadString(IDS_TYPE_STOCK);
		SOrSurfInfo.BeginPoint = SurfPoint;
		return;
	}
	if (BFaceN == 0)
	{
		if (N.D2() < MIND)
			SOrSurfInfo.Type.LoadString(IDS_TYPE_NDEF);
		else
		{
			SOrSurfInfo.Type.LoadString(IDS_TYPE_PLANE);
			SOrSurfInfo.Normal = N;
			SOrSurfInfo.BeginPoint = SurfPoint;
		}
	}
	if(BFaceN < 0 || BFaceN == ID_EMPTY)
	{
		SOrSurfInfo.Type.LoadString(IDS_TYPE_NDEF);
		SOrSurfInfo.BeginPoint = SurfPoint;
	}
	else
	{
		SOrSurfInfo.Type = MFOREST.GetOrigSurf(BFaceN)->GetTypeName();
		SOrSurfInfo.BeginPoint = MFOREST.GetOrigSurf(BFaceN)->GetCenter() * CMatr.Or();
		SOrSurfInfo.Normal = MFOREST.GetOrigSurf(BFaceN)->Get_N_Vector() * CMatr.Or();
		CString tmp;
		switch (MFOREST.GetOrigSurf(BFaceN)->GetType())
		{
		case CYLINDER:
			SOrSurfInfo.ExtR = ((SOCyl*)MFOREST.GetOrigSurf(BFaceN))->GetRadius();
			if (SOrSurfInfo.ExtR > 0.)
				tmp.LoadString(IDS_CYL_TYPE1);
			else
				tmp.LoadString(IDS_CYL_TYPE2);
			SOrSurfInfo.Type = SOrSurfInfo.Type + tmp;
			break;
		case PLANE:
			SOrSurfInfo.BeginPoint = SurfPoint;
			break;
		case SPHER:
			SOrSurfInfo.ExtR = ((SOSphere*)MFOREST.GetOrigSurf(BFaceN))->GetRadius();
			break;
		case TORUS:
			SOrSurfInfo.ExtR = ((SOTorus*)MFOREST.GetOrigSurf(BFaceN))->GetRLarge();
			SOrSurfInfo.Type = SOrSurfInfo.Type + tmp;
			SOrSurfInfo.IntR = ((SOTorus*)MFOREST.GetOrigSurf(BFaceN))->GetRSmall();
			if (SOrSurfInfo.IntR > 0.)
				tmp.LoadString(IDS_CYL_TYPE1);
			else
				tmp.LoadString(IDS_CYL_TYPE2);
			SOrSurfInfo.Type = SOrSurfInfo.Type + tmp;
			break;
		case CON:
			SOrSurfInfo.Angle = ((SOCone*)MFOREST.GetOrigSurf(BFaceN))->GetAngle();
			break;
		case ECYLINDER:
			SOrSurfInfo.Axis = ((SOECyl*)MFOREST.GetOrigSurf(BFaceN))->Get_Axis() * CMatr.Or();
			SOrSurfInfo.ExtR = ((SOCyl*)MFOREST.GetOrigSurf(BFaceN))->GetRadius();
			break;
		}
	}
}

void QMeasure::SetDefault(void)
{
	SetSurfInformation(BPoint(0.,0.,0.,1.), 0, cadrID(-1)); //Clear surf information
	MeasureDlg->SetNotDefine(1);
}

void QMeasure::GotoCurrentCadr() const
{
	Doc.Jump2AbsCadr(MeasureDlg->CadrID, false);
}
