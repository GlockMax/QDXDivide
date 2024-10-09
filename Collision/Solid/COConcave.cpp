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
#include <new>
#include <fstream>

#include "COConcave.h"
#include "COMinkSum.h"
#include "COSph.h"
#include "COTran.h"
#include "DT_Object.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

COConcave::COConcave(const DT_VertexBase *base)
  : m_base(base),
    m_count(0),
    m_leaves(0),
	m_nodes(0)
{ 
	assert(base);
	base->addComplex(this);
}


COConcave::~COConcave()
{
    DT_Index i;
    for (i = 0; i != m_count; ++i) 
    {
        delete m_leaves[i];
    }
    delete [] m_leaves;
    delete [] m_nodes;
    
    m_base->removeComplex(this);
//    if (m_base->isOwner()) 
    {
        delete m_base;
    }
}

void COConcave::finish(DT_Count n, const COConv *p[]) 
{
	m_count = n;

   
    assert(n >= 1);

    m_leaves = new const COConv *[n];
    assert(m_leaves);

    DT_CBox *boxes = new DT_CBox[n];
    DT_Index *indices = new DT_Index[n];
    assert(boxes);
       
    DT_Index i;
    for (i = 0; i != n; ++i) 
    {
        m_leaves[i] = p[i];
        boxes[i].set(p[i]->bbox());
        indices[i] = i;
    }

    m_cbox = boxes[0];
    for (i = 1; i != n; ++i) 
    {
        m_cbox = m_cbox.hull(boxes[i]);
    }

    if (n == 1)
    {
        m_nodes = 0;
        m_type = DT_BBoxTree::LEAF;
    }
    else 
    {
        m_nodes = new DT_BBoxNode[n - 1];
        assert(m_nodes);
    
        int num_nodes = 0;
        new(&m_nodes[num_nodes++]) DT_BBoxNode(0, n, num_nodes, m_nodes, boxes, indices, m_cbox);

        assert(num_nodes == n - 1);
        
        m_type = DT_BBoxTree::INTERNAL;
    }

    delete [] boxes;
	delete [] indices;
}


MT_BBox COConcave::bbox(const MT_Transform& t, MT_Scalar margin) const 
{
    MT_Matrix3x3 abs_b = t.getBasis().absolute();  
    MT_Point3 center = t(m_cbox.getCenter());
    MT_Vector3 extent(margin + abs_b[0].dot(m_cbox.getExtent()),
                      margin + abs_b[1].dot(m_cbox.getExtent()),
                      margin + abs_b[2].dot(m_cbox.getExtent()));
    
    return MT_BBox(center - extent, center + extent);
}

inline DT_CBox computeCBox(const COConv *p)
{
    return DT_CBox(p->bbox()); 
}

inline DT_CBox computeCBox(MT_Scalar margin, const MT_Transform& xform) 
{
    const MT_Matrix3x3& basis = xform.getBasis();
    return DT_CBox(MT_Point3(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0)), 
                   MT_Vector3(basis[0].length() * margin, 
                              basis[1].length() * margin, 
                              basis[2].length() * margin));
} 

void COConcave::refit()
{
    DT_RootData<const COConv *> rd(m_nodes, m_leaves);
    DT_Index i = m_count - 1;
    while (i--)
    {
        ::refit(m_nodes[i], rd);
    }
    m_cbox = m_type == DT_BBoxTree::LEAF ? computeCBox(m_leaves[0]) : m_nodes[0].hull();

	for (ObjectList::iterator it = m_objectList.begin(); it != m_objectList.end(); ++it)
	{
		(*it)->setBBox();
	}
}

inline bool ray_cast(const DT_RootData<const COConv *>& rd, DT_Index index, const MT_Point3& source, const MT_Point3& target, 
                     MT_Scalar& lambda, MT_Vector3& normal)
{
    return rd.m_leaves[index]->ray_cast(source, target, lambda, normal);
}

bool COConcave::ray_cast(const MT_Point3& source, const MT_Point3& target,
                          MT_Scalar& lambda, MT_Vector3& normal) const 
{
    DT_RootData<const COConv *> rd(m_nodes, m_leaves);

    return rayCast(DT_BBoxTree(m_cbox, 0, m_type), rd, source, target, lambda, normal);
}

inline bool intersect(const DT_Pack<const COConv *, MT_Scalar>& pack, DT_Index a_index, MT_Vector3& v) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    MT_Scalar a_margin = pack.m_a.m_plus;
    return ::intersect((a_margin > MT_Scalar(0.0) ? 
                        static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) :  
                        static_cast<const COConv&>(ta)), 
                       pack.m_b, v); 
}

bool intersect(const COConcave& a,  const MT_Transform& a2w,  MT_Scalar a_margin, 
               const COConv& b, MT_Vector3& v) 
{
    DT_Pack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin), b);

    return intersect(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type), pack, v);
}

inline bool intersect(const DT_DuoPack<const COConv *, MT_Scalar>& pack, DT_Index a_index, DT_Index b_index, MT_Vector3& v) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    MT_Scalar a_margin = pack.m_a.m_plus;
    COTran tb = COTran(pack.m_b.m_xform, *pack.m_b.m_leaves[b_index]);
    MT_Scalar b_margin = pack.m_b.m_plus;
    return ::intersect((a_margin > MT_Scalar(0.0) ?
                        static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) : 
                        static_cast<const COConv&>(ta)), 
                       (b_margin > MT_Scalar(0.0) ? 
                        static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : 
                        static_cast<const COConv&>(tb)), 
                       v);   
}

bool intersect(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin,
               const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin, MT_Vector3& v) 
{
    DT_DuoPack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin),
                                                  DT_ObjectData<const COConv *, MT_Scalar>(b.m_nodes, b.m_leaves, b2w, b_margin));


    return intersect(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type),
                     DT_BBoxTree(b.m_cbox + pack.m_b.m_added, 0, b.m_type), pack, v);
}

inline bool common_point(const DT_Pack<const COConv *, MT_Scalar>& pack, DT_Index a_index, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    MT_Scalar a_margin = pack.m_a.m_plus;
    return ::common_point((a_margin > MT_Scalar(0.0) ? 
                           static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) :
                           static_cast<const COConv&>(ta)), 
                          pack.m_b, v, pa, pb); 
}
    
bool common_point(const COConcave& a,  const MT_Transform& a2w,  MT_Scalar a_margin, 
                  const COConv& b, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
     DT_Pack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin), b);

    return common_point(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type), pack, v, pb, pa);
}

inline bool common_point(const DT_DuoPack<const COConv *, MT_Scalar>& pack, DT_Index a_index, DT_Index b_index, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    MT_Scalar a_margin = pack.m_a.m_plus;
    COTran tb = COTran(pack.m_b.m_xform, *pack.m_b.m_leaves[b_index]);
    MT_Scalar b_margin = pack.m_b.m_plus;
    return ::common_point((a_margin > MT_Scalar(0.0) ? 
                           static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) : 
                           static_cast<const COConv&>(ta)), 
                          (b_margin > MT_Scalar(0.0) ? 
                           static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : 
                           static_cast<const COConv&>(tb)), 
                          v, pa, pb);    
}
    
bool common_point(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin,
                  const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin, 
                  MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    DT_DuoPack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin),
                                                  DT_ObjectData<const COConv *, MT_Scalar>(b.m_nodes, b.m_leaves, b2w, b_margin));

    return common_point(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type),
                        DT_BBoxTree(b.m_cbox + pack.m_b.m_added, 0, b.m_type),  pack, v, pa, pb);
}

inline bool penetration_depth(const DT_HybridPack<const COConv *, MT_Scalar>& pack, DT_Index a_index, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    return ::hybrid_penetration_depth(ta, pack.m_a.m_plus, pack.m_b, pack.m_margin, v, pa, pb); 
}

bool penetration_depth(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin, 
                       const COConv& b, MT_Scalar b_margin, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    DT_HybridPack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin), b, b_margin);
     
    MT_Scalar  max_pen_len = MT_Scalar(0.0);
    return penetration_depth(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type), pack, v, pa, pb, max_pen_len);
}

inline bool penetration_depth(const DT_DuoPack<const COConv *, MT_Scalar>& pack, DT_Index a_index, DT_Index b_index, MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    COTran tb = COTran(pack.m_b.m_xform, *pack.m_b.m_leaves[b_index]);
    return ::hybrid_penetration_depth(ta, pack.m_a.m_plus, tb, pack.m_b.m_plus, v, pa, pb);  
}

bool penetration_depth(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin,
                       const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin, 
                       MT_Vector3& v, MT_Point3& pa, MT_Point3& pb) 
{
    DT_DuoPack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin),
                                                  DT_ObjectData<const COConv *, MT_Scalar>(b.m_nodes, b.m_leaves, b2w, b_margin));

    MT_Scalar  max_pen_len = MT_Scalar(0.0);
    return penetration_depth(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type),
                             DT_BBoxTree(b.m_cbox + pack.m_b.m_added, 0, b.m_type), pack, v, pa, pb, max_pen_len);
}



inline MT_Scalar closest_points(const DT_Pack<const COConv *, MT_Scalar>& pack, DT_Index a_index, MT_Scalar max_dist2, MT_Point3& pa, MT_Point3& pb) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    MT_Scalar a_margin = pack.m_a.m_plus;
    return ::closest_points((a_margin > MT_Scalar(0.0) ? 
                             static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) :  
                             static_cast<const COConv&>(ta)), 
                            pack.m_b, max_dist2, pa, pb); 
}

MT_Scalar closest_points(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin,
                         const COConv& b, MT_Point3& pa, MT_Point3& pb)
{
    DT_Pack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin), b);

    return closest_points(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type), pack, MT_INFINITY, pa, pb); 
}

inline MT_Scalar closest_points(const DT_DuoPack<const COConv *, MT_Scalar>& pack, DT_Index a_index, DT_Index b_index, MT_Scalar max_dist2, MT_Point3& pa, MT_Point3& pb) 
{
    COTran ta = COTran(pack.m_a.m_xform, *pack.m_a.m_leaves[a_index]);
    MT_Scalar a_margin = pack.m_a.m_plus;
    COTran tb = COTran(pack.m_b.m_xform, *pack.m_b.m_leaves[b_index]);
    MT_Scalar b_margin = pack.m_b.m_plus;
    return ::closest_points((a_margin > MT_Scalar(0.0) ? 
                             static_cast<const COConv&>(COMinkSum(ta, COSph(a_margin))) : 
                             static_cast<const COConv&>(ta)), 
                            (b_margin > MT_Scalar(0.0) ? 
                             static_cast<const COConv&>(COMinkSum(tb, COSph(b_margin))) : 
                             static_cast<const COConv&>(tb)), max_dist2, pa, pb);     
}

MT_Scalar closest_points(const COConcave& a, const MT_Transform& a2w, MT_Scalar a_margin,
                         const COConcave& b, const MT_Transform& b2w, MT_Scalar b_margin, 
                         MT_Point3& pa, MT_Point3& pb) 
{
    DT_DuoPack<const COConv *, MT_Scalar> pack(DT_ObjectData<const COConv *, MT_Scalar>(a.m_nodes, a.m_leaves, a2w, a_margin),
                               DT_ObjectData<const COConv *, MT_Scalar>(b.m_nodes, b.m_leaves, b2w, b_margin));

    return closest_points(DT_BBoxTree(a.m_cbox + pack.m_a.m_added, 0, a.m_type),
                          DT_BBoxTree(b.m_cbox + pack.m_b.m_added, 0, b.m_type), pack, MT_INFINITY, pa, pb);
}


