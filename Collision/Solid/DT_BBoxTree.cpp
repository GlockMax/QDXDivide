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
#include "DT_BBoxTree.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
//#endif

inline DT_CBox getBBox(int first, int last, const DT_CBox *boxes, const DT_Index *indices)
{
	assert(last - first >= 1);

	DT_CBox bbox = boxes[indices[first]];
	int i;
	for (i = first; i < last; ++i) 
	{
		bbox = bbox.hull(boxes[indices[i]]);
	}

	return bbox;
}

DT_BBoxNode::DT_BBoxNode(int first, int last, int& node, DT_BBoxNode *free_nodes, const DT_CBox *boxes, DT_Index *indices, const DT_CBox& bbox)
{
	assert(last - first >= 2);
	
	int axis = bbox.longestAxis();
	MT_Scalar abscissa = bbox.getCenter()[axis];
	int i = first, mid = last;
	while (i < mid) 
	{
		if (boxes[indices[i]].getCenter()[axis] < abscissa)
		{
			++i;
		}
		else
		{
			--mid;
			std::swap(indices[i], indices[mid]);
		}
	}

	if (mid == first || mid == last) 
	{
		mid = (first + last) / 2;
	}
	
	m_lbox = getBBox(first, mid, boxes, indices);
	m_rbox = getBBox(mid, last, boxes, indices);
	m_flags = 0x0;

	if (mid - first == 1)
	{
		m_flags |= LLEAF;
		m_lchild = indices[first];
	}
	else 
	{	
		m_lchild = node++;
		new(&free_nodes[m_lchild]) DT_BBoxNode(first, mid, node, free_nodes, boxes, indices, m_lbox);
	}

	if (last - mid == 1)
	{
		m_flags |= RLEAF;
		m_rchild = indices[mid];
	}
	else 
	{
		m_rchild = node++;
		new(&free_nodes[m_rchild]) DT_BBoxNode(mid, last, node, free_nodes, boxes, indices, m_rbox); 
	}
}
