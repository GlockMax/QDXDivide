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

#include <algorithm>

#include "MT_Transform.h"
#include "DT_VertexBase.h"

#include "COShape.h"
#include "DT_CBox.h"
#include "DT_BBoxTree.h"

class COConv;
class DT_Object;

class COConcave : public COShape  {
public:
	COConcave(const DT_VertexBase *base);
	virtual ~COConcave();
	
	void finish(DT_Count n, const COConv *p[]);
    
	virtual DT_ShapeType getType() const { return COMPLEX; }

    virtual MT_BBox bbox(const MT_Transform& t, MT_Scalar margin) const;

	virtual bool ray_cast(const MT_Point3& source, const MT_Point3& target, 
						  MT_Scalar& lambda, MT_Vector3& normal) const; 

	void refit();
	

    friend bool intersect(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
		                  const COConv& b, MT_Vector3& v);
    
    friend bool intersect(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
		                  const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin,
                          MT_Vector3& v);
   
    friend bool common_point(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
		                     const COConv& b, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);
    
    friend bool common_point(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
		                     const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin,
                             MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);
    
    friend bool penetration_depth(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
								  const COConv& b, MT_Scalar b_margin, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);
    
    friend bool penetration_depth(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
								  const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin,
								  MT_Vector3& v, MT_Point3& pa, MT_Point3& pb);

    friend MT_Scalar closest_points(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
		                            const COConv& b, MT_Point3& pa, MT_Point3& pb);
    
    friend MT_Scalar closest_points(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
		                            const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin,
									MT_Point3& pa, MT_Point3& pb);

	void subscribe(DT_Object* object) const
	{
		m_objectList.push_back(object);
	}
	
	void unsubscribe(DT_Object* object) const
	{
		ObjectList::iterator it = std::find(m_objectList.begin(), m_objectList.end(), object);
		assert(it != m_objectList.end());
		m_objectList.erase(it);
	}

	typedef std::vector<DT_Object *> ObjectList;

	mutable ObjectList     m_objectList;
	const DT_VertexBase   *m_base;
	DT_Count               m_count;
	const COConv      **m_leaves;
	DT_BBoxNode           *m_nodes;
	DT_CBox                m_cbox;
	DT_BBoxTree::NodeType  m_type;
};



