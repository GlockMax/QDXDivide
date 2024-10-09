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

#ifndef DT_RESPTABLE_H
#define DT_RESPTABLE_H

#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include "GEN_MinMax.h"
#include "DT_Response.h"

class DT_ResponseList : public std::list<DT_Response> {
public:
    DT_ResponseList() : m_type(DT_NO_RESPONSE) {}

	DT_ResponseType getType() const { return m_type; }

    void addResponse(const DT_Response& response) 
	{
        if (response.getType() != DT_NO_RESPONSE) 
		{
            push_back(response);
            GEN_set_max(m_type, response.getType());
        }
    }

    void removeResponse(const DT_Response& response) 
	{
		iterator it = std::find(begin(), end(), response);
		if (it != end()) 
		{
			erase(it);
			m_type = DT_NO_RESPONSE;
			for (it = begin(); it != end(); ++it) 
			{
				GEN_set_max(m_type, (*it).getType());
			}
		}
    }
	
    void append(const DT_ResponseList& responseList) 
	{
        if (responseList.getType() != DT_NO_RESPONSE) 
		{
			const_iterator it;
			for (it = responseList.begin(); it != responseList.end(); ++it) 
			{
				addResponse(*it);
			}
		}
	}

    DT_Bool operator()(void *a, void *b, const DT_CollData *coll_data) const 
	{
		DT_Bool done = DT_CONTINUE;
		const_iterator it;
        for (it = begin(); !done && it != end(); ++it) 
		{
            done = (*it)(a, b, coll_data);
        }
		return done;
    }
    
private:
	DT_ResponseType    m_type;
};

class DT_RespTable {
private:
	typedef std::map<void *, DT_ResponseClass> T_ObjectMap; 
	typedef std::vector<DT_ResponseList *> T_PairTable;
	typedef std::vector<DT_ResponseList> T_SingleList;

public:
	DT_RespTable() : m_responseClass(0) { genResponseClass(); }

	~DT_RespTable();

	DT_ResponseClass genResponseClass();
	
	void setResponseClass(void *object, DT_ResponseClass responseClass);
	DT_ResponseClass getResponseClass(void *object) const;
	
	void clearResponseClass(void *object);
	
	const DT_ResponseList& find(void *object1, void *object2) const;

    void addDefault(const DT_Response& response); 
    void removeDefault(const DT_Response& response); 

    void addSingle(DT_ResponseClass responseClass, 
				   const DT_Response& response);
    void removeSingle(DT_ResponseClass responseClass, 
					  const DT_Response& response);
	
    void addPair(DT_ResponseClass responseClass1, 
				 DT_ResponseClass responseClass2, 
				 const DT_Response& response);
    void removePair(DT_ResponseClass responseClass1, 
					DT_ResponseClass responseClass2, 
					const DT_Response& response);

private:
	static DT_ResponseList g_emptyResponseList;

	T_ObjectMap      m_objectMap;
	DT_ResponseClass m_responseClass;
	T_PairTable      m_table;
	T_SingleList     m_singleList;
    DT_ResponseList  m_default;
};

#endif




