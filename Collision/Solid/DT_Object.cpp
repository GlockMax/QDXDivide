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

#include "..\StdAfx.h"
#include "DT_Object.h"
#include "DT_AlgoTable.h"
#include "COConv.h" 
#include "COConcave.h" 
#include "COTran.h"
#include "COMinkSum.h"
#include "BP_Proxy.h"
#include "COSph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void DT_Object::setBBox()
{
	m_bbox = m_shape.bbox(m_xform, m_margin); 
	DT_Vector3 min, max;
	m_bbox.getMin().getValue(min);
	m_bbox.getMax().getValue(max);
	
	T_ProxyList::const_iterator it;
	for (it = m_proxies.begin(); it != m_proxies.end(); ++it) 
	{
//void BP_SetBBox(BP_ProxyHandle proxy, const DT_Vector3 min, const DT_Vector3 max)	
//{
//	((BP_Proxy *)proxy)->setBBox(min, max);
//}
//		BP_SetBBox(*it, min, max);
		((BP_Proxy *)*it)->setBBox(min, max);
	}
}

bool DT_Object::ray_cast(const MT_Point3& source, const MT_Point3& target, 
						 MT_Scalar& lambda, MT_Vector3& normal) const 
{	
	MT_Transform inv_xform = m_xform.inverse();
	MT_Point3 local_source = inv_xform(source);
	MT_Point3 local_target = inv_xform(target);
	MT_Vector3 local_normal;

	bool result = m_shape.ray_cast(local_source, local_target, lambda, local_normal);
    	
	if (result) 
	{
		normal = local_normal * inv_xform.getBasis();
		MT_Scalar len = normal.length();
		if (len > MT_Scalar(0.0))
		{
			normal /= len;
		}
	}

	return result;
}

typedef bool (*Intersect)(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
						  const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
						  MT_Vector3&);

typedef bool (*Common_point)(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
						     const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
			                 MT_Vector3&, MT_Point3&, MT_Point3&);

typedef bool (*Penetration_depth)(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
						          const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                                  MT_Vector3&, MT_Point3&, MT_Point3&);

typedef MT_Scalar (*Closest_points)(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
						            const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
									MT_Point3&, MT_Point3&);

typedef AlgoTable<Intersect> IntersectTable;
typedef AlgoTable<Common_point> Common_pointTable;
typedef AlgoTable<Penetration_depth> Penetration_depthTable;
typedef AlgoTable<Closest_points> Closest_pointsTable;


bool intersectConvexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
						   const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                           MT_Vector3& v) 
{
	COTran ta(a2w, (const COConv&)a);
	COTran tb(b2w, (const COConv&)b);
    return intersect((a_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) : static_cast<const COConv&>(ta)), 
			         (b_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : static_cast<const COConv&>(tb)), v);
}

bool intersectComplexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
						    const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                            MT_Vector3& v) 
{
	COTran tb(b2w, (const COConv&)b);
    return intersect((const COConcave&)a, a2w, a_margin, 
		             (b_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : static_cast<const COConv&>(tb)), v);
}

bool intersectComplexComplex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
							 const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                             MT_Vector3& v) 
{
    return intersect((const COConcave&)a, a2w, a_margin, 
					 (const COConcave&)b, b2w, b_margin, v);
}
static IntersectTable intersectTable;

IntersectTable *intersectInitialize() 
{
//    IntersectTable *p = new IntersectTable;
    intersectTable.addEntry(COMPLEX, COMPLEX, intersectComplexComplex);
    intersectTable.addEntry(COMPLEX, CONVEX, intersectComplexConvex);
    intersectTable.addEntry(CONVEX, CONVEX, intersectConvexConvex);
    return NULL;
}

bool intersect(const DT_Object& a, const DT_Object& b, MT_Vector3& v) 
{
    static IntersectTable *buf = intersectInitialize();
    Intersect intersect = intersectTable.lookup(a.getType(), b.getType());
    return intersect(a.m_shape, a.m_xform, a.m_margin, 
		             b.m_shape, b.m_xform, b.m_margin, v);
}

bool common_pointConvexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
							  const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
							  MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
	COTran ta(a2w, (const COConv&)a);
	COTran tb(b2w, (const COConv&)b);
    return common_point((a_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) : static_cast<const COConv&>(ta)), 
						(b_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : static_cast<const COConv&>(tb)), v, pa, pb);
}

bool common_pointComplexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
							   const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
							   MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
 	COTran tb(b2w, (const COConv&)b);
	return common_point((const COConcave&)a, a2w, a_margin,
			            (b_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : static_cast<const COConv&>(tb)), v, pa, pb);
}

bool common_pointComplexComplex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
								const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
								MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    return common_point((const COConcave&)a, a2w, a_margin, 
						(const COConcave&)b, b2w, b_margin, v, pa, pb);
}

Common_pointTable *common_pointInitialize() 
{
    Common_pointTable *p = new Common_pointTable;
    p->addEntry(COMPLEX, COMPLEX, common_pointComplexComplex);
    p->addEntry(COMPLEX, CONVEX, common_pointComplexConvex);
    p->addEntry(CONVEX, CONVEX, common_pointConvexConvex);
    return p;
}

bool common_point(const DT_Object& a, const DT_Object& b, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    static Common_pointTable *common_pointTable = common_pointInitialize();
    Common_point common_point = common_pointTable->lookup(a.getType(), b.getType());
    return common_point(a.m_shape, a.m_xform, a.m_margin, 
						b.m_shape, b.m_xform, b.m_margin, v, pa, pb);
}



bool penetration_depthConvexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
								   const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                                   MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    return hybrid_penetration_depth(COTran(a2w, (const COConv&)a), a_margin, 
									COTran(b2w, (const COConv&)b), b_margin, v, pa, pb);
}

bool penetration_depthComplexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
									const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                                    MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    return penetration_depth((const COConcave&)a, a2w, a_margin,
							 COTran(b2w, (const COConv&)b), b_margin, v, pa, pb);
}

bool penetration_depthComplexComplex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
									 const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
                                     MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    return penetration_depth((const COConcave&)a, a2w, a_margin, (const COConcave&)b, b2w, b_margin, v, pa, pb);
}

Penetration_depthTable *penetration_depthInitialize() 
{
    Penetration_depthTable *p = new Penetration_depthTable;
    p->addEntry(COMPLEX, COMPLEX, penetration_depthComplexComplex);
    p->addEntry(COMPLEX, CONVEX, penetration_depthComplexConvex);
    p->addEntry(CONVEX, CONVEX, penetration_depthConvexConvex);
    return p;
}

bool penetration_depth(const DT_Object& a, const DT_Object& b, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    static Penetration_depthTable *penetration_depthTable = penetration_depthInitialize();
    Penetration_depth penetration_depth = penetration_depthTable->lookup(a.getType(), b.getType());
    return penetration_depth(a.m_shape, a.m_xform, a.m_margin, 
		                     b.m_shape, b.m_xform, b.m_margin, v, pa, pb);
}


MT_Scalar closest_pointsConvexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
									 const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
									 MT_Point3& pa, MT_Point3& pb)
{
	COTran ta(a2w, (const COConv&)a);
	COTran tb(b2w, (const COConv&)b);
    return closest_points((a_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) : static_cast<const COConv&>(ta)), 
						  (b_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : static_cast<const COConv&>(tb)), MT_INFINITY, pa, pb);
}

MT_Scalar closest_pointsComplexConvex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
									  const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
									  MT_Point3& pa, MT_Point3& pb)
{
	COTran tb(b2w, (const COConv&)b);
    return closest_points((const COConcave&)a, a2w, a_margin,
						  (b_margin > MT_Scalar(0.0) ? static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : static_cast<const COConv&>(tb)), pa, pb);
}

MT_Scalar closest_pointsComplexComplex(const COShape& a, const MT_Transform& a2w, MT_Scalar a_margin,
									   const COShape& b, const MT_Transform& b2w, MT_Scalar b_margin,
									   MT_Point3& pa, MT_Point3& pb) 
{
    return closest_points((const COConcave&)a, a2w, a_margin, 
						  (const COConcave&)b, b2w, b_margin, pa, pb);
}

Closest_pointsTable *closest_pointsInitialize()
{
    Closest_pointsTable *p = new Closest_pointsTable;
    p->addEntry(COMPLEX, COMPLEX, closest_pointsComplexComplex);
    p->addEntry(COMPLEX, CONVEX, closest_pointsComplexConvex);
    p->addEntry(CONVEX, CONVEX, closest_pointsConvexConvex);
    return p;
}

MT_Scalar closest_points(const DT_Object& a, const DT_Object& b,
						 MT_Point3& pa, MT_Point3& pb) 
{
    static Closest_pointsTable *closest_pointsTable = closest_pointsInitialize();
    Closest_points closest_points = closest_pointsTable->lookup(a.getType(), b.getType());
    return closest_points(a.m_shape, a.m_xform, a.m_margin, 
						  b.m_shape, b.m_xform, b.m_margin, pa, pb);
}

