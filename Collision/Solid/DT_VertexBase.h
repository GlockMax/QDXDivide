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

#ifndef DT_VERTEXBASE_H
#define DT_VERTEXBASE_H

#include "MT_Point3.h"

#include <vector>

class COConcave;

typedef std::vector<COConcave *>DT_ComplexList;

class DT_VertexBase {
public:
    explicit DT_VertexBase(const void *base = 0, DT_Size stride = 0, bool owner = false) : 
        m_base((char *)base),
		m_stride(stride ? stride : 3 * sizeof(DT_Scalar)),
		m_owner(owner)
	{}
	
	~DT_VertexBase()
	{
		if (m_owner)
		{
			delete [] m_base;
		}
	}
    
    MT_Point3 operator[](DT_Index i) const 
	{ 
        return MT_Point3(reinterpret_cast<DT_Scalar *>(m_base + i * m_stride));
    }
    
    void setPointer(const void *base, bool owner = false)
	{
		m_base = (char *)base; 
		m_owner = owner;
	} 
	
    const void *getPointer() const { return m_base; }	
	bool        isOwner() const { return m_owner; }
    
	void addComplex(COConcave *complex) const { m_complexList.push_back(complex); }
	void removeComplex(COConcave *complex) const
	{
		DT_ComplexList::iterator it = std::find(m_complexList.begin(), m_complexList.end(), complex); 
		if (it != m_complexList.end())
		{
			m_complexList.erase(it);
		}
	}
	
	const DT_ComplexList& getComplexList() const { return m_complexList; }
	
private:    
    char                  *m_base;
    DT_Size                m_stride;
	bool                   m_owner;
	mutable DT_ComplexList m_complexList;
};

#endif
