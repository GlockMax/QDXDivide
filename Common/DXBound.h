#pragma once

class DXBound
{
protected:
	int IndexBuf[2]; // индекс точки грани в массиве BUF. IndexBuf[1] = IndexBuf[0] + 1 always
	UINT IndexSharp[2];
public:
	DXBound() { IndexSharp[0] = UINT_MAX; IndexSharp[1] = UINT_MAX; }
	DXBound(int Index) { IndexBuf[0] = Index; IndexBuf[1] = Index + 1; IndexSharp[0] = UINT_MAX; IndexSharp[1] = UINT_MAX; }
	~DXBound() {};
	const int* GetPoint() const { return IndexBuf; }
	void SetPointInd(int Index) { IndexBuf[0] = Index; IndexBuf[1] = Index + 1; }
	void AddSharpInd(UINT SharpInd) { IndexSharp[(IndexSharp[0] == UINT_MAX) ? 0 : 1] = SharpInd; }
	const UINT* GetIndexSharp() const { return IndexSharp; }
	bool Have2Edges() const { return (IndexSharp[0] != UINT_MAX) && (IndexSharp[1] != UINT_MAX);  }
};