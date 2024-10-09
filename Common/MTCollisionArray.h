#pragma once

class SCollision :public CObject
{
public:
	const SCollision& operator = (const SCollision &C);
	
	CString Axis1;//Axes names for collision control
	CString Axis2;
	BOOL Axis1Ch;//Control whole branch (axis + its children) or only axis
	BOOL Axis2Ch;
};

typedef CTypedPtrArray<CObArray,SCollision*> CollisionArray;

class AFX_EXT_CLASS MTCollisionArray :public CObject
{
public:
	DECLARE_SERIAL(MTCollisionArray)
	~MTCollisionArray();
	INT_PTR GetSize() const;
	void RemoveAll();
	void AddCol(SCollision &Col);//Add collision four only to MTCollisionArray
	void AddCol(class MTBase *MT, SCollision &Col);//Add a collision four either for MTCollisionArray and MachineTool
	int GetAxisColNum(const CString& AxisName);//Collisions number for specified axis
	SCollision* GetAxisCol(const CString& AxisName, int index = 1);
	int GetColIndex(const CString& Axis1, const CString& Axis2);
	SCollision* operator [] (int index) const;
	void RenameAxis(const CString& OldName, const CString& NewName);
	void RemoveCol(int ColIndex);
	
private:
	CollisionArray Collisions;
};
