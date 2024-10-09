// BStlB.cpp: implementation of the BStlB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GLee.h"
#include "GL\GL.h"
#include "GL\GLu.h"
#include "ConstDef.h"
#include "math.h"
#include "NCMProject.h"
#include "BStlB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_SERIAL(BStlB, CObject, 0)
 
BStlB::BStlB() :
	Base(0., 0., 0., 1.), FileName(_T("")), Triangles(nullptr), OrigZero(0., 0., 0., 1.)
{
}

void BStlB::MakeCopy(BStlB &object) const
{
	object.Base = this->Base;
	object.OrigZero = this->OrigZero;
	object.FileName = this->FileName;
	object.Gabar = this->Gabar;
	object.FileTime = this->FileTime;
	if (this->Triangles)
	{
		int count = int(this->Triangles[0]);
		if (object.Triangles)
			delete [] object.Triangles;
		object.Triangles = new GLfloat[18 * count + 1];
		object.Triangles[0] = this->Triangles[0];

		for (int TriInd = 0; TriInd < count; ++TriInd)
		{
			int j = TriInd * 18 + 1;
			for (int k = 0; k < 18; ++k)
				object.Triangles[j + k] = this->Triangles[j + k];
		}
	}
}

void BStlB::Clear()
{
	delete[] Triangles;
	Triangles = nullptr;
	Base.Set(0., 0., 0., 1.);
	FileName.Empty();
	OrigZero.Set(0., 0., 0., 1.);
}

bool BStlB::CreateFrMem(int TriNum, const GLfloat* data)
{
	Base.Set(0., 0., 0., 1.);
	OrigZero.Set(0., 0., 0., 1.);
	FileName = _T("");
	Gabar.Init();
	delete[] Triangles;
	Triangles = new GLfloat[18 * TriNum + 1];
	Triangles[0] = GLfloat(TriNum);
	memcpy_s(Triangles + 1, 18 * TriNum * sizeof(Triangles[0]), data, 18 * TriNum * sizeof(Triangles[0]));
	for (int i = 0; i < TriNum; ++i)
	{
		const int k = i * 6 + 4;
		Gabar.Expand(Triangles[k], Triangles[k + 1], Triangles[k + 2]);
	}
	RecalcNormals();
	return true;
}

BStlB::~BStlB()
{
	delete[] Triangles;
}

bool BStlB::Create(const CString &ProjFileName/* = CString()*/)
{
	bool IsCreated = false;
	Gabar.Init();
	CString NewFileName = FileName;
// Read stl file
	CStdioFile f;
	if(!f.Open(NewFileName, CFile::typeText | CFile::modeRead ))
	{//Try to find stl file in DEMO subdirectory
		int pos = NewFileName.ReverseFind('\\');
		CString ShortFileName = NewFileName.Right(NewFileName.GetLength() - pos - 1);
		NewFileName = NCM_PROJECT.GetDemoPath() + ShortFileName;
		if(!f.Open(NewFileName, CFile::typeText | CFile::modeRead ))
		{//Try to find stl file in the project subdirectory
			int pos = ProjFileName.ReverseFind('\\');
			CString ProjPath;
			if (pos >= 0)
			{
				ProjPath = ProjFileName.Left(pos + 1);
			}
			NewFileName = ProjPath + ShortFileName;

			if (ProjPath.IsEmpty() || !f.Open(NewFileName, CFile::typeText | CFile::modeRead))
			{
				CString mes;
				CString Format;
				if (Format.LoadString(IDS_MES_FILEO))
				{
					mes.Format(Format, NewFileName);
					AfxMessageBox(mes);
				}
				return IsCreated;
			}
		}
	}
	FileName = NewFileName;
	FILETIME Creation, Access, Write;
	if(::GetFileTime(f, &Creation, &Access, &Write))
		FileTime = CTime(Write);

	char s[40], str[1024];
	double x,y,z,xn,yn,zn;

	LPTSTR FileOK = f.ReadString(str, 6);
	f.Close();

	if(!FileOK)
		return IsCreated;

	s[0] = '\0';
	sscanf(str,_T("%s"), s);
	CString Title(s);
	Title.MakeLower();

// find gabar and triangles number
	int TriNum = 0;
	bool Cancelled = false;
	bool BinaryStl = false;
	if(Title != _T("solid"))
		BinaryStl = true;
	else
	{
		f.Open(NewFileName,CFile::typeText | CFile::modeRead );
		f.ReadString(str, 1024);//Skip title

		if(f.ReadString(str, 39))
		{
			if(sscanf(str, _T("%s "), s) != 1)
				BinaryStl = true;
			else
			{
				CString facet(s);
				facet.MakeLower();
				if (facet != _T("facet"))
				BinaryStl = true;
			}
		}
		else
		{
			BinaryStl = true;
		}
		f.Close();
	}
	if(BinaryStl)// f is binary STL
	{
		f.Open(NewFileName,CFile::typeBinary | CFile::modeRead );
		f.Seek(80 + 2, CFile::begin);//Skip title

		char buf[4*3*4+2];
		GLfloat *b;
		bool WarningWasFound = false;
		while(f.Read(buf,4*3*4+2) == 4*3*4+2 && !Cancelled)
		{
			b=(GLfloat *)(buf+2);
			if(!WarningWasFound)
				if(fabs(b[0]*b[0]+b[1]*b[1]+b[2]*b[2] - 1.) > 1.e-4)
				{
					if(AfxMessageBox(
						IDS_MES_STLR,
						MB_YESNO) == IDNO)
					{
						Cancelled = true; // The user cancels reading
						continue;
					}
					WarningWasFound = true;
				}

			b=(GLfloat *)(buf+2+4*3);
			Gabar.Expand(b[0],b[1],b[2]);

			b=(GLfloat *)(buf+2+4*3*2);
			Gabar.Expand(b[0],b[1],b[2]);

			b=(GLfloat *)(buf+2+4*3*3);
			Gabar.Expand(b[0],b[1],b[2]);

			TriNum++;
		}
	}
	else // f is ASCII STL
	{
		f.Open(NewFileName,CFile::typeText | CFile::modeRead );
		f.ReadString(str,1024);//Skip title

		for(int i=0;f.ReadString(str,1024);i++)
		{
			s[0] = '\0';
			sscanf(str, _T("%s"), s);
			CString Title(s);
			Title.MakeLower();
			if (Title == _T("endsolid"))
			{
				if (!f.ReadString(str, 1024))
					break;
				if (!f.ReadString(str, 1024))
					break;
			}

			if(sscanf(str, _T("%s %s %lf %lf %lf"),s,s,&x,&y,&z) != 5)
				break;

			if(!f.ReadString(str,1024))
				break;
			if(!f.ReadString(str,1024))
				break;
			if(sscanf(str,_T("%s %lf %lf %lf"),s,&x,&y,&z) != 4)
				break;
			Gabar.Expand(x,y,z);
	
			if(!f.ReadString(str,1024))
				break;
			if(sscanf(str,_T("%s %lf %lf %lf"),s,&x,&y,&z) != 4)
				break;
			Gabar.Expand(x,y,z);
	
			if(!f.ReadString(str,1024))
				break;
			if(sscanf(str,_T("%s %lf %lf %lf"),s,&x,&y,&z) != 4)
				break;
			Gabar.Expand(x,y,z);
	
			if(!f.ReadString(str,1024))
				break;
			if(!f.ReadString(str,1024))
				break;
			TriNum++;
		}
	}// read  ASCII
	f.SeekToBegin();
//	Gabar = Gabar.Move(Base.GetX(),Base.GetY(),Base.GetZ());
// Fill Triangles
	delete[] Triangles;
	Triangles = NULL;
	if (Cancelled)
		return IsCreated;
	Triangles = new GLfloat[18 * TriNum + 1];

	GLfloat dx = 0.;//(GLfloat)Base.GetX();
	GLfloat dy = 0.;//(GLfloat)Base.GetY();
	GLfloat dz = 0.;//(GLfloat)Base.GetZ();

	int TriInd = 1;
	if(BinaryStl)// f is binary STL ?????
	{
//		f.Open(NewFileName,CFile::typeBinary | CFile::modeRead );
		f.Seek(80 + 2, CFile::begin);//Skip title

		char buf[4*3*4+2];
		GLfloat *b,*n;
		bool WarningWasFound = false;
		while(f.Read(buf,4*3*4+2) == 4*3*4+2 && !Cancelled)
		{
			n=(GLfloat *)(buf+2);
			bool ZeroNormal = false;
			if(fabs(n[0] * n[0] + n[1] * n[1] + n[2] * n[2] - 1.) > 1.e-4)
				ZeroNormal = true;
			b=(GLfloat *)(buf+2+4*3);
			Triangles[TriInd++] = n[0];	Triangles[TriInd++] = n[1];	Triangles[TriInd++] = n[2];
			Triangles[TriInd++] = b[0] + dx;	Triangles[TriInd++] = b[1] + dy;	Triangles[TriInd++] = b[2] + dz;
			BPoint P0(b[0], b[1], b[2], 1.);

			b=(GLfloat *)(buf+2+4*3*2);
			Triangles[TriInd++] = n[0];	Triangles[TriInd++] = n[1];	Triangles[TriInd++] = n[2];
			Triangles[TriInd++] = b[0] + dx;	Triangles[TriInd++] = b[1] + dy;	Triangles[TriInd++] = b[2] + dz;
			BPoint P1(b[0], b[1], b[2], 1.);

			b=(GLfloat *)(buf+2+4*3*3);
			Triangles[TriInd++] = n[0];	Triangles[TriInd++] = n[1];	Triangles[TriInd++] = n[2];
			Triangles[TriInd++] = b[0] + dx;	Triangles[TriInd++] = b[1] + dy;	Triangles[TriInd++] = b[2] + dz;
			BPoint P2(b[0], b[1], b[2], 1.);
			// Remove degraded triangles
			if(ZeroNormal || (P0 - P1).D2() < MIND * MIND || (P0 - P2).D2() < MIND * MIND || (P2 - P1).D2() < MIND * MIND)
				TriInd -= 18;
		}
	}
	else // f is ASCII STL
	{
		f.ReadString(str,1024);//Skip title

		for(int i = 0; f.ReadString(str,1024) && i < TriNum; ++i)
		{
			s[0] = '\0';
			sscanf(str, _T("%s"), s);
			CString Title(s);
			Title.MakeLower();
			if (Title == _T("endsolid"))
			{
				if (!f.ReadString(str, 1024))
					break;
				if (!f.ReadString(str, 1024))
					break;
			}

			sscanf(str,_T("%s %s %lf %lf %lf"),s,s,&xn,&yn,&zn);
			f.ReadString(str,1024);
			f.ReadString(str,1024);
			sscanf(str,_T("%s %lf %lf %lf"),s,&x,&y,&z);
			Triangles[TriInd++] = (GLfloat)xn;	Triangles[TriInd++] = (GLfloat)yn;	Triangles[TriInd++] = (GLfloat)zn;
			Triangles[TriInd++] = (GLfloat)x + dx;	Triangles[TriInd++] = (GLfloat)y + dy;	Triangles[TriInd++] = (GLfloat)z + dz;
			BPoint P0(x, y, z, 1.);
	
			f.ReadString(str,1024);
			sscanf(str,_T("%s %lf %lf %lf"),s,&x,&y,&z);
			Triangles[TriInd++] = (GLfloat)xn;	Triangles[TriInd++] = (GLfloat)yn;	Triangles[TriInd++] = (GLfloat)zn;
			Triangles[TriInd++] = (GLfloat)x + dx;	Triangles[TriInd++] = (GLfloat)y + dy;	Triangles[TriInd++] = (GLfloat)z + dz;
			BPoint P1(x, y, z, 1.);
	
			f.ReadString(str,1024);
			sscanf(str,_T("%s %lf %lf %lf"),s,&x,&y,&z);
			Triangles[TriInd++] = (GLfloat)xn;	Triangles[TriInd++] = (GLfloat)yn;	Triangles[TriInd++] = (GLfloat)zn;
			Triangles[TriInd++] = (GLfloat)x + dx;	Triangles[TriInd++] = (GLfloat)y + dy;	Triangles[TriInd++] = (GLfloat)z + dz;
			BPoint P2(x, y, z, 1.);

			f.ReadString(str,1024);
			f.ReadString(str,1024);
			// Remove degraded triangles
			if((P0 - P1).D2() < MIND * MIND || (P0 - P2).D2() < MIND * MIND || (P2 - P1).D2() < MIND * MIND)
				TriInd -= 18;
		}
	}// read  ASCII
	Triangles[0] = GLfloat((TriInd - 1) / 18);
	OrigZero.Set(0., 0., 0., 1.);
	bool DoRecalcNormals = true;// TODO: пористая заготовка
	if(DoRecalcNormals)
		RecalcNormals();
	IsCreated = true;

	return IsCreated;
}



void BStlB::Serialize(CArchive &ar, double InFileVersion)
{
	SerializeElements(ar, &FileName, 1);
	if(ar.IsLoading() && InFileVersion < atof("3.2"))
	{
		double pbuf;
		SerializeElements(ar, &pbuf, 1);
	}
	SerializeElements(ar, &Base, 1);
	if(!(ar.IsLoading() && InFileVersion < atof("3.8")))
		SerializeElements(ar, &OrigZero, 1);
	if(ar.IsLoading())
		Create(ar.GetFile()->GetFilePath());
}

bool BStlB::CreateEx(const CString &name)
{
	FileName = name;
	return Create();
}

void BStlB::SetFileName(const CString & Name)
{
	FileName = Name;
}

const CString & BStlB::GetFileName() const
{
	return FileName;
}


const GLfloat * BStlB::GetTriangles(void) const
{
	return Triangles;
}
GLfloat *BStlB::MakeOneSection(const BPoint &P, const BPoint &V, bool Trimed, const BPoint &AddV) const
{// Make section by the plane containing P and normal to V
	if(!Triangles)
		return NULL;
	CArray<BPoint, const BPoint &> Lines;
	Lines.SetSize(0, 4096);
	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint p0(BPoint(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.));
		BPoint p1(BPoint(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.));
		BPoint p2(BPoint(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.));
		double d0 = (p0 - P) * V;
		double d1 = (p1 - P) * V;
		double d2 = (p2 - P) * V;
		if(fabs(d0) < MIND) d0 = 0.;
		if(fabs(d1) < MIND) d1 = 0.;
		if(fabs(d2) < MIND) d2 = 0.;
		int Count = 0;
		if(FindInt(p0, p1, d0, d1, Lines))
			++Count;
		if(FindInt(p1, p2, d1, d2, Lines))
			++Count;
		if(FindInt(p2, p0, d2, d0, Lines))
			++Count;
		if(Count != 2)
		{
			Lines.SetSize(Lines.GetSize() - Count);
			continue;
		}
		// Orient line
		BPoint n(Triangles[i + 0], Triangles[i + 1], Triangles[i + 2], 0.); 
		if((n % V) * (Lines[Lines.GetSize() - 1] - Lines[Lines.GetSize() - 2]) < 0)
		{
			BPoint b = Lines[Lines.GetSize() - 1];
			Lines[Lines.GetSize() - 1] = Lines[Lines.GetSize() - 2];
			Lines[Lines.GetSize() - 2] = b;
		}
		if(Trimed)
		{
			// Trim line
			BPoint N(AddV % V);
			BPoint q0(Lines[Lines.GetSize() - 2] - P);
			BPoint q1(Lines[Lines.GetSize() - 1] - P);
			double dt0 = N * q0;
			double dt1 = N * q1;
			if(dt1 >= 0. && dt0 >= 0.)
				continue;
			if(dt1 <= 0. && dt0 <= 0.)
			{
				Lines.SetSize(Lines.GetSize() - 2);
				continue;
			}
			// Find intersection
			double dd0 = fabs(dt0);
			double dd1 = fabs(dt1);
			double t = dd0 / (dd0 + dd1);
			BPoint qi = q0 * (1. - t) + q1 * t + P;
			if(dt0 > 0.)
				Lines.SetAt(Lines.GetSize() - 1, qi);
			else
				Lines.SetAt(Lines.GetSize() - 2, qi);
		}
	}	

	auto NNum = Lines.GetSize();
	if(NNum <= 0)
		return NULL;
	GLfloat *pC = new GLfloat[NNum * 3 + 1];
	pC[0] = GLfloat(NNum);
	for(int i = 0, j = 0; i < NNum; ++i)
	{
		BPoint p = Lines[i];
		pC[++j] = GLfloat(p.GetX());
		pC[++j] = GLfloat(p.GetY());
		pC[++j] = GLfloat(p.GetZ());
	}
	return pC;
}

bool BStlB::FindInt(const BPoint &p0, const BPoint &p1, double d0, double d1, CArray<BPoint, const BPoint &> &Lines) const
{
	if(d0 * d1 > 0.)
		return false;
	if(d0 == 0.)
	{
		Lines.Add(p0);
		return true;
	}
	if(d1 == 0.)
		return false;

	d0 = fabs(d0);
	d1 = fabs(d1);
	double t = d0 / (d0 + d1);

	Lines.Add(p0 * (1. - t) + p1 * t);

	return true;
}


void BStlB::Transform(const BMatr &M)
{
	if(!Triangles)
		return;
	bool ScaleChanged = (fabs(M.GetRow(0).D2() - 1.) > MIND * MIND)
						|| (fabs(M.GetRow(1).D2() - 1.) > MIND * MIND)
						|| (fabs(M.GetRow(2).D2() - 1.) > MIND * MIND);
	bool NotOrt = (fabs(M.GetRow(0).D2() - M.GetRow(1).D2()) > MIND * MIND)
					|| (fabs(M.GetRow(0).D2() - M.GetRow(2).D2()) > MIND * MIND);
	Gabar.Init();
	OrigZero = OrigZero * M;
	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int j = TriInd * 18 + 1;
		if(!NotOrt)
		{
			for(int k = 0; k < 18; k += 6)
			{
				int i = j + k;
				BPoint Nc = (BPoint(Triangles[i + 0], Triangles[i + 1], Triangles[i + 2], 0.)) * M;
				if(ScaleChanged)
					Nc = Nc * (1. / sqrt(Nc.D2()));
				Triangles[i + 0] = GLfloat(Nc.GetX());
				Triangles[i + 1] = GLfloat(Nc.GetY());
				Triangles[i + 2] = GLfloat(Nc.GetZ());
				BPoint Pc = (BPoint(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.)) * M; 
				Triangles[i + 3] = GLfloat(Pc.GetX());
				Triangles[i + 4] = GLfloat(Pc.GetY());
				Triangles[i + 5] = GLfloat(Pc.GetZ());
				Gabar.Expand(Pc);
			}
		}
		else
		{
			BPoint P[3];
			for(int k = 0; k < 18; k += 6)
			{
				int i = j + k;
				int l = k / 6;
				P[l] = (BPoint(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.)) * M; 
				Triangles[i + 3] = GLfloat(P[l].GetX());
				Triangles[i + 4] = GLfloat(P[l].GetY());
				Triangles[i + 5] = GLfloat(P[l].GetZ());
				Gabar.Expand(P[l]);
			}
			BPoint Nc = (BPoint(Triangles[j + 0], Triangles[j + 1], Triangles[j + 2], 0.)) * M;
			BPoint NcN = (P[1] - P[0]) % (P[2] - P[0]);
			double D = NcN.D2();
			if(D < MIND * MIND)
				NcN = BPoint(0., 0., 1., 0.);
			else
				NcN = NcN * (1. / sqrt(D));
			if(NcN * Nc < 0.)
				NcN = NcN * (-1.);
			for(int k = 0; k < 18; k += 6)
			{
				int i = j + k;
				Triangles[i + 0] = GLfloat(NcN.GetX());
				Triangles[i + 1] = GLfloat(NcN.GetY());
				Triangles[i + 2] = GLfloat(NcN.GetZ());
			}
		}

	}
}
void BStlB::WriteSTL(CStdioFile & f) const
{
	f.SeekToBegin();
	char s[5*16+1] = "STL export                                                                      ";
	f.Write(s, 5*16);// title

	__int32 TriNum = int(Triangles[0]);
	f.Write(&TriNum, 4);

	char buf[2];
	GLfloat fbuf[4*3];
	buf[0] = '\x0';
	buf[1] = '\x0';
	for(int TriInd = 0; TriInd < TriNum; ++TriInd)
	{
		int j = TriInd * 18 + 1;
		// Normal
		fbuf[0] = Triangles[j + 0];
		fbuf[1] = Triangles[j + 1];
		fbuf[2] = Triangles[j + 2];
		// Vertexes
		for(int k = 0; k < 18; k += 6)
		{
			int i = j + k;
			fbuf[k / 2 + 3] = Triangles[i + 3];
			fbuf[k / 2 + 4] = Triangles[i + 4];
			fbuf[k / 2 + 5] = Triangles[i + 5];
		}
		f.Write(fbuf, 4 * 3 * 4);
		f.Write(buf, 2);
	}
}

const BPoint BStlB::RayCasting(const BPoint & P, const BPoint & V, double MinW, BPoint &N) const
{
// Find the intersection point nearest to P in the V direction
	// N - external normal in P (from Stl)
	if(!Triangles)
		return BPoint(0., 0., 0., 1.);
	double MinT = 1.e12;
	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint n0(Triangles[i + 0], Triangles[i + 1], Triangles[i + 2], 0.);
		BPoint p0(BPoint(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.) - P);
		BPoint p1(BPoint(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.) - P);
		BPoint p2(BPoint(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.) - P);
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		n = ((n * n0 < 0) ? -1. : 1.) * n;
		double z =	n * V;
		if(fabs(z) < MINAR)
			continue;
		z = 1./z;
		double t = p0 * n * z;
		if(t < 0. || t > MinT)
			continue;
		BPoint IntP(V * t);
		if(((p1 - p0) % (p0 - IntP)) * n > 0 && IntP.LineD2(p1, p0) > MinW * MinW)
			continue;
		if(((p2 - p1) % (p1 - IntP)) * n > 0 && IntP.LineD2(p2, p1) > MinW * MinW)
			continue;
		if(((p0 - p2) % (p2 - IntP)) * n > 0 && IntP.LineD2(p0, p2) > MinW * MinW)
			continue;
		MinT = t;
		N = n0;
	}	
	if(MinT >= 1.e6)
		return 	BPoint(0., 0., 0., -1.);

	if(V * N > 0.)
		N = N * (-1.);
	return V * MinT + P;
}

size_t BStlB::RayShreddingTri(const BPoint& P, const BPoint& V, double MinW, std::vector<BPoint>& Pts) const
{
	BPoint N;
	BPoint Pi = P;
	BPoint Pt(0., 0., 0., 1.);
	while ((Pt = RayCasting(Pi, V, MinW, N)).IsPoint())
	{
		Pts.push_back(Pt);
		Pi = Pt + V * 0.001;
	}
	return Pts.size();
}

double BStlB::GetVolume(void)
{
	if(!Triangles)
		return 0.;

	double V = 0.;
	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint n0(Triangles[i + 0], Triangles[i + 1], Triangles[i + 2], 0.);
		BPoint p0(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.);
		BPoint p1(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.);
		BPoint p2(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.);
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		n = ((n * n0 < 0) ? -1. : 1.) * n;
		V += (1./6.) * (p0 * n);
	}	
	return V;
}

const BBox& BStlB::GetGabar() const
{
	return Gabar;
}

void BStlB::MoveBase(double x, double y, double z)
{
	Base.Set(x, y, z, 1.);
}
void BStlB::RecalcNormals()
{
	if(!Triangles)
		return;

	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint n0(Triangles[i + 0], Triangles[i + 1], Triangles[i + 2], 0.);
		BPoint p0(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.);
		BPoint p1(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.);
		BPoint p2(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.);
		//Calculate normal
		BPoint n = (p1 - p0) % (p2 - p0);
		n = ((n * n0 < 0) ? -1. : 1.) * n;
		double d2 = n.D2();
		if(d2 < MIND * MIND)
			n = n0;
		else
			n = n * (1. / sqrt(d2));
		Triangles[i + 0] = float(n.GetX());
		Triangles[i + 1] = float(n.GetY());
		Triangles[i + 2] = float(n.GetZ());
	}	
}

double BStlB::FindMaxZDist(void)
{
	double Dist = 0;
	if(!Triangles)
		return Dist;
	BPoint O(0., 0., 0., 1.);
	BPoint Z(0., 0., 1., 1.);
	for(int TriInd = 0; TriInd < int(Triangles[0]); ++TriInd)
	{
		int i = TriInd * 18 + 1;
		BPoint p0(Triangles[i + 3], Triangles[i + 4], Triangles[i + 5], 1.);
		BPoint p1(Triangles[i + 9], Triangles[i + 10], Triangles[i + 11], 1.);
		BPoint p2(Triangles[i + 15], Triangles[i + 16], Triangles[i + 17], 1.);
		double d = p0.LineD2(O, Z);
		Dist = max(Dist, d);
		d = p1.LineD2(O, Z);
		Dist = max(Dist, d);
		d = p2.LineD2(O, Z);
		Dist = max(Dist, d);
	}	
	return sqrt(Dist);
}