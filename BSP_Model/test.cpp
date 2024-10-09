#include "stdafx.h"
#include "test.h"
#include "BMatr.h"
#include "MFACE.h"
#include "MPlane.h"
#include "MRay.h"
#include "math.h"
#include "MBSPNode.h"
#include "MBSPTree.h"
#include "MQuadTree.h"
#include "MDivideTree.h"
#include "TestDlg.h"
#include "MBSPUnion.h"
#include "MBSPInter.h"
#include "GQuadRender.h"
#include "GRayShooting.h"
#include "Gl\Glu.h"
#include "TimeSet.h"

test::test(void)
{
}

test::~test(void)
{
}

void test::test04(void)
{
			TimeSet TmS;
			TmS.Start();
	MBSPTree *pTree1;// = MBSPForest::GetActive().CrBox(400,400,20,-200,-200,-20);
//	MBSPForest::GetActive().AddTree(pTree1);
	pTree1->CrStandard();
	MBSPInter Op;
	for(int i=0; i<0; ++i)
	{
		MBSPTree *pTree2;// = MBSPForest::GetActive().CrTorus(100-5*i,10,8,32);
		pTree2->CrStandard();
		pTree2->GetRoot()->Invert();
		Op.Merge(*pTree1->GetRoot(),*pTree2->GetRoot());
		pTree2->SetRoot((MBSPNode *)M_OUT_PTR);
	}
					TmS.Stop(true, true, tmVIS);
	pTree1->DrawFacets();
}
void test::test03(void)
{
	MBSPForest::GetActive().pQR->FillAllArrays(*pTree1);
	return;
	
	
	// Тест на величину погрешности. Работает при -7 не работает при -6
	MBSPTree *pT2;
//		pT2 = MBSPForest::GetActive().CrBox(300,100,100,10,0,0);
		int N = 2;//39;
		BMatr T;
		T.RotZ(BPoint(0.,0.,0.,1.),360./(30));
		BMatr Tr;
		Tr.Trans(0,0,10);
		T = T*Tr;
		MBSPUnion Op;
		for(int k = 1; k<N; k++)
		{
			MBSPForest::GetActive().Transform(T);
			MBSPTree *pT1;// = MBSPForest::GetActive().CrBox(300,100,100,10,0,0);
			if(k>1)
				continue;
			Op.Merge(*pT2->GetRoot(),*pT1->GetRoot());
			pT1->SetRoot((MBSPNode *)M_NDEF_PTR);
		}
		MBSPTree *pT1;// = MBSPForest::GetActive().CrBox(500,500,500,0,-100,0);
		bool f= pT2->GetRoot()->Contains(BPoint(-700,300,400));
		pT2->Merge(MBSPUnion(),*pT1);
		f= pT2->GetRoot()->Contains(BPoint(-700,300,400));
	Beep(500,100);
	glColor4d(0.,1.,0.,0.5);
	pT2->DrawTransp();
	SwapBuffers(wglGetCurrentDC());
}
void test::test01(void)
{ 
	if(!F.Open("C:\\Prog.txt",CFile::typeText | CFile::modeRead))
 		return;
			TimeSet TmS;
			TmS.Start();
	CString R;
	pTree1 = new MQuadTree(
		BPoint(-25,-65,-250,1),BPoint(185,185,0,1),
		128,128);
	MBSPForest::GetActive().AddTree(pTree1);
	nst=64;
	F.ReadString(R);
	sscanf(LPCTSTR(R),"%f,%f,%f",&xb,&yb,&zb);
	F.ReadString(R);
	float xi,yi,zi;
	sscanf(LPCTSTR(R),"%f,%f,%f,%f,%f,%f",&xe,&ye,&ze,&xi,&yi,&zi);
//	pTree1->CrPlaneArc(BPoint(xb,yb,zb,1),BPoint(xe,ye,ze,1),BPoint(xi,yi,zi,0),false,58,64,nst);
	pTree1->CrPlanePoint(BPoint(xb,yb,zb,1),58,nst);
//	pTree1->Mask(38,23);
//	pTree1->Mask(41,20);
//	pTree1->CreateTree();
	pTree1->CrStandard();
	for(int i=1 ; i < 0; i+=1)
	{
		xb = xe; yb = ye; zb = ze;
		MQuadTree *pTree2 = pTree1->CreateSame();
		if(!F.ReadString(R))
			break;;
		sscanf(LPCTSTR(R),"%f,%f,%f",&xe,&ye,&ze);
//		pTree2->CrCyl(BPoint(xb,yb,zb,1),BPoint(xe,ye,ze,1),12.5,nst);
//		pTree2->CrCyl(BPoint(100,step*(i-1),-10,1),BPoint(100,step*i,-10,1),10,nst);
//		pTree2->CreateTree();
		pTree2->CrStandard();
		pTree2->Merge(MBSPUnion(),*pTree1);
		pTree1->Reset();
		delete pTree1;
		pTree1=pTree2;
		if(i%8 == 7)
		{
			MBSPForest::GetActive().m_TreeArray.SetAt(0,pTree1);
			MBSPForest::GetActive().CompressFaces();
			MBSPForest::GetActive().CompressPoints();
		}

	}
//	F.Close();
			MBSPForest::GetActive().m_TreeArray.SetAt(0,pTree1);
//			MBSPForest::GetActive().CompressFaces();
//			MBSPForest::GetActive().CompressPoints();
	TmS.Stop(true, true, tmVIS);
			TmS.Start();
//	pTree1->CrDispList();
			GQuadRender *pQR = new GQuadRender(*pTree1);
			pQR->FillAllArrays(*pTree1);
			MBSPForest::GetActive().pQR = pQR;
				TmS.Stop(true, true, tmVIS);
//				delete pTree1;
//			TmS.Start();
//			GRayShooting Rsh;
//			Rsh.FillBuf(*pTree1);
//			TmS.Stop(true, true, tmVIS);
	return;
}
void test::test02(void)
{
		xb = xe; yb = ye; zb = ze;
		MQuadTree *pTree2 = pTree1->CreateSame();
		CString R;
		if(!F.ReadString(R))
			return;
	float xi,yi,zi;
	sscanf(LPCTSTR(R),"%f,%f,%f,%f,%f,%f",&xe,&ye,&ze,&xi,&yi,&zi);
//		while(ze<zb)
//		{
	//	xb = xe; yb = ye; zb = ze;
	//	if(!F.ReadString(R))
	//		return;
	//	sscanf(LPCTSTR(R),"%f,%f,%f",&xe,&ye,&ze);
	//	}
//		pTree2->CrPlane(BPoint(xb,yb,zb,1),BPoint(xe,ye,ze,1),8,nst);
	pTree2->CrPlaneArc(BPoint(xb,yb,zb,1),BPoint(xe,ye,ze,1),BPoint(xi,yi,zi,0),true,58,64,nst);
//	pTree2->Mask(38,23);
//	pTree1->Mask(41,20);
		pTree2->CrStandard();
		pTree1->SafeMerge(MBSPUnion(),*pTree2);
		pTree2->Reset();
		delete pTree2;
//		pTree1=pTree2;
		MBSPForest::GetActive().m_TreeArray.SetAt(0,pTree1);
		if(1)
		{
			MBSPForest::GetActive().CompressFaces();
			MBSPForest::GetActive().CompressPoints();
		}
		MBSPForest::GetActive().pQR->FillModifArrays(*pTree1);
//		pTree1->DrawFaces();
}

void test::test07(void)
{
}
void test::test06(void)
{
//	MBSPForest::GetActive().CrTorus(100,70,128,64);
			TimeSet TmS;
			TmS.Start();
	MQuadTree *pTree0 = MBSPForest::GetActive().CrTorusQ(180,20,32,32,2);
	pTree0->CrStandard();
				TmS.Stop(true, true, tmVIS);
//				int d = pTree0->Depth(pTree0->GetRoot());
//	CString t;
//	t.Format("%d",d);
//	AfxMessageBox(t);
//	pTree0->CrStandard();
	pTree0->DrawFacets();
				MBSPForest::GetActive().AddTree(pTree0);
}
void test::test05(void)
{
			TimeSet TmS;
			TmS.Start();
//	MBSPTree *pTree1 = MBSPForest::GetActive().CrBox(400,400,20,-200,-200,-20);
	MQuadTree *pTree1 = MBSPForest::GetActive().CrTorusQ(185,10,16,16, 2);

	pTree1->CreateTree();
	pTree1->CrStandard();
	MBSPForest::GetActive().AddTree(pTree1);

	int l;
	MBSPUnion Op;
	for(int i=1; i<32; ++i)
	{
		MQuadTree *pTree2 = MBSPForest::GetActive().CrTorusQ(185-4*i,10,16,16, 2/*i<31 ?-0.98:2*/);
		pTree2->CreateTree();
		pTree2->CrStandard();
//		MBSPForest::GetActive().AddTree((MBSPTree *)pTree2);
//			l = pTree2->Depth();
//			CString t;
//			t.Format("%d",l);
//			AfxMessageBox(t);

		pTree2->Merge(Op,*pTree1);
		pTree1=pTree2;
		if(i%8 == 7)
		{
			MBSPForest::GetActive().m_TreeArray.SetAt(0,pTree1);
			MBSPForest::GetActive().CompressFaces();
			MBSPForest::GetActive().CompressPoints();
		}
	
	}
					TmS.Stop(true, true, tmVIS);
//	MBSPForest::GetActive().m_TreeArray.SetAt(0,(MBSPTree *)pTree1);
	l = MBSPForest::GetActive().m_PointArray.GetSize();
	int n = MBSPForest::GetActive().m_FaceArray.GetSize();
			CString t;
			t.Format("F%d,P%d",n,l);
			AfxMessageBox(t);

//		n = MBSPForest::GetActive().CompressFaces();
//			CString t;
//			t.Format("%d",l);
//			AfxMessageBox(t);
//TmS.Start();
//		l = MBSPForest::GetActive().CompressPoints();
//			t.Format("F%d,P%d",n,l);
//			AfxMessageBox(t);
//			CString t;
//			t.Format("%d",l);
//			AfxMessageBox(t);
//					TmS.Stop(true, true, tmVIS);



			TmS.Start();
//	pTree1 = MBSPForest::GetActive().CrTorusQ(185,19,64,32);
//	pTree1->CrStandard();
	for(int i=0; i<0; ++i)
	{
		MQuadTree *pTree2 = MBSPForest::GetActive().CrTorusQ(180-2*i,10,64,32,-0.98, false);
//		MBSPForest::GetActive().AddTree((MBSPTree *)pTree2);
//		l = pTree2->ActiveCellsNum();
		pTree2->SortFaces(*pTree1);
//		n = pTree2->ActiveCellsNum();
//		t.Format("Before:%d,After:%d",l,n);
//			AfxMessageBox(t);
		pTree2->CreateTree();
		pTree2->CrStandard();
//			CString t;
//			l = pTree2->Depth();
//			CString t;
//			t.Format("%d",l);
//			AfxMessageBox(t);

		pTree2->Merge(Op,*pTree1);
		pTree1=pTree2;
		if(i%8 != 9)
		{
			MBSPForest::GetActive().m_TreeArray.SetAt(0,pTree1);
			MBSPForest::GetActive().CompressFaces();
			MBSPForest::GetActive().CompressPoints();
		}
	
	}
					TmS.Stop(true, true, tmVIS);


	l = MBSPForest::GetActive().m_PointArray.GetSize();
	n = MBSPForest::GetActive().m_FaceArray.GetSize();
//			CString t;
			t.Format("F%d,P%d",n,l);
			AfxMessageBox(t);
/*
		n = MBSPForest::GetActive().CompressFaces();
		l = MBSPForest::GetActive().CompressPoints();
			t.Format("F%d,P%d",n,l);
			AfxMessageBox(t);
			*/

	pTree1->CrDispList();


}
