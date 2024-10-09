#include "stdafx.h"
#include "GBodyRender.h"
#include "BDCEL_IND.h"
#include "GQuadRenderDX5X.h"
#include "NModRecord.h"


NModRecord::NModRecord()
{
	pThread = nullptr;
}


NModRecord::~NModRecord()
{
}

void NModRecord::Start()
{
	if (pThread != nullptr)
		Stop();

	pThread = AfxBeginThread(ThreadProc, &Queue, THREAD_PRIORITY_ABOVE_NORMAL, 20000000, CREATE_SUSPENDED);
	if (pThread)
	{
		pThread->m_bAutoDelete = FALSE;
		pThread->ResumeThread();
	}
}

void NModRecord::Stop()
{
	if (pThread == nullptr)
		return;
	Queue.push(nullptr);
	WaitForSingleObject(pThread->m_hThread, INFINITE);
	delete pThread;
	pThread = nullptr;
}

UINT NModRecord::ThreadProc(LPVOID pParam)
{
	ModelsQueue *pQueue = static_cast<ModelsQueue *>(pParam);
	if (pQueue == nullptr)
		return 1;

	while (1) 
	{ // Stop when queue element is nullptr
		GBodyRender *pRender = nullptr;
		bool Empty = !pQueue->try_pop(pRender);
		if (Empty)
			Sleep(10);
		else
		{
			if (pRender == nullptr)
				break;

			Store2U3D(pRender);

			delete pRender;
		}
	}

	return 0;
}

void NModRecord::Store2U3D(GBodyRender *pRender)
{
	GQuadRenderDX5X *pRenderDX = dynamic_cast<GQuadRenderDX5X *>(pRender);
	if (pRenderDX == nullptr)
		return;

	for (int i = 0; i < pRenderDX->GetAreasNum(); ++i)
	{
		RTriRandMesh *pTrianglArea = pRenderDX->GetArea(i);
		Store2U3D(pTrianglArea);
	}
}

void NModRecord::Store2U3D(RTriRandMesh *pTrianglArea)
{
	if (!pTrianglArea)
		return;

	BDCEL_IND EL;
	int NTri;// A triangles number
	unsigned int *Indexes; // The indexes of the triangles vertexes 
	pTrianglArea->GetIndexesArray(NTri, Indexes);
	size_t Size;
	const float *pBuf = pTrianglArea->Open4Read(Size);
	int *BackNums = nullptr;	// BackNums - связь между старыми и новыми номерами точек в хранилище точек. Для каждого старого номера хранит новый номер

	int PointsNum = EL.IdentifyPoints(pBuf, Indexes, NTri, BackNums);

	// Store data
	//const float *pNPBlock = EL.Get3NPBlock(i); gets normal and point coordinates by index. Format: Xn, Yn, Zn, Xp, Yp, Zp
	// END: Store data
	delete[] Indexes;
	delete[] BackNums;
}
