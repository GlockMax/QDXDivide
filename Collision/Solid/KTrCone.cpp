
#include "KTrCone.h"

MT_Point3 KTrCone::support(const MT_Vector3& v) const 
{
    double s = sqrt(v[0] * v[0] + v[2] * v[2]);
	if(s == 0.)
		return MT_Point3(v[1] > 0. ? -r1 : r0, v[1] > 0. ? halfHeight : -halfHeight, 0.0);

	double Length = v.length();
	double r, sign;
	if(v[1] > Length *sinA)
	{
		r = r1;
		sign = 1.;
	}
	else
	{
		r = r0;
		sign = -1.;
	}
	r /= s;
 	return MT_Point3(v[0] * r, halfHeight * sign, v[2] * r);
}
  
