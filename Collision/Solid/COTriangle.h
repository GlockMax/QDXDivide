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

#pragma once

#include "SOLID_types.h"

#include "COConv.h"
#include "DT_IndexArray.h"
#include "DT_VertexBase.h"

class COTriangle : public COConv {
public:
    COTriangle(const DT_VertexBase *base, DT_Index i0, DT_Index i1, DT_Index i2) : 
        m_base(base)
	{
		m_index[0] = i0;
		m_index[1] = i1;
		m_index[2] = i2;
	}

    COTriangle(const DT_VertexBase *base, const DT_Index *index) : 
        m_base(base)
	{
		m_index[0] = index[0];
		m_index[1] = index[1];
		m_index[2] = index[2];
	}

	virtual MT_BBox bbox() const;
    virtual MT_Scalar supportH(const MT_Vector3& v) const;
    virtual MT_Point3 support(const MT_Vector3& v) const;
	virtual bool ray_cast(const MT_Point3& source, const MT_Point3& target, MT_Scalar& lambda, MT_Vector3& normal) const;

    MT_Point3 operator[](int i) const { return (*m_base)[m_index[i]]; }

private:
    const DT_VertexBase *m_base;
    DT_Index             m_index[3];
};
