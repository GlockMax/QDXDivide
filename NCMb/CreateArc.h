#include "NCEnums.h"
#include "BPoint.h"

int CreateSimpleArc(BPoint *ControlP);
int CreateArc(BPoint pb, BPoint pe, BPoint i, BPoint n,
				int type,
				float *&ctlarray,float *&knot,
				double eps = 1.e-5,
				Plane pl = XY);
int CreateArbArc(
				BPoint pb,
				BPoint pe,
				BPoint i,
				int type,
				float *&ctlarray,float *&knot,
				double eps = 1.e-5,
				Plane pl = XY);