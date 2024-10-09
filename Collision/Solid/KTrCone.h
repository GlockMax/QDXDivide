#pragma once

#include "DT_Convex.h"

class KTrCone : public DT_Convex {
public:
    KTrCone(double R0, double R1, double H) : 
        r0(R0), 
		r1(R1),
		sinA((r0 - r1) / sqrt((r0 - r1) * (r0 - r1) + H * H)),
        halfHeight(H * 0.5) {}
    
    virtual MT_Point3 support(const MT_Vector3& v) const;
    
protected:
    double r0;
    double r1;
    double halfHeight;
	double sinA;
};

