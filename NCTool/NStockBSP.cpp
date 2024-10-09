#include "stdafx.h"
#include "resource.h"
#include "zlib.h"
#include "MBSPForest.h"
#include "MQuadTreeRot.h"
#include "DXSubModel5X.h"
#include "GQuadRenderRot.h"
#include "DXSubModel.h"
#include "GQuadRenderDX5X.h"
#include "GQuadRenderMill.h"
#include "NStockBSP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(NStockBSP, NStockOrigR, 0)


NStockBSP::NStockBSP()
{
	BSPTNumOrig = -1;
	BSPTNumRotOrig = -1;
	Invert = false;
	BaseCenter.Set(0., 0., 0., 1.);
}


NStockBSP::~NStockBSP()
{
}

NStock::StockCrType NStockBSP::GetType() const
{
	return SCT_IMPEXP;
}

void NStockBSP::Create()
{
	NStock::Create();
	IsCreated = false;
	Recalc();
	IsCreated = true;
}

void NStockBSP::Create(const CString &iFileName, MQuadTree * pTree, class MQuadTree *pTreeRot /*= nullptr*/)
{
	if (BSPTNumOrig >= 0)
		MFOREST.DeleteTree(BSPTNumOrig);
	if (BSPTNumRotOrig >= 0)
		MFOREST.DeleteTree(BSPTNumRotOrig);
	if (pTree != nullptr)
		BSPTNumOrig = MFOREST.AddTree(pTree);
	if (pTreeRot != nullptr)
		BSPTNumRotOrig = MFOREST.AddTree(pTreeRot);
	Create();
	if (IsCreated)
		FileName = iFileName;
}

bool NStockBSP::CreateDXBodyFrBSPRend()
{
	// make proper quad render
	MQuadBody* pOrigBody = static_cast<MQuadBody*>(MFOREST.GetTree(BSPTNumOrig));// BSPTNumOrig is >= 0
	GQuadRenderMill* pQuadRenderMill = new GQuadRenderMill(*pOrigBody);
	if (pQuadRenderMill == nullptr)
		return false;

	auto Buf = NCMComData::GetType();// we need to swtch of millturm mode to get quad render in proper position
	NCMComData::SetType(AT_MILL);
	pQuadRenderMill->FillAllArraysCh(*pOrigBody);
	NCMComData::SetType(Buf);

	// make DX body
	int DXNum = int(BDebugState::Get().DXNumber + 0.5);
	BBox Box = pQuadRenderMill->GetGabar();
	Box.Offset(+0.001, +0.001, +0.001);

	MakeBoxDX5X(DXNum, Box);
	DXSubModel5X* pBodyDX = (DXSubModel5X*)MFOREST.GetTree(BSPTNumDX);
	pBodyDX->SetBodyRot(pOrigBody->GetBodyRot().Or());//!!!
	pBodyDX->SetBodyRotActive(pOrigBody->IsBodyRotActive());//!!!

	pBodyDX->MakeFromQRendMill(pQuadRenderMill);
	delete pQuadRenderMill;
	delete pOrigRender;
	CreateRenderMill();
	pOrigRender = (GQuadRender*)pRender;
	pRender = NULL;
	CreateRenderMill();
	return true;
}

bool NStockBSP::CreateBody(void)
{
	bool res = true;
	Create();
	if (BSPTNumOrig >= 0)
	{
		MQuadBody *pOrigBody= static_cast<MQuadBody *>(MFOREST.GetTree(BSPTNumOrig));
		if (pOrigBody == nullptr)
			return false;
		MQuadTree* pOrigTree = dynamic_cast<MQuadTree*>(pOrigBody);
		switch (NCM_PROJECT.GetGlobalState().GetModelMode())
		{
		case NGlobalState::DX:
			if(pOrigTree != nullptr)// input file contains BSP model
//				res = CreateBodyFrBSP(pOrigTree);
				res = CreateDXBodyFrBSPRend();
			else // input file contains DX model
			{
				DXSubModel5X* pNewBody = new DXSubModel5X(DXWorld, BStockGrID());
				pOrigBody->CreateFullCopy(pNewBody);
				delete pOrigRender;
				BSPTNumDX = BSPTNumOrig;
				CreateRenderMill();
				pOrigRender = (GQuadRender*)pRender;
				pRender = NULL;
				BSPTNumDX = MFOREST.AddTree(pNewBody);
				CreateRenderMill();
			}
			break;
		case NGlobalState::BSP:
			if (pOrigTree != nullptr)// input file contains BSP model
			{
				MQuadTree* pTree = pOrigTree->CreateFullCopy();
				res = CreateBodyFrBSP(pTree);
			}
			else// input file contains DX model
				return false;
			break;
		}
	}
	if (BSPTNumRotOrig >= 0)
	{
		MQuadTreeRot *pOrigTree = static_cast<MQuadTreeRot *>(MFOREST.GetTree(BSPTNumRotOrig));
		if (pOrigTree == nullptr)
			return false;
		MQuadTreeRot *pTree = nullptr;
		pTree = pOrigTree->CreateFullCopy();
		if (pTree == nullptr)
			return false;
		if (BSPTNumRot < 0)
			BSPTNumRot = MFOREST.AddTree(pTree);
		else
		{
			MFOREST.DeleteTree(BSPTNumRot);
			MFOREST.SetTree(BSPTNumRot, pTree);
		}
		delete pOrigRenderRot;
		CreateRenderRot();
		pOrigRenderRot = pRenderRot;
		pRenderRot = NULL;
		CreateRenderRot();
	}
	MillStockChanged = false;
	TurnStockChanged = false;
	return res;
}

bool NStockBSP::LoadPacked(const CString& SourceName, BYTE*& pBuffer, unsigned long& UnpackedLength)
{
	CFile fp;
	if (!fp.Open(SourceName, CFile::modeRead | CFile::typeBinary))
		return false;
	fp.Read(&UnpackedLength, sizeof(UnpackedLength));
	UINT PackedLength = UINT(fp.GetLength() - sizeof(UnpackedLength));
	BYTE* FileBuf = new BYTE[PackedLength];
	fp.Read(FileBuf, PackedLength);
	fp.Close();
	pBuffer = new BYTE[UnpackedLength];
	if (Z_OK != uncompress(pBuffer, &UnpackedLength, FileBuf, PackedLength))
	{
		delete[] FileBuf;
		delete[] pBuffer;
		return false;
	}
	delete[] FileBuf;
	return true;
}

void NStockBSP::CreateEx(const CString & Name)
{
	IsCreated = false;

	BYTE* pBuffer = nullptr;
	unsigned long UnpackedLength = 0;
	if (!LoadPacked(Name, pBuffer, UnpackedLength))
		return;

	CMemFile f(pBuffer, UnpackedLength);

	NCMApplicationType InputType;
	f.Read(&InputType, sizeof(InputType));
	SetStockType(InputType);
	// Check input stock type and current project type compatibility
	if (InputType != NCMComData::GetType())
	{
		if (InputType == AT_MILL)
		{
			f.Close();
			delete[] pBuffer;
			return;
		}
		if (InputType == AT_MILL_TURN)
		{
			if (NCMComData::GetType() == AT_TURN)
			{// Skip mill body
				MQuadBody *pQuadBody = Load1Tree(f);
				delete pQuadBody;
			}
			SetStockType(NCMComData::GetType());
		}
	}
	MQuadBody* pQuadBody = Load1Tree(f);
	if(pQuadBody == nullptr)
	{
		f.Close();
		delete[] pBuffer;
		AfxMessageBox(IDS_MES_VERMFRWRONG);
		return;
	}
	pQuadBody->SetStockPos(GetStockGr());
	MQuadTree* pQuadTree = dynamic_cast<MQuadTree*>(pQuadBody);
	if(pQuadTree != nullptr)
		pQuadTree->CrStandard(false);
	switch (GetStockType())
	{
	case AT_MILL:
		BSPTNumOrig = MFOREST.AddTree(pQuadBody);
		break;
	case AT_TURN:
		BSPTNumRotOrig = MFOREST.AddTree(pQuadTree);
		break;
	case AT_MILL_TURN:
		BSPTNumOrig = MFOREST.AddTree(pQuadBody);
		MQuadTreeRot *pQuadTreeRot = (MQuadTreeRot *)Load1Tree(f);
		if (pQuadTreeRot == nullptr)// incorrect version
		{
			f.Close();
			delete[] pBuffer;
			AfxMessageBox(IDS_MES_VERMFRWRONG);
			return;
		}
		pQuadTreeRot->SetStockPos(GetStockGr());
		pQuadTreeRot->CrStandard(false);
		BSPTNumRotOrig = MFOREST.AddTree(pQuadTreeRot);
		SetStockState(AT_TURN);
		break;
	}
	if (InputType != NCMComData::GetType())
	{
		if (InputType == AT_MILL_TURN)
		{
			if (NCMComData::GetType() == AT_MILL)
				MFOREST.Transform1Tree(BSPTNumOrig, BMatr().RotX(0., 0., 0., 90.));
		}
		else if (InputType == AT_TURN)
		{
			SetStockType(NCMComData::GetType());
			if (BSPTNumRot >= 0)
				MFOREST.DeleteTree(BSPTNumRot);
			BSPTNumRot = BSPTNumRotOrig;
			CrBody4Mill();
			BSPTNumRotOrig = BSPTNumRot;
			BSPTNumRot = -1;
			BSPTNumOrig = BSPTNum;
			BSPTNum = -1;
			if (NCMComData::GetType() == AT_MILL)
			{
				MFOREST.DeleteTree(BSPTNumRotOrig);
				BSPTNumRotOrig = -1;
			}
			else
				SetStockState(AT_TURN);
		}
	}
	f.Close();
	delete[] pBuffer;
	Recalc();
	FileName = Name;
	IsCreated = true;
}

void NStockBSP::ReadGabarOnly(const CString & Name)
{
	IsCreated = false;

	BYTE* pBuffer = nullptr;
	unsigned long UnpackedLength = 0;
	if (!LoadPacked(Name, pBuffer, UnpackedLength))
		return;

	CMemFile f(pBuffer, UnpackedLength);

	NCMApplicationType buf;
	f.Read(&buf, sizeof(buf));
	SetStockType(buf);
	MQuadBody *pTree = Load1Tree(f, true);
	if (pTree == nullptr)// incorrect version
	{
		f.Close();
		AfxMessageBox(IDS_MES_VERMFRWRONG);
		delete[] pBuffer;
		return;
	}
	Gabar.Init();
	switch (GetStockType())
	{
	case AT_MILL:
		Gabar.Expand(pTree->GetMin());
		Gabar.Expand(pTree->GetMax());
		break;
	case AT_MILL_TURN:
		Gabar.Expand(BPoint(-pTree->GetMax().GetX(), -pTree->GetMax().GetX(), pTree->GetMin().GetY(), 1.));
		Gabar.Expand(BPoint(+pTree->GetMax().GetX(), +pTree->GetMax().GetX(), pTree->GetMax().GetY(), 1.));
		break;
	case AT_TURN:
		Gabar.Expand(BPoint(-pTree->GetMax().GetX(), -pTree->GetMax().GetX(), pTree->GetMin().GetY(), 1.));
		Gabar.Expand(BPoint(+pTree->GetMax().GetX(), +pTree->GetMax().GetX(), pTree->GetMax().GetY(), 1.));
		break;
	}
	f.Close();
	delete[] pBuffer;
	delete pTree;
	FileName = Name;
	IsCreated = true;
}

void NStockBSP::CreateCopyTo(class NStock* &stock) const
{
	if (!stock)
		stock = new NStockBSP();
	NStockOrigR::CreateCopyTo(stock);

	NStockBSP *pBSP = (NStockBSP *)stock;
	if (BSPTNumOrig >= 0)
	{
		MQuadTree *pOrigTree = static_cast<MQuadTree *>(MFOREST.GetTree(BSPTNumOrig));
		if (pOrigTree == nullptr)
			return;
		MQuadTree *pNewTree = pOrigTree->CreateFullCopy();
		pBSP->BSPTNumOrig = MFOREST.AddTree(pNewTree);
	}
	if (BSPTNumRotOrig >= 0)
	{
		MQuadTreeRot *pOrigTree = static_cast<MQuadTreeRot *>(MFOREST.GetTree(BSPTNumRotOrig));
		if (pOrigTree == nullptr)
			return;
		MQuadTreeRot *pNewTree = pOrigTree->CreateFullCopy();
		pBSP->BSPTNumRotOrig = MFOREST.AddTree(pNewTree);
		pBSP->BaseCenter = BaseCenter;
		pBSP->Invert = Invert;
	}
}

const BMatr& NStockBSP::GetMatr() const
{
	const MBody* pBody = GetBody();
	if (pBody == nullptr)
		return NMovableObj::GetMatr();
	static BMatr M;
	M = pBody->GetBodyRot().Or();
	BMatr Tr = NMovableObj::GetMatr() * M.GetRot();
	M.SetRow(3, Tr.GetRow(3));
	return M;
}

void NStockBSP::Transformm(const BMatr & M)
{
	BMatr Old = GetMatr();
	BMatr New = Old * M;
	NMovableObj::GetMatrRef() = New.GetTran() * New.GetRot().invr();
	MFOREST.Transform1Tree(BSPTNumOrig, M);
	MFOREST.Transform1Tree(BSPTNumRotOrig, M);
	Recalc();
}

void NStockBSP::Serialize(CArchive & ar)
{
	NStockOrigR::Serialize(ar);
	if (ar.IsStoring())
	{
		CMemFile File(2 * 1024 * 1024);
		if (BSPTNumOrig >= 0)
			Save1Tree(File, BSPTNumOrig, BMatr());
		if (BSPTNumRotOrig >= 0)
			Save1Tree(File, BSPTNumRotOrig, BMatr());
		ULONG Length = UINT(File.GetLength());
		SerializeElements(ar, &Length, 1);
		BYTE *pBuffer = File.Detach();
		ULONG DestLen = compressBound(Length);
		BYTE* pDest = new BYTE[DestLen];
		compress2(pDest, &DestLen, pBuffer, Length, Z_DEFAULT_COMPRESSION);
		SerializeElements(ar, &DestLen, 1);
		ar.Write(pDest, DestLen);
		delete[] pDest;
		delete[] pBuffer;
	}
	else
	{
		ULONG Length;
		SerializeElements(ar, &Length, 1);
		BYTE *pBuffer = new BYTE[Length];
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.2")))
		{
			ULONG DestLen;
			SerializeElements(ar, &DestLen, 1);
			BYTE* pDest = new BYTE[DestLen];
			ar.Read(pDest, DestLen);
			uncompress(pBuffer, &Length, pDest, DestLen);
			delete[] pDest;
		}
		else
			ar.Read(pBuffer, Length);

		CMemFile File(pBuffer, Length);
		MQuadBody *pQuadBody = nullptr;
		MQuadBody *pQuadBodyRot = nullptr;
		SetStockType(NCMComData::GetType());
		switch (GetStockType())
		{
		case AT_MILL:
			pQuadBody = Load1Tree(File);
			if(pQuadBody == nullptr)
				AfxMessageBox(IDS_MES_VERMFRWRONG);
			else
				pQuadBody->SetStockPos(GetStockGr());
			break;
		case AT_TURN:
			pQuadBodyRot = Load1Tree(File);
			if (pQuadBodyRot == nullptr)
				AfxMessageBox(IDS_MES_VERMFRWRONG);
			else
				pQuadBodyRot->SetStockPos(GetStockGr());
			break;
		case AT_MILL_TURN:
			pQuadBody = Load1Tree(File);
			pQuadBodyRot = Load1Tree(File);
			if (pQuadBody == nullptr || pQuadBodyRot == nullptr)
				AfxMessageBox(IDS_MES_VERMFRWRONG);
			else
			{
				pQuadBody->SetStockPos(GetStockGr());
				pQuadBodyRot->SetStockPos(GetStockGr());
			}
			break;
		}
		if (pQuadBody != nullptr)
		{
			BSPTNumOrig = MFOREST.AddTree(pQuadBody);
			MQuadTree* pQuadTree = dynamic_cast<MQuadTree*>(pQuadBody);
			if(pQuadTree != nullptr)
				pQuadTree->CrStandard(false);
		}

		if (pQuadBodyRot != nullptr)
		{
			BSPTNumRotOrig = MFOREST.AddTree(pQuadBodyRot);
			MQuadTree* pQuadTreeRot = dynamic_cast<MQuadTree*>(pQuadBodyRot);
			if(pQuadTreeRot != nullptr)
				pQuadTreeRot->CrStandard(false);
		}

		delete[] pBuffer;

	}
}

void NStockBSP::Recalc()
{
	Gabar.Init();
	if (BSPTNumOrig >= 0)
	{
		MQuadBody *pQuadBody = static_cast<MQuadBody *>(MFOREST.GetTree(BSPTNumOrig));
		Gabar.Expand(pQuadBody->GetMin());
		Gabar.Expand(pQuadBody->GetMax());
	}
	else if (BSPTNumRotOrig >= 0)
	{
		MQuadTree *pQuadTree = static_cast<MQuadTree *>(MFOREST.GetTree(BSPTNumRotOrig));
		Gabar.Expand(BPoint(-pQuadTree->GetMax().GetX(), -pQuadTree->GetMax().GetX(), pQuadTree->GetMin().GetY(), 1.));
		Gabar.Expand(BPoint(+pQuadTree->GetMax().GetX(), +pQuadTree->GetMax().GetX(), pQuadTree->GetMax().GetY(), 1.));
	}
	if (NCMComData::IsTurnEnabled())
		Gabar.Transform(BMatr().RotX(0., 0., 0., 90.));
}

void NStockBSP::MoveBase(double x, double y, double z, bool iInvert)
{
	// Для токарной и токарно-фрезерной заготовок модель повернута 
	// относительно истинного положения на -90 градусов вокруг OX
	Invert = iInvert;
	BPoint v = BPoint(x, y, z, 1.) * BMatr().RotX(0., 0., 0., 90.) - BaseCenter;
	if (Invert || v.D2() != 0.)
	{
		if (BSPTNumRotOrig >= 0)
		{
			BMatr M;
			M.Trans(BPoint(0., 0., 0., 0.), v);
			M = BMatr().RotX(0., 0., 0., 180.) * M;
			MQuadTreeRot *pOrigTree = static_cast<MQuadTreeRot *>(MFOREST.GetTree(BSPTNumRotOrig));
			if (pOrigTree == nullptr)
				return;
			pOrigTree->Translate(M);
			pOrigTree->ChangeYDir();
		}
		if (BSPTNumOrig >= 0)
		{
			BMatr M;
			M.Trans(BPoint(0., 0., 0., 0.), v);
			M = BMatr().RotX(0., 0., 0., 180.) * M;
			MQuadTree *pOrigTree = static_cast<MQuadTreeRot *>(MFOREST.GetTree(BSPTNumOrig));
			if (pOrigTree == nullptr)
				return;
			pOrigTree->Translate(M);
			pOrigTree->ChangeYDir();
		}
	}
	BaseCenter.Set(x, y, z, 1.);
	Create();
}

void NStockBSP::DrawGabar(void) const
{
	MBody *pBody = MFOREST.GetTree(BSPTNumOrig);
	if (pBody == nullptr)
		return;
	glPushMatrix();
	glMultMatrixd(pBody->GetBodyRot().Or().GetArray());
	NStock::DrawGabar();
	glPopMatrix();
}

BBox NStockBSP::GetGabar(void) const
{
	MBody *pBody = MFOREST.GetTree(BSPTNumOrig);
	if (pBody == nullptr)
		return NStockOrigR::GetGabar();
	if (!pBody->IsBodyRotActive())
		return NStockOrigR::GetGabar();
	BBox Tmp = Gabar;
	Tmp.Transform(pBody->GetBodyRot().Or());
	return Tmp;
}

const MBody* NStockBSP::GetOrigBody() const
{
	if (BSPTNumOrig >= 0)
		return MFOREST.GetTree(BSPTNumOrig);
	return nullptr;
}

