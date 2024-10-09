/*
  SOLID - Software Library for Interference Detection
  Copyright (C) 1997-1998  Gino van den Bergen

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

  Please send remarks, questions and bug reports to gino@win.tue.nl,
  or write to:
                  Gino van den Bergen
		  Department of Mathematics and Computing Science
		  Eindhoven University of Technology
		  P.O. Box 513, 5600 MB Eindhoven, The Netherlands
*/

/* CHANGED BY ALEX for use in NCManager */

#ifndef DT_SCENE_H
#define DT_SCENE_H

#include <vector>

#include "SOLID_broad.h"
#include "DT_Encounter.h"

class DT_Object;
class DT_RespTable;

class DT_Scene {
	enum { TESTING = 0x4 };
public:
    DT_Scene();
    ~DT_Scene();

    void addObject(DT_Object& object);
    void removeObject(DT_Object& object);

    void addEncounter(const DT_Encounter& e)
    {
		assert((m_state & TESTING) == 0x0);

		m_encounterTable.insert(e); 
    }
	
    void removeEncounter(const DT_Encounter& e)
    {
		assert((m_state & TESTING) == 0x0);

		DT_EncounterTable::iterator it = m_encounterTable.find(e);
		assert(it != m_encounterTable.end());
		m_encounterTable.erase(it);
    }



    int  handleCollisions(const DT_RespTable *respTable);

	void *rayCast(const void *ignore_client, 
				  const DT_Vector3 source, const DT_Vector3 target, 
				  DT_Scalar& lambda, DT_Vector3 normal) const;

private:
	typedef std::vector<std::pair<DT_Object *, BP_ProxyHandle> > T_ObjectList;

	BP_SceneHandle      m_broadphase;
	T_ObjectList        m_objectList;
    DT_EncounterTable   m_encounterTable;
	unsigned int        m_state;
};

#endif
