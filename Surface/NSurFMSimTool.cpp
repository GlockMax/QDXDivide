// NSurFMSimTool.cpp: implementation of the NSurFMSimTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NLine.h"
#include "BPoint.h"
#include "NSurFMSimTool.h"
#include "NSurFMContour.h"
#include "NSurFMCone.h"
#include "NSurFMSphere.h"
#include "NSurFMTorahs.h"
#include "NSurFMHorizontal.h"
#include "NContour.h"
#include "NCenums.h"

#include "NTool.h"
#include "NToolStd.h"
#include "NToolDiskStd.h"
#include "math.h"

bool SavePieces(PartArray& pcs);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//const double PI=3.14159265358979;
const float d = 1.f/3.f;

NSurFMSimTool::NSurFMSimTool()
{
	pTool = NULL;
}

NSurFMSimTool::~NSurFMSimTool()
{

}

// задание простого инструмента
void NSurFMSimTool::SetTool(NTool *pT)
{
	pTool = pT;
}

// построить поверность для простого инструмента
void NSurFMSimTool::BuildSurface(PartArray& OutArray, 
								 NLine DirLine,
								 BPoint cent,
								 DirectionType DirType,
								 BuildingType BuildType)
/*void NSurFMSimTool::BuildSurface(PartArray& OutArray, 
								 BPoint start,
								 BPoint end,
								 DirectionType DirType)*/
{
	if (!CreateContour()) 
		return;

	NSurFMCone surCone;
	NSurFMSphere surSphere;
	NSurFMHorizontal surHoriz;
	NSurFMTorahs surTorahs;
	
	BPoint dir, z;
	NLine pLine;
	int i, res = -1;
	bool angle_def = false, IsMarked = false;
	CArray<int,int> indArray;
	double alf_cur, beta_cur, alf_old=0, beta_old=0;

	bool temp = true;

	if (DirType == surLINE)
	{
		dir = DirLine.p[3];
		z = DirLine.p[0];
		dir.Norm();
		z.Norm();
		dir = dir - z;
	}
	else
	{
		dir = DirLine.p[1];
		z = DirLine.p[0];
		dir.Norm();
		z.Norm();
		dir = dir - z;
	}
	z.Set(0,0,1,0);

	indArray.RemoveAll();
	for (i=0; i<Contour.num; i++)
		indArray.Add(i);
	i = 0;
	while (indArray.GetSize() > 0)
	{
		switch(Contour.Types[indArray.GetAt(i)])
		{
		case CONE:
			Contour.GetLine(indArray.GetAt(i), pLine);

			alf_cur = dir.GetZ()*(pLine.p[0].GetX()-pLine.p[3].GetX());
			beta_cur = sqrt(dir.GetX()*dir.GetX()+dir.GetY()*dir.GetY())*
					   (pLine.p[0].GetZ()-pLine.p[3].GetZ());

			if (fabs(alf_cur) < fabs(beta_cur))
			{
				alf_cur = -alf_cur/beta_cur;

				alf_cur = acos(alf_cur);
				alf_cur = 2*180.*alf_cur/PI;
			}
			else
			{
				if (dir.GetZ() > 0.)
					alf_cur = 360.;
				else
					alf_cur = 0.;
			}
			
			if (0. < alf_cur  && alf_cur < 360.)
			{
				beta_cur = dir.GetX()/(sqrt(dir.GetX()*dir.GetX()+dir.GetY()*dir.GetY()));
				beta_cur = acos(beta_cur);
				if ((dir.GetX() < 0. && dir.GetY() < 0.) ||
					dir.GetY() < 0.)
					beta_cur = -beta_cur;				
				beta_cur = 180.*beta_cur/PI;
				beta_cur = beta_cur + 180.-alf_cur/2.;
			}
			else
				beta_cur = 0.;

			surCone.SetForming(pLine, alf_cur, beta_cur, true);
			surCone.SetDirection(DirLine, cent, DirType);
		//	surCone.SetDirection(start, end, DirType);
			res = surCone.BuildSurface(OutArray, BuildType);
			IsMarked = surCone.GetFlag();
			angle_def = true;
			indArray.RemoveAt(i);
			i = 0;
			break;
		case SPHERE:
			Contour.GetLine(indArray.GetAt(i), pLine);

			alf_cur = 180;

			beta_cur = acos(dir.GetX()/(sqrt(dir.GetX()*dir.GetX()+dir.GetY()*dir.GetY())));
			if ((dir.GetX() < 0. && dir.GetY() < 0.) ||
				dir.GetY() < 0.)
				beta_cur = -beta_cur;
			beta_cur = 180.*beta_cur/PI;
			beta_cur = beta_cur + 180.-alf_cur/2.;

			alf_cur = dir.Cos2(dir, z);
			alf_cur = alf_cur*sqrt(dir.D2())*sqrt(z.D2());	// cos alf
			alf_cur = acos(alf_cur);						// alf
			alf_cur = 180.*alf_cur/PI;
			alf_cur = 180 - alf_cur;

			surSphere.SetForming(pLine, alf_cur, beta_cur,true);
			surSphere.SetDirection(DirLine,cent, DirType);
		//	surSphere.SetDirection(start, end, DirType);
			res = surSphere.BuildSurface(OutArray, BuildType);
			indArray.RemoveAt(i);
			alf_cur = 180.;
			i = 0;
			break;
		case TORAHS:

		//	if (temp)
		//	{
				Contour.GetLine(indArray.GetAt(i), pLine);

				alf_cur = 180;

				beta_cur = acos(dir.GetX()/(sqrt(dir.GetX()*dir.GetX()+dir.GetY()*dir.GetY())));
				if ((dir.GetX() < 0. && dir.GetY() < 0.) ||
					dir.GetY() < 0.)
					beta_cur = -beta_cur;
				beta_cur = 180.*beta_cur/PI;
				beta_cur = beta_cur + 180.-alf_cur/2.;
				
				surTorahs.SetForming(pLine,alf_cur,beta_cur,false);
				surTorahs.SetDirection(DirLine, cent, DirType);
			//	surTorahs.SetDirection(start, end, DirType);

				res = surTorahs.BuildSurface(OutArray, BuildType);

		//		temp = false;
		//	}
			angle_def = true;
			indArray.RemoveAt(i);
			i = 0;
			break;
		case HORIZONTAL:
			if (!angle_def)
			{
				i++;
				break;
			}

			Contour.GetLine(indArray.GetAt(i), pLine);

			if(pLine.p[0].GetZ() == pTool->GetZdisp())
				surHoriz.SetForming(pLine, alf_cur, beta_cur, false);
			else
				surHoriz.SetForming(pLine, alf_cur, beta_cur, true);
			surHoriz.SetDirection(DirLine, cent, DirType);
		//	surHoriz.SetDirection(start, end, DirType);
			res = surHoriz.BuildSurface(OutArray, BuildType);
			indArray.RemoveAt(i);
			i = 0;
			break;
		case NOTKNOWN:
			indArray.RemoveAt(i);
			i = 0;
			break;
		}
	// построение недостающих поверхностей
	/*	if (!IsMarked &&
			alf_old != alf_cur &&
			alf_old != 0. && beta_old != 0.)
		{
			double alf, beta;
			BPoint c;
			for (j=0; j<2; j++)
			{
				if (j == 0)
				{
					beta = beta_cur;
					alf = beta_old - beta_cur;
				}
				else
				{
					beta = beta_old + alf_old;
					alf = (beta_cur + alf_cur) - beta;
				}
				c.Set(0., 0., pLine.p[0].GetZ(), 1.);
				res = surHoriz.BuildSurfaceFMArc(OutArray,
									 alf, beta,
									 pLine.p[0].GetX(), c,
									 DirLine.p[0], DirLine.p[3]);
			/	res = surHoriz.BuildSurfaceFMArc(OutArray,
			//						 alf, beta,
			//						 pLine.p[0].GetX(), c,
			//						 start, end);
			}
		}*/
		if (angle_def)
		{
			alf_old = alf_cur;
			beta_old = beta_cur;
		}
	}

	// запись координат порций в файл
//	SavePieces(OutArray);
}

// создание контура для простого инструмента
BOOL NSurFMSimTool::CreateContour()
{
// в одном контуре содержатся нерабочая и рабочая части
	if (!pTool) 
		return FALSE;
	
	int type = pTool->GetType(), index, j, k;
	BPoint pt0, pt1, pt2, pt3;
	double Zdisp = pTool->GetZdisp();
	
	Contour.Clear();
	if (10 <= type && type <= 14)
	{
		NToolStd *pToolStd;
		double dtop,dbot,h,alf,beta,r,hm;
		int LinesNumber; // Lines Number in a contour
		int PointsNumber;
		TypeLineInContour *types;
	//	int *types;
		GLfloat *ctlarray;
		
		pToolStd = (NToolStd*)pTool;
		pToolStd->GetParam(dtop,dbot,h,alf,beta,r,hm);
		
	/*	index = 0;
		for (i=0; i<2; i++)
		{
			if (i == 0) //определение не активной части инструмента
			{
				LinesNumber = 2;
				if ( h == hm ) // неактивная часть - только крышка
					LinesNumber--;
				PointsNumber = LinesNumber*3+1; // Points Number in a contour
				ctlarray = new GLfloat [4*PointsNumber];
				types = new TypeLineInContour[LinesNumber];
				// построение контура
				pToolStd->CreateToolContour(ctlarray, types, LinesNumber, FALSE);
			}
			if (i == 1)	//определение не активной части инструмента
			{
				LinesNumber = 3;
				if ( r == 0 || (r == 0 && beta == 0)	// нет скругления или фаски
					 || r == dbot/2.					// нет нижнего участка (Bottom line)
					 || r == dbot/2./tan((PI/2.-alf)/2.) )
					LinesNumber--;
				PointsNumber = LinesNumber*3+1; // Points Number in a contour
				ctlarray = new GLfloat [4*PointsNumber];
				types = new TypeLineInContour[LinesNumber];
				// построение контура
				pToolStd->CreateToolContour(ctlarray, types, LinesNumber, TRUE);
			}

			k = 0;
			for (j=0; j<LinesNumber; j++)
			{
				pt0.Set(ctlarray[k*4+0], 
						ctlarray[k*4+1], 
						ctlarray[k*4+2]+Zdisp, 
						ctlarray[k*4+3]);

				pt1.Set(ctlarray[(k+1)*4+0], 
						ctlarray[(k+1)*4+1], 
						ctlarray[(k+1)*4+2]+Zdisp, 
						ctlarray[(k+1)*4+3]);

				pt2.Set(ctlarray[(k+2)*4+0], 
						ctlarray[(k+2)*4+1], 
						ctlarray[(k+2)*4+2]+Zdisp, 
						ctlarray[(k+2)*4+3]);

				pt3.Set(ctlarray[(k+3)*4+0], 
						ctlarray[(k+3)*4+1], 
						ctlarray[(k+3)*4+2]+Zdisp, 
						ctlarray[(k+3)*4+3]);
				if (i == 0 && j == 0)
					Contour.AddFirst(pt0, pt1, pt2, pt3);
				else
					Contour.AddFollow(pt1, pt2, pt3);
				Contour.SetTypeLine(index+(j+1), types[j]);
				k+=3;
			}
			index = LinesNumber;
		//	delete []ctlarray;
		//	delete []types;
		}
	}*/
	// Top line
		pt0.Set(0., 0., h+Zdisp, 1.);

		pt3.Set(dtop/2.f, 0., h+Zdisp, 1.);

		pt1 = pt0*2.*d + pt3*d;
		pt2 = pt0*d+pt3*2*d;
		
		Contour.AddFirst(pt0, pt1, pt2, pt3);
		Contour.SetTypeLine(0,HORIZONTAL);

		k = 0;
		index = 0;
		LinesNumber = 3;
		if ( r == 0 || (r == 0 && beta == 0)	// нет скругления или фаски
			 || r == dbot/2.					// нет нижнего участка (Bottom line)
			 || r == dbot/2./tan((PI/2.-alf)/2.) )
			LinesNumber--;
		PointsNumber = LinesNumber*3+1; // Points Number in a contour
		ctlarray = new GLfloat [4*PointsNumber];
		types = new TypeLineInContour[LinesNumber];
		// построение контура
		pToolStd->CreateToolContour(ctlarray, types, LinesNumber, TRUE);

		k = 0;
		for (j=0; j<LinesNumber; j++)
		{
			pt0.Set(ctlarray[k*4+0], 
					ctlarray[k*4+1], 
					ctlarray[k*4+2]+Zdisp, 
					ctlarray[k*4+3]);

			pt1.Set(ctlarray[(k+1)*4+0], 
					ctlarray[(k+1)*4+1], 
					ctlarray[(k+1)*4+2]+Zdisp, 
					ctlarray[(k+1)*4+3]);

			pt2.Set(ctlarray[(k+2)*4+0], 
					ctlarray[(k+2)*4+1], 
					ctlarray[(k+2)*4+2]+Zdisp, 
					ctlarray[(k+2)*4+3]);

			pt3.Set(ctlarray[(k+3)*4+0], 
					ctlarray[(k+3)*4+1], 
					ctlarray[(k+3)*4+2]+Zdisp, 
					ctlarray[(k+3)*4+3]);
			Contour.AddFollow(pt1, pt2, pt3);
			Contour.SetTypeLine(index+(j+1), types[j]);
			k+=3;
		}
		delete []ctlarray;
		delete []types;
	}

	if (20 <= type && type <= 24)
	{
		NToolDiskStd *pToolDiskStd;
		double diam, b, alf, beta,
				r1, r2, r3, r4,
				l1, l2, hm1, hm2;
		int LinesNumber; // Lines Number in a contour
		int PointsNumber;
		TypeLineInContour *types;
	//	int *types;
		GLfloat *ctlarray;
		
		pToolDiskStd = (NToolDiskStd*)pTool;
		pToolDiskStd->GetParam(diam, b, alf, beta,
								r1, r2, r3, r4,
								l1, l2, hm1, hm2);
		k = 0;
		index = 0;
		double e, f, g;
		(l1 != 0. && alf !=0.) ? e = l1 + r3*tan((PI/2.-alf)/2.) 
			: e = r1;
		(l2 != 0. && beta != 0.) ? g = l2 + r4*tan((PI/2.-beta)/2.)
			: g = r2;
		f = b - e - g;
		LinesNumber = 9; // Lines Number in a contour
		if ( r1 == 0.)				// нет скруглений на инструменте
			LinesNumber--;
		if ( r2 == 0.)
			LinesNumber--;
		if ( r3 == 0.)
			LinesNumber--;
		if ( r4 == 0. )
			LinesNumber--;
		if (l1 == 0 && alf == 0.)	// нет фаски c правой стороны
			LinesNumber--;
		if (l2 == 0 && beta == 0.) 	// нет фаски с левой стороны
			LinesNumber--;
		if (f == 0. )//|| (R2 != 0. && R3 != 0.))
			LinesNumber--;
		
		
		PointsNumber = LinesNumber*3+1; // Points Number in a contour
		ctlarray = new GLfloat [4*PointsNumber];
		types = new TypeLineInContour[LinesNumber];
		// построение контура
		pToolDiskStd->CreateToolContour(ctlarray, types, LinesNumber, TRUE);

		k = 0;
		for (j=0; j<LinesNumber; j++)
		{
			pt0.Set(ctlarray[k*4+0], 
					ctlarray[k*4+1], 
					ctlarray[k*4+2]+Zdisp, 
					ctlarray[k*4+3]);

			pt1.Set(ctlarray[(k+1)*4+0], 
					ctlarray[(k+1)*4+1], 
					ctlarray[(k+1)*4+2]+Zdisp, 
					ctlarray[(k+1)*4+3]);

			pt2.Set(ctlarray[(k+2)*4+0], 
					ctlarray[(k+2)*4+1], 
					ctlarray[(k+2)*4+2]+Zdisp, 
					ctlarray[(k+2)*4+3]);

			pt3.Set(ctlarray[(k+3)*4+0], 
					ctlarray[(k+3)*4+1], 
					ctlarray[(k+3)*4+2]+Zdisp, 
					ctlarray[(k+3)*4+3]);
			if (j == 0)
				Contour.AddFirst(pt0, pt1, pt2, pt3);
			else
				Contour.AddFollow(pt1, pt2, pt3);
			Contour.SetTypeLine(index+j, types[j]);
			k+=3;
		}
		delete []ctlarray;
		delete []types;
	}

	return TRUE;
}

bool SavePieces(PartArray& pcs)
{
	return true;
	CStdioFile f;
	CString str;
	int i, j, k;
	CString path, fname;
	NSquarePiece *pPiece;

	path = _T("C:\\VCPROJECTS\\NCM29_work");
	fname = _T("Coord.txt");

	if(!f.Open(path+"\\"+fname,CFile::modeWrite | CFile::modeCreate))
	{
		AfxMessageBox("Невозможно открыть файл для записи координат!");
		return false;
	}

	str.Format("%s","Координаты порции поверхности:");
	f.WriteString(str+"\n");

	for (k=0; k<pcs.GetSize(); k++)
	{
		pPiece = (NSquarePiece *)pcs[k];
		str.Format("%s","Порция ");
		f.WriteString(str+"\n");

		for(i=0; i<3; i++)
		{
			for(j=0; j<3; j++)
			{
				str.Format("%f %f %f %f",
					pPiece->pc[i][j].GetX(),
					pPiece->pc[i][j].GetY(),
					pPiece->pc[i][j].GetZ(),
					pPiece->pc[i][j].GetH());
				f.WriteString(str+"\n");
			}
			f.WriteString("\n");
		}
	}
	f.Close();

	return true;
}