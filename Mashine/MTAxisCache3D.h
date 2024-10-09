#pragma once
#include <vector>
class MTAxisCache3D
{
public:
	MTAxisCache3D();
	~MTAxisCache3D();
	void Reset();
	void Clear();
	void Init();
	void Set(int i, int j, int k, void* newValue);
	void* Get(int i, int j, int k, bool& needNewValue);
protected:
	typedef std::vector<void*> Data3;
	typedef std::vector<Data3*> Data2;
	typedef std::vector<Data2*> Data1;
	Data1 storage_;
};

