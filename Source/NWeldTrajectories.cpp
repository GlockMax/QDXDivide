#include "stdafx.h"
#include "math.h"
#include "ConstDef.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "NProgramWeld.h"
#include "NCMProject.h"
#include "NWeldTrajectories.h"

NWeldTrajectories::NWeldTrajectories(const class NProgramWeld &Prog)
	: ColorBold(1., 0., 1., 1.)
{
	NTiParams &Par = NCM_PROJECT.Defaults;

	CString Key = _T("Defaults/Program/Weld/ToolPath");

	Tolerance = 0.01;
	First.SetSize(0, 2048); 
	Second.SetSize(0, 2048); 
	Root.SetSize(0, 2048); 
	Delta.SetSize(0, 2048); 
	FirstVis = Par.GetBool(Key + "@Visible1", true);
	SecondVis = Par.GetBool(Key + "@Visible2", true);
	RootVis = Par.GetBool(Key + "@Visible3", true);
	BeamVVis = Par.GetBool(Key + "@Visible4", true);
	FirstThick = int(Par.GetDouble(Key + "@THICK1", 1.));
	SecondThick = int(Par.GetDouble(Key + "@THICK2", 1.));
	RootThick = int(Par.GetDouble(Key + "@THICK3", 1.));
	BeamVThick = int(Par.GetDouble(Key + "@THICK4", 1.));
	FirstColor.R() = float(Par.GetDouble(Key + "/Color1@R", 0.5));
	FirstColor.G() = float(Par.GetDouble(Key + "/Color1@G", 0.5));
	FirstColor.B() = float(Par.GetDouble(Key + "/Color1@B", 0.5));
	FirstColor.A() = 1.;
	SecondColor.R() = float(Par.GetDouble(Key + "/Color2@R", 0.5));
	SecondColor.G() = float(Par.GetDouble(Key + "/Color2@G", 0.5));
	SecondColor.B() = float(Par.GetDouble(Key + "/Color2@B", 0.5));
	SecondColor.A() = 1.;
	RootColor.R() = float(Par.GetDouble(Key + "/Color3@R", 0.5));
	RootColor.G() = float(Par.GetDouble(Key + "/Color3@G", 0.5));
	RootColor.B() = float(Par.GetDouble(Key + "/Color3@B", 0.5));
	RootColor.A() = 1.;
	BeamVColor.R() = float(Par.GetDouble(Key + "/Color4@R", 0.5));
	BeamVColor.G() = float(Par.GetDouble(Key + "/Color4@G", 0.5));
	BeamVColor.B() = float(Par.GetDouble(Key + "/Color4@B", 0.5));
	BeamVColor.A() = 1.;
}


NWeldTrajectories::~NWeldTrajectories(void)
{
}


INT_PTR NWeldTrajectories::AddFour(const BPoint &PFirst, const BPoint &PSecond, const BPoint &PDelta, const BPoint &PRoot)
{
	First.Add(PFirst);
	Second.Add(PSecond);
	Delta.Add(PDelta);
	return Root.Add(PRoot);
}

bool NWeldTrajectories::DrawInt(bool WithNames, const std::vector<BProgVisInterval> & VisInt, int BoldCadr /*= -1*/, double t)
{
	if(VisInt.empty())
	{
		DrawPart(WithNames, 0, int(RElems.GetSize() - 1));
	}
	else
	{
		for (size_t iInt = 0; iInt < VisInt.size(); ++iInt)
		{
			const BProgVisInterval &Int = VisInt[iInt];
			auto First = RElems.FindFirst(Int.GetStart());
			auto Last = RElems.FindLast(Int.GetEnd());
			if(Last < 0 || First < 0)
				continue;
			if(t >= 0. && t < 1.)
				--Last;

			DrawPart(WithNames, First, Last);

			// Draw last partial cadr
			if(t > 0. && t < 1.)
			{
				++Last;
				DrawElemPart(Last, t);
			}
			// Draw bold cadr
			if(Int.Inside(BoldCadr))
			{
				bool IsEqual;
				First = int(RElems.Find(BoldCadr, &IsEqual));
				if(IsEqual)
				{
					glPushAttrib(GL_LINE_BIT | GL_COLOR_BUFFER_BIT);
					glColor4fv(ColorBold.array);
					glLineWidth(3.);
					DrawWithoutNames(First, First);
					glPopAttrib();
				}
			}
		}
	}

	return true;
}

bool NWeldTrajectories::AddElem(const BPointsBuf &Pts1, const BPointsBuf &Pts2, const BPointsBuf &Pts3, const BPointsBuf &Pts4, int FirstName, int SecondName, int k, const NColor &Col)
{
	if(Pts1.GetSize() != Pts2.GetSize())
		return false;

	int PtsStartInd = 1;
	if(VertInd == 0)// First cadr
		PtsStartInd = 0;

	int SInd = VertInd - PtsStartInd;

	for(int i = PtsStartInd; i < Pts1.GetSize(); ++i)
	{
		AddFour(Pts1.GetPoint(i), Pts2.GetPoint(i), Pts3.GetPoint(i), Pts4.GetPoint(i));
		++VertInd;
	}

	RElems.SetElem(ElemsInd, SInd, Pts1.GetSize(), FirstName, SecondName, k);

	++ElemsInd;
	ASSERT(ElemsInd <= RElems.GetSize());
	return true;
}

bool NWeldTrajectories::Create(int ElemsNum, const NColor &CBold)
{
	First.SetSize(0, 2048); 
	Second.SetSize(0, 2048); 
	Root.SetSize(0, 2048); 
	Delta.SetSize(0, 2048); 
	ColorBold = CBold;
	RElems.SetSize(ElemsNum);
	VertInd = 0;
	ElemsInd = 0;
	return true;
}

bool NWeldTrajectories::Close()
{
	RElems.SetSize(ElemsInd);
	return true;
}

void NWeldTrajectories::DrawWithNames(INT_PTR iFirst, INT_PTR iLast)
{
	for(auto i = iFirst; i <= iLast; ++i)
	{
		const RElem &Elem = RElems.GetElem(i);
		glPushName(Elem.FirstName);
		if(Elem.SecondName > 0)
			glPushName(Elem.SecondName);
		glBegin(GL_LINES);
		for(int l = 1; l < Elem.Size; ++l)
		{
			if(FirstVis)
			{
				if(First[Elem.StartInd + l].GetH() > 0. && First[Elem.StartInd + l - 1].GetH() > 0.)
				{
					glVertex3dv(First[Elem.StartInd + l - 1].GetArray());
					glVertex3dv(First[Elem.StartInd + l].GetArray());
				}
			}
			if(SecondVis)
			{
				if(Second[Elem.StartInd + l].GetH() > 0. && Second[Elem.StartInd + l - 1].GetH() > 0.)
				{
					glVertex3dv(Second[Elem.StartInd + l - 1].GetArray());
					glVertex3dv(Second[Elem.StartInd + l].GetArray());
				}
			}
			if(RootVis)
			{
				if(Root[Elem.StartInd + l].GetH() > 0. && Root[Elem.StartInd + l - 1].GetH() > 0.)
				{
					glVertex3dv(Root[Elem.StartInd + l - 1].GetArray());
					glVertex3dv(Root[Elem.StartInd + l].GetArray());
				}
			}
		}
		glEnd();
		glPopName();
		if(Elem.SecondName > 0)
			glPopName();
	}
}

void NWeldTrajectories::DrawPenCyl(INT_PTR iElem, double t)
{
	if(iElem < 0 || iElem >= RElems.GetSize())
		return;

	t = max(t, 0.);
	const RElem &CurElem = RElems.GetElem(iElem);
	int Count = int((CurElem.Size - 1) * t) + 1;
	double tau = t * (CurElem.Size - 1) - double(Count - 1);
	int LastInd = CurElem.StartInd + Count;
	if(LastInd > Root.GetSize())
		return;
	if(LastInd == Root.GetSize())
	{
		if(tau == 0.)
		{
			tau = 1.;
			--LastInd;
		}
		else
			return;
	}
	BPoint Rtau = Root[LastInd - 1] * (1. - tau) + Root[LastInd] * tau;
	BPoint Dtau = Delta[LastInd - 1] * (Delta[LastInd - 1].GetH() * (1. - tau)) + Delta[LastInd] * (Delta[LastInd].GetH() * tau);

	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(RootColor.array);

	double L = sqrt(Dtau.D2());
	if(L > MIND && Dtau.GetH() >= 0.)
	{
		GLUquadricObj *qobj = gluNewQuadric();
		BMatr M;
		BPoint P;
		P.Set(0., 0., -1., 0.);
		M.e0cong(P, Dtau);
		glPushMatrix();
		glTranslated(Rtau.GetX(), Rtau.GetY(), Rtau.GetZ());
		glMultMatrixd(M.GetArray());
		gluDisk(qobj, 0., NWeldCadr::RootR, 10, 1);
		gluCylinder(qobj, NWeldCadr::RootR, NWeldCadr::RootR, L, 10, 1);
		glTranslated(0., 0., L);
		gluDisk(qobj, 0., NWeldCadr::RootR, 10, 1);
		glPopMatrix();
		gluDeleteQuadric(qobj);
	}
	glPopAttrib();
}
void NWeldTrajectories::DrawWithoutNames(INT_PTR iFirst, INT_PTR iLast)
{
	if(iLast < iFirst)
		return;
	const RElem &LastElem = RElems.GetElem(iLast);
	const RElem &FirstElem = RElems.GetElem(iFirst);
	int Count = LastElem.StartInd + LastElem.Size - FirstElem.StartInd;
	if(Count <= 1)
		return;
//	glPushAttrib(GL_LIGHTING_BIT);// This doesn't work (sometimes) on NVIDIA GeForce 8500 GT ???
	glShadeModel(GL_FLAT);
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LINE_BIT);
	if(FirstVis)
	{
		glColor3fv(FirstColor.array);
		glLineWidth(GLfloat(FirstThick));
		glBegin(GL_LINES);
		for(int i = FirstElem.StartInd + 1; i < FirstElem.StartInd + Count ; ++i)
		{
			if(First[i].GetH() < 0. || First[i - 1].GetH() < 0.)
				continue;
			glVertex3dv(First[i - 1].GetArray());
			glVertex3dv(First[i].GetArray());
		}
		glEnd();
	}
	if(SecondVis)
	{
		glColor3fv(SecondColor.array);
		glLineWidth(GLfloat(SecondThick));
		glBegin(GL_LINES);
		for(int i = FirstElem.StartInd + 1; i < FirstElem.StartInd + Count ; ++i)
		{
			if(Second[i].GetH() < 0. || Second[i - 1].GetH() < 0.)
				continue;
			glVertex3dv(Second[i - 1].GetArray());
			glVertex3dv(Second[i].GetArray());
		}
		glEnd();
	}
	if(RootVis)
	{
		glColor3fv(RootColor.array);
		glLineWidth(GLfloat(RootThick));
		glBegin(GL_LINES);
		for(int i = FirstElem.StartInd + 1; i < FirstElem.StartInd + Count ; ++i)
		{
			if(Root[i].GetH() < 0. || Root[i - 1].GetH() < 0.)
				continue;
			glVertex3dv(Root[i - 1].GetArray());
			glVertex3dv(Root[i].GetArray());
		}
		glEnd();
	}
	if(BeamVVis)
	{
		glLineWidth(GLfloat(BeamVThick));
		glBegin(GL_LINES);
		for(auto ElemInd = iFirst; ElemInd <= iLast; ++ElemInd)
		{
			const RElem &Elem = RElems.GetElem(ElemInd);
			int i = Elem.StartInd + Elem.Size - 1;
			if(Root[i].GetH() < 0. || Delta[i].GetH() < 0.)
				continue;
			glColor3fv(RootColor.array);
			glVertex3dv((Root[i] - Delta[i] * Delta[i].GetH()).GetArray());
			glVertex3dv(Root[i].GetArray());
			BPoint V = Delta[i] * Root[i].GetH();
			glColor3fv(BeamVColor.array);
			glVertex3dv((Root[i] - Delta[i] * Delta[i].GetH() - V).GetArray());
			glVertex3dv((Root[i] - Delta[i] * Delta[i].GetH()).GetArray());
		}
		glEnd();
	}
	glPopAttrib();
	glShadeModel(GL_SMOOTH);
//	glPopAttrib();
//	glFinish();
}

void NWeldTrajectories::DrawElemPart(INT_PTR iElem, double t)
{
	// Draw complete lines
	const RElem &CurElem = RElems.GetElem(iElem);
	int Count = int((CurElem.Size - 1) * t) + 1;
	double tau = t * (CurElem.Size - 1) - double(Count - 1);
	int LastInd = CurElem.StartInd + Count;
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT);
	glShadeModel(GL_FLAT);
	glBegin(GL_LINES); 

	if(FirstVis)
	{
		glColor3fv(FirstColor.array);
		int i = CurElem.StartInd + 1;
		for (; i < CurElem.StartInd + Count; ++i)
		{
			if(First[i].GetH() < 0. || First[i - 1].GetH() < 0.)
				continue;
			glVertex3dv(First[i - 1].GetArray());
			glVertex3dv(First[i].GetArray());
		}
		if(First[i].GetH() > 0. && First[i - 1].GetH() > 0.)
		{
			BPoint Rtau = First[LastInd - 1] * (1. - tau) + First[LastInd] * tau;
			glVertex3dv(First[LastInd - 1].GetArray());
			glVertex3dv(Rtau.GetArray());
		}
	}
	if(SecondVis)
	{
		glColor3fv(SecondColor.array);
		int i = CurElem.StartInd + 1;
		for (; i < CurElem.StartInd + Count; ++i)
		{
			if(Second[i].GetH() < 0. || Second[i - 1].GetH() < 0.)
				continue;
			glVertex3dv(Second[i - 1].GetArray());
			glVertex3dv(Second[i].GetArray());
		}
		if(Second[i].GetH() > 0. && Second[i - 1].GetH() > 0.)
		{
			BPoint Rtau = Second[LastInd - 1] * (1. - tau) + Second[LastInd] * tau;
			glVertex3dv(Second[LastInd - 1].GetArray());
			glVertex3dv(Rtau.GetArray());
		}
	}
	if(RootVis)
	{
		glColor3fv(RootColor.array);
		int i = CurElem.StartInd + 1;
		for (; i < CurElem.StartInd + Count; ++i)
		{
			if(Root[i].GetH() < 0. || Root[i - 1].GetH() < 0.)
				continue;
			glVertex3dv(Root[i - 1].GetArray());
			glVertex3dv(Root[i].GetArray());
		}
		if(Root[i].GetH() > 0. && Root[i - 1].GetH() > 0.)
		{
			BPoint Rtau = Root[LastInd - 1] * (1. - tau) + Root[LastInd] * tau;
			glVertex3dv(Root[LastInd - 1].GetArray());
			glVertex3dv(Rtau.GetArray());
		}
	}

	////Draw delta
	//if(RootVis)
	//{
	//	BPoint Rtau = Root[LastInd - 1] * (1. - tau) + Root[LastInd] * tau;
	//	BPoint Dtau = Delta[LastInd - 1] * (1. - tau) + Delta[LastInd] * tau;
	//	glColor3fv(RootColor.array);
	//	if(Root[LastInd].IsPoint())
	//	{
	//		glVertex3dv((Rtau - Dtau).GetArray());
	//		glVertex3dv(Rtau.GetArray());
	//	}
	//}
	glEnd();
	glPopAttrib();

	//int StartInd = CurElem.StartInd + Count - 1;
	//GLfloat t0 = GLfloat(-1000. * tau);
	//GLfloat t1 = t0 + 1001.f;
	//GLuint TexName;
	//GLfloat Texture[8] = { 1., 1., 1., 1.,    0., 0., 0., 0.};
	//	
	//glPushAttrib(GL_ENABLE_BIT);
	//glEnable(GL_TEXTURE_1D);
	//glGenTextures(1, &TexName);
	//glBindTexture(GL_TEXTURE_1D, TexName);
	//glTexImage1D(GL_TEXTURE_1D, 0, 4, 2, 0, GL_RGBA, GL_FLOAT, Texture);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE); 
	//glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glBegin(GL_LINES);
	//glTexCoord1f(t0);
	//glArrayElement(StartInd);
	//glTexCoord1f(t1);
	//glArrayElement(StartInd + 1);
	//glEnd();
	//glDeleteTextures(1, &TexName);
	//glPopAttrib();
}
void NWeldTrajectories::DrawPart(bool WithNames, INT_PTR iFirst, INT_PTR iLast)
{
	if(WithNames)
		DrawWithNames(iFirst, iLast);
	else
		DrawWithoutNames(iFirst, iLast);
}
int NWeldTrajectories::GetCadrLenghs(int AbsCadrNum, double &FirstL, double &SecondL, double &RootL, double &PenDepth) const
{
	//int F = RElems.FindFirst(AbsCadrNum);
	FirstL = 0.;
	SecondL = 0.;
	RootL = 0.;
	PenDepth = 0.;
	auto Last = RElems.FindLast(AbsCadrNum);
	if(Last < 0)
		return 0;
	const RElem &Elem = RElems.GetElem(Last);
	PenDepth = Delta[Elem.StartInd + Elem.Size - 1].GetH();
	PenDepth = max(0., PenDepth);
	for(int l = 1; l < Elem.Size; ++l)
	{
		if(First[Elem.StartInd + l].GetH() > 0. && First[Elem.StartInd + l - 1].GetH() > 0.)
			FirstL += sqrt((First[Elem.StartInd + l - 1] - First[Elem.StartInd + l]).D2());

		if(Second[Elem.StartInd + l].GetH() > 0. && Second[Elem.StartInd + l - 1].GetH() > 0.)
			SecondL += sqrt((Second[Elem.StartInd + l - 1] - Second[Elem.StartInd + l]).D2());

		if(Root[Elem.StartInd + l].GetH() > 0. && Root[Elem.StartInd + l - 1].GetH() > 0.)
			RootL += sqrt((Root[Elem.StartInd + l - 1] - Root[Elem.StartInd + l]).D2());
	}
	return 0;
}
void NWeldTrajectories::Serialize(CArchive& ar)
{
	SerializeElements(ar, &FirstVis, 1);
	SerializeElements(ar, &SecondVis, 1);
	SerializeElements(ar, &RootVis, 1);
	SerializeElements(ar, &BeamVVis, 1);
	SerializeElements(ar, &FirstThick, 1);
	SerializeElements(ar, &SecondThick, 1);
	SerializeElements(ar, &RootThick, 1);
	SerializeElements(ar, &BeamVThick, 1);
	FirstColor.Serialize(ar);
	SecondColor.Serialize(ar);
	RootColor.Serialize(ar);
	BeamVColor.Serialize(ar);
}

BBox NWeldTrajectories::GetGabar(void)
{
	BBox Gabar;
	for(int i = 0; i < First.GetSize(); ++i)
	{
		BPoint *pP = &First.GetAt(i);
		if(pP->IsPoint())
			Gabar.Expand(*pP);
	}
	for(int i = 0; i < Second.GetSize(); ++i)
	{
		BPoint *pP = &Second.GetAt(i);
		if(pP->IsPoint())
			Gabar.Expand(*pP);
	}
	for(int i = 0; i < Root.GetSize(); ++i)
	{
		BPoint *pP = &Root.GetAt(i);
		if(pP->GetH() > 0.) // May be not equal to 1
			Gabar.Expand(*pP);
	}
	return Gabar;
}

void NWeldTrajectories::DrawPenetration(int Cadr, double t)
{
	// Draw penetration cylinder
	auto Ind = RElems.FindLast(Cadr);
	if(t < 0.)
		t = 1.;
	DrawPenCyl(Ind, t);
}
