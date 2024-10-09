#include "StdAfx.h"
#include "gmconst.h"
#include "Bmatr.h"
#include "bezier.h"

int piece::ispiece()
{
  return(pc[0][0].GetH() > 0.);
}
    piece piece::
     set(BPoint& pc00,BPoint& pc01,BPoint& pc02,BPoint& pc03,
         BPoint& pc10,BPoint& pc11,BPoint& pc12,BPoint& pc13,
         BPoint& pc20,BPoint& pc21,BPoint& pc22,BPoint& pc23,
         BPoint& pc30,BPoint& pc31,BPoint& pc32,BPoint& pc33)
   {
    pc[0][0]=pc00;pc[0][1]=pc01;pc[0][2]=pc02;pc[0][3]=pc03;
    pc[1][0]=pc10;pc[1][1]=pc11;pc[1][2]=pc12;pc[1][3]=pc13;
    pc[2][0]=pc20;pc[2][1]=pc21;pc[2][2]=pc22;pc[2][3]=pc23;
    pc[3][0]=pc30;pc[3][1]=pc31;pc[3][2]=pc32;pc[3][3]=pc33;
    return *this;
   }
   BPoint piece::v(int i,int j)
   {
     if(i<0 || i>3 || j<0 || j>3)
     { BPoint p;
       p.Set(0.,0.,0.,-1.);
       return p;
     }
     return pc[i][j];
   }
   segmt piece::operator [] (int i) const
   {
       segmt seg;
       for(int n=0;n<4;n++) seg[n]=pc[i][n];
       return seg;
   }
       
piece operator *(const piece& p,const BMatr& m)
{
  piece w;
  w.pc[0][0]=p.pc[0][0]*m;w.pc[0][1]=p.pc[0][1]*m;
  w.pc[0][2]=p.pc[0][2]*m;w.pc[0][3]=p.pc[0][3]*m;
  w.pc[1][0]=p.pc[1][0]*m;w.pc[1][1]=p.pc[1][1]*m;
  w.pc[1][2]=p.pc[1][2]*m;w.pc[1][3]=p.pc[1][3]*m;
  w.pc[2][0]=p.pc[2][0]*m;w.pc[2][1]=p.pc[2][1]*m;
  w.pc[2][2]=p.pc[2][2]*m;w.pc[2][3]=p.pc[2][3]*m;
  w.pc[3][0]=p.pc[3][0]*m;w.pc[3][1]=p.pc[3][1]*m;
  w.pc[3][2]=p.pc[3][2]*m;w.pc[3][3]=p.pc[3][3]*m;
  return w;
}
piece operator *(const BMatr& m,const piece& p)
{
  piece w;
  w.pc[0][0]=m*p.pc[0][0];w.pc[0][1]=m*p.pc[0][1];
  w.pc[0][2]=m*p.pc[0][2];w.pc[0][3]=m*p.pc[0][3];
  w.pc[1][0]=m*p.pc[1][0];w.pc[1][1]=m*p.pc[1][1];
  w.pc[1][2]=m*p.pc[1][2];w.pc[1][3]=m*p.pc[1][3];
  w.pc[2][0]=m*p.pc[2][0];w.pc[2][1]=m*p.pc[2][1];
  w.pc[2][2]=m*p.pc[2][2];w.pc[2][3]=m*p.pc[2][3];
  w.pc[3][0]=m*p.pc[3][0];w.pc[3][1]=m*p.pc[3][1];
  w.pc[3][2]=m*p.pc[3][2];w.pc[3][3]=m*p.pc[3][3];
  return w;
}

int piece::normdirn()
{// ориентация относительно оси Z
 // -1 - нормаль всюду отрицательна
 //  0 - меняет знак
 //  1 - положительна
 BPoint p1,p2,p3,pz;
 double z;
 int flag=0;
 pz.Set(0.,0.,1.,0.);
 p1=pc[0][0]; p2=pc[0][1]; p3=pc[1][0];
 p1.Norm();p2.Norm();p3.Norm();
 z=((p2-p1)%(p3-p1))*pz;
 if(z>0) flag=1;
 else if(z<0) flag=-1;
 p1=pc[3][0]; p2=pc[3][1]; p3=pc[2][0];
 p1.Norm();p2.Norm();p3.Norm();
 z=((p2-p1)%(p1-p3))*pz;
 if(flag*z<0.) return 0;
 if(flag==0) if(z>0) flag=1; else if(z<0) flag=-1;

 p1=pc[0][3]; p2=pc[0][2]; p3=pc[1][3];
 p1.Norm();p2.Norm();p3.Norm();
 z=((p1-p2)%(p3-p1))*pz;
 if(flag*z<0.) return 0;
 if(flag==0) if(z>0) flag=1; else if(z<0) flag=-1;

 p1=pc[3][3]; p2=pc[3][2]; p3=pc[2][3];
 p1.Norm();p2.Norm();p3.Norm();
 z=((p1-p2)%(p1-p3))*pz;
 if(flag*z<0.) return 0;
 if(flag==0) if(z>0) flag=1; else if(z<0) flag=-1;

 if(flag==0) flag=-1;
 return -flag;
}
piece& piece::t()
// транспонирование
{
  BPoint p;
  p=pc[0][1];pc[0][1]=pc[1][0];pc[1][0]=p;
  p=pc[0][2];pc[0][2]=pc[2][0];pc[2][0]=p;
  p=pc[0][3];pc[0][3]=pc[3][0];pc[3][0]=p;
  p=pc[1][2];pc[1][2]=pc[2][1];pc[2][1]=p;
  p=pc[1][3];pc[1][3]=pc[3][1];pc[3][1]=p;
  p=pc[2][3];pc[2][3]=pc[3][2];pc[3][2]=p;
  return *this;
}
piece badpiece()
{
  piece pie;
  pie.pc[0][0].Set(0.,0.,0.,-1);
  return pie;
}
// SEGMT
    segmt segmt::
     set(BPoint& pc0,BPoint& pc1,BPoint& pc2,BPoint& pc3)
   {
    sg[0]=pc0;sg[1]=pc1;sg[2]=pc2;sg[3]=pc3;
    return *this;
   }
int segmt::issegmt()
{
  return(sg[0].GetH() > 0.);
}
segmt badsegmt()
{
  segmt seg;
  seg.sg[0].Set(0.,0.,0.,-1);
  seg.sg[1].Set(0.,0.,0.,-1);
  seg.sg[2].Set(0.,0.,0.,-1);
  seg.sg[3].Set(0.,0.,0.,-1);
  return seg;
}
BPoint segmt::v(int i)
   {
     if(i<0 || i>3 )
     { BPoint p;
       p.Set(0.,0.,0.,-1.);
       return p;
     }
     return sg[i];
   }
BPoint segmt::operator [] (int i) const
{
    return sg[i];
}
BPoint segmt::pt(double u)
{
  double g0u,g1u,g2u,g3u;
  g0u=1.-u;g3u=u*u;g2u=2.*g0u*g3u;g1u=2.*(g0u*=g0u)*u;
  return sg[0]*g0u+sg[1]*g1u+sg[2]*g2u+sg[3]*g3u;
}
segmt operator *(const segmt& p,const BMatr& m)
{
  segmt w;
  w.sg[0]=p.sg[0]*m;w.sg[1]=p.sg[1]*m;
  w.sg[2]=p.sg[2]*m;w.sg[3]=p.sg[3]*m;
  return w;
}
 void piece::bernst(piece &pb)
// перевод порции в базис Бернштейна
{
  pc[0][0]=pb.pc[0][0];  pc[3][0]=pb.pc[3][0];
  pc[0][3]=pb.pc[0][3];  pc[3][3]=pb.pc[3][3];
  pc[0][1]=(pb.pc[0][0]+2.*pb.pc[0][1])*(1./3.);
  pc[1][0]=(pb.pc[0][0]+2.*pb.pc[1][0])*(1./3.);
  pc[0][2]=(pb.pc[0][3]+2.*pb.pc[0][2])*(1./3.);
  pc[2][0]=(pb.pc[3][0]+2.*pb.pc[2][0])*(1./3.);
  pc[3][1]=(pb.pc[3][0]+2.*pb.pc[3][1])*(1./3.);
  pc[1][3]=(pb.pc[0][3]+2.*pb.pc[1][3])*(1./3.);
  pc[3][2]=(pb.pc[3][3]+2.*pb.pc[3][2])*(1./3.);
  pc[2][3]=(pb.pc[3][3]+2.*pb.pc[2][3])*(1./3.);

  pc[1][1]=(pb.pc[0][0]+2.*pb.pc[1][0]+2.*pb.pc[0][1]
            +4.*pb.pc[1][1])*(1./9.);
  pc[2][1]=(pb.pc[3][0]+2.*pb.pc[2][0]+2.*pb.pc[3][1]
            +4.*pb.pc[2][1])*(1./9.);
  pc[1][2]=(pb.pc[0][3]+2.*pb.pc[1][3]+2.*pb.pc[0][2]
            +4.*pb.pc[1][2])*(1./9.);
  pc[2][2]=(pb.pc[3][3]+2.*pb.pc[3][2]+2.*pb.pc[2][3]
            +4.*pb.pc[2][2])*(1./9.);
}
void segmt::bernst(segmt &sb)
// перевод сегмента в базис Бернштейна
{
  sg[0]=sb.sg[0];  sg[3]=sb.sg[3];
  sg[1]=(sb.sg[0]+2.*sb.sg[1])*(1./3.);
  sg[2]=(sb.sg[3]+2.*sb.sg[2])*(1./3.);
}
//int piece::small()
//{
//  BPoint p1,p2;
//  double d1,d2,d3,d4;
//  p2=pc[3][0];p2.Norm();p1=pc[0][0];p1.Norm();d1=(p2-p1).d2();
//  p2=pc[3][1];p2.Norm();p1=pc[0][1];p1.Norm();d2=(p2-p1).d2();
//  p2=pc[3][2];p2.Norm();p1=pc[0][2];p1.Norm();d3=(p2-p1).d2();
//  p2=pc[3][3];p2.Norm();p1=pc[0][3];p1.Norm();d4=(p2-p1).d2();
//  if(d1<LMIN*LMIN && d2<LMIN*LMIN && d3<LMIN*LMIN && d4<LMIN*LMIN)
//    return 1;
//  p2=pc[0][3];p2.Norm();p1=pc[0][0];p1.Norm();d1=(p2-p1).d2();
//  p2=pc[1][3];p2.Norm();p1=pc[1][0];p1.Norm();d2=(p2-p1).d2();
//  p2=pc[2][3];p2.Norm();p1=pc[2][0];p1.Norm();d3=(p2-p1).d2();
//  p2=pc[3][3];p2.Norm();p1=pc[3][0];p1.Norm();d4=(p2-p1).d2();
//  if(d1<LMIN*LMIN && d2<LMIN*LMIN && d3<LMIN*LMIN && d4<LMIN*LMIN)
//    return 1;
//  return 0;
//}

