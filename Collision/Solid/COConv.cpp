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

#include "COConv.h"
#include "GEN_MinMax.h"

//#define DEBUG
#define SAFE_EXIT

#include "DT_GJK.h"
#include "DT_PenDepth.h"

#include <algorithm>
#include <new>

#include "MT_BBox.h"
#include "COSph.h"
#include "COMinkSum.h"

#include "DT_Accuracy.h"

#ifdef STATISTICS
int num_iterations = 0;
int num_irregularities = 0;
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

MT_BBox COConv::bbox() const
{
	MT_Point3 min(-supportH(MT_Vector3(-1.0f, 0.0f, 0.0f)),
                  -supportH(MT_Vector3(0.0f, -1.0f, 0.0f)),
				  -supportH(MT_Vector3(0.0f, 0.0f, -1.0f)));
	MT_Point3 max( supportH(MT_Vector3(1.0f, 0.0f, 0.0f)),
                   supportH(MT_Vector3(0.0f, 1.0f, 0.0f)),
				   supportH(MT_Vector3(0.0f, 0.0f, 1.0f)));

	
    return MT_BBox(min, max);
}

MT_BBox COConv::bbox(const MT_Matrix3x3& basis) const 
{
    MT_Point3 min(-supportH(-basis[0]),
                  -supportH(-basis[1]),
		          -supportH(-basis[2])); 
    MT_Point3 max( supportH( basis[0]),
                   supportH( basis[1]),
                   supportH( basis[2])); 
    return MT_BBox(min, max);
}

MT_BBox COConv::bbox(const MT_Transform& t, MT_Scalar margin) const 
{
    MT_Point3 min(t.getOrigin()[0] - supportH(-t.getBasis()[0]) - margin,
                  t.getOrigin()[1] - supportH(-t.getBasis()[1]) - margin,
		          t.getOrigin()[2] - supportH(-t.getBasis()[2]) - margin); 
    MT_Point3 max(t.getOrigin()[0] + supportH( t.getBasis()[0]) + margin,
                  t.getOrigin()[1] + supportH( t.getBasis()[1]) + margin,
                  t.getOrigin()[2] + supportH( t.getBasis()[2]) + margin); 
    return MT_BBox(min, max);
}

bool COConv::ray_cast(const MT_Point3& source, const MT_Point3& target, MT_Scalar& lambda, MT_Vector3& normal) const
{
	// Still working on this one...
    return false;
}

bool intersect(const COConv& a, const COConv& b, MT_Vector3& v)
{
	DT_GJK gjk;

#ifdef STATISTICS
    num_iterations = 0;
#endif
	MT_Scalar dist2 = MT_INFINITY;

	do
	{
		MT_Point3  p = a.support(-v);	
		MT_Point3  q = b.support(v);
		MT_Vector3 w = p - q; 
		
        if (v.dot(w) > MT_Scalar(0.0)) 
		{
			return false;
		}
 
		gjk.addVertex(w); 
        if (gjk.isAffinelyDependent())
        {
#ifdef STATISTICS
            ++num_irregularities;
#endif
            return false;
        }


#ifdef STATISTICS
        ++num_iterations;
#endif
        if (!gjk.closest(v)) 
		{
#ifdef STATISTICS
            ++num_irregularities;
#endif
            return false;
        }

#ifdef SAFE_EXIT
		MT_Scalar prev_dist2 = dist2;
#endif

		dist2 = v.length2();

#ifdef SAFE_EXIT
		if (prev_dist2 - dist2 <= MT_EPSILON * prev_dist2) 
		{
 			return false;
		}
#endif
    } 
    while (!gjk.fullSimplex() && dist2 > DT_Accuracy::tol_error * gjk.maxVertex()); 

    v.setValue(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));

    return true;
}




bool common_point(const COConv& a, const COConv& b,
                  MT_Vector3& v, MT_Point3& pa, MT_Point3& pb)
{
	DT_GJK gjk;

#ifdef STATISTICS
    num_iterations = 0;
#endif

	MT_Scalar dist2 = MT_INFINITY;

    do
	{
		MT_Point3  p = a.support(-v);	
		MT_Point3  q = b.support(v);
		MT_Vector3 w = p - q; 
		
        if (v.dot(w) > MT_Scalar(0.0)) 
		{
			return false;
		}
 
		gjk.addVertex(w, p, q); 
        if (gjk.isAffinelyDependent())
        {
#ifdef STATISTICS
            ++num_irregularities;
#endif 
            return false;
        }


#ifdef STATISTICS
        ++num_iterations;
#endif
        if (!gjk.closest(v)) 
		{
#ifdef STATISTICS
            ++num_irregularities;
#endif
            return false;
        }		
		
#ifdef SAFE_EXIT
		MT_Scalar prev_dist2 = dist2;
#endif

		dist2 = v.length2();

#ifdef SAFE_EXIT
		if (prev_dist2 - dist2 <= MT_EPSILON * prev_dist2) 
		{
            return false;
		}
#endif
    }
    while (!gjk.fullSimplex() && dist2 > DT_Accuracy::tol_error * gjk.maxVertex()); 
    
	gjk.compute_points(pa, pb);

    v.setValue(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));

    return true;
}






	
bool penetration_depth(const COConv& a, const COConv& b,
                       MT_Vector3& v, MT_Point3& pa, MT_Point3& pb)
{
	DT_GJK gjk;

#ifdef STATISTICS
    num_iterations = 0;
#endif

	MT_Scalar dist2 = MT_INFINITY;

    do
	{
		MT_Point3  p = a.support(-v);	
		MT_Point3  q = b.support(v);
		MT_Vector3 w = p - q; 
		
        if (v.dot(w) > MT_Scalar(0.0)) 
		{
			return false;
		}
 
		gjk.addVertex(w, p, q); 
        
        if (gjk.isAffinelyDependent())
        {
#ifdef STATISTICS
            ++num_irregularities;
#endif
 
            return false;
        }


#ifdef STATISTICS
        ++num_iterations;
#endif
        if (!gjk.closest(v)) 
		{
#ifdef STATISTICS
            ++num_irregularities;
#endif
            return false;
        }		
		
#ifdef SAFE_EXIT
		MT_Scalar prev_dist2 = dist2;
#endif

		dist2 = v.length2();

#ifdef SAFE_EXIT
		if (prev_dist2 - dist2 <= MT_EPSILON * prev_dist2) 
		{
 			return false;
		}
#endif
    }
    while (!gjk.fullSimplex() && dist2 > DT_Accuracy::tol_error * gjk.maxVertex()); 
    

	return penDepth(gjk, a, b, v, pa, pb);

}

bool hybrid_penetration_depth(const COConv& a, MT_Scalar a_margin, 
							  const COConv& b, MT_Scalar b_margin,
                              MT_Vector3& v, MT_Point3& pa, MT_Point3& pb)
{
	MT_Scalar margin = a_margin + b_margin;
	if (margin > MT_Scalar(0.0))
	{
		MT_Scalar margin2 = margin * margin;

		DT_GJK gjk;

#ifdef STATISTICS
		num_iterations = 0;
#endif
		MT_Scalar dist2 = MT_INFINITY;

		do
		{
			MT_Point3  p = a.support(-v);	
			MT_Point3  q = b.support(v);
			
			MT_Vector3 w = p - q; 
			
			MT_Scalar delta = v.dot(w);
			
			if (delta > MT_Scalar(0.0) && delta * delta > dist2 * margin2) 
			{
				return false;
			}
			
			if (gjk.inSimplex(w) || dist2 - delta <= dist2 * DT_Accuracy::rel_error2)
			{
				gjk.compute_points(pa, pb);
				MT_Scalar s = MT_sqrt(dist2);
				assert(s > MT_Scalar(0.0));
				pa -= v * (a_margin / s);
				pb += v * (b_margin / s);
				return true;
			}
			
			gjk.addVertex(w, p, q); 

            if (gjk.isAffinelyDependent())
            {
#ifdef STATISTICS
                ++num_irregularities;
#endif
                gjk.compute_points(pa, pb);
				MT_Scalar s = MT_sqrt(dist2);
				assert(s > MT_Scalar(0.0));
				pa -= v * (a_margin / s);
				pb += v * (b_margin / s);
				return true;
            }

			
#ifdef STATISTICS
			++num_iterations;
#endif
			if (!gjk.closest(v)) 
			{
#ifdef STATISTICS
				++num_irregularities;
#endif
				gjk.compute_points(pa, pb);
				MT_Scalar s = MT_sqrt(dist2);
				assert(s > MT_Scalar(0.0));
				pa -= v * (a_margin / s);
				pb += v * (b_margin / s);
				return true;
			}
			
#ifdef SAFE_EXIT
			MT_Scalar prev_dist2 = dist2;
#endif
			
			dist2 = v.length2();
			
#ifdef SAFE_EXIT
			if (prev_dist2 - dist2 <= MT_EPSILON * prev_dist2) 
			{
  				gjk.backup_closest(v);
				dist2 = v.length2();
				gjk.compute_points(pa, pb);
				MT_Scalar s = MT_sqrt(dist2);
				assert(s > MT_Scalar(0.0));
				pa -= v * (a_margin / s);
				pb += v * (b_margin / s);
				return true;
			}
#endif
		}
		while (!gjk.fullSimplex() && dist2 > DT_Accuracy::tol_error * gjk.maxVertex()); 
		
	}
	// Second GJK phase. compute points on the boundary of the offset object
	
	return penetration_depth((a_margin > MT_Scalar(0.0) ? 
							  static_cast<const COConv&>(COMinkSum(a, COSph(a_margin))) : 
							  static_cast<const COConv&>(a)), 
							 (b_margin > MT_Scalar(0.0) ? 
							  static_cast<const COConv&>(COMinkSum(b, COSph(b_margin))) : 
							  static_cast<const COConv&>(b)), v, pa, pb);
}


MT_Scalar closest_points(const COConv& a, const COConv& b, MT_Scalar max_dist2,
                         MT_Point3& pa, MT_Point3& pb) 
{
	MT_Vector3 v(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));
	
    DT_GJK gjk;

#ifdef STATISTICS
    num_iterations = 0;
#endif

	MT_Scalar dist2 = MT_INFINITY;

    do
	{
		MT_Point3  p = a.support(-v);	
		MT_Point3  q = b.support(v);
		MT_Vector3 w = p - q; 

		MT_Scalar delta = v.dot(w);
		if (delta > MT_Scalar(0.0) && delta * delta > dist2 * max_dist2) 
		{
			return MT_INFINITY;
		}

		if (gjk.inSimplex(w) || dist2 - delta <= dist2 * DT_Accuracy::rel_error2) 
		{
            break;
		}

		gjk.addVertex(w, p, q);
        if (gjk.isAffinelyDependent())
        {
#ifdef STATISTICS
            ++num_irregularities;
#endif
            break;
        }

#ifdef STATISTICS
        ++num_iterations;
        if (num_iterations > 1000) 
		{
			std::cout << "v: " << v << " w: " << w << std::endl;
		}
#endif
        if (!gjk.closest(v)) 
		{
#ifdef STATISTICS
            ++num_irregularities;
#endif
            break;
        }

#ifdef SAFE_EXIT
		MT_Scalar prev_dist2 = dist2;
#endif

		dist2 = v.length2();

#ifdef SAFE_EXIT
		if (prev_dist2 - dist2 <= MT_EPSILON * prev_dist2) 
		{
            gjk.backup_closest(v);
            dist2 = v.length2();
			break;
		}
#endif
    }
    while (!gjk.fullSimplex() && dist2 > DT_Accuracy::tol_error * gjk.maxVertex()); 

	assert(!gjk.emptySimplex());
	
	if (dist2 <= max_dist2)
	{
		gjk.compute_points(pa, pb);
	}
	
	return dist2;
}
