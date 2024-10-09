#include "stdafx.h"
#include "ConstDef.h"
#include "BSurfaceTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BSurfaceTool::BSurfaceTool(void)
{
	selem = NULL;
	modify = NULL;
}
BSurfaceTool::~BSurfaceTool(void)
{
	delete [] selem;
	selem = NULL;
	delete [] modify;
	modify = NULL;
}
void BSurfaceTool::ModifyLists(BSurfaceElemTool * selem, int kol, const BMatrPair & MatrixOfTool, const NContour & Contour, const BGrazingCurve * bgc)
{
	//f=true - �������������� ������ � ��������� ���� 360 ��������
	//f=false - �������������� ������ �� � ��������� ����� ��������� � �������� �������
	BAnglesPoint ** ap = NULL;
	NLine Line;
	
	CUIntArray * p = NULL;
	for(int i = 0; i < kol; i++)
	{
		ap = selem[i].GetAPoint();
		if(selem[i].Getkol() > 0)
		{
			ap[0]->Insert(p, MatrixOfTool, selem[i].Getb(0), selem[i].Gete(0));
			delete p;
			p = NULL;
			if(Contour.Types[i] == HORIZONTAL || Contour.Types[i] == SPHERE)
			{
				p = ap[1]->Getlist();//???� ������ �� ������ � ������� �������� ���� ������ ���������� �������� �� ����������(���� ��� ��� � ���� �� �����,�.�.������ ����������)
			}
			else
			{
				Contour.GetLine(i, Line);
				p = BSurfaceElemTool::ModifyList(MatrixOfTool, Line.p[3].GetZ(), Line.p[3].GetX(), *(ap[0]), *(ap[1]), selem[i].Getb(1), selem[i].Gete(1));
				if (selem[i].Getkol() == 2)
				{
					delete p;
					p = BSurfaceElemTool::ModifyList(MatrixOfTool, Line.p[3].GetZ(), Line.p[3].GetX(), *(ap[2]), *(ap[3]), selem[i].Getb(3), selem[i].Gete(3));
				}
			}
		}
		else
		{
			BGrazingCurveElemContour::BSegmentGrazingCurve * segm = NULL;
			if(segm = bgc->GetCont(i, true, false, false))//Isgcldown
			{
				delete p;
				p = new BGrazingCurveElemContour::BSegmentGrazingCurve;
				p->Append(*segm);
			}
			if(segm = bgc->GetCont(i, false, false, false))//Isgcrdown
			{
				if(!p)
					p = new BGrazingCurveElemContour::BSegmentGrazingCurve;
				p->Append(*segm);
			}
		}
	}
	delete p;
	p = NULL;
	ap = NULL;

	for (int i = kol - 1; i > -1; i--)
	{
		ap = selem[i].GetAPoint();
		if(selem[i].Getkol() > 0)
		{
			Contour.GetLine(i, Line);

			if(selem[i].Getkol() == 2)
			{
				ap[3]->Insert(p, MatrixOfTool, selem[i].Getb(3), selem[i].Gete(3));
				delete p;
				p = BSurfaceElemTool::ModifyList(MatrixOfTool, Line.p[0].GetZ(), Line.p[0].GetX(), *(ap[3]), *(ap[2]), selem[i].Getb(2), selem[i].Gete(2));
			}
			else
				ap[1]->Insert(p, MatrixOfTool, selem[i].Getb(1), selem[i].Gete(1));

			delete p;
			if(Contour.Types[i] == HORIZONTAL || Contour.Types[i] == SPHERE)
				p = ap[0]->Getlist();
			else
				p = BSurfaceElemTool::ModifyList(MatrixOfTool, Line.p[0].GetZ(), Line.p[0].GetX(), *(ap[1]), *(ap[0]), selem[i].Getb(0), selem[i].Gete(0));
		}
		else
		{
			BGrazingCurveElemContour::BSegmentGrazingCurve * segm = NULL;
			if(segm = bgc->GetCont(i, true, true, false))//Isgclup
			{
				delete p;
				p = new BGrazingCurveElemContour::BSegmentGrazingCurve;
				p->Append(*segm);
			}
			if(segm = bgc->GetCont(i, false, true, false))//Isgcrup
			{
				if(!p)
					p = new BGrazingCurveElemContour::BSegmentGrazingCurve;
				p->Append(*segm);
			}
		}
	}
	delete p;
}
bool BSurfaceTool::CreateSurface(
								 BGrazingCurve & bgc, 
								 const BMatrPair & MatrixOfTool,
								 const BMatr & TotalDeriv,
								 const NContour & Contour, 
								 bool * direct,
								 const bool f)
{
	MBSPForest *pForest = &MBSPForest::GetActive();
	//f=true - �������� ����������� ������� ����� �����������
	//f=false - ��������
	//direct - ����������� ������ ������������� � ������ ����� �� ������ ����� true (true - "����" �� Oz, false - "�����" �� Oz)
	const int kol = bgc.Getkol();	//���������� ��������� ������� �����������
	if(kol < 1)
		return false;

	selem = new BSurfaceElemTool [kol];
	modify = new bool [kol];//true - ������ ��������(�� ��� 360 ��������), false - ���
	
	BAnglesPoint *p = NULL;
	NLine Line;
	bool loop = false;//true - ���� ���� �� ���� �����, false - ��� �� ����� �����
	for(int i = 0; i < kol; i++)
		modify[i] = false;
	TypeLineInContour Type;

	BGrazingCurveElemContour *bge = new BGrazingCurveElemContour();
	int j = 0;//���������� ������������ �� ����������� ��������� ��������� ������ ����������
	for(int i = 0; i < kol; i++)//��� ������� �������� �����������
	{
		Contour.GetLine(i, Line);
		Type = Contour.Types[i];
		BGrazingCurveElemContour::BSegmentGrazingCurve	* left = bgc.GetGCurve(i)->GetCont(true, true, true),
					* right = bgc.GetGCurve(i)->GetCont(false, true, true),
					* left1 = bgc.GetGCurve(i)->GetCont(true, false, true),
					* right1 = bgc.GetGCurve(i)->GetCont(false, false, true);
		double d = 1.;
		BPoint R, L, P, Ext;//P - ����� �������, �� ������ � ���������� �� ��� � ������� ����������� ���� ���������� ������������

		bge->Copy(*(bgc.GetGCurve(i)), false);//������� �����������
		loop = false;//��� ���������� �������� ������� ���� ���������� ���� �� �����

		switch(Type)
		{
			case SPHERE:
				p = selem[i].CreateListSphere(Line, *(bgc.GetGCurve(i)), bgc.GetGCurve(i+1), p, MatrixOfTool, TotalDeriv, f);
				break;
			case HORIZONTAL:
				direct[i] = true;//����������� ����� ��������� "����"
				loop = true;
				if(left)
				{
					R = (*MFOREST.GetPoint(right->GetAt(0))) * MatrixOfTool.Inv();
					L = (*MFOREST.GetPoint(left->GetAt(0))) * MatrixOfTool.Inv();
					if( (R-L).D2() > MIND*MIND)
						P = Line.p[0];
					else
					{
						P = Line.p[3];
						R = (*MFOREST.GetPoint(right->GetAt(right->GetUpperBound()))) * MatrixOfTool.Inv();
						L = (*MFOREST.GetPoint(left->GetAt(left->GetUpperBound()))) * MatrixOfTool.Inv();
					}
				}
				else
				{
					if(left1)
					{
						R = (*MFOREST.GetPoint(right1->GetAt(right1->GetUpperBound()))) * MatrixOfTool.Inv();
						L = (*MFOREST.GetPoint(left1->GetAt(left1->GetUpperBound()))) * MatrixOfTool.Inv();
						P = Line.p[3];
					}
					else
					{
						BPoint v = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, Line.p[3]);
						BPoint n = GrazingCurveElemContour::NormalToSurfaceOfPoint(Line, Line.p[3]);
						d = v * n;//��������� �����������
						if((d > -MIND && f) || (d < MIND && !f))//���� ���� ���������� ������ �����(��� ������) � ���� ���� ���� ������� ��� �����������
							//������� �������� ����������� ����� �������� �����������
							p = selem[i].CreateListOutLoop(Line, bge, bgc.GetGCurve(i+1), p, MatrixOfTool, Contour.Types[i], modify[i], f);
						loop = false;
					}
				}

				if(loop)
				{
					direct[i] = true;//����������� ����� ��������� "����"

					double angle1 = BSurfaceElemTool::Angle(R);
					double angle2 = BSurfaceElemTool::Angle(L);
					angle2 = BAnglesArray::GetDelta(angle2, angle1);//������������� �������
					angle2 = BAnglesPoint::Normalize(angle2/2);
					angle2 = BAnglesPoint::Normalize(angle1 + angle2);//���������� �������� �������� ���� � �����
					P = BAnglesPoint::RotatePoint(angle2, P);//����� ���������� ����� � ����� �������� ����� �� ����� ����
					BPoint v = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, P);
					BPoint n = GrazingCurveElemContour::NormalToSurfaceOfPoint(Line, P);
					d = v * n;//��������� �����������
					if(d*d < v.D2()*MINAR*MINAR)//���� �������� ���������� ������������ ������ � ����
					{
						direct[i] = false;//����������� ����� ��������� "�����"
						if(!f)//���� ������ ��������� �����
						{
							bge->Copy(*(bgc.GetGCurve(i)), true);
							direct[i] = true;//����������� ����� ��������� "����"
						}
					}
					else
					{
						if( (d > 0 && !f) || (d < 0 && f) )
							//���� ����� R � L �� ���������� ����, �� �� �������� ����� � ����. �.�. ����� ���������� ������ �����, � ,������, ����� R � L (� �� ��������)
							bge->Copy(*(bgc.GetGCurve(i)), true);//�������� �� ������ ������ � ����� �������
						else
							direct[i] = false;//����������� ����� ��������� "�����"(���������)
					}
					//��� ������ ����� R � L, �� ��� ��������� ������ �����
					p = selem[i].CreateListInLoop(Line, bge, bgc.GetGCurve(i+1), p, MatrixOfTool, Contour.Types[i], true);
				}
				break;
			case CONE:
				loop = true;//����� ���� ����� ����������� �����
				if(left)//���� ���� ������� ������� ����
				{
					Ext = (*MFOREST.GetPoint(right->GetAt(right->GetUpperBound()))) * MatrixOfTool.Inv();//��������� ������� ����� ��� ������������� ������������ ����� ������/��� �����
					P = Line.p[0];//��������� ����� P ��� �������� �����
					if(  ((*MFOREST.GetPoint(left->GetAt(left->GetUpperBound()))) - (*MFOREST.GetPoint(right->GetAt(right->GetUpperBound())))).D2() > MIND*MIND )
					{
						R = (*MFOREST.GetPoint(bge->GetCont(false, true, true)->GetAt(right->GetUpperBound()))) * MatrixOfTool.Inv();
						L = (*MFOREST.GetPoint(bge->GetCont(true, true, true)->GetAt(left->GetUpperBound()))) * MatrixOfTool.Inv();

						double angle1 = BSurfaceElemTool::Angle(R);
						double angle2 = BSurfaceElemTool::Angle(L);
						angle2 = BAnglesArray::GetDelta(angle2, angle1);
						angle2 = BAnglesPoint::Normalize(angle2/2);
						Ext = BAnglesPoint::RotatePoint(angle2, R);
						P = Line.p[3];
					}
				}
				else
				{
					if(left1)//���� ���� ������ ������� ����
					{
						Ext = (*MFOREST.GetPoint(right1->GetAt(0))) * MatrixOfTool.Inv();//��������� ������� ����� ��� ������������� ������������ ����� ������/��� �����
						P = Line.p[3];//��������� ����� P ��� �������� �����
					}
					else//���� ��� ������� ���
					{
						BPoint v = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, Line.p[3]);
						BPoint n = GrazingCurveElemContour::NormalToSurfaceOfPoint(Line, Line.p[3]);
						d = v * n;//��������� �����������
						//���� ���� ���������� ������ �����(��� ������) � ���� ���� ���� ������� ��� �����������
						//������� �������� ����������� ����� �������� �����������
						if((d > -MIND && f) || (d < MIND && !f))
							if( abs(Line.p[0].GetX() - Line.p[3].GetX()) > MIND )//���� ������� ������ ������ - ��������
								p = selem[i].CreateListOutLoop(Line, bge, bgc.GetGCurve(i+1), p, MatrixOfTool, Contour.Types[i], modify[i], f);
							else
								if(!f)//������� �� ����������� ��������
									p = selem[i].CreateListOutLoop(Line, bge, bgc.GetGCurve(i+1), p, MatrixOfTool, Contour.Types[i], modify[i], f);
						
						loop = false;
						direct[i] = true;//����������� ����� ��������� "����"(���������)
					}
				}
				if(loop)//���� ���� �����
				{
					double angle = BSurfaceElemTool::Angle(Ext);//���� ����� � ���������� �����
					P = BAnglesPoint::RotatePoint(angle, P);//����� ���������� ����� � ����� �������� ����� �� ����� ����
					BPoint v = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, P);
					BPoint n = GrazingCurveElemContour::NormalToSurfaceOfPoint(Line, P);
					d = v * n;//��������� �����������

					if( (d > 0 && f) || (d < 0 && !f) )//���� ������ ����� ��������� ������������ ������������
						p = selem[i].CreateListInLoop(Line, bge, bgc.GetGCurve(i+1), p, MatrixOfTool, Contour.Types[i], f);
					else
						p = selem[i].CreateListOutLoop(Line, bge, bgc.GetGCurve(i+1), p, MatrixOfTool, Contour.Types[i], modify[i], f);

					direct[i] = true;//����������� ����� ��������� "����"
				}
				break;
			default:
				break;
		}
	}
	delete p;
	bge->Clear();
	delete bge;
	ModifyLists(selem, kol, MatrixOfTool, Contour, &bgc);//���������� ����� � ������, ���� ��� �� ����� �� ���� ����� ����� ��������� ������� � ������ � ���� �������

	for(int i = 0; i < kol; i++)//��� ������� �������� �����������
		selem[i].Update(bgc, i, MatrixOfTool);	

	return true;
}

void BSurfaceTool::CreateSurfaceTri(	MeshArr *pMeshArr, 
									const BMatrPair & MatrixOfTool, 
									const NContour & Contour) const
{
	int kol = Contour.num;
	TypeLineInContour Type;
	NLine Line;
	for(int i = 0; i < kol; i++)//��� ������� �������� �����������
	{
		Contour.GetLine(i, Line);
		Type = Contour.Types[i];
		selem[i].CreateSurfaceTri(pMeshArr, MatrixOfTool, Line, Type, i == 0, modify[i]);
	}
}