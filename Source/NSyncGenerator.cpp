#include "stdafx.h"
#include "NCUnit.h"
#include "NSyncCheckPoints.h"
#include "NSyncGenerator.h"

NSyncGenerator::NSyncGenerator(CNCMDoc& InDoc) : Doc(InDoc)
{
	for (int iCh = 0; iCh < Doc.GetChannelsCount(); ++iCh)
	{
		ChanDatas.push_back(InDoc.GetChannel(iCh));
	}
}

NSyncGenerator::~NSyncGenerator()
{
}

void NSyncGenerator::SetTimes()
{
	while(true)
	{
		for(int i = 0; i < ChanDatas.size(); ++i)
		{
			ChanDatas.at(i).SetTimesCn();
		}
		const NCheckPoint* pFreeCP = FindNextFreeCP();
		if (pFreeCP == nullptr)
			break;
		if (pFreeCP->IsWait())
		{
			auto it = ActSyncIDs.find(pFreeCP->GetSyncID());
			if (it == ActSyncIDs.end())
				ChanDatas.at(pFreeCP->GetChanInd()).ID2Wait = pFreeCP->GetSyncID();
		}
		else if (pFreeCP->IsEventRem())
		{
			ActSyncIDs.erase(pFreeCP->GetSyncID());
		}
		else// pFreeCP->IsEvent
		{
			ActSyncIDs.insert(pFreeCP->GetSyncID());
			for(int i = 0; i < ChanDatas.size(); ++i)
			{
				auto& ChData = ChanDatas.at(i);
				if (ChData.ID2Wait.IsCorr(pFreeCP->GetSyncID()))
				{
					ChData.ID2Wait.SetEmpty();
					ChData.CurTime = pFreeCP->GetTime();
				}
			}
		}
	}
	for each (auto Ch in ChanDatas)
	{
		if (!Ch.ID2Wait.IsEmpty())
			AfxMessageBox(IDS_MES_ERRSYN);
	}
	// set 0 channel end time after all other channels
	if (ChanDatas.size() > 1)
	{
		MainTime EndTime = 0;
		for (int i = 1; i < ChanDatas.size(); ++i)
		{
			MainTime CurEndTime = ChanDatas.at(i).GetChannel().GetEndTime();
			EndTime = CurEndTime > EndTime ? CurEndTime : EndTime;
		}
		if(ChanDatas.at(0).GetChannel().GetEndTime() <= EndTime)
			ChanDatas.at(0).GetChannel().SetEndTime(EndTime);
	}

}

const NCheckPoint* NSyncGenerator::FindNextFreeCP()
{
	const NCheckPoint* pResult = nullptr;
	MainTime MinTime = MainT_UNDEF;
	int ChanFound = -1;
	for each (auto ChData in ChanDatas)
	{
		const NCheckPoint* pCP = ChData.GetFreeCP();
		if (pCP != nullptr)
		{
			MainTime TmpTime = pCP->GetTime();
			if (MinTime > TmpTime)
			{
				MinTime = TmpTime;
				pResult = pCP;
				ChanFound = ChData.pChannel->GetChannelInd();
			}
		}
	}
	if (ChanFound >= 0)
	{
		auto &ChannelData = ChanDatas.at(ChanFound);
		ChannelData.FreeCPInd++;
		ChannelData.CPNextInd.Cadr = ChannelData.FreeCPInd;
		BProgram* pNewProg = pResult->GetProg();
		if (ChannelData.pNextProg != pNewProg)
		{
			ChannelData.CPNextInd.Prog = ChannelData.pChannel->NGetProgramListCn()->GetIndex(pNewProg);
			ChannelData.pNextProg = pNewProg;
		}

	}
	return pResult;
}

NSyncGenerator::ChannelData::ChannelData(NChannel* pInChannel) : CPCurInd(-1), CPNextInd(-1)
{
	pNextProg = nullptr;
	CurTime = 0;
	pChannel = pInChannel;
	FreeCPInd = 0;
	auto pCP = pChannel->GetUnitCn()->GetCheckPoint(0);
	if (pCP != nullptr)
	{
		CPNextInd.Cadr = 0;
		pNextProg = pCP->GetProg();
		CPNextInd.Prog = pChannel->NGetProgramListCn()->GetIndex(pNextProg);
	}
}

NSyncGenerator::ChannelData::~ChannelData()
{
}

void NSyncGenerator::ChannelData::SetTimesCn()
{
	if (!ID2Wait.IsEmpty())
		return;// Channel is waiting
	const NCUnit& Unit = *pChannel->GetUnitCn();
	bool Before = true;
	int CPStartInd = 0;
	if (CPCurInd.IsEmpty())
	{
		CPCurInd.Prog = 0;
		pCurProg = pChannel->NGetProgramListCn()->GetProg(CPCurInd);
	}
	else
	{
		if (CPCurInd == CPNextInd)
			return;
		const NCheckPoint* pCurCheckPoint = Unit.GetCheckPoint(CPCurInd.Cadr);
		if (pCurCheckPoint != nullptr)
		{
			Before = pCurCheckPoint->GetBefore();
			CPStartInd = pCurCheckPoint->GetCadrHistInd();
			pCurProg = pCurCheckPoint->GetProg();
		}
	}
	if (pCurProg == nullptr)
		return;
	if (pCurProg->GetSize() == 0 || pCurProg->HaveErr())
		return;
	if (pCurProg->GetGeomArraySize() == 0)
		return;
	if (Before)
		pCurProg->SetWorkT(CPStartInd, CurTime);
	else
		pCurProg->SetEndT(CPStartInd, CurTime);
	if (CPNextInd.IsEmpty())
	{
		CPNextInd.Prog = pChannel->NGetProgramListCn()->GetMainProgCount() - 1;
		CPNextInd.Cadr = max(0, Unit.GetCheckPointCount() - 1);
		pNextProg = pChannel->NGetProgramListCn()->GetProg(CPNextInd);
	}

	const NCheckPoint* pNextCheckPoint = Unit.GetCheckPoint(CPNextInd.Cadr);
	int CPEndInd = (pNextCheckPoint == nullptr) ? int(pNextProg->GetGeomArraySize() - 1) : pNextCheckPoint->GetCadrHistInd();
	BProgram* pProg = pCurProg;
	MainTime StartTime = pCurProg->GetCompGeomArray(0).GetAt(CPStartInd)->GetEndTime();
	for (int ip = CPCurInd.Prog; ip <= CPNextInd.Prog; pProg = (++ip == CPNextInd.Prog) ? pNextProg : pChannel->NGetProgramListCn()->GetProg(cadrID(ip, 0)))
	{
		int StartInd = 0;
		if (ip == CPCurInd.Prog)
			StartInd = CPStartInd + 1;
		int EndInd = -1;
		if (ip == CPNextInd.Prog)
			EndInd = CPEndInd;
		StartTime = pProg->SetTimesInt(StartInd, EndInd, StartTime);
	}
	CPCurInd = CPNextInd;
	CurTime = CPEndInd < 0 ? StartTime : pNextProg->GetCompGeomArray(0).GetAt(CPEndInd)->GetStartTime();
}

const NCheckPoint* NSyncGenerator::ChannelData::GetFreeCP() const
{
	if (!ID2Wait.IsEmpty())
		return nullptr;// Channal is waiting
	return pChannel->GetUnitCn()->GetCheckPoint(FreeCPInd);
}
