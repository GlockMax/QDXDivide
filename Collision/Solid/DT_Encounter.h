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

#ifndef DT_ENCOUNTER_H
#define DT_ENCOUNTER_H

#include <set>

#include "MT_Vector3.h"
#include "DT_Object.h"
#include "COShape.h"

class DT_RespTable;

class DT_Encounter {
public:
    DT_Encounter() {}
    DT_Encounter(DT_Object *obj_ptr1, DT_Object *obj_ptr2) 
        : m_sep_axis(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0)) 
    {
		assert(obj_ptr1 != obj_ptr2);
        if (obj_ptr2->getType() < obj_ptr1->getType() || 
            (obj_ptr2->getType() == obj_ptr1->getType() &&
             obj_ptr2 < obj_ptr1))
        { 
            m_obj_ptr1 = obj_ptr2; 
            m_obj_ptr2 = obj_ptr1; 
        }
        else 
        { 
            m_obj_ptr1 = obj_ptr1; 
            m_obj_ptr2 = obj_ptr2; 
        }
    }

    DT_Object         *first()          const { return m_obj_ptr1; }
    DT_Object         *second()         const { return m_obj_ptr2; }
    const MT_Vector3&  separatingAxis() const { return m_sep_axis; }

 	DT_Bool exactTest(const DT_RespTable *respTable, int& count) const;

private:
    DT_Object          *m_obj_ptr1;
    DT_Object          *m_obj_ptr2;
    mutable MT_Vector3  m_sep_axis;
};

inline bool operator<(const DT_Encounter& a, const DT_Encounter& b) 
{ 
    return a.first() < b.first() || 
        (a.first() == b.first() && a.second() < b.second()); 
}



inline std::ostream& operator<<(std::ostream& os, const DT_Encounter& a) {
    return os << '(' << a.first() << ", " << a.second() << ')';
}



typedef std::set<DT_Encounter> DT_EncounterTable;

#endif
