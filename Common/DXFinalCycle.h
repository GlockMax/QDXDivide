#pragma once

class DXFinalCycle
{
public:
	class Elem
	{
	public:
		int Number;// номер ребера или грани, в которых есть точки входящие в цикл
		bool BoundOrEdge; // True if bound
	public:
		Elem(int Num, bool key) { Number = Num; BoundOrEdge = key;}
		Elem() {}
		~Elem() {}
	};

protected:
	Elem Cycle[18];
	int Size;
	int TrueCount;
public:
	DXFinalCycle() {};
	~DXFinalCycle() {};
	void Clear(void) { Size = 0; TrueCount = 0;}
	void PutBack(int Val, bool Key) {Cycle[Size].Number = Val; Cycle[Size++].BoundOrEdge = Key; if(Key) ++TrueCount;}
	int GetValAt(int Index) const {return Cycle[Index].Number;}
	bool GetKeyAt(int Index) const {return Cycle[Index].BoundOrEdge;}
	int GetSize() const {return Size;}
	int GetTrueCount() const { return TrueCount;}
	int FirstTrue() const;
	const Elem &operator[] (int Ind) const { return Cycle[Ind];}
};