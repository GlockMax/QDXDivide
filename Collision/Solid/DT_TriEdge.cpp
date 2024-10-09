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

#include "DT_TriEdge.h"

TriangleStore g_triangleStore;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

bool link(const Edge& edge0, const Edge& edge1)
{
	bool ok = edge0.getSource() == edge1.getTarget() && edge0.getTarget() == edge1.getSource();
    
    //assert(ok);
    
    if (ok)
    {
	edge0.triangle()->m_adjEdges[edge0.index()] = edge1;
    edge1.triangle()->m_adjEdges[edge1.index()] = edge0;
}

    return ok;
}

void half_link(const Edge& edge0, const Edge& edge1) 
{
	assert(edge0.getSource() == edge1.getTarget() && edge0.getTarget() == edge1.getSource());
    
	edge0.triangle()->m_adjEdges[edge0.index()] = edge1;
}


bool Triangle::computeClosest(const MT_Vector3 *verts)
{
    const MT_Vector3& p0 = verts[m_indices[0]]; 

    MT_Vector3 v1 = verts[m_indices[1]] - p0;
    MT_Vector3 v2 = verts[m_indices[2]] - p0;
    MT_Scalar v1dv1 = v1.length2();
    MT_Scalar v1dv2 = dot(v1, v2);
    MT_Scalar v2dv2 = v2.length2();
    MT_Scalar p0dv1 = dot(p0, v1); 
    MT_Scalar p0dv2 = dot(p0, v2);
    
    m_det = v1dv1 * v2dv2 - v1dv2 * v1dv2; // non-negative
    m_lambda1 = p0dv2 * v1dv2 - p0dv1 * v2dv2;
    m_lambda2 = p0dv1 * v1dv2 - p0dv2 * v1dv1; 
    
    if (m_det > MT_Scalar(0.0)) 
	{
		m_closest = p0 + (m_lambda1 * v1 + m_lambda2 * v2) / m_det;
		m_dist2 = m_closest.length2();

		return true;
    }

    return false;
} 

bool Edge::silhouette(const MT_Vector3 *verts, Index_t index, TriangleStore& triangleStore) const 
{
    if (!m_triangle->isObsolete()) 
    {
		if (!m_triangle->isVisibleFrom(verts, index)) 
        {
			Triangle *triangle = triangleStore.newTriangle(verts, index, getTarget(), getSource());

			if (triangle)
			{
				half_link(Edge(triangle, 1), *this);
				return true;
			}

			return false;
		}	
        else 
        {
            m_triangle->setObsolete(true); // Triangle is visible 
         
			int backup = triangleStore.getFree();

            if (!m_triangle->getAdjEdge(circ_next(m_index)).silhouette(verts, index, triangleStore))
			{
				m_triangle->setObsolete(false);
				
				Triangle *triangle = triangleStore.newTriangle(verts, index, getTarget(), getSource());

				if (triangle)
				{
					half_link(Edge(triangle, 1), *this);
					return true;
				}

				return false;
			}
			else if (!m_triangle->getAdjEdge(circ_prev(m_index)).silhouette(verts, index, triangleStore))
			{
				m_triangle->setObsolete(false);

				triangleStore.setFree(backup);
								
				Triangle *triangle = triangleStore.newTriangle(verts, index, getTarget(), getSource());

				if (triangle)
				{
					half_link(Edge(triangle, 1), *this);
					return true;
				}

				return false;
			}
        }
    }

	return true;
}

bool Triangle::silhouette(const MT_Vector3 *verts, Index_t index, TriangleStore& triangleStore) 
{
	//assert(isVisibleFrom(verts, index));

	int first = triangleStore.getFree();

	setObsolete(true);
	
	bool result = m_adjEdges[0].silhouette(verts, index, triangleStore) &&
	              m_adjEdges[1].silhouette(verts, index, triangleStore) &&
	              m_adjEdges[2].silhouette(verts, index, triangleStore);

	if (result)
	{
		int i, j;
		for (i = first, j = triangleStore.getFree()-1; i != triangleStore.getFree(); j = i++)
		{
			Triangle *triangle = &triangleStore[i];
			half_link(triangle->getAdjEdge(1), Edge(triangle, 1));
            if (!link(Edge(triangle, 0), Edge(&triangleStore[j], 2)))
            {
                return false;
            }
		}
	}

	return result;
}

