#include "stdafx.h"
#include "BPoint.h"
#include "MFace.h"

void SavePoints(BPoint *pts, int indS, int num, bool newf)
{
	CStdioFile f;
	CString str;
	int k;
	BOOL res;
	CString path, fname;

	path = _T("D:\\VCPROJECTS\\NCM25_t\\Report");
	fname = _T("Point.txt");

	if(newf)
		res = f.Open(path+"\\"+fname,CFile::modeWrite | CFile::modeCreate);
	else
		res = f.Open(path+"\\"+fname,CFile::modeWrite);
	if (!res)
	{
		AfxMessageBox("Невозможно открыть файл для записи координат!");
		return;
	}
	//поставить позицию курсора в конец файла
	f.SeekToEnd();
	f.WriteString("---------------------");
	str.Format("%s", "Точки - ");
	f.WriteString(str);
	COleDateTime t = COleDateTime::GetCurrentTime();
	str = t.Format("%d %B %Y : %H %M %S");
	f.WriteString(str+"\n");

	for (k=0; k<num; k++)
	{
		str.Format("Точка %d: %f %f %f %f",
			indS+k,
			pts[k].GetX(),
			pts[k].GetY(),
			pts[k].GetZ(),
			pts[k].GetH());
		f.WriteString(str+"\n");
	}
	f.Close();
	return;
}

void SaveFace(MFace *face, int num, bool newf)
{
	CStdioFile f;
	CString str;
	int k;
	BOOL res;
	CString path, fname;

	path = _T("D:\\VCPROJECTS\\NCM25_t\\Report");
	fname = _T("Face.txt");

	if(newf)
		res = f.Open(path+"\\"+fname,CFile::modeWrite | CFile::modeCreate);
	else
		res = f.Open(path+"\\"+fname,CFile::modeWrite);
	if (!res)	
	{
		AfxMessageBox("Невозможно открыть файл для записи координат!");
		return;
	}
	//поставить позицию курсора в конец файла	
	f.SeekToEnd();
	f.WriteString("---------------------");
	str.Format("%s: %d - ", "Грань", num);
	f.WriteString(str);
	COleDateTime t = COleDateTime::GetCurrentTime();
	str = t.Format("%d %B %Y : %H %M %S");
	f.WriteString(str+"\n");
	str.Format("Нормаль: %f %f %f %f", 
		face->GetPlane().GetNormal().GetX(),
		face->GetPlane().GetNormal().GetY(),
		face->GetPlane().GetNormal().GetZ(),
		face->GetPlane().GetNormal().GetH());
	f.WriteString(str+"\n");
	str.Format("Базовая точка: %f %f %f %f", 
		face->GetPlane().GetNormal().GetX(),
		face->GetPlane().GetNormal().GetY(),
		face->GetPlane().GetNormal().GetZ(),
		face->GetPlane().GetNormal().GetH());
	f.WriteString(str+"\n");
	for (k=0; k<face->GetCount(); k++)
	{
		str.Format("Точка %d: %f %f %f %f",
			face->GetIndPoint(k),
			face->GetPoint(k)->GetX(),
			face->GetPoint(k)->GetY(),
			face->GetPoint(k)->GetZ(),
			face->GetPoint(k)->GetH());
		f.WriteString(str+"\n");
	}
	f.Close();

	return;
}

void SaveIndex(IndexArray &ind, bool newf)
{
	CStdioFile f;
	CString str;
	int k,i;
	BOOL res;
	CString path, fname;

	path = _T("D:\\VCPROJECTS\\NCM25_t\\Report");
	fname = _T("Index.txt");

	if(newf)
		res = f.Open(path+"\\"+fname,CFile::modeWrite | CFile::modeCreate);
	else
		res = f.Open(path+"\\"+fname,CFile::modeWrite);
	if (!res)
	{
		AfxMessageBox("Невозможно открыть файл для записи координат!");
		return;
	}
	//поставить позицию курсора в конец файла
	f.SeekToEnd();
	f.WriteString("---------------------");
	str.Format("%s", "Номера - ");
	f.WriteString(str);
	COleDateTime t = COleDateTime::GetCurrentTime();
	str = t.Format("%d %B %Y : %H %M %S");
	f.WriteString(str+"\n");

	for (k=0; k<ind.GetSize(); k++)
	{
		if (ind[k]->is_top)
			str.Format("Линия %d - верх", ind[k]->ind_l);
		else
			str.Format("Линия %d - низ", ind[k]->ind_l);
		f.WriteString(str+"\n");
		str.Format("  Передняя часть:");
		f.WriteString(str+"\n\t");
		for (i=0; i<ind[k]->aInd[0].GetSize(); i++)
		{
			str.Format("%d ", ind[k]->aInd[0][i]);
			f.WriteString(str);
		}
		str.Format("  Средняя часть:");
		f.WriteString("\n"+str+"\n\t");
		for (i=0; i<ind[k]->aInd[1].GetSize(); i++)
		{
			str.Format("%d ", ind[k]->aInd[1][i]);
			f.WriteString(str);
		}
		str.Format("  Задняя часть:");
		f.WriteString("\n"+str+"\n\t");
		for (i=0; i<ind[k]->aInd[2].GetSize(); i++)
		{
			str.Format("%d ", ind[k]->aInd[2][i]);
			f.WriteString(str);
		}
		f.WriteString("\n");
	}
	f.Close();

	return;
}
