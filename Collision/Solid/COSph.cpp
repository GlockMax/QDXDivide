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

#include "COSph.h"
#include "GEN_MinMax.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MT_Scalar COSph::supportH(const MT_Vector3& v) const
{
	return m_radius * v.length();
}

MT_Point3 COSph::support(const MT_Vector3& v) const 
{
   MT_Scalar s = v.length();
	
	if (s > MT_Scalar(0.0))
	{
		s = m_radius / s;
		return MT_Point3(v[0] * s, v[1] * s, v[2] * s);
	}
	else
	{
		return MT_Point3(m_radius, MT_Scalar(0.0), MT_Scalar(0.0));
	}
}

bool COSph::ray_cast(const MT_Point3& source, const MT_Point3& target,
						 MT_Scalar& param, MT_Vector3& normal) const 
{
	MT_Vector3 r = target - source;
	MT_Scalar  delta = -source.dot(r);  
	MT_Scalar  r_length2 = r.length2();
	MT_Scalar  sigma = delta * delta - r_length2 * (source.length2() - m_radius * m_radius);

	if (sigma >= MT_Scalar(0.0))
		// The line trough source and target intersects the sphere.
	{
		MT_Scalar sqrt_sigma = MT_sqrt(sigma);
		// We need only the sign of lambda2, so the division by the positive 
		// r_length2 can be left out.
		MT_Scalar lambda2 = (delta + sqrt_sigma) /* / r_length2 */ ;
		if (lambda2 >= MT_Scalar(0.0))
			// The ray points at the sphere
		{
			MT_Scalar lambda1 = (delta - sqrt_sigma) / r_length2;
			if (lambda1 <= param)
				// The ray hits the sphere, since 
				// [lambda1, lambda2] overlaps [0, param]. 
			{
				if (lambda1 > MT_Scalar(0.0))
				{
					param = lambda1;
					normal = (source + r * lambda1) / m_radius;
					// NB: division by m_radius to normalize the normal.
				}
				else
				{
					param = MT_Scalar(0.0);
					normal.setValue(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));
				}
						
				return true;
			}
		}
	}

	return false;
}


