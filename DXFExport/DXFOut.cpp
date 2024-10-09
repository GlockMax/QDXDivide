// DXFOut.cpp: implementation of the DXFOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResLocal.h"
#include "BGeomArray.h"
#include "NCadrGeom.h"
#include "math.h"
#include "BMatr.h"
#include "BPoint.h"
#include "NCMProject.h"
#include "DXFOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
    
DXFOut::DXFOut()
{

}

DXFOut::~DXFOut()
{

}
DXFOut::DXFOut(const NCadrGeomArray *InArray, const CString& DefaultName /*= CString()*/)
{
   	FILE *DXF_Standart, *My_DXF;  //создаем потоки для файлов header.dll и исходящего файла
	CString filename;             //Описываем переменную которая будет харнить имя(задается пользователем) исходного DXF файла
	CString FilePath = NCM_PROJECT.GetSettingsPath();//Описываем переменную которая будет содержать путь нахождения файла header.dll
  	CString filter,ext="dxf"; // описываем переменные которые будут использоваться при запросе имени исходного файла DXF
	char stroka[10];  // описываем переменную, которую будем использовать для переписывания стандартных частей DXF файла из файла header.dll в исходный файл
	INT_PTR Kol_Cadr = InArray->GetSize(); // описание переменной, которая будет хранить информацию о количестве кадров,и передаем ей значение
	int Number_Line=500;  //Нумерация кадров для DXFа начинается с 500 во избежание совпадения со служебными метками DXF формата
	BMatr Matrix;         // Описывает переменную, которая будет являться наследнико класса BMatr и будет исользоваться для поворота дуг, чтобы они попадали в плоскость XOY и там можно было вычислить начальный и конечный угол дуги
	BPoint I,N,P0,P1,C;   // Описываем переменные, которые будут являться наследниками класса PBoint и буду хранить информацию о начальной точке дуги(P0), о конечной точке дуги(P1),о нормали(N) к плоскости, в которой лежит дуга, о центре дуги(С), о векторе(I), который направлен из начальной точки дуги к ее центру
	double x,y,z;  // описываем переменные для промежуточного использования
	double Radius; // описываем переменную, которая будет хранить информацию о радиусе дуги
	double Corner; // описываем переменную для промежуточного использования
	double CornerB;//описываем переменные, которая будет содержать информацию о начальном углу дуги 
	double CornerE;//описываем переменные, которая будет содержать информацию о конечном углу дуги
	filter = "*." + ext +"|*." + ext + "|"; // создание фильтра для ограничения на расширение сходящего DXF файла
	CFileDialog fd(FALSE, ext, DefaultName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter); //диалгоговое окно запроса имеени исходящего файла
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;
	if( (DXF_Standart  = fopen( FilePath + "\\header.dll", "r"  )) == NULL )
	{
		AfxMessageBox(IDS_FIND_ERR);
		return;
	}
	My_DXF = fopen( filename, "w" ); //открываем выходной файл
	for(int i = 0; i < 1536; i++)    //Чтение верхней части файла header.dll и запись прочитанного в наш исходящий файл
	{
	   fread(stroka,sizeof(char),5,DXF_Standart);
	   fwrite(stroka,sizeof(char),5,My_DXF);
	}
	for (int i = 0; i < Kol_Cadr; i++)
	{
		NCadrGeom Cur = *InArray->GetAt(i);
		if(Cur.IsLine()) //Прямой ход и холостой ход
		{
			fprintf(My_DXF,"  0\nLINE\n  5\n");
			fprintf(My_DXF,"%d\n",Number_Line);  //Номер линии
			fprintf(My_DXF,"100\nAcDbEntity\n  8\n0\n  6\n");
			if(Cur.IsFast())
				fprintf(My_DXF,"DASHED2\n 62\n     1\n100\nAcDbLine\n"); //fast тип
			else
				fprintf(My_DXF,"ByLayer\n 62\n     0\n100\nAcDbLine\n"); //линия
			Cur.GetB(&x,&y,&z);
			P0.Set(x,y,z,1);
			fprintf(My_DXF," 10\n%.16g\n 20\n%.16g\n 30\n%.16g\n",P0.GetX(),P0.GetY(),P0.GetZ());
			Cur.GetE(&x,&y,&z);
			P1.Set(x,y,z,1);
			fprintf(My_DXF," 11\n%.16g\n 21\n%.16g\n 31\n%.16g\n",P1.GetX(),P1.GetY(),P1.GetZ());
			Number_Line++;
		}

		if(Cur.IsArc()) //Повороты
		{
		   int Flag=0;  // описываем и обнуляем флаг, который будет учытывать замкнутость дуги, т.е. является ли дуга окружностью
		   Cur.GetB(&x,&y,&z);	   P0.Set(x,y,z,1);
		   Cur.GetE(&x,&y,&z);	   P1.Set(x,y,z,1);
		   Cur.GetN(&x,&y,&z);	   N.Set(x,y,z,0);
		   Cur.GetI(&x,&y,&z);	   I.Set(x,y,z,0);
		   if(I.D2() == 0.) // проверка на то, чтобы дуга не имела нулевого радиуса, если это так то программа пропускает этот кадр
			   continue;
		   Radius=sqrt(I.D2());
		   C=P0+I;// центр дуги
		   if(P0.Dist2(P0,P1)<1.e-12)//проверяем является ли дуга окружность и если это так то меняем имя темы и "поднимаем" флаг 
		   {
			   fprintf(My_DXF,"  0\nCIRCLE\n  5\n");
			   Flag=1;
		   }
		   else 
			   fprintf(My_DXF,"  0\nARC\n  5\n");//если дуга не замкнута то записываем информацию в выходной файл в соответствии со стандартами DXFа
		   fprintf(My_DXF,"%d\n",Number_Line);  //Номер поворота
		   fprintf(My_DXF,"100\nAcDbEntity\n  8\n0\n");
		   fprintf(My_DXF,"100\nAcDbCircle\n");
			if(Cur.GetPlane()==XZ) //проверка принадлежности дуги плоскости XOZ
			{
				Matrix.RotX(BPoint(0.,0.,0.,1.),-90.); //создание матрицы поворота относительно оси X на угол 270 градусов
				Matrix = Matrix * BMatr().RotY(BPoint(0.,0.,0.,1.), -180.);
				P0=P0*Matrix;	//поворот точки P0 относительно оси X на 270 градусов
				P1=P1*Matrix;	//поворот точки P1 относительно оси X на 270 градусов	
				I=I*Matrix;		//поворот вектора I относительно оси X на 270 градусов
   				C=C*Matrix;		//поворот точки C относительно оси X на 270 градусов
			}
			if(Cur.GetPlane()==YZ) //проверка принадлежности дуги плоскости YOZ
			{
				Matrix.RotY(BPoint(0.,0.,0.,1.),-90);  //создание матрицы поворота относительно оси Y на угол 90 градусов	
				Matrix = Matrix * BMatr().RotZ(BPoint(0.,0.,0.,1.), -90.);
				P0=P0*Matrix;	//поворот точки P0 относительно оси Y на 90 градусов
				P1=P1*Matrix;	//поворот точки P1 относительно оси Y на 90 градусов
				I=I*Matrix;		//поворот вектора I относительно оси Y на 90 градусов
   				C=C*Matrix;		//поворот точки C относительно оси Y на 90 градусов
			}
		   fprintf(My_DXF," 10\n%.16g\n 20\n%.16g\n 30\n%.16g\n 40\n%.16g\n",C.GetX(),C.GetY(),C.GetZ(),Radius);
		   fprintf(My_DXF,"210\n%.16g\n220\n%.16g\n230\n%.16g\n",N.GetX(),N.GetY(),N.GetZ());
		   if(Flag!=1) //проверяется замкнутости дуги
		   {
			   fprintf(My_DXF,"100\nAcDbArc\n"); //запись в файл метки класса и имени
			   Corner=57.2957795130823208767981548141052*acos(-I.GetX()/sqrt(I.D2())); //находи угол между осью OX и точной P0
			   if(I.GetY()>=0) Corner=360-Corner; //проверяем полученный угол 
			   CornerE=Corner;
			   I=C-P1;  //переопределяем вектор I для нахождения угла тежду осью OX и точкой P1
			   Corner=57.2957795130823208767981548141052*acos(-I.GetX()/sqrt(I.D2())); //находи угол между осью OX и точной P1
			   if(I.GetY()>=0) Corner=360-Corner;//проверяем полученный угол 
		
			   if(Cur.IsCWArc())	CornerB=Corner; //если дуга по часовой стрелки
			   if(Cur.IsCCWArc()) //если дуга против часовой стрелки
			   {
				   CornerB=CornerE;
				   CornerE=Corner;
			   }    		      

			   fprintf(My_DXF," 50\n%.16g\n 51\n%.16g\n",CornerB,CornerE); //сохраняем полученные углы в исходный файл
		   }
		   Number_Line++;//номер объекта увеличиваем на 1
		}
	}
	while (!feof(DXF_Standart))//Чтение нижней части шапки DXFа
	{
		fread(stroka,sizeof(char),1,DXF_Standart);
		fwrite(stroka,sizeof(char),1,My_DXF);
	}
	fclose( DXF_Standart );
	fclose( My_DXF );
}
