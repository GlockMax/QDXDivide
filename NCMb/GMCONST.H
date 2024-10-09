#ifndef __GMCONST_H
  #define __GMCONST_H

#define MAXINT 32765

struct gabar { float xmin,ymin,zmin,
                     xmax,ymax,zmax;};
/***************************************/
const double HMIN=0.001;
const double LMIN=0.1;
const double PS=0.0031;/* точность для сравнения углов          */
const double IDMIN=7.;
const int    MAXCOL=15;
const int    NENT=-32767;

/***************************************/

#endif
