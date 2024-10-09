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

#include "DT_Box.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MT_Scalar DT_Box::supportH(const MT_Vector3& v) const
{
    return v.absolute().dot(m_extent);
}

MT_Point3 DT_Box::support(const MT_Vector3& v) const 
{
    return MT_Point3(v[0] < MT_Scalar(0.0) ? -m_extent[0] : m_extent[0],
                     v[1] < MT_Scalar(0.0) ? -m_extent[1] : m_extent[1],
                     v[2] < MT_Scalar(0.0) ? -m_extent[2] : m_extent[2]); 
    
}


bool DT_Box::ray_cast(const MT_Point3& source, const MT_Point3& target,
					  MT_Scalar& param, MT_Vector3& normal) const 
{
	T_Outcode source_bits = outcode(source);
	T_Outcode target_bits = outcode(target);

	if ((source_bits & target_bits) == 0x0)
		// None of the side planes separate the ray from the box.
	{
		MT_Scalar lambda_enter = MT_Scalar(0.0);
		MT_Scalar lambda_exit  = param;
		MT_Vector3 r = target - source;
		T_Outcode normal_bit = 0x0; // Indicates the axis that is returned as normal.
		T_Outcode bit = 0x01;
		int i;
		for (i = 0; i != 3; ++i)
		{
			if (source_bits & bit)
				// Point of intersection is entering
			{
				MT_Scalar lambda = (-source[i] - m_extent[i]) / r[i];
				if (lambda_enter < lambda)
				{
					lambda_enter = lambda;
					normal_bit = bit;
				}
			}
			else if (target_bits & bit) 
				// Point of intersection is exiting
			{
				MT_Scalar lambda = (-source[i] - m_extent[i]) / r[i];
				GEN_set_min(lambda_exit, lambda);
			}
			bit <<=1;
			if (source_bits & bit)
				// Point of intersection is entering
			{
				MT_Scalar lambda =  (-source[i] + m_extent[i]) / r[i];
				if (lambda_enter < lambda)
				{
					lambda_enter = lambda;
					normal_bit = bit;
				}
			}
			else if (target_bits & bit) 
				// Point of intersection is exiting
			{
				MT_Scalar lambda =  (-source[i] + m_extent[i]) / r[i];
				GEN_set_min(lambda_exit, lambda);
			}
			bit <<=1;
		}
		if (lambda_enter <= lambda_exit)
			// The ray intersects the box
		{
			param = lambda_enter;
			normal.setValue(normal_bit == 0x01 ? -MT_Scalar(1.0) : 
							normal_bit == 0x02 ?  MT_Scalar(1.0) : 
							MT_Scalar(0.0),
							normal_bit == 0x04 ? -MT_Scalar(1.0) : 
							normal_bit == 0x08 ?  MT_Scalar(1.0) : 
							MT_Scalar(0.0),
							normal_bit == 0x10 ? -MT_Scalar(1.0) : 
							normal_bit == 0x20 ?  MT_Scalar(1.0) : 
							MT_Scalar(0.0));
			return true;
		}
	}

	return false;
}


