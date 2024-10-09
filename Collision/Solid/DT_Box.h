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

#ifndef DT_BOX_H
#define DT_BOX_H

#include "COConv.h"

class DT_Box : public COConv {
public:
    DT_Box(MT_Scalar x, MT_Scalar y, MT_Scalar z) : 
        m_extent(x, y, z) 
	{}

    DT_Box(const MT_Vector3& e) : 
		m_extent(e) 
	{}

    virtual MT_Scalar supportH(const MT_Vector3& v) const;
    virtual MT_Point3 support(const MT_Vector3& v) const;
	virtual bool ray_cast(const MT_Point3& source, const MT_Point3& target,
						  MT_Scalar& param, MT_Vector3& normal) const;
    
    const MT_Vector3& getExtent() const { return m_extent; }
	
protected:
	typedef unsigned int T_Outcode;
	
	T_Outcode outcode(const MT_Point3& p) const
	{
		return (p[0] < -m_extent[0] ? 0x01 : 0x0) |    
			   (p[0] >  m_extent[0] ? 0x02 : 0x0) |
			   (p[1] < -m_extent[1] ? 0x04 : 0x0) |    
			   (p[1] >  m_extent[1] ? 0x08 : 0x0) |
			   (p[2] < -m_extent[2] ? 0x10 : 0x0) |    
			   (p[2] >  m_extent[2] ? 0x20 : 0x0);
	}
    
    MT_Vector3 m_extent;
};

#endif
