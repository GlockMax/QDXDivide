#include "stdafx.h"
#include "ConstDef.h"
#include "NCMProject.h"
#include "SOCyl.h"
#include "SOCone.h"
#include "SOSphere.h"
#include "SOPlane.h"
#include "SOTorus.h"
#include "NMeshArray.h"
#include "BSurfaceElemTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

BSurfaceElemTool::BSurfaceElemTool(void)
{
	APoint = NULL;
	kl = 0;
	b[0] = -1;
	b[1] = -1;
	b[2] = -1;
	b[3] = -1;
	e[0] = -1;
	e[1] = -1;
	e[2] = -1;
	e[3] = -1;
	begin[0] = -1;
	begin[1] = -1;
	begin[2] = -1;
	begin[3] = -1;
}
BSurfaceElemTool::~BSurfaceElemTool(void)
{
	for(int i = 0; i < kl*2; i++)
		delete APoint[i];
	delete [] APoint;
}
double BSurfaceElemTool::Angle(const BPoint & P)
{
	BPoint i, v = P;
	v.Norm();
	v.SetH(0.0);
	v.ProjXY();
	i.Set(1.0, 0.0, 0.0, 0.0);
	double angle = 0.0;
	if(v.D2() > 1.e-12)
	{
		angle = i.Angle(v);
		angle = BAnglesPoint::Normalize(angle);
	}
	return angle;
}
CUIntArray * BSurfaceElemTool::ModifyList(	
											const BMatrPair & MatrixOfTool,
											const double z,
											const double r,
											BAnglesPoint & ap0, 
											BAnglesPoint & ap1, 
											const int num1,
											const int num2)
{
	//Если в ap1 нет точек между(по углу) парой точек в ap0, то в ap1 создается точка между(по углу) точками из ap0
	//num1 - номер в MFOREST начальной точки в ap1
	//num2 - номер в MFOREST конечной точки в ap1
	//f=true - модифицировать список в диапазоне всех 360 градусов
	//f=false - модифицировать список от в диапазоне между начальной и конечной точками

	BPoint i, P;
	i.Set(1.0, 0.0, 0.0, 0.0);
	int k0 = ap0.GetSize();
	int k1 = ap1.GetSize();
	if(k0 <= 0 || k1 <= 0)
		return NULL;
	double b, e, a0, a1;
	if(num1 < 0)
	{
		if(num2 < 0)
			return NULL;
		else
		{
			b = Angle(ap0.GetPoint( 0, MatrixOfTool));
			for(int j = 1; j < k0+1; j++)
			{
				e = Angle(ap0.GetPoint( j%k0, MatrixOfTool));
				if(!(ap1.FindBetween(b-MINAD, e+MINAD)))//если нет точек в ap1 между b и e
				{
					P = i * r;
					P.SetH(1.0);
					P = BAnglesPoint::RotatePoint(b, P);
					P.SetZ(z);
					ap1.Insert( P, MatrixOfTool);
				}
				b = e;
			}
			return ap1.Getlist();
		}
	}
	else
	{
		int n1 = ap1.Find(num1), n2;
		if(n1 < 0)
			return NULL;
		if(num2 < 0)
			n2 = (n1+k1-1)%k1;
		else
		{
			n2 = ap1.Find(num2);
			if(n2 < 0)//если нет таких номеров в списке
				return NULL;
		}
		a0 = Angle(ap1.GetPoint( n1, MatrixOfTool));
		a1 = Angle(ap1.GetPoint( n2, MatrixOfTool));
		
		b = Angle(ap0.GetPoint( 0, MatrixOfTool));
		for(int j = 1; j < k0+1; j++)
		{
			e = Angle(ap0.GetPoint( j%k0, MatrixOfTool));
			if(!(ap1.FindBetween(b-MINAD, e+MINAD)))//если нет точек в ap1 между b и e
			{
				P = i * r;
				P.SetH(1.0);
				P = BAnglesPoint::RotatePoint(b, P);
				P.SetZ(z);
				if(BAnglesArray::IsBetween(b, a0, a1))
					ap1.Insert( P, MatrixOfTool);
			}
			b = e;
		}
		// (EK) n1 and n2 should be recalculated
		n1 = ap1.Find(num1);
		if(num2 < 0)
			n2 = (n1+k1-1)%k1;
		else
			n2 = ap1.Find(num2);
		if(n1 < 0 || n2 < 0)
			return ap1.Getlist();
		return ap1.Getlist(n1, n2);
	}
	return NULL;
}
bool BSurfaceElemTool::IsVert(CUIntArray *n, const BMatrPair & MatrixOfTool, const bool f)
{
	//если f=true, то n - список "верхнего" сегмента
	//f=false - "нижнего"
	return BAnglesPoint::IsVert( n, MatrixOfTool, f);
}
int BSurfaceElemTool::Getkol(void) const
{
	//Возвращает количество пар списков
	return kl;
}
int BSurfaceElemTool::Getbegin(int i) const
{
	//Возвращает номер начальной точки из i-го списка 
	if(i < 0 || i > kl*2-1 || !begin)
		return -1;
	return begin[i];
}
int BSurfaceElemTool::Getbeginindex(int i) const
{
	//Возвращает индекс начальной точки в i-ом списке 
	if(i < 0 || i > kl*2-1 || !begin)
		return -1;
	return APoint[i]->Find(begin[i]);
}
int BSurfaceElemTool::Getb(int i) const
{
	//Возвращает номер начала диапазона вставки в i-й список 
	return b[i];
}
int BSurfaceElemTool::Gete(int i) const
{
	//Возвращает номер конца диапазона вставки в i-й список 
	return e[i];
}
BAnglesPoint ** BSurfaceElemTool::GetAPoint(void) const 
{
	return APoint;
}
BAnglesPoint * BSurfaceElemTool::CreateListInLoop(	
													const NLine & Line,
													BGrazingCurveElemContour * bgc, 
													BGrazingCurveElemContour * next, 
													BAnglesPoint * p,
													const BMatrPair & MatrixOfTool,
													TypeLineInContour Type,
													bool f)
{
	//f=true - создание режущей части
	//f=false - наоборот
	kl = 0;
	if(bgc->GetCont(true, true, true))
		kl++;
	if(bgc->GetCont(true, false, true))
		kl++;

	if(kl == 0)//если кривой скольжения нет вообще
		return NULL;

	BPoint P;//точка для нахождения радиуса основания

	APoint = new BAnglesPoint * [kl*2];//списков в 2 раза больше чем пар боковых сегментов

	//добавление номеров точек в списки

	CUIntArray	*Isgclup	= bgc->GetCont(true, true, false),
				*Isgcrup	= bgc->GetCont(false, true, false),
				*Isgcleft	= bgc->GetCont(true, true, true),
				*Isgcright	= bgc->GetCont(false, true, true),

				*Isgcldown	= bgc->GetCont(true, false, false),
				*Isgcrdown	= bgc->GetCont(false, false, false),
				*Isgcleft1	= bgc->GetCont(true, false, true),
				*Isgcright1	= bgc->GetCont(false, false, true),

				*Isgclupn = NULL, 
				*Isgcrupn = NULL;

	if(!f)
	{
		Isgclup		= bgc->GetCont(false, true, false),
		Isgcrup		= bgc->GetCont(true, true, false),
		Isgcleft	= bgc->GetCont(false, true, true),
		Isgcright	= bgc->GetCont(true, true, true),

		Isgcldown	= bgc->GetCont(false, false, false),
		Isgcrdown	= bgc->GetCont(true, false, false),
		Isgcleft1	= bgc->GetCont(false, false, true),
		Isgcright1	= bgc->GetCont(true, false, true);
	}

	double a0, a1;
	bool flag = false;//true - если дуги нужно добавлять

	if(Isgcleft)//Если есть верхние боковые сегменты
	{
		APoint[0] = new BAnglesPoint;
		APoint[1] = new BAnglesPoint;//создадим пару верхнему списку

		APoint[0]->Insert( Isgcleft, 0, MatrixOfTool);
		APoint[0]->Insert( Isgcright, 0, MatrixOfTool);

		b[0] = Isgcright->GetAt(0);
		e[0] = Isgcleft->GetAt(0);

		if(p)
		{
			CUIntArray UIA;
			APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool, b[0], e[0]);
			delete p;
			p = NULL;
		}

		APoint[0]->Insert( Isgclup, MatrixOfTool, b[0], e[0]);
		APoint[0]->Insert( Isgcrup, MatrixOfTool, b[0], e[0]);

		UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, b[0]);

		if(!Isgcleft1)//если нет нижних боковых сегментов
		{
			int x=0, y=0, z=0;
			BPoint	A = (*MFOREST.GetPoint(Isgcleft->GetAt(Isgcleft->GetUpperBound()))) * MatrixOfTool.Inv(),
				B = (*MFOREST.GetPoint(Isgcright->GetAt(Isgcright->GetUpperBound()))) * MatrixOfTool.Inv();
			if( (A - B).D2() < MIND*MIND  )
				x = 1;
			if(   A.GetX()*A.GetX()+A.GetY()*A.GetY() < MIND*MIND  &&  B.GetX()*B.GetX()+B.GetY()*B.GetY() < MIND*MIND)
				y = 1;
			if(  IsVert( Isgcleft, MatrixOfTool, true)  )
				z = 1;
			int t = x+y*2+z*4;
			switch(t)
			{
				case 0:
				//case 2://общий случай
					//Isgcleft
					a0 = Angle((*MFOREST.GetPoint(Isgcleft->GetAt(0))) * MatrixOfTool.Inv());
					a1 = Angle((*MFOREST.GetPoint(Isgcleft->GetAt(Isgcleft->GetUpperBound()))) * MatrixOfTool.Inv());
					if(BAnglesArray::Compare(a0, a1) < 0)//если сверху вниз поворот осуществляется по часовой стрелке (см. с конца оси Oz)
						APoint[1]->Insert( Isgcleft, MatrixOfTool);
					else
					{
						APoint[0]->Insert( Isgcleft, MatrixOfTool);
						APoint[1]->Insert( Isgcleft, Isgcleft->GetUpperBound(), MatrixOfTool);
					}

					//Isgcright
					a0 = Angle((*MFOREST.GetPoint(Isgcright->GetAt(0))) * MatrixOfTool.Inv());
					a1 = Angle((*MFOREST.GetPoint(Isgcright->GetAt(Isgcright->GetUpperBound()))) * MatrixOfTool.Inv());
					if(BAnglesArray::Compare(a0, a1) > 0)//если сверху вниз поворот осуществляется против часовой стрелки (см. с конца оси Oz)
					{
						APoint[1]->Insert( Isgcright, MatrixOfTool);
						begin[0] = begin[1] = Isgcright->GetAt(0);
					}
					else
					{
						APoint[0]->Insert( Isgcright, MatrixOfTool);
						APoint[1]->Insert( Isgcright, Isgcright->GetUpperBound(), MatrixOfTool);
						begin[0] = begin[1] = Isgcright->GetAt(Isgcright->GetUpperBound());
					}
					break;
				case 1:
				//case 5://добавить в нижний список
					APoint[1]->Insert( Isgcleft, MatrixOfTool);
					APoint[1]->Insert( Isgcright, MatrixOfTool);
					begin[0] = begin[1] = Isgcright->GetAt(0);							
					break;
				case 3:
				case 4:
				//case 6:
				case 7://вертикальные сегменты
					APoint[1]->Insert( Isgcleft, 1, MatrixOfTool);
					APoint[1]->Insert( Isgcright, 1, MatrixOfTool);
					begin[0] = Isgcright->GetAt(0);
					begin[1] = Isgcright->GetAt(1);
					break;
				default://???(Можно удалить, если убедиться, что такой ситуации не будет)
					CString str;
					str.Format("BSurfaceElemTool::CreateListInLoop: Невозможный случай только с верхними боковыми сегментами!!!");
					AfxMessageBox(str);
					throw;
			}

			if(Isgcleft->GetAt(Isgcleft->GetUpperBound()) != Isgcright->GetAt(Isgcright->GetUpperBound()))//если нет петли экстремумом вниз
			{
				b[1] = Isgcright->GetAt(Isgcright->GetUpperBound());
				e[1] = Isgcleft->GetAt(Isgcleft->GetUpperBound());
				flag = true;
			}
		}
		else//если есть нижние боковые сегменты
		{
			APoint[2] = new BAnglesPoint;
			APoint[3] = new BAnglesPoint;

			//Isgcright
			if(IsVert( Isgcright, MatrixOfTool, true))//если сегмент вертикален по углу "сверху"
			{
				APoint[1]->Insert( Isgcright, 1, Isgcright->GetUpperBound(), MatrixOfTool);
				begin[0] = Isgcright->GetAt(0);
				begin[1] = Isgcright->GetAt(1);
			}
			else
			{
				APoint[1]->Insert( Isgcright, MatrixOfTool);
				begin[0] = begin[1] = Isgcright->GetAt(0);
			}

			//Isgcleft
			if(IsVert( Isgcleft, MatrixOfTool, true))//если сегмент вертикален по углу "сверху"
				APoint[1]->Insert( Isgcleft, 1, Isgcleft->GetUpperBound(), MatrixOfTool);
			else
				APoint[1]->Insert( Isgcleft, MatrixOfTool);
  
			//Isgcright1
			if(IsVert( Isgcright1, MatrixOfTool, false))//если сегмент вертикален по углу "снизу"
			{
				APoint[2]->Insert( Isgcright1, 0, Isgcright1->GetUpperBound()-1, MatrixOfTool);
				APoint[3]->Insert( Isgcright1, Isgcright1->GetUpperBound(), MatrixOfTool);
				begin[2] = Isgcright1->GetAt(Isgcright1->GetUpperBound()-1);
				begin[3] = Isgcright1->GetAt(Isgcright1->GetUpperBound());
			}
			else
			{
				APoint[2]->Insert( Isgcright1, MatrixOfTool);
				APoint[3]->Insert( Isgcright1, Isgcright1->GetUpperBound(), MatrixOfTool);
				begin[2] = begin[3] = Isgcright1->GetAt(Isgcright1->GetUpperBound());
			}

			//Isgcleft1
			if(IsVert( Isgcleft1, MatrixOfTool, false))//если сегмент вертикален по углу "снизу"
			{
				APoint[2]->Insert( Isgcleft1, 0, Isgcleft1->GetUpperBound()-1, MatrixOfTool);
				APoint[3]->Insert( Isgcleft1, Isgcleft1->GetUpperBound(), MatrixOfTool);
			}
			else
			{
				APoint[2]->Insert( Isgcleft1, MatrixOfTool);
				APoint[3]->Insert( Isgcleft1, Isgcleft1->GetUpperBound(), MatrixOfTool);
			}

			b[3] = Isgcright1->GetAt(Isgcright1->GetUpperBound());
			e[3] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());

			flag = true;
		}
	}
	else//Если нет верхних боковых сегментов
	{
		delete p;
		p = NULL;

		APoint[0] = new BAnglesPoint;
		APoint[1] = new BAnglesPoint;

		APoint[1]->Insert( Isgcleft1, Isgcleft1->GetUpperBound(), MatrixOfTool);
		APoint[1]->Insert( Isgcright1, Isgcright1->GetUpperBound(), MatrixOfTool);

		APoint[0]->Insert( Isgcleft1, MatrixOfTool);
		APoint[0]->Insert( Isgcright1, MatrixOfTool);

		b[1] = begin[0] = begin[1] = Isgcright1->GetAt(Isgcright1->GetUpperBound());
		e[1] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());

		flag = true;
	}
	
	BAnglesPoint *ap = NULL;
	
	if(flag)//если нужно добавлять в список нижние сегменты
	{
		APoint[kl*2-1]->Insert( Isgcldown, MatrixOfTool, b[kl*2-1], e[kl*2-1]);
		APoint[kl*2-1]->Insert( Isgcrdown, MatrixOfTool, b[kl*2-1], e[kl*2-1]);

		if(next)
		{
			Isgclupn = next->GetCont(true, true, false);
			Isgcrupn = next->GetCont(false, true, false);

			APoint[kl*2-1]->Insert( Isgclupn, MatrixOfTool, b[kl*2-1], e[kl*2-1]);
			APoint[kl*2-1]->Insert( Isgcrupn, MatrixOfTool, b[kl*2-1], e[kl*2-1]);
		}

		UpdatingOfList( APoint[kl*2-1], Line.p[3].GetX(), MatrixOfTool, begin[kl*2-1]);

		ap = new BAnglesPoint;
		//отправка в следующий элемент инструмента нижних дуг граничной кривой (они точно прилегают к нижнему основанию)
		//запись в список без сортировки по углу. При добавлении этих точек в верхний список след. элемента инструмента работает Insert(...) с сортировкой по углу.
		ap->Insert( b[kl*2-1], MatrixOfTool);
		ap->Insert( e[kl*2-1], MatrixOfTool);
		CUIntArray UIA;
		ap->Insert(APoint[kl * 2 - 1]->Getlist(UIA), MatrixOfTool, b[kl * 2 - 1], e[kl * 2 - 1]);

		//отправка в следующий элемент инструмента нижних дуг граничной кривой (они точно прилегают к нижнему основанию)
		//запись в список без сортировки по углу. При добавлении этих точек в верхний список след. элемента инструмента работает Insert(...) с сортировкой по углу.
		ap->Insert( Isgcldown, MatrixOfTool);
		ap->Insert( Isgcrdown, MatrixOfTool);
	}
	return ap;
}
BAnglesPoint * BSurfaceElemTool::CreateListOutLoop(	
													const NLine & Line,
													BGrazingCurveElemContour * bgc, 
													BGrazingCurveElemContour * next, 
													BAnglesPoint * p,
													const BMatrPair & MatrixOfTool,
													TypeLineInContour Type,
													bool &modify,
													bool f)
{
	kl = 0;
	BAnglesPoint *ap = NULL;//список точек, передаваемых, следующему элементу контура
	//if(bgc->GetCont(true, true, true) || bgc->GetCont(true, false, true))
		//kl = 1;
	//else//если нет боковых сегментов кривой скольжения
	kl = 1;
	if(!bgc->GetCont(true, true, true) && !bgc->GetCont(true, false, true))
	{
		switch(Type)
		{
			case CONE:
				ap = CreateListCone( Line, *bgc, next, p, MatrixOfTool);
				break;
			case HORIZONTAL:
				ap = CreateListHorizontal( Line, *bgc, next, p, MatrixOfTool);
				break;
			default:
				ap = NULL;
				break;
		}
		modify = true;//создание цикличного списка
		return ap;
	}

	BPoint P;//точка для нахождения радиуса основания

	//добавление номеров точек в списки

	CUIntArray	*Isgclup	= bgc->GetCont(true, true, false),
				*Isgcrup	= bgc->GetCont(false, true, false),
				*Isgcleft	= bgc->GetCont(true, true, true),
				*Isgcright	= bgc->GetCont(false, true, true),

				*Isgcldown	= bgc->GetCont(true, false, false),
				*Isgcrdown	= bgc->GetCont(false, false, false),
				*Isgcleft1	= bgc->GetCont(true, false, true),
				*Isgcright1	= bgc->GetCont(false, false, true),

				*Isgclupn = NULL, 
				*Isgcrupn = NULL;

	if(f)
	{
		Isgclup		= bgc->GetCont(false, true, false),
		Isgcrup		= bgc->GetCont(true, true, false),
		Isgcleft	= bgc->GetCont(false, true, true),
		Isgcright	= bgc->GetCont(true, true, true),

		Isgcldown	= bgc->GetCont(false, false, false),
		Isgcrdown	= bgc->GetCont(true, false, false),
		Isgcleft1	= bgc->GetCont(false, false, true),
		Isgcright1	= bgc->GetCont(true, false, true);
	}

	double a0, a1;	

	if(Isgcleft)//Если есть верхние боковые сегменты
	{
		if(!Isgcleft1)//если нет нижних боковых сегментов
		{
			APoint = new BAnglesPoint * [2];
			APoint[0] = new BAnglesPoint;
			APoint[1] = new BAnglesPoint;

			APoint[0]->Insert( Isgcleft, 0, MatrixOfTool);
			APoint[0]->Insert( Isgcright, 0, MatrixOfTool);

			b[0] = Isgcleft->GetAt(0);
			e[0] = Isgcright->GetAt(0);

			if(p)
			{
				CUIntArray UIA;
				APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool, b[0], e[0]);
				delete p;
				p = NULL;
			}

			APoint[0]->Insert( Isgclup, MatrixOfTool, b[0], e[0]);
			APoint[0]->Insert( Isgcrup, MatrixOfTool, b[0], e[0]);

			UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, b[0]);

			//Isgcleft
			if(IsVert( Isgcleft, MatrixOfTool, true))//если весь сегмент вертикален по углу
			{
				APoint[1]->Insert( Isgcleft, 1, MatrixOfTool);
				begin[0] = Isgcleft->GetAt(0);
				begin[1] = Isgcleft->GetAt(1);
			}
			else
			{
				a0 = Angle((*MFOREST.GetPoint(Isgcleft->GetAt(0))) * MatrixOfTool.Inv());
				a1 = Angle((*MFOREST.GetPoint(Isgcleft->GetAt(Isgcleft->GetUpperBound()))) * MatrixOfTool.Inv());
				if(BAnglesArray::Compare(a0, a1) < 0)//если сверху вниз поворот осуществляется по часовой стрелке (см. с конца оси Oz)
				{
					APoint[0]->Insert( Isgcleft, MatrixOfTool);
					begin[0] = begin[1] = Isgcleft->GetAt(Isgcleft->GetUpperBound());
				}
				else
				{
					APoint[1]->Insert( Isgcleft, MatrixOfTool);
					begin[0] = begin[1] = Isgcleft->GetAt(0);
				}
			}

			//Isgcright
			if(IsVert( Isgcright, MatrixOfTool, true))//если весь сегмент вертикален по углу
				APoint[1]->Insert( Isgcright, 1, MatrixOfTool);
			else
			{
				a0 = Angle((*MFOREST.GetPoint(Isgcright->GetAt(0))) * MatrixOfTool.Inv());
				a1 = Angle((*MFOREST.GetPoint(Isgcright->GetAt(Isgcright->GetUpperBound()))) * MatrixOfTool.Inv());
				if(BAnglesArray::Compare(a0, a1) > 0)//если сверху вниз поворот осуществляется против часовой стрелки (см. с конца оси Oz)
					APoint[0]->Insert( Isgcright, MatrixOfTool);
				else
					APoint[1]->Insert( Isgcright, MatrixOfTool);
			}
			if( Isgcleft->GetAt(Isgcleft->GetUpperBound()) == Isgcright->GetAt(Isgcright->GetUpperBound()) )//случай с несовпадающими нижними концами Isgcleft и Isgcright
			{ 
				double angle = APoint[0]->NSortedArray::GetAt(Getbeginindex(0));//угол нач. точки в верхнем списке
				BPoint A = BAnglesPoint::RotatePoint(angle, Line.p[3]);//точка с тем же углом на уровне нижнего списка
				begin[1] = APoint[1]->Insert( A, MatrixOfTool);//индекс нач. точки в нижнем списке 
				begin[1] = APoint[1]->GetNumBDCELPPoint(begin[1]);//номер в MFOREST нач. точки нижнего списка 
				if(GrazingCurveElemContour::EPS() > Line.p[3].GetX())
					a0 = 90.0;
				else
					a0 = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[3].GetX())*180/PI;
				APoint[1]->Insert( BAnglesPoint::RotatePoint(-a0, Line.p[3]), MatrixOfTool);//конечная точка

				e[1] = begin[1];//при вставке в диапазоне будет вставка всех точек в 1й список (т.к. b<0, e>=0)
				modify = true;
			}
			else
			{
				APoint[1]->Insert( Isgcleft, Isgcleft->GetUpperBound(), MatrixOfTool);
				APoint[1]->Insert( Isgcright, Isgcright->GetUpperBound(), MatrixOfTool);
				b[1] = Isgcleft->GetAt(Isgcleft->GetUpperBound());
				e[1] = Isgcright->GetAt(Isgcright->GetUpperBound());
			}
		}
		else//если есть нижние боковые сегменты
		{
			if(IsVert( Isgcright, MatrixOfTool, true))//Isgcright. Если один из сегментов вертикален, то остальные тоже вертикальны
			{
				kl = 2;

				APoint = new BAnglesPoint * [4];
				for(int i = 0; i < 4; i++)
					APoint[i] = new BAnglesPoint;

				APoint[0]->Insert( Isgcleft, 0, MatrixOfTool);
				APoint[0]->Insert( Isgcright, 0, MatrixOfTool);

				b[0] = Isgcleft->GetAt(0);
				e[0] = Isgcright->GetAt(0);

				if(p)
				{
					CUIntArray UIA;
					APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool, b[0], e[0]);
					delete p;
					p = NULL;
				}

				APoint[0]->Insert( Isgclup, MatrixOfTool, b[0], e[0]);
				APoint[0]->Insert( Isgcrup, MatrixOfTool, b[0], e[0]);

				UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, b[0]);//????????

				begin[0] = Isgcleft->GetAt(0);

				APoint[1]->Insert( Isgcleft1, 0, Isgcleft1->GetUpperBound()-1, MatrixOfTool);
				APoint[1]->Insert( Isgcright1, 0, Isgcright1->GetUpperBound()-1, MatrixOfTool);
				begin[1] = Isgcright1->GetAt(Isgcright1->GetUpperBound()-1);

				APoint[2]->Insert( Isgcleft, 1, Isgcleft->GetUpperBound(), MatrixOfTool);
				APoint[2]->Insert( Isgcright, 1, Isgcright->GetUpperBound(), MatrixOfTool);
				begin[2] = Isgcright->GetAt(1);//???

				APoint[3]->Insert( Isgcleft1, Isgcleft1->GetUpperBound(), MatrixOfTool);
				APoint[3]->Insert( Isgcright1, Isgcright1->GetUpperBound(), MatrixOfTool);

				b[3] = begin[3] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());
				e[3] = Isgcright1->GetAt(Isgcright1->GetUpperBound());
			}
			else
			{
				APoint = new BAnglesPoint * [2];
				APoint[0] = new BAnglesPoint;
				APoint[1] = new BAnglesPoint;

				APoint[0]->Insert( Isgcleft, MatrixOfTool);
				APoint[0]->Insert( Isgcright, MatrixOfTool);

				b[0] = Isgcleft->GetAt(0);
				e[0] = Isgcright->GetAt(0);
				begin[0] = Isgcleft->GetAt(Isgcleft->GetUpperBound());

				if(p)
				{
					CUIntArray UIA;
					APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool, b[0], e[0]);
					delete p;
					p = NULL;
				}

				APoint[0]->Insert( Isgclup, MatrixOfTool, b[0], e[0]);
				APoint[0]->Insert( Isgcrup, MatrixOfTool, b[0], e[0]);

				UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, b[0], e[0]);

				APoint[1]->Insert( Isgcleft1, MatrixOfTool);
				APoint[1]->Insert( Isgcright1, MatrixOfTool);

				b[1] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());
				e[1] = Isgcright1->GetAt(Isgcright1->GetUpperBound());

				double angle = APoint[0]->NSortedArray::GetAt(Getbeginindex(0));//угол нач. точки в верхнем списке
				BPoint A = BAnglesPoint::RotatePoint(angle, Line.p[3]);//точка с тем же углом на уровне нижнего списка
				begin[1] = APoint[1]->Insert( A, MatrixOfTool);//индекс в списке нач. точки в нижнем списке 
				begin[1] = APoint[1]->GetNumBDCELPPoint(begin[1]);//номер в MFOREST нач. точки в нижнем списке 
				modify = true;
			}
		}
	}
	else//Если нет верхних боковых сегментов
	{
		APoint = new BAnglesPoint * [kl*2];
		APoint[0] = new BAnglesPoint;
		APoint[1] = new BAnglesPoint;
		
		APoint[1]->Insert( Isgcleft1, Isgcleft1->GetUpperBound(), MatrixOfTool);
		APoint[1]->Insert( Isgcright1, Isgcright1->GetUpperBound(), MatrixOfTool);

		b[1] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());
		e[1] = Isgcright1->GetAt(Isgcright1->GetUpperBound());

		begin[1] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());//вместо 0

		APoint[1]->Insert( Isgcleft1, MatrixOfTool);
		APoint[1]->Insert( Isgcright1, MatrixOfTool);

		APoint[0]->Insert( Isgclup, MatrixOfTool);
		APoint[0]->Insert( Isgcrup, MatrixOfTool);

		if(p)
		{
			CUIntArray UIA;
			APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool);
			delete p;
			p = NULL;
		}

		BPoint A = Line.p[0];
		
		double angle = APoint[1]->NSortedArray::GetAt(Getbeginindex(1));//угол нач. точки в верхнем списке
		A = BAnglesPoint::RotatePoint(angle, A);//точка с тем же углом на уровне нижнего списка
		begin[0] = APoint[0]->Insert( A, MatrixOfTool);//индекс в списке нач. точки в нижнем списке 
		begin[0] = APoint[0]->GetNumBDCELPPoint(begin[0]);//номер из MFOREST нач. точки в нижнем списке
		if(GrazingCurveElemContour::EPS() > Line.p[0].GetX())
			a0 = 90.0;
		else
			a0 = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[0].GetX())*180/PI;
		APoint[0]->Insert( BAnglesPoint::RotatePoint(-a0, Line.p[0]), MatrixOfTool);//конечная точка

		UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, begin[0]);

		modify = true;
	}

	APoint[kl*2-1]->Insert( Isgcldown, MatrixOfTool, b[kl*2-1], e[kl*2-1]);
	APoint[kl*2-1]->Insert( Isgcrdown, MatrixOfTool, b[kl*2-1], e[kl*2-1]);

	if(next)
	{
		Isgclupn = next->GetCont(true, true, false);
		Isgcrupn = next->GetCont(false, true, false);

		APoint[kl*2-1]->Insert( Isgclupn, MatrixOfTool, b[kl*2-1], e[kl*2-1]);		
		APoint[kl*2-1]->Insert( Isgcrupn, MatrixOfTool, b[kl*2-1], e[kl*2-1]);	
	}

	UpdatingOfList( APoint[kl*2-1], Line.p[3].GetX(), MatrixOfTool, b[kl*2-1], e[kl*2-1]);

	ap = new BAnglesPoint;
	//отправка в следующий элемент инструмента прилегающей к нижнему основанию части(в частном случае всего) списка точек
	ap->Insert( b[kl*2-1], MatrixOfTool);
	ap->Insert( e[kl*2-1], MatrixOfTool);
	CUIntArray UIA;
	ap->Insert(APoint[kl * 2 - 1]->Getlist(UIA), MatrixOfTool, b[kl * 2 - 1], e[kl * 2 - 1]);

	//отправка в следующий элемент инструмента нижних дуг граничной кривой (они точно прилегают к нижнему основанию)
	//запись в список без сортировки по углу. При добавлении этих точек в верхний список след. элемента инструмента работает Insert(...) с сортировкой по углу.
	ap->Insert( Isgcldown, MatrixOfTool);
	ap->Insert( Isgcrdown, MatrixOfTool);
	return ap;
}
BAnglesPoint * BSurfaceElemTool::CreateListHorizontal(	
														const NLine & Line, 
														BGrazingCurveElemContour & bgc, 
														BGrazingCurveElemContour * next,  
														BAnglesPoint * p, 
														const BMatrPair & MatrixOfTool)
{
	double a;
	BAnglesPoint *ap = new BAnglesPoint;//список точек, передаваемых, следующему элементу контура
	APoint = new BAnglesPoint * [2];
	kl = 1;

	APoint[0] = new BAnglesPoint;
	APoint[1] = new BAnglesPoint;

	if(Line.p[0].GetX() < GrazingCurveElemContour::EPS() || Line.p[3].GetX() < GrazingCurveElemContour::EPS())//если имеется хотя бы одно основание нулевого радиуса
	{
		BPoint P;
		int n = 0;//номер списка на большем основании
		if(Line.p[0].GetX() < Line.p[3].GetX())
		{
			P = Line.p[3];
			n = 1;
		}
		else
			P = Line.p[0];

		int g;
		if(p)
		{
			CUIntArray UIA;
			APoint[n]->Insert(p->Getlist(UIA), MatrixOfTool);
			delete p;
			p = NULL;
		}
		else
			APoint[n]->Insert( P, MatrixOfTool);

		g = APoint[n]->GetNumBDCELPPoint(0);
		APoint[(n+1)%2]->Insert( g, MatrixOfTool);

		if(GrazingCurveElemContour::EPS() > P.GetX())
			a = 90.0;
		else
			a = 2*acos(1-GrazingCurveElemContour::EPS()/P.GetX())*180/PI;

		begin[0] = APoint[0]->GetNumBDCELPPoint(0);
		begin[1] = APoint[1]->GetNumBDCELPPoint(0);

		b[0] = -1;
		b[1] = -1;
		e[n] = begin[n];

		if(next)
		{
			CUIntArray * Isgclupn = next->GetCont(true, true, false),
						* Isgcrupn = next->GetCont(false, true, false);
			APoint[1]->Insert( Isgclupn, MatrixOfTool);
			APoint[1]->Insert( Isgcrupn, MatrixOfTool);
		}

		if(APoint[n]->GetSize() == 1)
		{
			APoint[n]->Insert( BAnglesPoint::RotatePoint(-a, P), MatrixOfTool);//конечная точка
			APoint[n]->Insert( P, MatrixOfTool);//конечная точка
		}

		CUIntArray * Isgclup = bgc.GetCont(true, true, false),
					* Isgcrup = bgc.GetCont(false, true, false),
					* Isgcldown = bgc.GetCont(true, false, false),
					* Isgcrdown = bgc.GetCont(false, false, false);

		APoint[0]->Insert( Isgclup, MatrixOfTool);
		APoint[0]->Insert( Isgcrup, MatrixOfTool);

		APoint[1]->Insert( Isgcldown, MatrixOfTool);
		APoint[1]->Insert( Isgcrdown, MatrixOfTool);

		UpdatingOfList( APoint[n], P.GetX(), MatrixOfTool, APoint[n]->GetNumBDCELPPoint(1));
	}
	else
	{
		if (p)
		{
			CUIntArray UIA;
			APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool);
		}
		else
			APoint[0]->Insert( Line.p[0], MatrixOfTool);
		if(GrazingCurveElemContour::EPS() > Line.p[0].GetX())
			a = 90.0;
		else
			a = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[0].GetX())*180/PI;
		if(APoint[0]->GetSize() == 1)
			APoint[0]->Insert( BAnglesPoint::RotatePoint(-a, Line.p[0]), MatrixOfTool);//конечная точка

		begin[0] = APoint[0]->GetNumBDCELPPoint(0);
		a = Angle((*MFOREST.GetPoint(begin[0])) * MatrixOfTool.Inv());//взяли начальную точку
		APoint[1]->Insert( BAnglesPoint::RotatePoint(a, Line.p[3]), MatrixOfTool);//повернули на тот же угол на "нижнем" основании точку контура и занесли ее в "нижний" список
		begin[1] = APoint[1]->GetNumBDCELPPoint(0);//тут же взяли номер начальной точки(точка "напротив" начальной точки "верхнего" списка) "нижнего" списка
		if(GrazingCurveElemContour::EPS() > Line.p[3].GetX())
			a = 90.0;
		else
			a = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[3].GetX())*180/PI;
		APoint[1]->Insert(BAnglesPoint::RotatePoint(a, (*MFOREST.GetPoint(begin[1])) * MatrixOfTool.Inv()), MatrixOfTool);
		UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, begin[0]);
		UpdatingOfList( APoint[1], Line.p[1].GetX(), MatrixOfTool, begin[1]);

	}
	CUIntArray UIA;
	ap->Insert(APoint[1]->Getlist(UIA), MatrixOfTool);
	return ap;
}
BAnglesPoint * BSurfaceElemTool::CreateListCone(
												const NLine & Line, 
												BGrazingCurveElemContour & bgc, 
												BGrazingCurveElemContour * next,  
												BAnglesPoint * p, 
												const BMatrPair & MatrixOfTool)
{
	kl = 1;
	BAnglesPoint *ap = NULL;//список точек, передаваемых, следующему элементу контура
	double a;
	APoint = new BAnglesPoint * [2];
	if(p)
	{
		APoint[0] = p;
		p = NULL;
		begin[0] = APoint[0]->Insert( Line.p[0], MatrixOfTool);
		begin[0] = APoint[0]->GetNumBDCELPPoint(begin[0]);
	}
	else
	{
		APoint[0] = new BAnglesPoint;
		
		if(GrazingCurveElemContour::EPS() > Line.p[0].GetX())
			a = 90.0;
		else
			a = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[0].GetX())*180/PI;

		APoint[0]->Insert( Line.p[0], MatrixOfTool);
		begin[0] = APoint[0]->GetNumBDCELPPoint(0);
		APoint[0]->Insert( BAnglesPoint::RotatePoint(-a, Line.p[0]), MatrixOfTool);//конечная точка
	}

	CUIntArray	* Isgclup = bgc.GetCont(true, true, false),
				* Isgcrup = bgc.GetCont(false, true, false);

	APoint[0]->Insert( Isgclup, MatrixOfTool);
	APoint[0]->Insert( Isgcrup, MatrixOfTool);

	UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, begin[0]);
	e[0] = begin[0];

	if(GrazingCurveElemContour::EPS() > Line.p[3].GetX())
		a = 90.0;
	else
		a = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[3].GetX())*180/PI;

	APoint[1] = new BAnglesPoint;
	APoint[1]->Insert( Line.p[3], MatrixOfTool);
	begin[1] = APoint[1]->GetNumBDCELPPoint(0);
	APoint[1]->Insert( BAnglesPoint::RotatePoint(-a, Line.p[3]), MatrixOfTool);//конечная точка
	
	if(next)
	{
		CUIntArray * Isgcldown = bgc.GetCont(true, false, false),		
					* Isgcrdown = bgc.GetCont(false, false, false);

		APoint[1]->Insert( Isgcldown, MatrixOfTool);		
		APoint[1]->Insert( Isgcrdown, MatrixOfTool);
	}
	UpdatingOfList( APoint[1], Line.p[3].GetX(), MatrixOfTool, begin[1]);
	e[1] = begin[1];

	ap = new BAnglesPoint;
	CUIntArray UIA;
	ap->Insert(APoint[1]->Getlist(UIA), MatrixOfTool);
	return ap;
}
BAnglesPoint * BSurfaceElemTool::CreateListSphere(	
													const NLine & Line, 
													BGrazingCurveElemContour & bgc, 
													BGrazingCurveElemContour * next, 
													BAnglesPoint * p, 
													const BMatrPair & MatrixOfTool,
													const BMatr & TotalDeriv,
													bool f)
{
	CUIntArray	*Isgclup	= bgc.GetCont(true, true, false),
				*Isgcrup	= bgc.GetCont(false, true, false),
				*Isgcleft	= bgc.GetCont(true, true, true),
				*Isgcright	= bgc.GetCont(false, true, true),

				*Isgcldown	= bgc.GetCont(true, false, false),
				*Isgcrdown	= bgc.GetCont(false, false, false),
				*Isgcleft1	= bgc.GetCont(true, false, true),
				*Isgcright1	= bgc.GetCont(false, false, true),

				*Isgclupn = NULL, 
				*Isgcrupn = NULL;

	if(!f)//если нережущая часть
	{
		Isgclup	= bgc.GetCont(false, true, false),
		Isgcrup	= bgc.GetCont(true, true, false),
		Isgcleft	= bgc.GetCont(false, true, true),
		Isgcright	= bgc.GetCont(true, true, true),

		Isgcldown	= bgc.GetCont(false, false, false),
		Isgcrdown	= bgc.GetCont(true, false, false),
		Isgcleft1	= bgc.GetCont(false, false, true),
		Isgcright1	= bgc.GetCont(true, false, true);
	}

	if(next)
	{
		Isgclupn = next->GetCont(true, true, false);
		Isgcrupn = next->GetCont(false, true, false);
	}

	BAnglesPoint *ap = NULL;

	//петля может быть только одна на сфере. Либо - экстремумом вниз, либо - вверх

	if(Isgcleft)//Означает и то, что сфера выпуклая по оси Oz "вниз"
	{
		APoint = new BAnglesPoint * [2];
		APoint[0] = new BAnglesPoint;
		APoint[1] = new BAnglesPoint;
		kl = 1;

		APoint[0]->Insert( Isgcright, 0, MatrixOfTool);
		APoint[0]->Insert( Isgcleft, 0, MatrixOfTool);
		b[0] = begin[1] = begin[0] = Isgcright->GetAt(0);
		e[0] = Isgcleft->GetAt(0);

		if(p)
		{
			CUIntArray UIA;
			APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool, b[0], e[0]);
			delete p;
			p = NULL;
		}

		APoint[0]->Insert( Isgclup, MatrixOfTool, b[0], e[0]);
		APoint[0]->Insert( Isgcrup, MatrixOfTool, b[0], e[0]);

		UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, b[0]);

		BPoint A = (*MFOREST.GetPoint(Isgcright->GetAt(Isgcright->GetUpperBound())));//не нужно "* M", т.к. разница между точкками та же.
		BPoint B = (*MFOREST.GetPoint(Isgcleft->GetAt(Isgcleft->GetUpperBound())));
		if((A-B).D2() > 1.e-12)//если экстремума нет(вернее он просто выходит за границы элемента инструмента)
		{
			b[1] = Isgcright->GetAt(Isgcright->GetUpperBound());
			e[1] = Isgcleft->GetAt(Isgcleft->GetUpperBound());

			APoint[1]->Insert( Isgcldown, MatrixOfTool, b[1], e[1]);
			APoint[1]->Insert( Isgcrdown, MatrixOfTool, b[1], e[1]);

			APoint[1]->Insert( Isgclupn, MatrixOfTool, b[1], e[1]);
			APoint[1]->Insert( Isgcrupn, MatrixOfTool, b[1], e[1]);

			UpdatingOfList( APoint[1], Line.p[3].GetX(), MatrixOfTool, b[1]);

			ap = new BAnglesPoint;
			ap->Insert( b[1], MatrixOfTool);
			ap->Insert( e[1], MatrixOfTool);
			CUIntArray UIA;
			ap->Insert(APoint[1]->Getlist(UIA), MatrixOfTool, b[1], e[1]);
		}
		else//если есть экстремум на элементе инструмента
			if(!f)
				APoint[1]->SetPoint( Line.p[3], MatrixOfTool);

		APoint[1]->Setlist( Isgcright, MatrixOfTool, 1, Isgcright->GetUpperBound());
		APoint[1]->Setlist( Isgcleft, MatrixOfTool, 1, Isgcleft->GetUpperBound()-1);

		return ap;
	}

	if(Isgcleft1)//Означает и то, что сфера выпуклая по оси Oz "вверх"
	{
		delete p;
		p = NULL;

		APoint = new BAnglesPoint * [2];
		APoint[0] = new BAnglesPoint;
		APoint[1] = new BAnglesPoint;
		kl = 1;

		APoint[1]->Insert( Isgcright1, Isgcright1->GetUpperBound(), MatrixOfTool);
		APoint[1]->Insert( Isgcleft1, Isgcleft1->GetUpperBound(), MatrixOfTool);

		b[1] = begin[1] = begin[0] = Isgcright1->GetAt(Isgcright1->GetUpperBound());
		e[1] = Isgcleft1->GetAt(Isgcleft1->GetUpperBound());

		APoint[1]->Insert( Isgcldown, MatrixOfTool, b[1], e[1]);
		APoint[1]->Insert( Isgcrdown, MatrixOfTool, b[1], e[1]);
		
		APoint[1]->Insert( Isgclupn, MatrixOfTool, b[1], e[1]);
		APoint[1]->Insert( Isgcrupn, MatrixOfTool, b[1], e[1]);

		UpdatingOfList( APoint[1], Line.p[3].GetX(), MatrixOfTool, b[1], e[1]);

		ap = new BAnglesPoint;
		ap->Insert( b[1], MatrixOfTool);
		ap->Insert( e[1], MatrixOfTool);
		CUIntArray UIA;
		ap->Insert(APoint[1]->Getlist(UIA), MatrixOfTool, b[1], e[1]);

		APoint[0]->Setlist( Isgcright1, MatrixOfTool, 0, Isgcright1->GetUpperBound()-1);
		APoint[0]->Setlist( Isgcleft1, MatrixOfTool, 1, Isgcleft1->GetUpperBound()-1);
		APoint[0]->SetPoint( Line.p[0], MatrixOfTool);

		APoint[0]->Insert( Line.p[0], MatrixOfTool);

		return ap;
	}

	//Проверяем знак поверхности
	BPoint Normal = GrazingCurveElemContour::NormalToSurfaceOfLine(Line, 0.5);
	BPoint v = Line.GetCenter();
	v = GrazingCurveElemContour::SpeedOfPointTool(MatrixOfTool, TotalDeriv, v);
	double d = v * Normal;
	// Если нужно создать сферический элемент инструмента полностью(на всем сегменте поверхности один знак)
	if(f && d > 0.0  ||  !f && d < 0.0)
	{
		APoint = new BAnglesPoint * [2];
		APoint[0] = new BAnglesPoint;
		APoint[1] = new BAnglesPoint;
		kl = 1;

		if(p)
		{
			CUIntArray UIA;
			APoint[0]->Insert(p->Getlist(UIA), MatrixOfTool);
			delete p;
			p = NULL;
		}

		//заносим сегменты по всем 360 градусам
		APoint[0]->Insert( Isgclup, MatrixOfTool);
		APoint[0]->Insert( Isgcrup, MatrixOfTool);

		APoint[1]->Insert( Isgcldown, MatrixOfTool);
		APoint[1]->Insert( Isgcrdown, MatrixOfTool);
		
		APoint[1]->Insert( Isgclupn, MatrixOfTool);
		APoint[1]->Insert( Isgcrupn, MatrixOfTool);

		//Если в верхний список не добавились точки из сегментов, то границы с предыдущим элементом нет,
		//а значит, предыдущий сегмент тоже был создан на все 360 градусов, и точки добавились обязательно через "p"

		double a = 0.0;
		if(GrazingCurveElemContour::EPS() > Line.p[0].GetX())
			a = 90.0;
		else
			a = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[0].GetX())*180/PI;
		begin[0] = APoint[0]->Insert( Line.p[0], MatrixOfTool);
		begin[0] = APoint[0]->GetNumBDCELPPoint(begin[0]);
		e[0] = APoint[0]->Insert( BAnglesPoint::RotatePoint(-a, Line.p[0]), MatrixOfTool);//конечная точка. b[0]<0,e[0]>=0 - вставляем точку без интервалов
		e[0] =  APoint[0]->GetNumBDCELPPoint(e[0]);
		UpdatingOfList( APoint[0], Line.p[0].GetX(), MatrixOfTool, b[0], e[0]);
		
		if(GrazingCurveElemContour::EPS() > Line.p[3].GetX())
			a = 90.0;
		else
			a = 2*acos(1-GrazingCurveElemContour::EPS()/Line.p[3].GetX())*180/PI;
		begin[1] = APoint[1]->Insert( Line.p[3], MatrixOfTool);
		begin[1] = APoint[1]->GetNumBDCELPPoint(begin[1]);
		e[1] = APoint[1]->Insert( BAnglesPoint::RotatePoint(-a, Line.p[3]), MatrixOfTool);//конечная точка. b[1]<0,e[1]>=0 - вставляем точку без интервалов
		e[1] =  APoint[1]->GetNumBDCELPPoint(e[1]);
		UpdatingOfList( APoint[1], Line.p[1].GetX(), MatrixOfTool, b[1], e[1]);

		ap = new BAnglesPoint;
		CUIntArray UIA;
		ap->Insert(APoint[1]->Getlist(UIA), MatrixOfTool);
	}
	return ap;
}

void BSurfaceElemTool::CreateSurfaceHorizontal(NTriMesh *pMesh, const NLine & Line) const
{
	int kol0 = APoint[0]->GetSize(),
		kol1 = APoint[1]->GetSize();

	if(kol1 == 1)
	{
		int beg = Getbeginindex(0);
		for(int i = 0; i < kol0 - 1; i++)
			pMesh->AddTri(APoint[1]->GetNumForestPoint( 0),
						APoint[0]->GetNumForestPoint( (beg+i+1)%kol0),
						APoint[0]->GetNumForestPoint( (beg+i)%kol0));
		return;
	}
	
	if(kol0 == 1)
	{
		int beg = Getbeginindex(1);
		for(int i = 0; i < kol1 - 1; i++)
			pMesh->AddTri(APoint[0]->GetNumForestPoint( 0),
						APoint[1]->GetNumForestPoint( (beg+i)%kol1),
						APoint[1]->GetNumForestPoint( (beg+i+1)%kol1));
		return;
	}
}

void BSurfaceElemTool::CreateSurfaceCone(NTriMesh *pMesh, const NLine & Line) const
{
	int kol0 = APoint[0]->GetSize(),
		kol1 = APoint[1]->GetSize();

	if(kol1 == 1)
	{
		int beg = Getbeginindex(0);
		for(int i = 0; i < kol0 - 1; i++)
			pMesh->AddTri(APoint[1]->GetNumForestPoint( 0),
						APoint[0]->GetNumForestPoint( (beg+i+1)%kol0),
						APoint[0]->GetNumForestPoint( (beg+i)%kol0));
		return;
	}
	
	if(kol0 == 1)
	{
		int beg = Getbeginindex(1);
		for(int i = 0; i < kol1 - 1; i++)
			pMesh->AddTri(APoint[0]->GetNumForestPoint( 0),
						APoint[1]->GetNumForestPoint( (beg+i+1)%kol1),
						APoint[1]->GetNumForestPoint( (beg+i)%kol1));
		return;
	}
}

MVRDS * BSurfaceElemTool::CreateSurfaceSphere(const NLine & Line, const BMatrPair & MatrixOfTool) const
{
	MVRDS * List = new MVRDS;
	NTriMesh *pMesh = new NTriMesh;
	int v1, v2, n;
	if(Getb(0) != -1 || Gete(0) != -1)
	{
		List = new MVRDS;

		int num = Getbeginindex(0);//???отладочная версия
		num = APoint[0]->GetNumForestPoint( num);
		v1 = List->AddVertex(num);

		num = (Getbeginindex(0)+1)%APoint[0]->GetCount();//???отладочная версия
		num = APoint[0]->GetNumForestPoint( num);
		v2 = List->AddVertex(num);

		//v1 = List->AddVertex(APoint[0]->GetNumForestPoint( Getbeginindex(0)));
		//v2 = List->AddVertex(APoint[0]->GetNumForestPoint( (Getbeginindex(0)+1)%APoint[0]->GetCount()));
		n = 0;
	}
	else
		if(Getb(1) != -1 || Gete(1) != -1)
		{
			List = new MVRDS;
			v2 = List->AddVertex(APoint[1]->GetNumForestPoint( Getbeginindex(1)));
			v1 = List->AddVertex(APoint[1]->GetNumForestPoint( (Getbeginindex(1)+1)%APoint[1]->GetCount()));
			n = 1;
		}
		else
			return NULL;
	
	int num;
	auto kol = APoint[n]->GetCount();//список, у которого уже взяли 2 точки, для определения направления обхода контура по кривой
	for(int i = (Getbeginindex(n)+2)%kol, j = 2; j < kol; i++, j++)
	{
		num = APoint[n]->GetNumForestPoint( i%kol);
		List->AddVertex(num);
	}

	auto k = APoint[(n+1)%2]->GetCount();
	for(int i = 0; i < k; ++i)
	{
		num = APoint[(n+1)%2]->GetNumForestPoint( i);
		List->AddVertex(num);
	}

	List->AddActiveEdge(MEdge(List, v2, v1, MEdge::NOT_DEFINED, MEdge::EMPTY));

	BPoint C = Line.GetCenter() * MatrixOfTool.Or();
	double R = Line.GetRad();

	BMatr Matr = MatrixOfTool.Inv();
	if(List->MakeLimTolTri(GrazingCurveElemContour::EPS(), C, R) < 0)
		return NULL;

//	List->ModifyTriFace();//поменяли MFOREST-номера у граней на порядковые номера в MVRDS

	//Меняем MFOREST-номера на BDCELP-номера
	int j = 0;
	for(int i = (Getbeginindex(n))%kol; j < kol; i++, j++)
	{
		num = APoint[n]->GetNumBDCELPPoint(i%kol);
//		List->SetVertex(j, num);
	}

	for(int i = 0; i < k; i++, j++)
	{
		num = APoint[(n+1)%2]->GetNumBDCELPPoint(i);
//		List->SetVertex(j, num);
	}
	int VNum = 0, TNum = 0;
//	int * VList = List->GetVList(&VNum);
//	int ** EList = List->GetEList(&TNum);
//	int ** TList = List->GetTList(&TNum);
//	bool * def = List->GetTDefList(&TNum);
	//VRS.AddMVRDS(	VList, List->GetVNumMax(),
	//				EList, 
	//				TList, List->GetTNumMax(),
	//				def,
	//				j);
	return List;
}
int BSurfaceElemTool::UpdatingOfList(BAnglesPoint * bap, double r, const BMatrPair & MatrixOfTool, int b, int e)
{
	//num - индекс начальной точки в MFOREST
	if(!bap)
		return -1;
	int kol = bap->GetSize();
	if(kol < 2)
		return -1;
	int beg = bap->Find(b);//индекс начальной точки в bap.nlist
	if(beg < 0)
		return -1;
	BPoint P;
	double a0, a1, da, a = 90.0;
	if(r > GrazingCurveElemContour::EPS())
		a = 2*acos(1-GrazingCurveElemContour::EPS()/r)*180/PI;
	for(int i = 0; i < kol-1; i++)
	{
		a0 = bap->GetAt((beg+i)%kol);
		a1 = bap->GetAt((beg+i+1)%kol);
		da = BAnglesArray::GetDelta(a1, a0);
		P = (*MFOREST.GetPoint(bap->GetNumBDCELPPoint((beg + i) % kol))) * MatrixOfTool.Inv();
		BPoint Pb = P;
		if (P.IsPoint() && Pb.ProjXY().D2() > MIND * MIND)// TEMPORARY: Защита от непонятной ошибки
		{
			while (da > a)
			{
				P = BAnglesPoint::RotatePoint(a, P);
				a0 = Angle(P);
				da = BAnglesArray::GetDelta(a1, a0);
				if (360.0 - abs(da) < MINAR)
					break;
				bap->Insert(P, MatrixOfTool, b, e);
			}
		}
		else
		{
			P.Set(0, 0, 0, 0);
		}
	}
	return 1;
}
void BSurfaceElemTool::Update(BGrazingCurve & bgc, int i, const BMatrPair & MatrixOfTool) const
{
	BGrazingCurveElemContour *bgce, *prev, *next;
	CUIntArray *segm = NULL;

	bgce = bgc.GetGCurve(i);
	prev = bgc.GetGCurve(i-1);
	next = bgc.GetGCurve(i+1);

	segm = bgce->GetCont(true, true, false);//Isgclup
	if(segm)
		Update( segm, MatrixOfTool, true);
	segm = bgce->GetCont(false, true, false);//Isgcrup
	if(segm)
		Update( segm, MatrixOfTool, true);
	segm = bgce->GetCont(true, false, false);//Isgcldown
	if(segm)
		Update( segm, MatrixOfTool, false);
	segm = bgce->GetCont(false, false, false);//Isgcrdown
	if(segm)
		Update( segm, MatrixOfTool, false);

	if(prev)
	{
		segm = prev->GetCont(true, false, false);//Isgcldown
		if(segm)
			Update( segm, MatrixOfTool, true);
		segm = prev->GetCont(false, false, false);//Isgcrdown
		if(segm)
			Update( segm, MatrixOfTool, true);
	}
	if(next)
	{
		segm = next->GetCont(true, true, false);//Isgclup
		if(segm)
			Update( segm, MatrixOfTool, false);
		segm = next->GetCont(false, true, false);//Isgcrup
		if(segm)
			Update( segm, MatrixOfTool, false);
	}
}
void BSurfaceElemTool::Update(CUIntArray *segm, const BMatrPair & MatrixOfTool, bool f) const
{
	//f=true - добавить точки из верхнего списка, f=false - добавить точки из нижнего списка
	//left=true - обновляем левый сегмент
	if(kl <= 0)
		return;
	if(!APoint)
		return;
	if(!segm)
		return;
	int n = 0;
	if(!f)
		n = kl*2-1;
	APoint[n]->Getsegm( segm, MatrixOfTool);//(EK) заполняет segm
}

int BSurfaceElemTool::MakeToolSurface(const NLine & Line, TypeLineInContour Type, const BMatrPair & MatrixOfTool)
{

	BPoint Pb = Line.p[0];
	BPoint Pe = Line.p[3];
	double sgn = -1.;
	switch (Type)
	{
	case HORIZONTAL:
	{
		bool First = (fabs(Pb.GetX()) < MIND);
		if (First)
			sgn *= -1;
		return MFOREST.AddOrigSurf(new SOPlane(
			BPoint(0., 0., 1., 0.) * MatrixOfTool.Or() * sgn,
			Pb * MatrixOfTool.Or()));
	}
	case CONE:
		// Create surfaces
		if (fabs(Pb.GetX() - Pe.GetX()) < MIND)
		{// Cylinder
			return MFOREST.AddOrigSurf(new SOCyl(
				BPoint(0., 0., 0., 1.) * MatrixOfTool.Or(),
				BPoint(0., 0., 1., 0.) * MatrixOfTool.Or(),
				Pb.GetX()));
		}
		else
		{// Cone
			return MFOREST.AddOrigSurf(new SOCone(
				BPoint(0., 0., 0., 1.) * MatrixOfTool.Or(),
				BPoint(0., 0., 1., 0.) * MatrixOfTool.Or(),
				Pb * MatrixOfTool.Or(),
				Pe * MatrixOfTool.Or()));
		}
	case TORAHS:
	case SPHERE:
	{
		// Create surfaces
		BPoint C = Line.GetCenter();
		if (fabs(C.GetX()) < MIND)
		{// Sphere
			return MFOREST.AddOrigSurf(new SOSphere(
				Line.GetRad(),
				C * MatrixOfTool.Or()));
		}
		else
		{// Tor
			return MFOREST.AddOrigSurf(new SOTorus(
				C.GetX(),
				Line.GetRad(),
				BPoint(0., 0., C.GetZ(), 1.) * MatrixOfTool.Or(),
				BPoint(0., 0., 1., 0.) * MatrixOfTool.Or()));
		}
	}
	default:
		return -1;
	}
}

const CUIntArray * BSurfaceElemTool::CreateSurfaceTri(	MeshArr *pMeshArr, 			//ВРС
													const BMatrPair & MatrixOfTool, 
													const NLine & Line, 
													TypeLineInContour Type, 
													bool MakeTop,
													bool f) const
{
	
	//f=true - рисуем циклично, false - нет
	if(kl <= 0)
		return NULL;
	NTriMesh *pMesh = new NTriMesh;
	if(NCM_PROJECT.GetGlobalState().GetModelMode() == NGlobalState::BSP)
		pMesh->m_iSurf = MakeToolSurface(Line, Type, MatrixOfTool);// new surf	
	pMeshArr->Add(pMesh);
	if(Type == CONE && (APoint[0]->GetSize() == 1 || APoint[1]->GetSize() == 1))
	{
		CreateSurfaceCone( pMesh, Line);
		return NULL;
	}
	if(Type == HORIZONTAL && (APoint[0]->GetSize() == 1 || APoint[1]->GetSize() == 1))
	{
		CreateSurfaceHorizontal( pMesh, Line);
		return NULL;
	}

	if(Type == SPHERE)
	{
		CreateSurfaceSphere( Line, MatrixOfTool);
		return NULL;
	}

	bool flag = false;

	for(int l = 0; l < kl; l++)
	{
		int i = 0;
		int j = 0;

		int k0 = APoint[l*2]->GetSize();
		int k1 = APoint[l*2+1]->GetSize();

		int b0 = Getbeginindex(l*2);//индекс начальной точки в верхнем списке
		int b1 = Getbeginindex(l*2+1);//индекс начальной точки в верхнем списке;//индекс точки в нижнем списке, расположенной напротив начальной точки верхнего списка

		if (b0 < 0 || b1 < 0)
			return NULL;
		//TEMPORARY
		if (MakeTop && l == 0)
		{
			int Start = APoint[i]->Find(b[l * 2]);
			int End = APoint[i]->Find(e[l * 2]);
			if (Start >= 0 && End >= 0)
			{
				for (int ii = 2; ii < k0; ++ii)
				{
					pMesh->AddTri(APoint[0]->GetNumForestPoint(Start % k0),
						APoint[0]->GetNumForestPoint((Start + ii - 1) % k0),
						APoint[0]->GetNumForestPoint((Start + ii) % k0));
					if ((ii + Start) % k0 == End)
						break;
				}
			}
		}

		double ai = APoint[l*2]->GetAt(b0);			//угол начала списка
		double aj = APoint[l*2]->GetAt((b0+k0-1)%k0);	//угол конца списка

		//если точка из нижнего списка не лежит в диапазоне углов верхнего списка
		if( BAnglesArray::IsBetween(APoint[l*2+1]->GetAt((b1+1)%k1), ai-MINAR, aj+MINAR) == false)//??? непонятно зачем нужно. Списки всегда лежат в одинаковых угловых диапазонах
		{
			flag = true;
			b1 = k1-b1-1;
			return NULL;
		}

		if(Getbegin(l*2) == Getbegin(l*2+1))
		{
			ai = APoint[l*2]->GetAt((b0+1)%k0); 
			aj = APoint[l*2+1]->GetAt((b1+1)%k1, flag);
			if(BAnglesArray::IsBetween(aj, APoint[l*2]->GetAt(b0)-MINAR, ai+MINAR) == false)
				i++;
			else
				j++;
		}

		while(i < k0 || j < k1)
		{
 			if( (k0+b0-1)%k0 == (b0+i)%k0 && (k1+b1-1)%k1 == (b1+j)%k1)//если стоим на конечных точках
				break;
			if((k0+b0-1)%k0 == (b0+i)%k0)//если закончили i-й список
			{
				pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
							APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
							APoint[l*2+1]->GetNumForestPoint( (b1+j+1)%k1));
				j++;
			}
			else
			{
				if((k1+b1-1)%k1 == (b1+j)%k1)//если закончили j-й список
				{
					pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
								APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
								APoint[l*2]->GetNumForestPoint( (b0+i+1)%k0));
					i++;
				}
				else
				{
					ai = APoint[l*2]->GetAt((b0+i+1)%k0); 
					aj = APoint[l*2+1]->GetAt((b1+j+1)%k1, flag);
					if(fabs(ai-aj) < MINAR)//если следующие точки на одинаковом удалении по углу
					{
						ai = APoint[l*2]->GetAt((b0+i+2)%k0); //проверим следующие после них
						aj = APoint[l*2+1]->GetAt((b1+j+2)%k1, flag);
						if(BAnglesArray::Compare(ai, aj) > 0)//выберем ближнюю, чтоб в будущем не перепрыгнуть
							ai = APoint[l*2]->GetAt((b0+i+1)%k0);
						else
							aj = APoint[l*2+1]->GetAt((b1+j+1)%k1, flag);
					}
					if(BAnglesArray::IsBetween(aj, APoint[l*2]->GetAt((b0+i)%k0)-MINAR, ai+MINAR) == false)//!!!
					{
						pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
									APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
									APoint[l*2]->GetNumForestPoint( (b0+i+1)%k0));
						i++;
					}
					else
					{
						pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
									APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
									APoint[l*2+1]->GetNumForestPoint( (b1+j+1)%k1));
						j++;
					}
				}
			}
		}
		if(f)//если строим поверхность циклично
		{
			ai = APoint[l*2]->GetAt((b0+i+1)%k0); 
			aj = APoint[l*2+1]->GetAt((b1+j+1)%k1, flag);
			if(fabs(ai-aj) < MINAR)//если следующие точки на одинаковом удалении по углу
			{
				ai = APoint[l*2]->GetAt((b0+i+2)%k0); //проверим следующие после них
				aj = APoint[l*2+1]->GetAt((b1+j+2)%k1, flag);
				if(BAnglesArray::Compare(ai, aj) > 0)//выберем ближнюю, чтоб в будущем не перепрыгнуть
					ai = APoint[l*2]->GetAt((b0+i+1)%k0);
				else
					aj = APoint[l*2+1]->GetAt((b1+j+1)%k1, flag);
			}
			if(BAnglesArray::IsBetween(aj, APoint[l*2]->GetAt((b0+i)%k0)-MINAR, ai+MINAR) == false)
			{
				pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
							APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
							APoint[l*2]->GetNumForestPoint( (b0+i+1)%k0));

				pMesh->AddTri(APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
							APoint[l*2+1]->GetNumForestPoint( (b1+j+1)%k1),
							APoint[l*2]->GetNumForestPoint( (b0+i+1)%k0));
			}
			else
			{
				pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
							APoint[l*2+1]->GetNumForestPoint( (b1+j)%k1),
							APoint[l*2+1]->GetNumForestPoint( (b1+j+1)%k1));

				pMesh->AddTri(APoint[l*2]->GetNumForestPoint( (b0+i)%k0),
							APoint[l*2+1]->GetNumForestPoint( (b1+j+1)%k1),
							APoint[l*2]->GetNumForestPoint( (b0+i+1)%k0));
			}
		}
	}
	return NULL;
}
