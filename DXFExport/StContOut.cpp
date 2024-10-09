// StContOut.cpp: implementation of the StContOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResLocal.h"
#include "NCMProject.h"
#include "NSurfSettings.h"
#include "NCadrGeom.h"
#include "math.h"
#include "BMatr.h"
#include "BPoint.h"
#include "BFileDialogWPath.h"
#include "N_dl_import.h"
#include "src\DL_Dxf.h"
#include "StContOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
    
StContOut::StContOut()
{
	ErrorCode = 0;
}

StContOut::~StContOut()
{

}

size_t StContOut::ReadFromFile(NCadrGeom *& OutArray, const CString &filename)
{
	int index = filename.ReverseFind('.');
	if (index <= 0)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	CString RecExt = filename.Right(filename.GetLength() - index - 1);
	RecExt.MakeUpper();

	std::list<NCadrGeom> ResList;
	size_t Res = 0;
	if (RecExt == "F2C")
		Res = ReadFromFileF2C(ResList, filename);
	else if (RecExt == "STC")
		Res = ReadFromFileStC(ResList, filename);
	else if (RecExt == "DXF")
		Res = ReadFromFileDXF(ResList, filename);
	else
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	if (Res == 0)
		return 0;
	if ((ResList.front().GetB() - ResList.back().GetE()).D2() > 1.e-12)
	{
		NCadrGeom Geom;
		Geom.Set(line, ResList.back().GetE(), ResList.front().GetB());
		ResList.push_back(Geom);
	}

	auto OASize = ResList.size();
	if (OASize < 3)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}

	///Detect direction of contur (cwarc or ccwarc)
	double SumConer = 0;
	NCadrGeom* pPrevGeom = &ResList.back();
	for (auto it = ResList.begin(); it != ResList.end(); it++)
	{
		BPoint P0(pPrevGeom->GetB());
		BPoint P1(pPrevGeom->GetE());
		BPoint P2(it->GetE());
		BPoint V0 = P1 - P0;
		BPoint V1 = P2 - P1;
		double d0 = sqrt(V0.D2());
		double d1 = sqrt(V1.D2());
		if (d1 == 0)
			continue;
		if (d0 == 0 )
		{
			ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
			return 0;
		}
		double tmp1 = V0.GetX() * V1.GetY() - V0.GetY() * V1.GetX();
		double tmp = (V0 * V1) / (d0 * d1);
		if (tmp > 1.)
			tmp = 1.;
		tmp = acos(tmp);
		if (tmp1 < 0.)
			tmp *= -1.;
		SumConer += (tmp);
		pPrevGeom = &(*it);
	}

	//const double PI = 3.1415926535897932384626433832795;
	//if (fabs(fabs(SumConer) - 2. * PI) > 1.e-2)  //check that summ of all coners of contur = 2PI
	//{
	//	ErrorCode = IDS_STATE_NOTE_Electr;//INSERT ID
	//	return 0;
	//}
	if (SumConer < 0.)
	{
		// Invert contour
		for (auto it = ResList.begin(); it != ResList.end(); it++)
			it->Reverse();
		ResList.reverse();
	}
	// Fill OutArray
	OutArray = new NCadrGeom[OASize];
	int i = 0;
	for (auto it = ResList.cbegin(); it != ResList.cend(); it++, i++)
		OutArray[i] = *it;

	return int(OASize);

}

size_t StContOut::ReadFromStr(NCadrGeom*& OutArray, size_t Size, CString& ContStr)
{
	std::list<NCadrGeom> ResList;
	ReadFromStrStC(ResList, Size, ContStr);
	// Fill OutArray
	OutArray = new NCadrGeom[Size];
	int i = 0;
	for (auto it = ResList.cbegin(); it != ResList.cend(); it++, i++)
		OutArray[i] = *it;
	return ResList.size();
}

int StContOut::Load(NCadrGeom *& OutArray, CString &filename)
{
	// Returns -1 if user refused from loading
	// 0 - if loading failed 
	// and loaded cadrs number if success
	if(filename.IsEmpty())
	{
		CString filter;
		filter.LoadString(IDS_FILESIMPORT_FILTER);
		BFileDialogWPath fd(_T("LastStockPath"), TRUE,NULL,NULL,NULL,filter); 
		if(fd.DoModal() == IDOK)
			filename = fd.GetPathName();
		else 
			return -1;
	}
	return int(ReadFromFile(OutArray, filename));
}
size_t StContOut::ReadFile(CString &Text, const CString &filename)
{
	// Returns :
	// 0 - if loading failed 
	// and loaded cadrs number if success
	// Text and filename may be the same strings
	CStdioFile f;
	if (!f.Open(filename, CFile::typeText | CFile::modeRead)) //открываем входной файл
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	const int StrLen = 256;
	char str[StrLen];
	// Read to Text and calculate file size
	Text.Empty();
	int FileSize = 0;
	while(f.ReadString(str, StrLen))
	{
		Text += CString(str) + _T("\n");
		++FileSize;
	}
	return FileSize;
}
size_t StContOut::ReadFromStrF2C(std::list<NCadrGeom>& ResList, int Size, CString &Text)
{
	return 0;
}
size_t StContOut::ReadFromStrDXF(std::list<NCadrGeom>& ResList, int Size, CString &Text)
{
	return 0;
}
size_t StContOut::ReadFromStrStC(std::list<NCadrGeom>& ResList, size_t Size, CString &Text)
{
	int Pos = Text.Find('G', 1);
	CString str = Text.Left(Pos);
	Text.Delete(0, str.GetLength());
	if (Text.IsEmpty())
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	Pos = Text.Find('G', 1);
	CString str1 = Pos > 0 ? Text.Left(Pos) : Text;
	Text.Delete(0, str1.GetLength());

	while(true)
	{
		int g;
		double xb, yb, i, j, xe, ye;
		int tn = sscanf(str, "G%dX%lfY%lfI%lfJ%lf", &g, &xb, &yb, &i, &j);
		int tn1 = sscanf(str1, "G%dX%lfY%lfI%lfJ%lf", &g, &xe, &ye, &i, &j);
		switch(g)
		{
		case 0:
			if(tn < 3 || tn1 < 3)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			break;
		case 1:
			if(tn < 3 || tn1 < 3)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			ResList.push_back(NCadrGeom());
			ResList.back().Set(line, xb, yb, 0., xe, ye, 0.);
			break;
		case 2:
			if(tn < 3 || tn1 < 5)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			ResList.push_back(NCadrGeom());
			ResList.back().Set(cwarc, xb, yb, 0., xe, ye, 0., i, j, 0., XY);
			break;
		case 3:
			if(tn < 3 || tn1 < 5)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			ResList.push_back(NCadrGeom());
			ResList.back().Set(ccwarc, xb, yb, 0., xe, ye, 0., i, j, 0., XY);
			break;
		default:
		{
			ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
			return 0;
		}
		}
		if(Text.IsEmpty())
			break;
		str = str1;
		Pos = Text.Find('G', 1);
		str1 = Pos > 0 ? Text.Left(Pos) : Text;
		Text.Delete(0, str1.GetLength());
	}
	return ResList.size();
}

size_t StContOut::ReadFromFileStC(std::list<NCadrGeom>& ResList, const CString &filename)
{
	// Returns :
	// 0 - if loading failed 
	// and loaded cadrs number if success
	CString Text;
	size_t FileSize = ReadFile(Text, filename);
	if(FileSize <= 0)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	return ReadFromStrStC(ResList, FileSize, Text);
}

size_t StContOut::ReadFromFileF2C(std::list<NCadrGeom>& ResList,const CString &filename)
{
	// Returns :
	// 0 - if loading failed 
	// and loaded cadrs number if success
	CStdioFile f;
	if(!f.Open( filename, CFile::typeText | CFile::modeRead )) //открываем входной файл
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	const int StrLen = 128;
	char str[StrLen];
	// Calculate geom elems number
	int FileSize = 0;
	while(f.ReadString(str, StrLen))
	{
		CString line(str);
		int ind = line.Find('*');
		if(ind < 0)
			continue;
		line.Delete(0, ind + 1);
		if(line.Left(4) == "Arc:" || line.Left(5) == "Line:")
			++FileSize;
	}

	// Read file
	f.SeekToBegin();
	CString cl;
	if(!f.ReadString(cl))
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	if(cl != "F2C 1.0")
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}

	CString Whole;
	CString Line;
	while(f.ReadString(Line))
		Whole += Line + " ";
	f.Close();

	BPoint Xa(1.,0.,0.,0.), Ya(0.,1.,0.,0.), Za(0.,0.,1.,0.);
	bool Stop = false;
	double AngleSign = 1;
	while(!Stop)
	{
		CString Block;
		int ind = Whole.Find('*');
		if(ind >= 0)
		{
			Block = Whole.Left(ind);
			Whole.Delete(0, ind + 1);
		}
		else
		{
			Block = Whole;
			Stop = true;
		}
		if(Block.Left(9) == "Sketcher:")
		{
			Block.Delete(0,9);
			int k = Block.Find("PlanePoints:");
			if(k < 0)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			Block.Delete(0, k + 12);

			double x0, y0, z0, x1, y1, z1, x2, y2, z2;
			if(sscanf(Block, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
				&x0, &y0, &z0, &x1, &y1, &z1, &x2, &y2, &z2) != 9)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			Xa.Set(x0, y0, z0, 0.);
			Ya.Set(x1, y1, z1, 0.);
			Za.Set(x2, y2, z2, 0.);
			if((Xa % Ya).GetY() > 0.)
				AngleSign = -1.;
			continue;
		}
		if(Block.Left(5) == "Line:")
		{
			Block.Delete(0, 5);
			int k = Block.Find("Points=");
			if(k < 0)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			Block.Delete(0, k + 7);
			double x0, y0, x1, y1;
			if(sscanf(Block, "%lf %lf %lf %lf",
				&x0, &y0, &x1, &y1) != 4)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			if(fabs(x0) < 1.e-5) x0 = 0.;
			if(fabs(y0) < 1.e-5) y0 = 0.;
			if(fabs(x1) < 1.e-5) x1 = 0.;
			if(fabs(y1) < 1.e-5) y1 = 0.;
			NCadrGeom NewGeom;
			NewGeom.Set(line, 0., 0., 0., 0., 0., 0., 0., 0., 0., XZ);
			if(!ResList.empty())
				NewGeom.SetB(ResList.back().GetE());
			else
				NewGeom.SetB(Xa * x0 + Ya * y0);
			ResList.back().SetE(Xa * x1 + Ya * y1);
			ResList.push_back(NewGeom);
			ResList.back().Tr(BMatr().RotX(0., 0., 0., -90.));
		}
		if(Block.Left(4) == "Arc:")
		{
			Block.Delete(0, 4);
			int k = Block.Find("Center=");
			if(k < 0)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			Block.Delete(0, k + 7);
			double xC, yC;
			if(sscanf(Block, "%lf %lf",
				&xC, &yC) != 2)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}

			k = Block.Find("R=");
			if(k < 0)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			Block.Delete(0, k + 2);
			double R;
			if(sscanf(Block, "%lf", &R) != 1)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}

			k = Block.Find("Angles=");
			if(k < 0)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			Block.Delete(0, k + 7);
			double aS, aC;
			if(sscanf(Block, "%lf %lf", &aS, &aC) != 2)
			{
				ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
				return 0;
			}
			aS *= AngleSign;
			aC *= AngleSign;
			BPoint C(Xa * xC + Ya * yC + BPoint(0., 0., 0., 1.));
			BPoint Rv(R, 0., 0., 0.);
			BPoint Rs(Rv * BMatr().RotY(0., 0., 0., -aS));
			BPoint Re(Rs * BMatr().RotY(0., 0., 0., -aC));

			BPoint Ps;
			if(!ResList.empty())
				Ps = ResList.back().GetE();
			else
				Ps = C + Rs;
			BPoint Pe = C + Re;
			Ps.GravTo0();
			Pe.GravTo0();
			enum curve type = aC > 0. ? cwarc : ccwarc;
			ResList.push_back(NCadrGeom());
			ResList.back().Set(type
				, Ps.GetX(), Ps.GetY(), Ps.GetZ()
				, Pe.GetX(), Pe.GetY(), Pe.GetZ()
				, -Rs.GetX(), -Rs.GetY(), -Rs.GetZ()
				, XZ);
			ResList.back().Tr(BMatr().RotX(0., 0., 0., -90.));
		}
	}
	return ResList.size();
}

StContOut::StContOut(const NCadrGeomArray *InArray)
{
	ErrorCode = 0;

   	FILE *My_DXF;  //создаем поток для исходящего файла
	CString filename;             //Описываем переменную которая будет харнить имя(задается пользователем) исходного DXF файла
  	CString filter,ext = "StC"; // описываем переменные которые будут использоваться при запросе имени исходного файла DXF
	INT_PTR Kol_Cadr = InArray->GetSize(); // описание переменной, которая будет хранить информацию о количестве кадров,и передаем ей значение
	filter = "*." + ext +"|*." + ext + "|"; // создание фильтра для ограничения на расширение сходящего DXF файла
	BFileDialogWPath fd(_T("LastStockPath"), FALSE,ext,filename,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter); //диалгоговое окно запроса имеени исходящего файла
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;

	double xe, ye, ze;
	bool firstCadr = true;
	My_DXF = fopen( filename, "w" ); //открываем выходной файл
	for(int n = 0; n < Kol_Cadr; ++n)
	{
		NCadrGeom *pCur = InArray->GetAt(n);

		double xb, yb, zb;
		pCur->GetB(&xb, &yb, &zb);
		pCur->GetE(&xe, &ye, &ze);
		double i, j, k;
		pCur->GetI(&i, &j, &k);

		if((pCur->IsFast()))
		{
			fprintf(My_DXF,"G0X%lfY%lf\n", xe, ye);
			firstCadr = false;
			continue;
		}
		if((pCur->IsLine()))
		{
			if(firstCadr)
			{
				fprintf(My_DXF,"G0X%lfY%lf\n", xb, yb);
				firstCadr = false;
			}
			fprintf(My_DXF,"G1X%lfY%lf\n", xe, ye);
			continue;
		}


		if(pCur->IsCWArc())
		{
			if(firstCadr)
			{
				fprintf(My_DXF,"G0X%lfY%lf\n", xb, yb);
				firstCadr = false;
			}
			fprintf(My_DXF,"G2X%lfY%lfI%lfJ%lf\n", xe, ye, i, j);
			continue;
		}
		if(pCur->IsCCWArc())
		{
			if(firstCadr)
			{
				fprintf(My_DXF,"G0X%lfY%lf\n", xb, yb);
				firstCadr = false;
			}
			fprintf(My_DXF,"G3X%lfY%lfI%lfJ%lf\n", xe, ye, i, j);
			continue;
		}
	}
	fclose( My_DXF );
}

int StContOut::ReadFromFileDXFPoly(NCadrGeom *& OutArray,const CString &filename)
{
	// For TECHTRAN DXF ONLY

	// Returns :
	// 0 - if loading failed 
	// and loaded cadrs number if success
	CStdioFile f,log;
	if(!f.Open( filename, CFile::typeText | CFile::modeRead )) //открываем входной файл
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}

	const int StrLen = 128;
	char str[StrLen];
	
	f.SeekToBegin();
	// Calculate file size
	int FileSize = 0;
	int countPolyline = 0;
	while(f.ReadString(str, StrLen) && countPolyline < 2)
	{
		if(CString(str) == "VERTEX\n")
			++FileSize;
		if(CString(str) == "POLYLINE\n")
			++countPolyline;
	}

	if(FileSize < 3)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	OutArray = new NCadrGeom[FileSize];
	double *ArrX = new double[FileSize];
	double *ArrY = new double[FileSize];
	double *Ang = new double[FileSize]; //Coner of arc-section

	// Read file
	f.SeekToBegin();
	bool stop = false;
	bool VertexStarted = false;
	bool XCoord = false;
	bool YCoord = false;
	bool Bulge = false;
	double x = 0., y = 0., a = 0.;
	countPolyline = 0;
	for(int ind = 0; (!stop) && f.ReadString(str, StrLen);)
	{
		CString CurStr(str);
		if(CurStr == "POLYLINE\n")
		{
			++countPolyline;
			if(countPolyline > 1) 
				break;
		}
		if(CurStr == "VERTEX\n")
		{
			if(VertexStarted)
			{
				Ang[ind] = 0.;
				++ind;
			}
			VertexStarted = true;
			continue;
		}
		if(VertexStarted)
		{
			if(CurStr == " 10\n")
			{
				XCoord = true;
				continue;
			}
				if(XCoord)
				{
					XCoord = false;
					int tn = sscanf(str, "%lf", &x);
					if(tn != 1)
					{
						delete [] ArrX;
						delete [] ArrY;
						delete [] Ang;
						{
							ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
							return 0;
						}
					}
				}
			if(CurStr == " 20\n")
			{
				YCoord = true;
				continue;
			}
				if(YCoord)
				{
					YCoord = false;
					int tn = sscanf(str, "%lf", &y);
					if(tn != 1)
					{
						delete [] ArrX;
						delete [] ArrY;
						delete [] Ang;
						{
							ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
							return 0;
						}
					}
					//if(y < 0.)               
					//	return 0;
					ArrX[ind] = x;
					ArrY[ind] = y;
				}
			if(CurStr == " 42\n")
			{
				Bulge = true;
				continue;
			}
				if(Bulge)
				{
					Bulge = false;
					VertexStarted = false;
					int tn = sscanf(str, "%lf", &a);
					if(tn != 1)
					{
						delete [] ArrX;
						delete [] ArrY;
						delete [] Ang;
						{
							ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
							return 0;
						}
					}
					Ang[ind] = a;
					++ind;
				}
		}
	}
	f.Close();

	BPoint P0,P1,P2,V0,V1; 
///Detect direction contur cwarc or ccwarc
	double PI = 3.1415926535897932384626433832795;
	double SumConer = 0.;
	bool go_cwarc = true; 
	int LastInd = FileSize - 1;
	if(fabs(ArrX[0] - ArrX[LastInd]) < 1.e-4 && fabs(ArrY[0] - ArrY[LastInd]) < 1.e-4)
		--LastInd ;
	
	for(int i = 0; i <= LastInd; ++i)
	{
		if(i == 0) //if its first point than take P0 = Last point
           P0.Set(ArrX[LastInd], ArrY[LastInd], 0., 1.);
		else 
			P0.Set(ArrX[i-1],ArrY[i-1], 0.,1.);
		P1.Set(ArrX[i],ArrY[i], 0.,1.);
		P2.Set(ArrX[(i + 1) % (LastInd + 1)],ArrY[(i + 1) % (LastInd + 1)], 0.,1.);
		V0 = P1 - P0;
		V1 = P2 - P1;
		double d0 = sqrt(V0.D2());
		double d1 = sqrt(V1.D2());
		double tmp, tmp1;
		if(d0 == 0 || d1 == 0)
		{
			ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
			return 0;
		}
		tmp1 = V0.GetX()*V1.GetY() - V0.GetY()*V1.GetX();
		tmp = (V0 * V1)/ (d0 * d1);
		if(tmp > 1.)
			tmp = 1.;
		tmp = acos(tmp);
		if(tmp1 < 0.)
			tmp *= -1.;
		SumConer += (tmp);
	}
	if(fabs(fabs(SumConer) - 2. * PI) > 1.e-6)  //check that summ of all coners of contur = 2PI
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}

	int beginPoint = 0;//begin point of cadr
	int endPoint = FileSize -1;//end point of cadr
	int numberArc, curentPoint, nextPoint, cadr_count = 0;

	if(SumConer > 0.) //if ccwarc
	{
		beginPoint = FileSize -1;
		endPoint = 0;	
		go_cwarc = false;
	}
	
	for(int i = beginPoint; (beginPoint > endPoint)? i > endPoint : i < endPoint; (beginPoint > endPoint)? --i : ++i)
	{
		if(go_cwarc)
		{
			curentPoint = i;
			nextPoint = i+1;
			numberArc = i;
		
		}
		else
		{
			curentPoint = i;
			nextPoint = i-1;
			numberArc = i-1;
			Ang[numberArc] *= -1.;//инвертируем направление дуги
		}

		if(Ang[numberArc] == 0.) //if cadr is line
			OutArray[cadr_count].Set(line, ArrY[curentPoint], ArrX[curentPoint], 0., ArrY[nextPoint], ArrX[nextPoint], 0.);
		else //if cadr is arc 
		{
				BPoint C;
				P0.Set(ArrX[curentPoint],ArrY[curentPoint], 0.,1.); 
				P1.Set(ArrX[nextPoint],ArrY[nextPoint], 0.,1.);
				V0 = P1 - P0;
				double d = sqrt(V0.D2());
				V0 = V0 * (1. / d);
				double conerArad = 4. * tanh(Ang[numberArc]);
				double conerBrad = (PI / 2.) - (fabs(conerArad) / 2.);
				double Radius =sqrt ((d * d) / (2. - 2. * cos(fabs(conerArad))));
				BMatr matrix;
				double conerB = 180. * conerBrad / PI;
				if(Ang[numberArc] < 0.)
					conerB = -conerB;
				matrix.RotZ(P0,conerB);
				V0 = V0 * matrix;
				C = P0 + (V0 * Radius);
				V0 = C - P0;
				if( Ang[numberArc] > 0.)
					OutArray[cadr_count].Set(cwarc, P0.GetY(), P0.GetX(), 0., P1.GetY(), P1.GetX(), 0.,V0.GetY(), V0.GetX(), 0., XY);		
				else
					OutArray[cadr_count].Set(ccwarc, P0.GetY(), P0.GetX(), 0., P1.GetY(), P1.GetX(), 0.,V0.GetY(), V0.GetX(), 0., XY);		
		}	
		++cadr_count;
	}
	delete [] ArrX;
	delete [] ArrY;
	delete [] Ang;
	// Add last cadr if needed
	int k = FileSize - 1;
	if((OutArray[0].GetB() - OutArray[k - 1].GetE()).D2()> 1.e-12)
	{
		OutArray[k].SetType(line);
		OutArray[k].SetB(OutArray[k - 1].GetE());
		OutArray[k].SetE(OutArray[0].GetB());
		k++;
	}
	return k;
}

size_t StContOut::ReadFromFileDXF(std::list<NCadrGeom>& ResList, const CString& filename)
{
	// Load DXF file into memory:
	N_dl_import creationClass;
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(std::string(filename.GetString()), &creationClass))
	{ // if file open failed
		{
			ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
			return 0;
		}
	}
	if (creationClass.GetErrorCode() != 0)
	{
		ErrorCode = creationClass.GetErrorCode();
		return 0;
	}
	delete dxf;
	auto& GeomList = creationClass.GetContCadrs();
	bool SpliceDone = !GeomList.empty();
	if (!SpliceDone)
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	if (creationClass.GetPolylinePresent())
	{
		ResList.splice(ResList.cbegin(), GeomList);
	}
	else
	{
		// Make consecutive
		BPoint Cur = GeomList.front().GetE();
		ResList.splice(ResList.cbegin(), GeomList, GeomList.cbegin());
		double Tol = 0.0001;
		double* pTol = &Tol;
		if (NCM_PROJECT.DoubleParams.Lookup("ContTolerance", pTol))
			Tol = *pTol;
		while (SpliceDone)
		{
			SpliceDone = false;
			for (auto it = GeomList.begin(); it != GeomList.cend(); it++)
			{
				bool Found = false;
				double BDist2 = (Cur - it->GetB()).D2();
				double EDist2 = (Cur - it->GetE()).D2();
				bool Eidn = EDist2 < Tol * Tol;
				bool Bidn = BDist2 < Tol * Tol;
				if (Eidn && Bidn)
				{
					if (BDist2 > EDist2)
						Bidn = false;
					else
						Eidn = false;
				}
				if (Eidn || Bidn)
				{
					Found = true;
					if(Eidn)
						it->Reverse();
				}
				if (Found)
				{
					// Make additional line if nececcary
					double ModelTol = NSurfSettings::GetTolerance();
					if (Eidn)
					{
						if (EDist2 > ModelTol * ModelTol)
						{
							NCadrGeom Geom;
							Geom.Set(line, Cur, it->GetE());
							ResList.push_back(Geom);
						}
					}
					else if (Bidn)
					{
						if (BDist2 > ModelTol * ModelTol)
						{
							NCadrGeom Geom;
							Geom.Set(line, Cur, it->GetB());
							ResList.push_back(Geom);
						}
					}
					// END:Make additional line if nececcary
					Cur = it->GetE();
					SpliceDone = true;
					ResList.splice(ResList.cend(), GeomList, it);
					break;
				}
			}
		}
		// Try to find second branch
		Cur = ResList.front().GetB();
		SpliceDone = !GeomList.empty();
		while (SpliceDone)
		{
			SpliceDone = false;
			for (auto it = GeomList.begin(); it != GeomList.cend(); it++)
			{
				bool Found = false;
				double BDist2 = (Cur - it->GetB()).D2();
				double EDist2 = (Cur - it->GetE()).D2();
				bool Eidn = EDist2 < Tol * Tol;
				bool Bidn = BDist2 < Tol * Tol;
				if (Eidn && Bidn)
				{
					if (BDist2 > EDist2)
						Bidn = false;
					else
						Eidn = false;
				}
				if (Eidn || Bidn)
				{
					Found = true;
					if (Bidn)
						it->Reverse();
				}
				if (Found)
				{
					Cur = it->GetB();
					SpliceDone = true;
					ResList.splice(ResList.cbegin(), GeomList, it);
					break;
				}
			}
		}
	}
	if (!GeomList.empty())
	{
		ErrorCode = IDS_STOCK_WRONG_CONTOUR;//INSERT ID
		return 0;
	}
	return ResList.size();
}
