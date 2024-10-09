#include "StdAfx.h"
#include "ConstDef.h"
#include "DXRasterDisk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define sign(a)            ((a<0) ? -1 : 1)

//flat disk path rasterization (inclined cylinder, cylinder, disk)

///����������� ������� ��������� � �������� ����� ����������, ������ � ������ �� ������, �������� ��������� ������
DXRastDisk::DXRastDisk(const BPoint & P0, const BPoint & P1, double Radius, DXDirection3X &DXDir) 
	: A(P0), B(P1)
{
	//initial copy
	DXD = &DXDir;
	R = Radius;
	step0 = DXD->GetStep0();
	n0 = int(DXD->GetN0());
}

DXRastDisk::~DXRastDisk(void)
{
}

int DXRastDisk::RightCylinder(const BPoint & ArcCntr, DX_ID PairID)
{///������������ ����������� ��������	
	ElID = PairID;

	Lv = B-A;//A � B ������ �� ���� �������������
	if (Lv.D2()<MIND*MIND)//������� ���� ����������� - ������ �������� �� ����
		return 0;

	//�� ������� ��� �� �����
	if (fabs(Lv.GetX())<MIND && fabs(Lv.GetY())<MIND)
		return 0;

	Nv = Lv.Unit();

	BPoint tmp = Nv;
	tmp.SetY(0.);
	if(tmp.D2()<MIND*MIND)//������� ���������� ��� Y
		Ry = 0.;
	else
		Ry = R*(Nv*tmp.Unit());//defining radius projection on y axis (for Ymin max)

	tmp = ArcCntr%Nv;
	if (tmp.D2()<MIND*MIND)//������ ������ ������, ��������� ����� �����������
		return -1;
	else
		Rv = tmp.Unit()*R*sign(Nv*Lv);

	Cv = Nv%Rv;
	D = A+Rv;
	E = A-Rv;
	F = B+Rv;
	G = B-Rv;

	CommonInit();

	return 0;
}

int DXRastDisk::RightCylinderSegment(const BPoint & iS, const BPoint & iE, DX_ID PairID)
{///������������ ����������� ��������	
	ElID = PairID;

	Lv = B-A;//A � B ������ �� ���� �������������
	if (Lv.D2()<MIND*MIND)//������� ���� ����������� - ������ �������� �� ����
		return 0;

	//�� ������� ��� �� �����
	if (fabs(Lv.GetX())<MIND && fabs(Lv.GetY())<MIND)
		return 0;

	Nv = Lv;
	Nv.Unit();

	BPoint tmp = BPoint(Nv[0],0.,Nv[2],Nv[3]);
	if(tmp.D2()<MIND*MIND)//������� ���������� ��� Y
		Ry = 0.;
	else
		Ry = R*(Nv*tmp.Unit());//defining radius projection on y axis (for Ymin max)

	tmp = iS%Nv;
	if (tmp.D2()<MIND*MIND)//������ ������ ������ ������ ��������
		return -1;
	tmp = iE%Nv;
	if (tmp.D2()<MIND*MIND)//������ ������ ������ ����� ��������
		return -1;
	//��������� ��� ������� S � E ���� �� ������� ������ ������� ������������ Lv
	//��������� ������� � ������ �� �������, ���� �����
	if((iS%iE)*Lv>MIND)
	{
		Sv = iS;
		Ev = iE;
	}
	else
	{
		Sv = iE;
		Ev = iS;
	}

	Sv.Unit();
	Ev.Unit();

	D = A+Sv*R;
	E = A+Ev*R;
	F = B+Sv*R;
	G = B+Ev*R;

	//������� �������������� ������ � ��������� ���������
	Horizv = BPoint(0,1,0)%Nv;
	Horizv *= (Horizv.GetX()<0?-1.:1.);
	//������� ������ �� ������ � ����������� �������� ���������� �� Y � ��������� ���������
	Vertv = Nv%Horizv;
	Vertv *= (Vertv.GetY()<0?-1.:1.);

	//�������� ��������� �������� �� �������������
	ArcDegraded = (fabs(Nv.GetZ())<MIND);

	CylSegmentInit();

	return 0;
}

int DXRastDisk::InclinedCylinder(const BPoint & N, DX_ID PairID)
{///������������ ���������� ��������
	ElID = PairID;
	Nv = N;

	Lv = B-A;//A � B ������ �� ���� �������������
	if (Lv.D2()<MIND*MIND || (fabs(Lv.GetX())<MIND && fabs(Lv.GetY())<MIND) )
		return 0;//������� ���� ����������� ��� ������� �������� - ������ �������� �� ����

	if (Nv.D2()<MIND*MIND)//������� � ��������� - ���� ������
		return -1;
	Nv=Nv.Unit();

	BPoint tmp = Nv;
	tmp.SetY(0.);
	if (tmp.D2()<MIND*MIND)//������� ���������� ��� Y
		Ry = 0.;
	else
		Ry = R*(Nv*tmp.Unit());//defining radius projection on y axis (for Ymin max)

	tmp = Lv%Nv;
	if (tmp.D2()<MIND*MIND)//������� ������! ������� � ��-87!
		return -1;
	Rv = (tmp).Unit()*R*sign(Nv*Lv);

	Cv = Nv%Rv;
	D = A+Rv;
	E = A-Rv;
	F = B+Rv;
	G = B-Rv;

	CommonInit();

	return 0;
}

int DXRastDisk::DegradedCylinder(const BPoint & N, DX_ID PairID)
{///������������ ���������� ������������ � ��������� ��������
	ElID = PairID;
	Nv = N;

	Lv = B-A;//A � B ������ �� ���� �������������
	if (Lv.D2()<MIND*MIND)//������� ���� ����������� - ������ �������� �� ����
		return 0;

	if(fabs(Nv.GetZ())<MIND)//����������� ������
		return -1;//������ ��������� ��������� �����
	Nv.Unit();
	
	if (Lv.Unit()*Nv>MINAR)//������� �� �������! ������� � ��-87!
		return -1;
	Rv = (Lv%Nv).Unit()*R;
	Rv = Rv*sign((Rv%Lv)*Nv);
	D = A+Rv;
	E = A-Rv;
	F = B+Rv;
	G = B-Rv;
	Cv = Nv % Rv;

	DXRDProcessFlat();
	RastDisk(A, Nv, R);
	return 0;
}

int DXRastDisk::FlatDisk(const BPoint & N, DX_ID PairID)
{///������������ �������� ����� � ����� ����������
	Nv = N;
	if (Nv.D2()<MIND*MIND)//������� � ��������� - ���� ������
		return -1;

	Nv = Nv.Unit();

	BPoint tmp = BPoint(Nv[0],0.,Nv[2],Nv[3]);
	if(tmp.D2()<MIND*MIND)//������� ���������� ��� Y
		Ry = 0.;
	else
		Ry = R*(Nv*tmp.Unit());//defining radius projection on y axis (for Ymin max)

	ElID = PairID;

	RastDisk(B, Nv, R);

	return 0;
}

void DXRastDisk::CommonInit(void)
{
	//����� �������� ��� ���� ����� ���������
	//Defining Ymin and Ymax
	if (fabs(Cv.GetY())>MIND)
	{//��������� �� Y ����� �� ����
		HINeeded = true;//������������� ����� �����
		if (Cv.GetY()>0)
		{//�� ���� ����� ��������
			Hy = A.GetY()+Ry;
			Iy = B.GetY()+Ry;
			Ymax = max(Hy,Iy);
			Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
			YminOnArc = false;
		}
		else
		{//�� ���� ����� �������
			Hy = A.GetY()-Ry;
			Iy = B.GetY()-Ry;
			Ymin = min(Hy,Iy);
			Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
			//Then Hy or Iy is a minimum Y, so the initial state of up/down solution for arcs is
			YminOnArc = true;
		}
	}
	else
	{//��������� �� Y ��������� � ������ ����
		HINeeded = false;//������������� ����� �� �����, ��� ��� ��������� � ������� ����
		Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
		Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
		YminOnArc = false;
	}

	//������� ����� ������
	//Initially SL doesn's switch up/down solutions
	BPoint Nalpha = BPoint(-Lv.GetY(),Lv.GetX(),0);
	BPoint Mv = Nv%Nalpha.Unit();
	BPoint vAS = Nv%Mv;//������ AS �� ������ ��������� � ����� ������� ����� ������
	Dv = Lv%vAS;
	Dv = Dv.Unit()*sign(Dv.GetZ());
	if ((vAS%Rv).D2()>MINAR*MINAR)
	{//SL exists
		SL = true;
		vAS = BPoint(vAS*sign(vAS*Cv)).Unit()*R;
		//������� ����� ����� ������
		S = A+vAS;
		T = B+vAS;
		//���������� ������� � ������ ������� DF � EG
		if (Rv*Dv>0)
		{//upper solution for DF
			S1up = true;//DF - �������
			S2up = false;//EG - ������
		}
		else
		{
			S1up = false;
			S2up = true;
		}
	}
	else
	{
		SL = false;//����� ������ �����������, �.�. ��������� � �������� �� ������� ��������
		//���������� ������� � ������ ������� DF � EG
		if (Cv*Dv>0)
			Cup = true;//��� ������� �������
		else
			Cup = false;//��� ������� ������
		S1up = Cup;
		S2up = Cup;
	}

	if(fabs(Nv.GetZ())<MIND)
		ArcDegraded = true;//������� �����������
	else
		ArcDegraded = false;//������� �� �����������

	BPoint Horiz = Nv%BPoint(0.,1.,0.,0.);//���� ������ � ��������� ��������� ���������������� Y (��������������)
	Horiz = Horiz*sign(Horiz.GetX());//������ �������� �� X �������������
	//���������� ������ � ����� �������
	bool LeftSUp, RightSUp;//������ � ����� ������� ������� ��� ������
	if(!ArcDegraded)//���� ��������� �������� �� ���������
	{
		double dtmp = Rv*Horiz;
		if (fabs(dtmp)>MIND)
		{//�������� R �� �������������� ������ �� �������
			if (Rv*Horiz>0)
			{//�������� R �� �������������� ������ ��������� �������������
				S1Lft = false;//S1 - ������
				RightSUp = S1up;//������ ������� ���������� � S1
				S2Lft = true;
				LeftSUp = S2up;
			}
			else
			{
				S1Lft = true;
				LeftSUp = S1up;
				S2Lft = false;
				RightSUp = S2up;
			}
		}
		else
		{//�������� R �� �������������� ������ �������, ������ �������� ������ ������ ��� ������ ����� ����
			if(Rv.GetY()<0.)
			{//������ ����� �� ���� � S1
				LeftSUp = S1up;
				RightSUp = S1up;
			}
			else
			{//������ ����� �� ���� � S2
				LeftSUp = S2up;
				RightSUp = S2up;
			}
			if(Cv.GetX()>0)//�������� �������, ������������ � �������� ���� �� ������ ���������
				//�������� ���������, ����� ������� ��� �� ����������, ��� ��� ���������
			{//�������� ������ ����
				S1Lft = false;
				S2Lft = false;
			}
			else
			{//�������� ����� ����
				S1Lft = true;
				S2Lft = true;
			}
		}
	}
	else
	{//��������� �������� ���������, �� ���� Nz = 0
		//����� ������ ����������
		if(HINeeded)//���� ��������� �� ����������� ����
		{
			S1Lft = true;
			LeftSUp = S1up;//������� ��� S1 - �����
			S2Lft = false;
			RightSUp = S2up;//� S2 - ������
		}
		else
		{//���������� �� ����������� ���� ���
			S1Lft = true;
			LeftSUp = S1up;//������� ��� S1 �����
			S2Lft = true;
			RightSUp = S2up;//� S2 �����
		}
	}

	if(SL)
	{		
		//����, ����� ������ ��� ����������
		double vASHorProj = vAS*Horiz;//���������� ������ AS �� �������������� ������

		if(!ArcDegraded)
			{
				if(fabs(vASHorProj)>MIND)
				{
					if(vAS*Horiz>0)
						SLft = false;//Silhouette line switch the right arc solution
					else
						SLft = true;//Silhouette line switch the left arc solution
					SRht = !SLft;
				}
				else
				{//�������� �������, SL ��������� � Ymin �� ����
				SLft = false;
				SRht = false;
				}
			}
			else
			{
				//����� ����� ������ ����������
				//���� ��������� ���������, �� ����� ������ ��������� � �����������, � ������ ������ �� �����������
				SLft = false;
				SRht = false;
			}

		if(YminOnArc)
		{//������� �� Y - � Hy ��� Iy, ���������� ��������� ��������� ��� ��������� ������ � ����� ���
			if(!ArcDegraded)
			{//��������� �������� �����������
				if(fabs(vASHorProj)>MIND)
				{//�������� �� �������, ������ ����� S �� ����� ��� �� ������ ����
					if(vASHorProj>0)
					{//Ymin belongs to right arc
						RA1Up = LeftSUp;//Initial solution from left segment
						LA1Up = LeftSUp;
						RA2Up = LeftSUp;
						LA2Up = LeftSUp;
					}
					else
					{//Ymin belongs to left arc
						RA1Up = RightSUp;
						LA1Up = RightSUp;
						RA2Up = RightSUp;
						LA2Up = RightSUp;
					}
				}
				else
				{//�������� �������, SL ��������� � Ymin �� ����
					RA1Up = RightSUp;
					LA1Up = LeftSUp;
					RA2Up = RightSUp;
					LA2Up = LeftSUp;
				}
			}
			else
			{//���� ��������� �������� ���������
				//���� ��������� ���������, � ��� ������� ����� ������ � ����� �����, �� ���������
				//��������� �������� ���������
				/* ��� ������� �� ���� ����, �� ������� �����������!!!
				S1Lft = true;
				LeftSUp = S1up;//������� ��� S1 - �����
				S2Lft = false;
				RightSUp = S2up;//� S2 - ������
				*/
				RA1Up = RightSUp;
				LA1Up = LeftSUp;
				RA2Up = RightSUp;
				LA2Up = LeftSUp;
			}
		}
		else
		{//��� �������� �� Y
			RA1Up = RightSUp;
			LA1Up = LeftSUp;
			RA2Up = RightSUp;
			LA2Up = LeftSUp;
		}
	}
	else
	{//��� ����� ������
		//���������� �������������� ������� �� ������ � ����� ���������
		//� ������ ������ ����� ������ ��� - ��� ������� ����������
		RA1Up = Cup;
		LA1Up = Cup;
		RA2Up = Cup;
		LA2Up = Cup;
	}
	//Defining Y axis segments and their intersections
	DXRDMakeYSegments();
	//processing 'em
	DXRDProcYLines();
}

void DXRastDisk::CylSegmentInit(void)
{
	if(Horizv.D2()>MIND*MIND)
	{//��������� �������� �� �������������
		Horizv.Unit();
		if ( ((Sv*Horizv)*(Ev*Horizv)>=0) || ((fabs(Sv*Horizv)<MINAR) || (fabs(Ev*Horizv)<MINAR)) )
		{//�������� Sv � Ev �� Horizv ����� ���������� �����, ��� ���� �� ��� �������, ������ ���������� ���
			HINeeded = false;
			Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
			Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
			YminOnArc = false;
		}
		else
		{//�������� Sv � Ev �� Horizv ����� ������ �����, ������ ���������� ���
			HINeeded = true;
			double proj1 = (Sv%Ev)*Nv;
			double proj2 = (Sv%(Vertv*(-1.)))*Nv;
			double proj3 = ((Vertv*-(1.))%Ev)*Nv;
			if( (sign(proj1) == sign(proj2)) && (sign(proj2)==sign(proj3)) )
			{//������ Vertv  ����� ����� Sv � Ev, ������ ��� ���������� �������� �� Y
				Hy = A.GetY()-Ry;
				Iy = B.GetY()-Ry;
				Ymin = min(Hy,Iy);
				Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
				//Then Hy or Iy is a minimum Y, so the initial state of up/down solution for arcs is
				YminOnArc = true;
			}
			else
			{
				Hy = A.GetY()+Ry;
				Iy = B.GetY()+Ry;
				Ymax = max(Hy,Iy);
				Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
				YminOnArc = false;
			}
		}
	}
	else
	{//��������� �������� � �������������� ���������
		HINeeded = false;
		Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
		Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
		YminOnArc = false;
	}

	//������� ����� ������
	//Initially SL doesn's switch up/down solutions
	BPoint Nalpha = BPoint(-Lv.GetY(),Lv.GetX(),0);
	BPoint Mv = Nv%Nalpha.Unit();
	BPoint vAS = Nv%Mv;//������ AS �� ������ ��������� � ����� ������� ����� ������
	Dv = Lv%vAS;//������ � ������� ���������, ����������� ������ � ������ �������
	Dv = Dv.Unit()*sign(Dv.GetZ());
	SL = false;
	for(int i=0; i<2; i++)//���� ��� ����, ����� ��������� ��� ����� ������
	{
		vAS*=-1.;//��������� ������ AS
		double proj1 = (Sv%vAS)*Nv;
		double proj2 = (vAS%Ev)*Nv;
		double proj3 = (Sv%Ev)*Nv;
		if( (proj1*proj2>=0) && (proj2*proj3>=0)  )
		{//����� ������ ����� �� ����
			SL = true;
			vAS = BPoint(vAS).Unit()*R;
			//������� ����� ����� ������
			S = A+vAS;
			T = B+vAS;
			break;
		}
	}

	//���������� ������� � ������ �������
	if(fabs(Sv*Dv)>fabs(Ev*Dv))
	{//�������� �� ����� ����
		S1up = ((Sv*Dv)>0);
		if(SL)
			S2up = !S1up;
		else
		{
			S2up = S1up;
			Cup = S1up;
		}
	}
	else
	{//�������� ������ � ����
		S2up = ((Ev*Dv)>0);//��� �������� �� ������� �� ����� ���� ������ � ����
		if(SL)
			S1up = !S2up;
		else
		{
			S1up = S2up;
			Cup = S2up;
		}
	}

	//���������� ����� ������� �����, ����� ������
	if(!ArcDegraded)
	{//��������� �����������
		if(HINeeded)
		{//���� ����� � ������
			if(fabs(Sv*Horizv)>MIND)
			{
				S1Lft = (Sv*Horizv)<0;
				S2Lft = !S1Lft;
			}
			else
			{
				S2Lft = (Ev*Horizv)<0;
				S1Lft = !S2Lft;
			}
		}
		else
		{//��� ����� ��� ��� ������
			if(fabs(Sv*Horizv)>MIND)
				S1Lft = (Sv*Horizv)<0;
			else
				S1Lft = (Ev*Horizv)<0;
			S2Lft = S1Lft;
		}
	}
	else
	{//��������� ���������
		if(HINeeded)//���� ��������� �� ����������� ����
		{
			S1Lft = true;//������� ��� S1 - �����
			S2Lft = false;//� S2 - ������
		}
		else
		{//���������� �� ����������� ���� ���
			S1Lft = true;//������� ��� S1 �����
			S2Lft = true;//� S2 �����
		}
	}

	//���������� ����� ���� ������������� ����� ������
	if(SL && (fabs(vAS*Horizv)>MIND))
	{//������ ����� ������ ����������, ������ �� �� ����������
		SLft = (vAS*Horizv)<0;
		SRht = !SLft;
	}
	else
	{
		SLft = false;
		SRht = false;
	}

	//���������� � ���� ���������� ����
	bool LeftSup = (S1up && S1Lft) || (S2up && S2Lft);
	bool RightSup = (S1up && !S1Lft) || (S2up && !S2Lft);
	if(YminOnArc)
	{//������� ����� �� ����
		if(!(SLft || SRht))
		{//������ ������ ������ �� �����������
			RA1Up = RightSup;
			LA1Up = LeftSup;
		}
		else
		{
			RA1Up = (RightSup && SLft) || (LeftSup && SRht);//Initial solution from left segment
			LA1Up = RA1Up;
		}		
	}
	else
	{//�� ���� �������� ���
		RA1Up = RightSup;
		LA1Up = LeftSup;
	}
	RA2Up = RA1Up;
	LA2Up = LA1Up;

	//Defining Y axis segments and their intersections
	DXRDMakeYSegments();
	//processing 'em
	DXRDProcYLines();
}

void DXRastDisk::DXRDProcessFlat(void)
{//processing flat cylinder case
	//Defining Ymin and Ymax
	YminOnArc = false;
	BPoint tmp = Nv;
	tmp.SetY(0.);
	if (tmp.D2() < MIND * MIND)//������� ���������� ��� Y
		Ry = 0.;
	else
		Ry = R*(Nv*tmp.Unit());//defining radius projection on y axis (for Ymin max)

	if (fabs(Cv.GetY()) > MIND)
	{//��������� �� Y ����� �� ����
		HINeeded = true;//������������� ����� �����
		if (Cv.GetY()>0)
		{//�� ���� ����� ��������
			Hy = A.GetY()+Ry;
			Iy = B.GetY()+Ry;
			Ymax = max(Hy,Iy);
			Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
			YminOnArc = false;
		}
		else
		{//�� ���� ����� �������
			Hy = A.GetY()-Ry;
			Iy = B.GetY()-Ry;
			Ymin = min(Hy,Iy);
			Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
			//Then Hy or Iy is a minimum Y, so the initial state of up/down solution for arcs is
			YminOnArc = true;
		}
	}
	else
	{//��������� �� Y ��������� � ������ ����
		HINeeded = false;//������������� ����� �� �����, ��� ��� ��������� � ������� ����
		Ymin = min(min(D.GetY(),E.GetY()),min(F.GetY(),G.GetY()));
		Ymax = max(max(D.GetY(),E.GetY()),max(F.GetY(),G.GetY()));
		YminOnArc = false;
	}

	BPoint Horiz = Nv%BPoint(0.,1.,0.,0.);//���� ������ � ��������� ��������� ���������������� Y (��������������)
	Horiz = Horiz*sign(Horiz.GetX());//������ �������� �� X �������������
	//���������� ������ � ����� �������
	double dtmp = Rv*Horiz;
	if (fabs(dtmp)>MIND)
	{//�������� R �� �������������� ������ �� �������
		if (dtmp>0)
		{//�������� R �� �������������� ������ ��������� �������������
			S1Lft = false;//S1 - ������
			S2Lft = true;
		}
		else
		{
			S1Lft = true;
			S2Lft = false;
		}
	}
	else
	{//�������� R �� �������������� ������ �������, ������ �������� ������ ������ ��� ������ ����� ����
		if(Cv.GetX()>0)//�������� �������, ������������ � �������� ���� �� ������ ���������
			//�������� ���������, ����� ������� ��� �� ����������, ��� ��� ���������
		{//�������� ������ ����
			S1Lft = false;
			S2Lft = false;
		}
		else
		{//�������� ����� ����
			S1Lft = true;
			S2Lft = true;
		}
	}

	SL = false;//no silhuette line here
	Cup = (Nv.GetZ()<0.);
	DXRDMakeYSegments();
	DXRDProcYLinesFlat();
}

void DXRastDisk::DXRDMakeYSegments(void)
{//Defining Y segments and their types

	YSeg[0].t.p=YSType::D;
	YSeg[0].y=D.GetY();
	YSeg[1].t.p=YSType::E;
	YSeg[1].y=E.GetY();
	YSeg[2].t.p=YSType::F;
	YSeg[2].y=F.GetY();
	YSeg[3].t.p=YSType::G;
	YSeg[3].y=G.GetY();
	if(HINeeded)
	{
		YSeg[4].t.p=YSType::H;
		YSeg[4].y=Hy;
		YSeg[5].t.p=YSType::I;
		YSeg[5].y=Iy;
	}
	else
	{
		YSeg[4].t.p=YSType::none;
		YSeg[4].y=0.;
		YSeg[5].t.p=YSType::none;
		YSeg[5].y=0.;
	}
	if(SL)//silhouette line is present
	{
		YSeg[6].t.p=YSType::S;
		YSeg[6].y=S.GetY();
		YSeg[7].t.p=YSType::T;
		YSeg[7].y=T.GetY();
	}
	else
	{
		YSeg[6].t.p=YSType::none;
		YSeg[6].y=0.;
		YSeg[7].t.p=YSType::none;
		YSeg[7].y=0.;
	}

	//Sort YFlags and YCoords
	qsort(YSeg,8,sizeof(YSeg[0]),cmpYSeg);

	//Define individual flags for each segment
	YSType fl;//temporary flag
	fl.i=0x0;
	fl.RA1Up = RA1Up;
	fl.LA1Up = LA1Up;
	fl.RA2Up = RA2Up;
	fl.LA2Up = LA2Up;

	for(int i=0; i<8; i++)
	{
		switch (YSeg[i].t.p)
		{
		case YSType::D:
			if (!S1Lft)
				fl.RA1 = !fl.RA1;
			else
				fl.LA1 = !fl.LA1;
			fl.S1 = !fl.S1;
			break;
		case YSType::E:
			if (!S2Lft)
				fl.RA1 = !fl.RA1;
			else
				fl.LA1 = !fl.LA1;
			fl.S2 = !fl.S2;
			break;
		case YSType::F:
			if (!S1Lft)
				fl.RA2 = !fl.RA2;
			else
				fl.LA2 = !fl.LA2;
			fl.S1 = !fl.S1;
			break;
		case YSType::G:
			if (!S2Lft)
				fl.RA2 = !fl.RA2;
			else
				fl.LA2 = !fl.LA2;
			fl.S2 = !fl.S2;
			break;
		case YSType::S://Silhouette line intersection
			fl.RA1Up ^= (SL && SRht);
			fl.LA1Up ^= (SL && SLft);
			fl.SL=!fl.SL;
			break;
		case YSType::T://Silhouette line intersection
			fl.RA2Up ^= (SL && SRht);
			fl.LA2Up ^= (SL && SLft);
			fl.SL=!fl.SL;
			break;
		case YSType::H:
			fl.RA1=!fl.RA1;//RA1 - right branch of arc one (DSE)
			fl.LA1=!fl.LA1;
			break;
		case YSType::I:
			fl.RA2=!fl.RA2;//RA1 - right branch of arc two (GTF)
			fl.LA2=!fl.LA2;
			break;
		}
		YSeg[i].t = fl;
	}
}

void DXRastDisk::DXRDProcYLines(void)
{//processisng Y=const lines
	//prepare constants for cylinder vs line solution
	ICylSolverPrepare();

	double Ydn,Yup = YSeg[0].y;
	double step1 = DXD->GetStep1();
	int ymin,ymax,n1;
	n1 = int(DXD->GetN1());

	double dLx = (fabs(Lv.GetY())>MIND ? Lv.GetX()/Lv.GetY() : 0.);

	for(int i=0; i<7; i++)
	{
		Ydn=Yup;
		Yup=YSeg[i+1].y;
		//find top and bottom rasters of Y segment
		ymin = max(0,int(Ydn/step1+1.-MIND));
		if( (Yup<=ymin*step1) || ( YSeg[i].t.i == 0x780) )
			continue;
		ymax = max(0,min(n1,int(Yup/step1+1.)));
		for(int y = ymin; y<ymax; y++)
		{//process rasters of Y segment
			int j=0;
			double Ycoord = step1*y;
			//mark X segments
			XSType fl;
			fl.i = 0x0;
			if (YSeg[i].t.S1)
			{
				XSeg[j].x = (Ycoord-D.GetY())*dLx+D.GetX();
				fl.i=0x0;
				fl.Up = S1up;
				fl.Down = !S1up;
				XSeg[j].t=fl;
				j++;
			}
			if (YSeg[i].t.S2)
			{
				XSeg[j].x = (Ycoord-E.GetY())*dLx+E.GetX();
				fl.i=0x0;
				fl.Up = S2up;
				fl.Down = !S2up;
				XSeg[j].t=fl;
				j++;
			}
			if (YSeg[i].t.SL)
			{
				XSeg[j].x = (Ycoord-S.GetY())*dLx+S.GetX();
				fl.i=0x0;
				fl.Up = true;
				fl.Down = true;
				XSeg[j].t=fl;
				j++;
			}
			if (YSeg[i].t.RA1 || YSeg[i].t.LA1)
			{//there are intersections with first arc
				BPoint P0,P1;
				//find both intersections
				ArcSolver(A,R,Nv,Ycoord,P0,P1);
				if (YSeg[i].t.LA1)
				{
					XSeg[j].x = P0.GetX();
					fl.i=0x0;
					fl.Up = YSeg[i].t.LA1Up;
					fl.Down = !YSeg[i].t.LA1Up;
					XSeg[j].t = fl;
					j++;
				}
				if (YSeg[i].t.RA1)
				{
					XSeg[j].x = P1.GetX();
					fl.i=0x0;
					fl.Up = YSeg[i].t.RA1Up;
					fl.Down = !YSeg[i].t.RA1Up;
					XSeg[j].t = fl;
					j++;
				}
			}
			if (YSeg[i].t.RA2 || YSeg[i].t.LA2)
			{//there are intersections with second arc
				BPoint P0,P1;
				//find both intersections
				ArcSolver(B,R,Nv,Ycoord,P0,P1);
				if (YSeg[i].t.LA2)
				{
					XSeg[j].x = P0.GetX();
					fl.i=0x0;
					fl.Up = YSeg[i].t.LA2Up;
					fl.Down = !fl.Up;
					XSeg[j].t = fl;
					j++;
				}
				if (YSeg[i].t.RA2)
				{
					XSeg[j].x = P1.GetX();
					fl.i=0x0;
					fl.Up = YSeg[i].t.RA2Up;
					fl.Down = !fl.Up;
					XSeg[j].t = fl;
					j++;
				}				
			}

			//fill empty segments
			for(int k = j; k<8; k++)
			{
				XSeg[k].x = 0.;
				XSeg[k].t.i = 0x0;
			}

			//sort segments
			qsort(XSeg,j,sizeof(XSeg[0]),cmpXSeg);

			//build flag states for segments
			fl.i = 0x0;
			for(int k = 0; k<8; k++)
			{
				fl.i ^= XSeg[k].t.i;
				XSeg[k].t = fl;
			}

			//X segments ready. Processing em.
			DXRDProcXSeg(y, Ycoord);
		}
	}
}

void DXRastDisk::DXRDProcYLinesFlat(void)
{	//processisng Y=const lines
	//prepare constants for cylinder vs line solution

	double Yup = YSeg[0].y;
	double step1 = DXD->GetStep1();
	int n1 = int(DXD->GetN1());

	double dLx = Lv.GetX()/Lv.GetY();

	for(int i=0; i<7; i++)
	{
		double Ydn = Yup;
		Yup = YSeg[i+1].y;
		//find top and bottom rasters of Y segment
		int ymin = max(0, int(Ydn / step1 + 1. - MIND));
		if(Yup <= ymin * step1)
			continue;
		int ymax = max(0, min(n1, int(Yup / step1 + 1.)));
		for(int y = ymin; y<ymax; y++)
		{//process rasters of Y segment
			int j = 0;
			double Ycoord = step1*y;
			//mark X segments
			if (YSeg[i].t.S1)
			{
				XSeg[j].x = (Ycoord-D.GetY())*dLx+D.GetX();
				XSeg[j].t.Up = true;
				j++;
			}
			if (YSeg[i].t.S2)
			{
				XSeg[j].x = (Ycoord-E.GetY())*dLx+E.GetX();
				XSeg[j].t.Up = true;
				j++;
			}
			if (YSeg[i].t.RA1 || YSeg[i].t.LA1)
			{//there are intersections with first arc
				BPoint P0,P1;
				//find both intersections
				ArcSolver(A,R,Nv,Ycoord,P0,P1);
				if (YSeg[i].t.LA1)
				{
					XSeg[j].x = P0.GetX();
					XSeg[j].t.Up = true;
					j++;
				}
				if (YSeg[i].t.RA1)
				{
					XSeg[j].x = P1.GetX();
					XSeg[j].t.Up = true;
					j++;
				}
			}
			if (YSeg[i].t.RA2 || YSeg[i].t.LA2)
			{//there are intersections with second arc
				BPoint P0,P1;
				//find both intersections
				ArcSolver(B,R,Nv,Ycoord,P0,P1);
				if (YSeg[i].t.LA2)
				{
					XSeg[j].x = P0.GetX();
					XSeg[j].t.Up = true;
					j++;
				}
				if (YSeg[i].t.RA2)
				{
					XSeg[j].x = P1.GetX();
					XSeg[j].t.Up = true;
					j++;
				}				
			}

			//fill empty segment
			//for(int k = j; k < j + 1; k++)
			//{
			//	XSeg[k].x = 0.;
			//	XSeg[k].t.i = 0x0;
			//}

			//sort segments
			qsort(XSeg, j, sizeof(XSeg[0]), cmpXSeg);

			//build flag states for segments
			XSType fl;
			fl.i = 0x0;
			for(int k = 0; k < j; k++)
			{
				fl.i ^= XSeg[k].t.i;
				XSeg[k].t = fl;
			}

			//X segments ready. Processing em.
			DXRDProcXSegFlat(y, Ycoord, j - 1);
		}
	}
}

void  DXRastDisk::ICylSolverPrepare()
{//prepare constants for solution
	icsC1 = Lv*Nv;
	icsC2 = Lv.D2();
	icsA = icsC1*(icsC1-2*Nv.GetZ()*Lv.GetZ())+icsC2*Nv.GetZ()*Nv.GetZ();
	icsAinv = 1. / (2. * icsA);
	icsB = 2*(icsC2*Nv.GetX()*Nv.GetZ()-icsC1*(Lv.GetX()*Nv.GetZ()+Lv.GetZ()*Nv.GetX()));
	icsC = 2*(icsC2*Nv.GetY()*Nv.GetZ()-icsC1*(Lv.GetY()*Nv.GetZ()+Lv.GetZ()*Nv.GetY()));
	icsD = -2*icsC1*Lv.GetX()*Nv.GetX()+icsC2*Nv.GetX()*Nv.GetX()+icsC1*icsC1;
	icsE = 2* (-icsC1*(Lv.GetX()*Nv.GetY()+Nv.GetX()*Lv.GetY())+icsC2*Nv.GetX()*Nv.GetY());
	icsE1 = -2*icsC1*Nv.GetY()*Lv.GetY()+icsC2*Nv.GetY()*Nv.GetY()+icsC1*icsC1;
	icsF = R*R*icsC1*icsC1;
}

void  DXRastDisk::ICylSolverYPrepare(const double Ycoord)
{//prepare solution for y = const
	double qy = Ycoord-A.GetY();
	icsG = qy*icsC;
	icsH = qy*icsE;
	icsI = qy*qy*icsE1-icsF;
}

void DXRastDisk::DXRDProcXSeg(unsigned int y, double Ycoord)
{//process ready X segments
	ICylSolverYPrepare(Ycoord);
	double Xup = XSeg[0].x;
	double Zstep0 = 1. / step0;
	for(int i = 0; i < 7; ++i)
	{
		double Xdn = Xup;
		Xup=XSeg[i+1].x;
		if(XSeg[i].t.i == 0x0)
			continue;
		bool Down = XSeg[i].t.Down;
		bool Up = XSeg[i].t.Up;
		//find top and bottom rasters of Y segment
		int xmin = int(Xdn * Zstep0 + 1. - MIND);
		xmin = max(0, xmin);
		int xmax = int(Xup * Zstep0) + 1;
		xmax = min(n0, xmax);
		xmax = max(0, xmax);
		double qx = step0 * xmin - A.GetX();
		for(int x = xmin; x < xmax; ++x, qx += step0)
		{
			//quadratic equation constants (qeA = icsA)
			double qeB = (qx*icsB+icsG);
			double qeC = qx*(qx*icsD+icsH)+icsI;
			double qeD = qeB*qeB-4*icsA*qeC;
			if(qeD < 0.)
				continue;
			double z1 = A.GetZ() - qeB * icsAinv;
			qeD = sqrt(qeD) * icsAinv;
			double z0 = z1 - qeD ;
			z1 += qeD;
			if(Down)
			{//lower solution
				DX_DEPTH &CDepthD = DXD->DepthEl(0, x, y);
				if(z0 < CDepthD)
				{		
					CDepthD = (DX_DEPTH)z0;					
					DX_ID &CIDD = DXD->IDsEl(0, x, y);
					CIDD = ElID;
				}
			}
			
			if(Up)
			{//upper solution
				DX_DEPTH &CDepthU = DXD->DepthEl(1, x, y);
				if(z1 > CDepthU)
				{
					CDepthU = (DX_DEPTH)z1;					
					DX_ID &CIDU = DXD->IDsEl(1, x, y);
					CIDU = ElID;
				}
			}
		}
	}
}

void DXRastDisk::DXRDProcXSegFlat(unsigned int y, double Ycoord, int PairsNum)
{//process ready X segments
	double Zz = 1. / Nv.GetZ();
	double Zb = A.GetZ() - ((Ycoord - A.GetY()) * Nv.GetY() - A.GetX() * Nv.GetX()) * Zz;
	Zz *= Nv.GetX();
	double Xup = XSeg[0].x;
	for(int i = 0; i < PairsNum; ++i)
	{
		double Xdn = Xup;
		Xup = XSeg[i + 1].x;
//		Xup = max(0., Xup);
		if(XSeg[i].t.i==0x0)
			continue;
		//find top and bottom rasters of Y segment
		int xmin = max(0, int(Xdn / step0 + 1. - MIND));
		int xmax = max(0, min(int(Xup / step0) + 1, n0));
		if(Cup)
		{//upper solution
			for (int x = xmin; x < xmax; x++)
			{
				double Z = Zb - step0 * x * Zz;
				DX_DEPTH &CDepthU = DXD->DepthEl(1, x, y);
				if (Z > CDepthU)
				{
					DX_ID &CIDU = DXD->IDsEl(1, x, y);
					CDepthU = (DX_DEPTH)Z;
					CIDU = ElID;
				}
			}
		}
		else
		{//lower solution
			for (int x = xmin; x < xmax; x++)
			{
				double Z = Zb - step0 * x * Zz;
				DX_DEPTH &CDepthD = DXD->DepthEl(0, x, y);
				if (Z < CDepthD)
				{
					DX_ID &CIDD = DXD->IDsEl(0, x, y);
					CDepthD = (DX_DEPTH)Z;
					CIDD = ElID;
				}
			}
		}
	}
}

void DXRastDisk::ArcSolver(const BPoint &A, const double R, const BPoint &n, const double Y, BPoint &P0, BPoint &P1)
{//find intersection points with circle with center A, radius R, normal to n with plane Y = const, P0x < P1x - solutions
	BPoint Nalpha(0., 1., 0., 0.);
	BPoint lv = n % Nalpha;
	double lvD2 = lv.D2();
	if (lvD2 < MIND*MIND)
		return;
	BPoint ev = n % lv;
	double t = (Y - A.GetY()) / ev.GetY();
	ev = ev * t;
	double VV = R*R - ev.D2();
	VV = max(VV, 0.);
	lv *= sqrt(VV / lvD2);
	if (lv.GetX() < 0.)
		lv = lv * (-1.);
	BPoint C(A + ev);
	P0 = C - lv;//left solution
	P1 = C + lv;//right solution
}

void DXRastDisk::RastDisk(const BPoint &Cp, const BPoint &N, const double Rad)
{
	int n1 = int(DXD->GetN1());
	double step1 = DXD->GetStep1();
	int ymin = max(0, int((Cp.GetY() - Ry) / step1 + 1. - MIND));
	int ymax = max(0, min(n1, int((Cp.GetY() + Ry) / step1 + 1.)));
	bool Up = (N.GetZ() < 0);
	for(int y = ymin; y < ymax; y++)
	{
		double YCoord = step1 * y;
		BPoint P0, P1;
		ArcSolver(Cp, Rad, N, YCoord, P0, P1);
		int xmin = max(0, int(P0.GetX() / step0 + 1. - MIND));
		int xmax = max(0, min(n0, int(P1.GetX() / step0) + 1));
		double Yconst = (YCoord - Cp.GetY()) * N.GetY();
		for(int x = xmin; x<xmax; x++)
		{
			double XCoord = x * step0;
			double Z = Cp.GetZ() - ((XCoord - Cp.GetX()) * N.GetX() + Yconst) / N.GetZ();
			if(!Up)
			{//lower solution
				DX_DEPTH &CDepthD = DXD->DepthEl(0, x, y);
				if(Z < CDepthD)
				{		
					CDepthD = (DX_DEPTH)Z;
					DX_ID &CIDD = DXD->IDsEl(0, x, y);
					CIDD = ElID;
				}
			}
			else
			{//upper solution
				DX_DEPTH &CDepthU = DXD->DepthEl(1, x, y);
				if(Z > CDepthU)
				{
					CDepthU = (DX_DEPTH)Z;
					DX_ID &CIDU = DXD->IDsEl(1, x, y);
					CIDU = ElID;
				}
			}
		}
	}
}
