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


#ifndef DT_ACCURACY_H
#define DT_ACCURACY_H

#include "MT_Scalar.h"

class DT_Accuracy {
public:
	static MT_Scalar rel_error2; // squared relative error in the computed distance
	static MT_Scalar depth_tolerance; // terminate EPA if upper_bound <= depth_tolerance * dist2
	static MT_Scalar tol_error; // error tolerance if the distance is almost zero
	
	static void setAccuracy(MT_Scalar rel_error) 
	{ 
		rel_error2 = rel_error * rel_error;
		depth_tolerance = MT_Scalar(1.0) + MT_Scalar(2.0) * rel_error;
	}	
   
	static void setTolerance(MT_Scalar epsilon) 
	{ 
		tol_error = epsilon;
	}
};

#endif
