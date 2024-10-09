#pragma once
#include "concurrent_queue.h"
#include "NRecord_API.h"

using namespace Concurrency;

class NRECORD_API NModRecord
{
typedef concurrent_queue<class GBodyRender *> ModelsQueue;
public:
	NModRecord();
	~NModRecord();
	void Start();
	void Stop();
	ModelsQueue &GetQueue() { return Queue; }
protected:
	static UINT ThreadProc(LPVOID pParam);
	static void Store2U3D(GBodyRender *pRender);
	static void Store2U3D(class RTriRandMesh *pTrianglArea);

protected:
	ModelsQueue Queue;
	CWinThread *pThread;
};

