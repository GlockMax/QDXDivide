// NCallStack.h: interface for the NCallStack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class AFX_EXT_CLASS NCallStack : public CPtrArray 
// Subprogram call stack 
{
public:
	void Clear();
	int Pop(class cadrID &elem, int &num);
	int Push(class cadrID &elem, const int &num);
	NCallStack();
	NCallStack(const NCallStack&);
	NCallStack& operator = (const NCallStack&);
	virtual ~NCallStack();

};
