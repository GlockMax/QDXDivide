#pragma once
#include "UNITS_API.h"
#include <map>

class UNITS_API GotoMarks
{
private:
	class KeyMark
	{
	public:
		KeyMark();
		KeyMark(int n);

		int name;
		int count;

		bool operator >(KeyMark k) const;
		bool operator <(KeyMark k) const;
	};

public:
	GotoMarks();
	~GotoMarks();
	void addMark(int name, int str);
	int findMark(int name, int lineNumber, bool UpDirection);
	void Clear(void);
	bool CheckRange(int Source, int Dest) const;
	bool isCreated;
protected:
	std::map<KeyMark, int> dict;
};
