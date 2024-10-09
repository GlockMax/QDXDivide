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

#include "DT_RespTable.h"
#include "DT_Encounter.h"
#include "DT_Object.h"
#include "GEN_MinMax.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

DT_Bool DT_Encounter::exactTest(const DT_RespTable *respTable, int& count) const
{
	const DT_ResponseList& responseList = respTable->find(m_obj_ptr1, m_obj_ptr2);

   switch (responseList.getType()) 
   {
   case DT_SIMPLE_RESPONSE: 
	   if (intersect(*m_obj_ptr1, *m_obj_ptr2, m_sep_axis)) 
	   {
		   ++count;
		   return (respTable->getResponseClass(m_obj_ptr1) < respTable->getResponseClass(m_obj_ptr2)) ?
			   responseList(m_obj_ptr1->getClientObject(), m_obj_ptr2->getClientObject(), 0) :   
			   responseList(m_obj_ptr2->getClientObject(), m_obj_ptr1->getClientObject(), 0);    
 
	   }
	   break;
   case DT_WITNESSED_RESPONSE: {
	   MT_Point3  p1, p2;
	   
	   if (common_point(*m_obj_ptr1, *m_obj_ptr2, m_sep_axis, p1, p2)) 
	   { 
		   ++count;
           if (respTable->getResponseClass(m_obj_ptr1) < respTable->getResponseClass(m_obj_ptr2))
           {
			   DT_CollData coll_data;
			   
			   p1.getValue(coll_data.point1);
			   p2.getValue(coll_data.point2);
			   
               return responseList(m_obj_ptr1->getClientObject(), m_obj_ptr2->getClientObject(), &coll_data);
           }
           else
           {
			   DT_CollData coll_data;
			   
			   p1.getValue(coll_data.point2);
			   p2.getValue(coll_data.point1);
			   
               return responseList(m_obj_ptr2->getClientObject(), m_obj_ptr1->getClientObject(), &coll_data);
           }
	   }
	   break;
   }
   case DT_DEPTH_RESPONSE: {
	   MT_Point3  p1, p2;
	   
	   if (penetration_depth(*m_obj_ptr1, *m_obj_ptr2, m_sep_axis, p1, p2)) 
	   { 
		   ++count;
           if (respTable->getResponseClass(m_obj_ptr1) < respTable->getResponseClass(m_obj_ptr2))
           {
			   DT_CollData coll_data;
			   
			   p1.getValue(coll_data.point1);
			   p2.getValue(coll_data.point2);	
               (p2 - p1).getValue(coll_data.normal);
			   
               return responseList(m_obj_ptr1->getClientObject(), m_obj_ptr2->getClientObject(), &coll_data);
           }
           else
           {
			   DT_CollData coll_data;
			   
			   p1.getValue(coll_data.point2);
			   p2.getValue(coll_data.point1); 
               (p1 - p2).getValue(coll_data.normal);
			   
               return responseList(m_obj_ptr2->getClientObject(), m_obj_ptr1->getClientObject(), &coll_data);
           }
	   }
	   break;
   }
   case DT_NO_RESPONSE:
	   break;
   default:
	   assert(false);
   }
   return DT_CONTINUE;
}
