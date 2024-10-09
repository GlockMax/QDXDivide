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

//#define BACKFACE_CULLING

#include "COTriangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MT_BBox COTriangle::bbox() const
{
	return MT_BBox((*this)[0]).hull((*this)[1]).hull((*this)[2]);
}

MT_Scalar COTriangle::supportH(const MT_Vector3& v) const
{
    return GEN_max(GEN_max(v.dot((*this)[0]), v.dot((*this)[1])), v.dot((*this)[2]));
}

MT_Point3 COTriangle::support(const MT_Vector3& v) const
{
    MT_Vector3 dots(v.dot((*this)[0]), v.dot((*this)[1]), v.dot((*this)[2]));

	return (*this)[dots.maxAxis()];
}

bool COTriangle::ray_cast(const MT_Point3& source, const MT_Point3& target, 
						   MT_Scalar& param, MT_Vector3& normal) const 
{
	MT_Vector3 d1 = (*this)[1] - (*this)[0];
	MT_Vector3 d2 = (*this)[2] - (*this)[0];
	MT_Vector3 n = d1.cross(d2);
	MT_Vector3 r = target - source;
	MT_Scalar delta = -r.dot(n);

	MT_Scalar rounding_error = GEN_max(GEN_max(MT_abs(n[0]), MT_abs(n[1])), MT_abs(n[2])) * MT_EPSILON; 

#ifdef BACKFACE_CULLING	
   if (delta > rounding_error)
#else
	if (MT_abs(delta) > rounding_error)
#endif      
		// The ray is not parallel to the triangle's plane. 
		// (Coplanar rays are ignored.)
	{
		MT_Vector3 b = source - (*this)[0];
		MT_Scalar lambda = b.dot(n) / delta;

		if (MT_Scalar(0.0) <= lambda && lambda <= param)
			// The ray intersects the triangle's plane.
		{
			MT_Vector3 u = b.cross(r);
			MT_Scalar mu1 = d2.dot(u) / delta;

			if (MT_Scalar(0.0) <= mu1 && mu1 <= MT_Scalar(1.0)) 
			{
				MT_Scalar mu2 = -d1.dot(u) / delta;

				if (MT_Scalar(0.0) <= mu2 && mu1 + mu2 <= MT_Scalar(1.0)) 
					// The ray intersects the triangle.
				{
					param = lambda;
					// Return a normal that points at the source.
#ifdef BACKFACE_CULLING
               normal = n;
#else
					normal = delta > MT_Scalar(0.0) ? n : -n;
#endif
					return true;
				}
			}
		}
	}

	return false;
}


