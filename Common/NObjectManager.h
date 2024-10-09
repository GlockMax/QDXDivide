#pragma once
#include "NOMScaner.h"
#include "BRelation.h"
#include "NActiveObject.h"
#include "NPassiveObject.h"


template <class OBJECT_CLASS, unsigned int OBJECTS>
class NObjects
{
public:
	NObjects()
	{
		Size = 0;
		for(int i = 0; i < OBJECTS; ++i)
		{
			Obj[i] = NULL;
			Groups[i] = OG_GR_EMPTY;
		}
	};
	~NObjects()
	{
	};
private:
	OBJECT_CLASS *Obj[OBJECTS];
	int Groups[OBJECTS];
	int Size;
public:
	OBJECT_CLASS *operator [](int Ind)
	{
		return Obj[Ind];
	};

	int operator [](const OBJECT_CLASS * pObj) const
	{
		for(int i = 0; i < OBJECTS; ++i)
			if(Obj[i] == pObj)
				return i;
		return -1;
	};

	int GetSize(void) const
	{
		return Size;
	};

	int Add(OBJECT_CLASS * pObj, int Gr = OG_GR_EMPTY)
	{
		int Ind = -1;
		for(int i = 0; i < Size; ++i)
			if(Obj[i] == pObj)
			{
				Ind = i;
				break;
			}
		if(Ind < 0)
		{
			for(int i = 0; i < Size; ++i)
				if(Obj[i] == NULL)
				{
					Ind = i;
					break;
				}
		}
		if(Ind < 0)
		{
			Ind = Size;
			++Size;
		}
		Obj[Ind] = pObj;
		Groups[Ind] = Gr;
		return Ind;
	}; 
	int Delete(OBJECT_CLASS * pObj)
	{
		int Ind = operator[] (pObj);
		if(Ind < 0)
			return Ind;
		return Delete(Ind);
	};
	int Delete(int Ind)
	{
		Obj[Ind] = NULL;
		Groups[Ind] = OG_GR_EMPTY;
		if(Ind == Size - 1)
			--Size;
		return Ind;
	};
	int SetGroup(OBJECT_CLASS * pObj, PObjectGroup Gr)
	{
		int Ind = operator[] (pObj);
		if(Ind < 0)
			return Ind;
		return SetGroup(Ind, Gr);
	}; 
	int SetGroup(int Ind, PObjectGroup Gr)
	{
		Groups[Ind] = Gr;
		return Ind;
	}; 
	int GetGroup(const OBJECT_CLASS * pObj) const
	{
		int Ind = operator[] (pObj);
		if(Ind < 0)
			return OG_GR_EMPTY;
		return Groups[Ind];
	}; 
	int GetGroup(int Ind)
	{
		return Groups[Ind];
	}; 

};
template <unsigned int ACTIVE_OBJECTS, unsigned int PASSIVE_OBJECTS, unsigned int RELATIONS>
class NObjectManager : public CObject
{
	friend class NOMScaner;
public:
	NObjectManager() :
	  PassiveAddingBlocked(false),
	  ActiveAddingBlocked(false)
	{
	};
	virtual ~NObjectManager()
	{
	};
protected:
	BRelation<ACTIVE_OBJECTS, PASSIVE_OBJECTS> Relations[RELATIONS];
	NObjects<NActiveObject, ACTIVE_OBJECTS> ActiveObjects[RELATIONS];
	NObjects<NPassiveObject, PASSIVE_OBJECTS> PassiveObjects;
	bool PassiveAddingBlocked;
	bool ActiveAddingBlocked;
public:
	void BlockPassiveAdding(void) { PassiveAddingBlocked = true;}
	void BlockActiveAdding(void) { ActiveAddingBlocked = true;}
	void RemoveBlocking(void) { PassiveAddingBlocked = false; ActiveAddingBlocked = false;}
	int AddActiveObject(NActiveObject * pObj, int RelationNum, int Gr = OG_GR_EMPTY)
	{
		int Ind = ActiveObjects[RelationNum].Add(pObj, Gr);
		if(Ind < 0)
			return Ind;
		//for(int k = 0; k < RELATIONS; ++k)
		BRelation<ACTIVE_OBJECTS, PASSIVE_OBJECTS> &Rel = Relations[RelationNum];
		for(int i = 0; i < PassiveObjects.GetSize(); ++i)
			Rel.SetAt(Ind, i, ST_EMPTY);
		for(int i = 0; i < PassiveObjects.GetSize(); ++i)
		{
			if(PassiveObjects.GetGroup(i) & ActiveObjects[RelationNum].GetGroup(Ind))
			{
				Rel.SetAt(Ind, i, ST_EMPTY);
				Rel.SetAt(Ind, i, ActiveObjects[RelationNum][Ind]->OnStatusChanged(i, ST_EMPTY));
			}
		}
		return Ind;
	};
	int ChangedPassiveObject(NPassiveObject * pObj)
	{
		int Ind = PassiveObjects[pObj];
		if(Ind < 0)
			return Ind;
		for(int k = 0; k < RELATIONS; ++k)
		{
			BRelation<ACTIVE_OBJECTS, PASSIVE_OBJECTS> &Rel = Relations[k];
			for(int i = 0; i < ActiveObjects[k].GetSize(); ++i)
			{
				if(Rel.GetAt(i, Ind) != ST_EMPTY)
				{
					Rel.SetAt(i, Ind, CHANGED);
					Rel.SetAt(i, Ind, ActiveObjects[k][i]->OnStatusChanged(Ind, CHANGED));
				}
			}
		}
		return Ind;
	};
	int AddPassiveObject(NPassiveObject * pObj, PObjectGroup Gr = EMPTY)
	{
		int Ind = PassiveObjects.Add(pObj, Gr);
		if(Ind < 0)
			return Ind;
		for(int k = 0; k < RELATIONS; ++k)
		{
			BRelation<ACTIVE_OBJECTS, PASSIVE_OBJECTS> &Rel = Relations[k];
			for(int i = 0; i < ActiveObjects[k].GetSize(); ++i)
			{
				if(PassiveObjects.GetGroup(Ind) & ActiveObjects[k].GetGroup(i))
				{
					Rel.SetAt(i, Ind, NEW);
					Rel.SetAt(i, Ind, ActiveObjects[k][i]->OnStatusChanged(Ind, NEW));
				}
			}
		}
		return Ind;
	};

	int GetPassiveGroup(const NObject * pObj) const
	{
		return PassiveObjects.GetGroup(static_cast<const NPassiveObject *>(pObj));
	}

	//PObjectGroup GetActiveGroup(NObject * pObj)
	//{
	//	return ActiveObjects.GetGroup(pObj);
	//}

	int DeletePassiveObject(NPassiveObject * pObj)
	{
		int Ind = PassiveObjects[pObj];
		if(Ind < 0)
			return Ind;
		for(int k = 0; k < RELATIONS; ++k)
		{
			BRelation<ACTIVE_OBJECTS, PASSIVE_OBJECTS> &Rel = Relations[k];
			for(int i = 0; i < ActiveObjects[k].GetSize(); ++i)
			{
				if(Rel.GetAt(i, Ind) != ST_EMPTY)
				{
					Rel.SetAt(i, Ind, DELETED);
					Rel.SetAt(i, Ind, ActiveObjects[k][i]->OnStatusChanged(Ind, DELETED));
				}
			}
		}
		PassiveObjects.Delete(Ind);// place here becouse pObj shold be valid in OnStatusChanged
		return Ind;
	};
	void DeleteActiveObject(NActiveObject * pObj)
	{
		for(int k = 0; k < RELATIONS; ++k)
			ActiveObjects[k].Delete(pObj);
	};
	int SetGroup(NPassiveObject * pObj, PObjectGroup Gr)
	{
		if(PassiveAddingBlocked)
			return -1;
		return AddPassiveObject(pObj, Gr);
	};
 
	int SetGroup(int Ind, PObjectGroup Gr)
	{
		if(PassiveAddingBlocked)
			return -1;
		return PassiveObjects.SetGroup(Ind, Gr);
	}; 

	int SetGroupMask(NActiveObject * pObj, int RelationNum, PObjectGroup Gr)
	{
		return ActiveObjects[RelationNum].SetGroup(pObj, Gr);
	}; 
	int SetGroupMask(int Ind, int RelationNum, PObjectGroup Gr)
	{
		return ActiveObjects[RelationNum].SetGroup(Ind, Gr);
	}; 
	void MakeScaner(NOMScaner *pScaner, NActiveObject *pActiveObject)
	{
		int RelationInd, Ind;
		if(!FindActiveObject(pActiveObject, &RelationInd, &Ind))
			return;
		pScaner->SetRelationInd(RelationInd);
		pScaner->SetActiveInd(Ind);
		MakeNextScaner(pScaner);
	}
protected:
	void MakeNextScaner(NOMScaner *pScaner)
	{
		int RelationInd = pScaner->GetRelationInd();
		int ActiveInd = pScaner->GetActiveInd();
		int PassiveInd = pScaner->GetPassiveInd();
		PObjectStatus Status = ST_EMPTY;
		int j; 
		for( j = PassiveInd + 1; j < PassiveObjects.GetSize(); ++j)
		{
			Status = PObjectStatus(Relations[RelationInd].GetAt(ActiveInd, j));
			if(Status != ST_EMPTY)
				break;
		}
		pScaner->SetStatus(Status);
		if(Status != ST_EMPTY)
		{
			pScaner->SetPassiveInd(j);
			pScaner->SetPassiveObject(PassiveObjects[j]);
			pScaner->SetPassiveObjectGroup(PObjectGroup(PassiveObjects.GetGroup(j)));
		}
	}
	bool FindActiveObject(NActiveObject *pActiveObject, int *Relation, int *Ind)
	{
		*Ind = -1;
		for(*Relation = 0; *Relation < RELATIONS; ++(*Relation))
		{
			*Ind = ActiveObjects[*Relation][pActiveObject];
			if(*Ind >= 0)
				return true;
		}
		return false;
	}

};


