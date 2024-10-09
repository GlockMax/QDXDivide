#pragma once

#include "Collision_API.h"
#include <string>
#include "cadrID.h"
#include "NCollisionScene.h"

const int MAX_COLLISIONS_STORED = 1024;

class COLLISION_API NCollLogElem
{
	friend class NCollisionLog;
public:
	NCollLogElem(void) {};
	NCollLogElem(const ProgTPoint& inStartPTPoint, const ProgTPoint& inPTPoint, CollLogElemType inType) { StartPTPoint = inStartPTPoint; PTPoint = inPTPoint; Type = inType; }
	~NCollLogElem(void){};
	void SetCollNote(const std::string& NoteText) { CollNote = NoteText; }
	const std::string& GetCollNote() const { return CollNote; }
	const ProgTPoint& GetCadrID() const { return PTPoint; }

	ProgTPoint StartPTPoint;
	ProgTPoint PTPoint;
	CollLogElemType Type;
	std::string CollNote;
};
class COLLISION_API NCollisionLog
{
public:
	NCollisionLog(void);
	~NCollisionLog(void);
	void Clear(void) { LogSize = 0;}
	int GetSize(void) const { return LogSize;}
	const NCollLogElem &GetElem(int i) const { return Log[i];}
	bool AddElem(int pos, const NCollLogElem &Elem);
	int FindElem(const cadrID &CadrID) const;

protected:
	NCollLogElem Log[MAX_COLLISIONS_STORED];
	int LogSize;

};
