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

#ifndef DT_ARRAY_H
#define DT_ARRAY_H

#include <cassert>

template <typename Data, typename Size = size_t>
class DT_Array {
public:
	DT_Array() 
      :	m_count(0), 
		m_data(0) 
	{}

	explicit DT_Array(Size count)
	  :	m_count(count),
		m_data(new Data[count]) 
	{
		assert(m_data);
	}
	
	DT_Array(Size count, const Data *data) 
	  :	m_count(count),
		m_data(new Data[count]) 
	{
		assert(m_data);		
		std::copy(&data[0], &data[count], m_data);
	}
	
	~DT_Array() 
	{ 
		delete [] m_data; 
	}
	
	const Data& operator[](int i) const { return m_data[i]; }
	Data&       operator[](int i)       { return m_data[i]; }

	Size size() const { return m_count; }
	
private:
	DT_Array(const DT_Array&);
	DT_Array& operator=(const DT_Array&);

	Size  m_count;
	Data *m_data;
};
  
#endif

