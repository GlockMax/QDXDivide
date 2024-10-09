#include "stdafx.h"
#include "GotoMarks.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GotoMarks::KeyMark::KeyMark() : name(0), count(0)
{
}

GotoMarks::KeyMark::KeyMark(int n) : name(n), count(0)
{
}

bool GotoMarks::KeyMark::operator >(KeyMark k) const
{
	if(name != k.name) return (name > k.name);
	else return(count > k.count);
	//return (name > k.name && count >= k.count);
}

bool GotoMarks::KeyMark::operator <(KeyMark k) const
{
	if(name != k.name) return (name < k.name);
	else return(count < k.count);
	//return (name < k.name && count <= k.count);
}
// ----------------------------------------------------------

GotoMarks::GotoMarks()
{
}

GotoMarks::~GotoMarks()
{
}

void GotoMarks::addMark(int name, int str)
{
	KeyMark keyp(name);
	bool keyNew = false;
	keyp.count=0;

	do
	{
		auto it = dict.find(keyp);
		if(it != dict.end() && it->first.count == keyp.count)
			keyp.count++;
		else
			keyNew = true;
	}
	while(keyNew == false);
	
	dict.insert(pair<KeyMark,int>(keyp, str));
}

int GotoMarks::findMark(int name,int lineNumber, bool UpDirection)
{
	KeyMark keyp(name);
	bool stop = false;
	int line = -1;
	keyp.count = 0;

	auto it = dict.begin();
	while(it != dict.end())
	{
		it = dict.find(keyp);
		if(it != dict.end())
		{
			if(it->second > line)
			{
				if(it->second > lineNumber) 
				{
					if(UpDirection)
						return line;
					else
						return it->second;
				}
				else if(it->second == lineNumber)
				{
					if(UpDirection)
						return lineNumber;
				}
				line = it->second;
				keyp.count++;
			}
		}
	}
	if(UpDirection)
		return line;
	else
		return (-1);
}

void GotoMarks::Clear(void)
{
	dict.clear();
}

bool GotoMarks::CheckRange(int Source, int Dest) const
{
	// Returns true if map contains elements with the line number between Source and Dest
	int Start = min(Source, Dest);
	int End = max(Source, Dest);
	for(std::map<KeyMark, int>::const_iterator it = dict.cbegin(); it != dict.cend(); ++it)
	{
		if(it->second >= Start && it->second < End)
			return true;
	}
	return false;
}
