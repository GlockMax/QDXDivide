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

#include "COShape.h"

#include "MT_Vector3.h"
#include "MT_Point3.h"

#include "MT_Matrix3x3.h"
#include "MT_Transform.h"

class COConv : public COShape {
public:
    virtual ~COConv() {}
	virtual DT_ShapeType getType() const { return CONVEX; } 
    
	virtual MT_Scalar supportH(const MT_Vector3& v) const {	return v.dot(support(v)); }
    virtual MT_Point3 support(const MT_Vector3& v) const = 0;
	virtual MT_BBox bbox() const;
    virtual MT_BBox bbox(const MT_Matrix3x3& basis) const;
    virtual MT_BBox bbox(const MT_Transform& t, MT_Scalar margin = MT_Scalar(0.0)) const;
	virtual bool ray_cast(const MT_Point3& source, const MT_Point3& target, MT_Scalar& param, MT_Vector3& normal) const;
	
protected:
	COConv() {}
};


bool intersect(const COConv& a, const COConv& b, MT_Vector3& v);

bool common_point(const COConv& a, const COConv& b, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);

MT_Scalar closest_points(const COConv&, const COConv&, MT_Scalar max_dist2, MT_Point3& pa, MT_Point3& pb);

bool penetration_depth(const COConv& a, const COConv& b, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);

bool hybrid_penetration_depth(const COConv& a, MT_Scalar a_margin, 
							  const COConv& b, MT_Scalar b_margin,
                              MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);
