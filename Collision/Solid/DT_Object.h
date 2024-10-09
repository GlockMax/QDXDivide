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

#ifndef DT_OBJECT_H
#define DT_OBJECT_H

#include <vector>

#include "SOLID.h"
#include "SOLID_broad.h"

#include "MT_Transform.h"
#include "MT_Quaternion.h"
#include "MT_BBox.h"
#include "COShape.h"
#include "COConcave.h"

class COConv;

class DT_Object {
public:
    DT_Object(void *client_object, const COShape& shape) :
		m_client_object(client_object),
		m_shape(shape), 
		m_margin(MT_Scalar(0.0)),
        m_responseClass(0)
	{
		m_xform.setIdentity();
		if (m_shape.getType() == COMPLEX)
		{
			static_cast<const COConcave&>(m_shape).subscribe(this);
		}
		setBBox();
	}

	~DT_Object()
	{
		if (m_shape.getType() == COMPLEX)
		{
			static_cast<const COConcave&>(m_shape).unsubscribe(this);
		}
	}

	void setMargin(MT_Scalar margin) 
	{ 
		m_margin = margin; 
		setBBox();
	}

	void setScaling(const MT_Vector3& scaling)
	{
        m_xform.scale(scaling);
        setBBox();
    }

    void setPosition(const MT_Point3& pos) 
	{ 
        m_xform.setOrigin(pos);
        setBBox();
    }
    
    void setOrientation(const MT_Quaternion& orn)
	{
		m_xform.setRotation(orn);
		setBBox();
    }

	void setMatrix(const float *m) 
	{
        m_xform.setValue(m);
		assert(m_xform.getBasis().determinant() != MT_Scalar(0.0));
        setBBox();
    }

    void setMatrix(const double *m)
	{
        m_xform.setValue(m);
		assert(m_xform.getBasis().determinant() != MT_Scalar(0.0));
        setBBox();
    }

    void getMatrix(float *m) const
	{
        m_xform.getValue(m);
    }

    void getMatrix(double *m) const 
	{
        m_xform.getValue(m);
    }

	void setBBox();

	const MT_BBox& getBBox() const { return m_bbox; }	
	
    DT_ResponseClass getResponseClass() const { return m_responseClass; }
    
	void setResponseClass(DT_ResponseClass responseClass) 
	{ 
		m_responseClass = responseClass;
	}

	DT_ShapeType getType() const { return m_shape.getType(); }

	const COShape& getShape() const { return m_shape; }

    void *getClientObject() const { return m_client_object; }

	bool ray_cast(const MT_Point3& source, const MT_Point3& target, 
				  MT_Scalar& param, MT_Vector3& normal) const; 

	void addProxy(BP_ProxyHandle proxy) { m_proxies.push_back(proxy); }

	void removeProxy(BP_ProxyHandle proxy) 
	{ 
		T_ProxyList::iterator it = std::find(m_proxies.begin(), m_proxies.end(), proxy);
		if (it != m_proxies.end()) {
			m_proxies.erase(it);
		}
	}


	friend bool intersect(const DT_Object&, const DT_Object&, MT_Vector3& v);
	
	friend bool common_point(const DT_Object&, const DT_Object&, MT_Vector3&, 
							 MT_Point3&, MT_Point3&);
	
	friend bool penetration_depth(const DT_Object&, const DT_Object&, 
								  MT_Vector3&, MT_Point3&, MT_Point3&);
	
	friend MT_Scalar closest_points(const DT_Object&, const DT_Object&, 
									MT_Point3&, MT_Point3&);

private:
	typedef std::vector<BP_ProxyHandle> T_ProxyList;

	void              *m_client_object;
	DT_ResponseClass   m_responseClass;
    const COShape&    m_shape;
    MT_Scalar          m_margin;
	MT_Transform       m_xform;
	T_ProxyList		   m_proxies;
	MT_BBox            m_bbox;
};

#endif







