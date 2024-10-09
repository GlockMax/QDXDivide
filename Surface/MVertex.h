#pragma once

class MVertex
{
public:
	MVertex(void);
	~MVertex(void);
	inline bool IsDefined() const { return (NPoint >= 0); }
	inline int GetPointNum() const { return NPoint;}
	inline void SetPointNum(int Num) { NPoint = Num;}
	inline int GetEdgeNum() const { return NEdge;}
	inline void SetEdgeNum(int Num) { NEdge = Num;}
protected:
	int NPoint;
	int NEdge;// One of incidental edges
};
