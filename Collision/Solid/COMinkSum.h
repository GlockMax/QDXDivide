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

#include "COConv.h"

class COMinkSum : public COConv {
public:
	COMinkSum(const COConv& lchild, const COConv& rchild) 
     : m_lchild(lchild), 
       m_rchild(rchild) 
   {}

	virtual MT_Scalar supportH(const MT_Vector3& v) const 
	{
		return m_lchild.supportH(v) + m_rchild.supportH(v); 
	}

	virtual MT_Point3 support(const MT_Vector3& v) const 
	{
		return m_lchild.support(v) + (MT_Vector3)m_rchild.support(v); 
	}

private:
	const COConv& m_lchild;
	const COConv& m_rchild;
};
