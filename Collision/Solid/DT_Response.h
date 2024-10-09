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

#ifndef DT_RESPONSE_H
#define DT_RESPONSE_H

#include "SOLID.h"

class DT_Response {
public:
    DT_Response(DT_ResponseCallback response    = 0, 
				DT_ResponseType     type        = DT_NO_RESPONSE, 
				void               *client_data = 0) 
	  : m_response(response), 
		m_type(type), 
		m_client_data(client_data) {}
    
	DT_ResponseType getType() const { return m_type; }

	DT_Bool operator()(void *a, void *b, const DT_CollData *coll_data) const 
	{  
		return (*m_response)(m_client_data, a, b, coll_data); 
	}

	friend bool operator==(const DT_Response& a, const DT_Response& b) 
	{
		return a.m_response == b.m_response;
	}
    
	friend bool operator!=(const DT_Response& a, const DT_Response& b) 
	{
		return a.m_response != b.m_response;
	}
    
private:
    DT_ResponseCallback  m_response;
    DT_ResponseType      m_type;
    void                *m_client_data;
};

#endif  


