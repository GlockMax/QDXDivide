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

#ifndef TRIEDGE_H
#define TRIEDGE_H

#include <vector>
#include <iostream>

#include <MT_Vector3.h>

using namespace MT;

//#define DEBUG

class Triangle;

typedef unsigned short Index_t;

class Edge;
class TriangleStore;


bool link(const Edge& edge0, const Edge& edge1);
void half_link(const Edge& edge0, const Edge& edge1);


class Edge {
private:    
    Triangle *m_triangle;
    int       m_index;

public:
    Edge() {}
    Edge(Triangle *triangle, int index) 
      : m_triangle(triangle), 
        m_index(index) 
	{}

    bool silhouette(const MT_Vector3 *verts, Index_t index, TriangleStore& triangleStore) const;

    Triangle *triangle() const { return m_triangle; }
    int       index()    const { return m_index; }

	Index_t  getSource() const;
	Index_t  getTarget() const;
};



class Triangle {
private:
    Index_t     m_indices[3];
    Edge        m_adjEdges[3];

    bool        m_obsolete;
    	
    MT_Scalar   m_det;
    MT_Scalar   m_lambda1;
    MT_Scalar   m_lambda2;
    MT_Vector3  m_closest;
    MT_Scalar   m_dist2;

public:
    Triangle() {}
    Triangle(Index_t i0, Index_t i1, Index_t i2) 
	  :	m_obsolete(false) 
    {
		m_indices[0] = i0; 
		m_indices[1] = i1; 
		m_indices[2] = i2;
    }
	
    Index_t operator[](int i) const { return m_indices[i]; } 

	const Edge& getAdjEdge(int i) const { return m_adjEdges[i]; }

    void setObsolete(bool obsolete) 
	{ 
#ifdef DEBUG
		std::cout << "Triangle " <<  m_indices[0] << ' ' << m_indices[1] << ' ' << m_indices[2] << " obsolete" << std::endl;
#endif	
		m_obsolete = obsolete; 
	}
    
	bool isObsolete() const { return m_obsolete; }
    

    bool computeClosest(const MT_Vector3 *verts);
    
    const MT_Vector3& getClosest() const { return m_closest; } 
    
    bool isClosestInternal() const
	{ 
		return m_lambda1 >= MT_Scalar(0.0) && 
			   m_lambda2 >= MT_Scalar(0.0) && 
			   m_lambda1 + m_lambda2 <= m_det;
    } 

	bool isVisibleFrom(const MT_Vector3 *verts, Index_t index) const
	{
		MT_Vector3 lever = verts[index] - m_closest;
		return dot(m_closest, lever) > MT_Scalar(0.0);
	}

    MT_Scalar getDist2() const { return m_dist2; }
	
    MT_Vector3 getClosestPoint(const MT_Vector3 *points) const 
	{
		const MT_Vector3& p0 = points[m_indices[0]];
		
		return p0 + (m_lambda1 * (points[m_indices[1]] - p0) + 
					 m_lambda2 * (points[m_indices[2]] - p0)) / m_det;
    }
    
    bool silhouette(const MT_Vector3 *verts, Index_t index, TriangleStore& triangleStore); 
	
	friend bool link(const Edge& edge0, const Edge& edge1);
	friend void half_link(const Edge& edge0, const Edge& edge1);
};
  
	
static const int MaxTriangles = 200;

class TriangleStore
{
private:


	Triangle m_triangles[MaxTriangles];
	int      m_free;
public:
	TriangleStore()
	  : m_free(0)
	{}

	void clear() { m_free = 0; }

	int getFree() const { return m_free; }

	Triangle& operator[](int i) { return m_triangles[i]; }
	Triangle& last() { return m_triangles[m_free - 1]; }

	void setFree(int backup) { m_free = backup; }


	Triangle *newTriangle(const MT_Vector3 *verts, Index_t i0, Index_t i1, Index_t i2) 
	{ 
		Triangle *newTriangle = 0;
		if (m_free != MaxTriangles)
		{
			newTriangle = &m_triangles[m_free++];
			new (newTriangle) Triangle(i0, i1, i2);
			if (!newTriangle->computeClosest(verts))
			{
				--m_free;
				newTriangle = 0;
			}
		}

		return newTriangle;
	}
};

extern TriangleStore g_triangleStore;


inline int circ_next(int i) { return (i + 1) % 3; } 
inline int circ_prev(int i) { return (i + 2) % 3; } 

inline Index_t Edge::getSource() const 
{ 
	return (*m_triangle)[m_index]; 
}

inline Index_t Edge::getTarget() const
{
	return (*m_triangle)[circ_next(m_index)]; 
}

#endif
