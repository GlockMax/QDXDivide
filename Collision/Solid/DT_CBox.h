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

#ifndef DT_CBOX_H
#define DT_CBOX_H

#include "MT_BBox.h"

struct DT_CBox {
    DT_CBox() {}
    DT_CBox(const MT_Point3& center, const MT_Vector3& extent) 
      : m_center(center),
        m_extent(extent)
    {}

    explicit DT_CBox(const MT_BBox& bbox) { set(bbox); }

    const MT_Point3& getCenter() const { return m_center; }
    const MT_Vector3& getExtent() const { return m_extent; }

    void set(const MT_BBox& bbox)
    {
        m_center = bbox.getCenter();
        m_extent = bbox.getExtent();
    }
 
    MT_BBox get() const
    {
        return MT_BBox(m_center - m_extent, m_center + m_extent);
    }

    MT_Scalar size() const  
    {
        return GEN_max(GEN_max(m_extent[0], m_extent[1]), m_extent[2]);
    }


    DT_CBox& operator+=(const DT_CBox& box)
    {
        m_center += box.getCenter();
        m_extent += box.getExtent();
        return *this;
    }
    
    int longestAxis() const { return m_extent.closestAxis(); }
        
    DT_CBox hull(const DT_CBox& b) const 
    {
        return DT_CBox(this->get().hull(b.get()));
    }

    bool overlaps(const DT_CBox& b) const 
    {
        return MT_abs(m_center[0] - b.m_center[0]) <= m_extent[0] + b.m_extent[0] &&
               MT_abs(m_center[1] - b.m_center[1]) <= m_extent[1] + b.m_extent[1] &&
               MT_abs(m_center[2] - b.m_center[2]) <= m_extent[2] + b.m_extent[2];
    }
    
    bool overlapsLineSegment(const MT_Point3& p, const MT_Point3& q) const 
    {
        MT_Vector3 r = q - p;   
        MT_Vector3 r_abs = r.absolute();
        
        if (!overlaps(DT_CBox(p + r * MT_Scalar(0.5), r_abs * MT_Scalar(0.5))))
        {
            return false;
        }
        
        MT_Vector3 s = p - m_center;

        if (MT_abs(r[2] * s[1] - r[1] * s[2]) > r_abs[2] * m_extent[1] + r_abs[1] * m_extent[2])
        {
            return false;
        }
                    
        if (MT_abs(r[0] * s[2] - r[2] * s[0]) > r_abs[0] * m_extent[2] + r_abs[2] * m_extent[0])
        {
            return false;
        }
                    
        if (MT_abs(r[1] * s[0] - r[0] * s[1]) > r_abs[1] * m_extent[0] + r_abs[0] * m_extent[1])
        {
            return false;
        }
            
        return true;
    }
    
    MT_Point3 support(const MT_Vector3& v) const 
    {
        return m_center + MT_Vector3(v[0] < MT_Scalar(0.0) ? -m_extent[0] : m_extent[0],
                                     v[1] < MT_Scalar(0.0) ? -m_extent[1] : m_extent[1],
                                     v[2] < MT_Scalar(0.0) ? -m_extent[2] : m_extent[2]); 
    
    }

private:
    MT_Point3  m_center;
    MT_Vector3 m_extent;
};

inline DT_CBox operator+(const DT_CBox& b1, const DT_CBox& b2) 
{
    return DT_CBox(b1.getCenter() + b2.getCenter(), 
                   b1.getExtent() + b2.getExtent());
}

inline DT_CBox operator-(const DT_CBox& b1, const DT_CBox& b2) 
{
    return DT_CBox(b1.getCenter() - b2.getCenter(), 
                   b1.getExtent() + b2.getExtent());
}

#endif
