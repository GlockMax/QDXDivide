#include "stdafx.h"
#include <atomic>
#include "ConstDef.h"
#include "MQuadTreeRot.h"
#include "BMatr.h"
#include "NCadrGeom.h"
#include "GQuadRenderRot.h"
#include "NTurnStockSkeleton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

NTurnStockSkeleton::NTurnStockSkeleton()
{
	Sorted = false;
}


NTurnStockSkeleton::~NTurnStockSkeleton()
{
}

bool NTurnStockSkeleton::Create(MQuadTreeRot &QuadTreeRot)
{
	SectGeom1.RemoveAll();
	SectGeom2.RemoveAll();
	Arcs.RemoveAll();

	QuadTreeRot.GetContour(SectGeom1);

	double Tol = MIND;
	double *pTol = &Tol;
	NCM_PROJECT.DoubleParams.Lookup("ContTolerance", pTol);
	Sorted = (SectGeom1.Sort(*pTol) >= 0);

	// Make arcs
	for (int i = 1; i < SectGeom1.GetSize(); ++i)
	{
		if ((SectGeom1[i - 1]->GetE() - SectGeom1[i]->GetB()).D2() > *pTol * *pTol)
			continue;
		double Angle = fabs(SectGeom1[i]->Angle(*SectGeom1[i - 1]));
		if (Angle > GQuadRender::GetSmoothAngle() && Angle < 178.)
		{
			NCadrGeom *pArc = new NCadrGeom;
			BPoint Pb(SectGeom1[i]->GetB());
			pArc->Set(ccwarc, Pb.GetX(), Pb.GetY(), Pb.GetZ(), 0., 0., 0., 0., 0., 0., XY);
			Arcs.Add(pArc);
		}
	}
	RecalcArcs();

	double StartAngle, Angle, AngleStep;
	int AngleStepsNum;
	GQuadRenderRot::GetVisParam(StartAngle, Angle, AngleStep, AngleStepsNum);
	BMatr Matr(BMatr().RotX(0., 0., 0., 90.) * BMatr().RotZ(0., 0., 0., StartAngle));
	BMatr Matr2(BMatr().RotZ(0., 0., 0., Angle));
	SectGeom2.SetSize(SectGeom1.GetSize());
	for (int i = 0; i < SectGeom1.GetSize(); ++i)
	{
		SectGeom1[i]->Tr(Matr);
		SectGeom2[i] = (new NCadrGeom(*SectGeom1[i]));
		SectGeom2[i]->Tr(Matr2);
	}
	MakeUuid();

	return true;
}

void NTurnStockSkeleton::Draw()
{
	double StartAngle, Angle, AngleStep;
	int AngleStepsNum;
	GQuadRenderRot::GetVisParam(StartAngle, Angle, AngleStep, AngleStepsNum);
	glPushName(SkelArcs);
	Arcs.Draw(-1, -1, -1);
	glPopName();
	glPushName(SkelContour1);// Stock section
	SectGeom1.Draw(-1, -1, -1);
	glPopName();
	glPushName(SkelContour2);
	SectGeom2.Draw(-1, -1, -1);
	glPopName();
}

void NTurnStockSkeleton::RecalcArcs(void)
{
	// recalc Pb, I and Pe according to angles
	double StartAngle, Angle, AngleStep;
	int AngleStepsNum;
	GQuadRenderRot::GetVisParam(StartAngle, Angle, AngleStep, AngleStepsNum);
	BMatr Matr(BMatr().RotX(0., 0., 0., 90.) * BMatr().RotZ(0., 0., 0., StartAngle));
	BMatr Matr2(BMatr().RotZ(0., 0., 0., Angle));
	for (int i = 0; i < Arcs.GetSize(); ++i)
	{
		NCadrGeom *pGeom = Arcs[i];
		BPoint Pb(pGeom->GetB());
		Pb = Pb * Matr;
		BPoint Pi(BPoint(0, 0, Pb.GetZ(), 1) - Pb);
		BPoint Pe(Pb * Matr2);
		pGeom->Set(ccwarc, Pb.GetX(), Pb.GetY(), Pb.GetZ(), Pe.GetX(), Pe.GetY(), Pe.GetZ(), Pi.GetX(), Pi.GetY(), Pi.GetZ(), XY);
	}
}

bool NTurnStockSkeleton::MakeUuid()
{
//	bool result = false;
//
//	HCRYPTPROV hCryptProv;
//	HCRYPTHASH hHash;
//
//	if (!::CryptAcquireContext(&hCryptProv, 0, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) goto exit1;
//
//	if (!::CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash)) goto exit2;
//
//	for (int i = 0; i < SectGeom1.GetSize(); ++i)
//		if (!::CryptHashData(hHash, (BYTE *)(SectGeom1[i]), sizeof(NCadrGeom), 0)) goto exit3;
//	for (int i = 0; i < SectGeom2.GetSize(); ++i)
//		if (!::CryptHashData(hHash, (BYTE *)(SectGeom2[i]), sizeof(NCadrGeom), 0)) goto exit3;
//
//	DWORD length = 16;
//	if (!::CryptGetHashParam(hHash, HP_HASHVAL, Hash, &length, 0)) goto exit4;
//
//	result = true;
//
//exit4:
//exit3: ::CryptDestroyHash(hHash);
//exit2: ::CryptReleaseContext(hCryptProv, 0);
//exit1: return result;
	static std::atomic<int> ID{ 0 };
	Uuid = ++ID;
	return true;
}

const BGeomArray *NTurnStockSkeleton::GetSectGeom(int Part) const
{
	switch (Part)
	{
	case SkelContour1:
		return &SectGeom1;
	case SkelContour2:
		return &SectGeom2;
	case SkelArcs:
		return &Arcs;
	default:
		return nullptr;
	}
}
