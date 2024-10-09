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
   	FILE *DXF_Standart, *My_DXF;  //������� ������ ��� ������ header.dll � ���������� �����
	CString filename;             //��������� ���������� ������� ����� ������� ���(�������� �������������) ��������� DXF �����
	CString FilePath = NCM_PROJECT.GetSettingsPath();//��������� ���������� ������� ����� ��������� ���� ���������� ����� header.dll
  	CString filter,ext="dxf"; // ��������� ���������� ������� ����� �������������� ��� ������� ����� ��������� ����� DXF
	char stroka[10];  // ��������� ����������, ������� ����� ������������ ��� ������������� ����������� ������ DXF ����� �� ����� header.dll � �������� ����
	INT_PTR Kol_Cadr = InArray->GetSize(); // �������� ����������, ������� ����� ������� ���������� � ���������� ������,� �������� �� ��������
	int Number_Line=500;  //��������� ������ ��� DXF� ���������� � 500 �� ��������� ���������� �� ���������� ������� DXF �������
	BMatr Matrix;         // ��������� ����������, ������� ����� �������� ���������� ������ BMatr � ����� ������������� ��� �������� ���, ����� ��� �������� � ��������� XOY � ��� ����� ���� ��������� ��������� � �������� ���� ����
	BPoint I,N,P0,P1,C;   // ��������� ����������, ������� ����� �������� ������������ ������ PBoint � ���� ������� ���������� � ��������� ����� ����(P0), � �������� ����� ����(P1),� �������(N) � ���������, � ������� ����� ����, � ������ ����(�), � �������(I), ������� ��������� �� ��������� ����� ���� � �� ������
	double x,y,z;  // ��������� ���������� ��� �������������� �������������
	double Radius; // ��������� ����������, ������� ����� ������� ���������� � ������� ����
	double Corner; // ��������� ���������� ��� �������������� �������������
	double CornerB;//��������� ����������, ������� ����� ��������� ���������� � ��������� ���� ���� 
	double CornerE;//��������� ����������, ������� ����� ��������� ���������� � �������� ���� ����
	filter = "*." + ext +"|*." + ext + "|"; // �������� ������� ��� ����������� �� ���������� ��������� DXF �����
	CFileDialog fd(FALSE, ext, DefaultName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter); //����������� ���� ������� ������ ���������� �����
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return;
	if( (DXF_Standart  = fopen( FilePath + "\\header.dll", "r"  )) == NULL )
	{
		AfxMessageBox(IDS_FIND_ERR);
		return;
	}
	My_DXF = fopen( filename, "w" ); //��������� �������� ����
	for(int i = 0; i < 1536; i++)    //������ ������� ����� ����� header.dll � ������ ������������ � ��� ��������� ����
	{
	   fread(stroka,sizeof(char),5,DXF_Standart);
	   fwrite(stroka,sizeof(char),5,My_DXF);
	}
	for (int i = 0; i < Kol_Cadr; i++)
	{
		NCadrGeom Cur = *InArray->GetAt(i);
		if(Cur.IsLine()) //������ ��� � �������� ���
		{
			fprintf(My_DXF,"  0\nLINE\n  5\n");
			fprintf(My_DXF,"%d\n",Number_Line);  //����� �����
			fprintf(My_DXF,"100\nAcDbEntity\n  8\n0\n  6\n");
			if(Cur.IsFast())
				fprintf(My_DXF,"DASHED2\n 62\n     1\n100\nAcDbLine\n"); //fast ���
			else
				fprintf(My_DXF,"ByLayer\n 62\n     0\n100\nAcDbLine\n"); //�����
			Cur.GetB(&x,&y,&z);
			P0.Set(x,y,z,1);
			fprintf(My_DXF," 10\n%.16g\n 20\n%.16g\n 30\n%.16g\n",P0.GetX(),P0.GetY(),P0.GetZ());
			Cur.GetE(&x,&y,&z);
			P1.Set(x,y,z,1);
			fprintf(My_DXF," 11\n%.16g\n 21\n%.16g\n 31\n%.16g\n",P1.GetX(),P1.GetY(),P1.GetZ());
			Number_Line++;
		}

		if(Cur.IsArc()) //��������
		{
		   int Flag=0;  // ��������� � �������� ����, ������� ����� ��������� ����������� ����, �.�. �������� �� ���� �����������
		   Cur.GetB(&x,&y,&z);	   P0.Set(x,y,z,1);
		   Cur.GetE(&x,&y,&z);	   P1.Set(x,y,z,1);
		   Cur.GetN(&x,&y,&z);	   N.Set(x,y,z,0);
		   Cur.GetI(&x,&y,&z);	   I.Set(x,y,z,0);
		   if(I.D2() == 0.) // �������� �� ��, ����� ���� �� ����� �������� �������, ���� ��� ��� �� ��������� ���������� ���� ����
			   continue;
		   Radius=sqrt(I.D2());
		   C=P0+I;// ����� ����
		   if(P0.Dist2(P0,P1)<1.e-12)//��������� �������� �� ���� ���������� � ���� ��� ��� �� ������ ��� ���� � "���������" ���� 
		   {
			   fprintf(My_DXF,"  0\nCIRCLE\n  5\n");
			   Flag=1;
		   }
		   else 
			   fprintf(My_DXF,"  0\nARC\n  5\n");//���� ���� �� �������� �� ���������� ���������� � �������� ���� � ������������ �� ����������� DXF�
		   fprintf(My_DXF,"%d\n",Number_Line);  //����� ��������
		   fprintf(My_DXF,"100\nAcDbEntity\n  8\n0\n");
		   fprintf(My_DXF,"100\nAcDbCircle\n");
			if(Cur.GetPlane()==XZ) //�������� �������������� ���� ��������� XOZ
			{
				Matrix.RotX(BPoint(0.,0.,0.,1.),-90.); //�������� ������� �������� ������������ ��� X �� ���� 270 ��������
				Matrix = Matrix * BMatr().RotY(BPoint(0.,0.,0.,1.), -180.);
				P0=P0*Matrix;	//������� ����� P0 ������������ ��� X �� 270 ��������
				P1=P1*Matrix;	//������� ����� P1 ������������ ��� X �� 270 ��������	
				I=I*Matrix;		//������� ������� I ������������ ��� X �� 270 ��������
   				C=C*Matrix;		//������� ����� C ������������ ��� X �� 270 ��������
			}
			if(Cur.GetPlane()==YZ) //�������� �������������� ���� ��������� YOZ
			{
				Matrix.RotY(BPoint(0.,0.,0.,1.),-90);  //�������� ������� �������� ������������ ��� Y �� ���� 90 ��������	
				Matrix = Matrix * BMatr().RotZ(BPoint(0.,0.,0.,1.), -90.);
				P0=P0*Matrix;	//������� ����� P0 ������������ ��� Y �� 90 ��������
				P1=P1*Matrix;	//������� ����� P1 ������������ ��� Y �� 90 ��������
				I=I*Matrix;		//������� ������� I ������������ ��� Y �� 90 ��������
   				C=C*Matrix;		//������� ����� C ������������ ��� Y �� 90 ��������
			}
		   fprintf(My_DXF," 10\n%.16g\n 20\n%.16g\n 30\n%.16g\n 40\n%.16g\n",C.GetX(),C.GetY(),C.GetZ(),Radius);
		   fprintf(My_DXF,"210\n%.16g\n220\n%.16g\n230\n%.16g\n",N.GetX(),N.GetY(),N.GetZ());
		   if(Flag!=1) //����������� ����������� ����
		   {
			   fprintf(My_DXF,"100\nAcDbArc\n"); //������ � ���� ����� ������ � �����
			   Corner=57.2957795130823208767981548141052*acos(-I.GetX()/sqrt(I.D2())); //������ ���� ����� ���� OX � ������ P0
			   if(I.GetY()>=0) Corner=360-Corner; //��������� ���������� ���� 
			   CornerE=Corner;
			   I=C-P1;  //�������������� ������ I ��� ���������� ���� ����� ���� OX � ������ P1
			   Corner=57.2957795130823208767981548141052*acos(-I.GetX()/sqrt(I.D2())); //������ ���� ����� ���� OX � ������ P1
			   if(I.GetY()>=0) Corner=360-Corner;//��������� ���������� ���� 
		
			   if(Cur.IsCWArc())	CornerB=Corner; //���� ���� �� ������� �������
			   if(Cur.IsCCWArc()) //���� ���� ������ ������� �������
			   {
				   CornerB=CornerE;
				   CornerE=Corner;
			   }    		      

			   fprintf(My_DXF," 50\n%.16g\n 51\n%.16g\n",CornerB,CornerE); //��������� ���������� ���� � �������� ����
		   }
		   Number_Line++;//����� ������� ����������� �� 1
		}
	}
	while (!feof(DXF_Standart))//������ ������ ����� ����� DXF�
	{
		fread(stroka,sizeof(char),1,DXF_Standart);
		fwrite(stroka,sizeof(char),1,My_DXF);
	}
	fclose( DXF_Standart );
	fclose( My_DXF );
}
