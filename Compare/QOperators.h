/** operators : Basic arithmetic operation using INFTY numbers
 * 
 * David Coeurjolly (david.coeurjolly@liris.cnrs.fr) - Sept. 2004
 *
**/
#pragma once

/////////Basic functions to handle operations with INFTY

/** 
 **************************************************
 * @b sum
 * @param a QRCellIndexType number with INFTY
 * @param b QRCellIndexType number with INFTY
 * @return The sum of a and b handling INFTY
 **************************************************/
QRCellIndexType sum(QRCellIndexType a, QRCellIndexType b) 
{
  if ((a==INFTY) || (b==INFTY))     
    return INFTY;    
  else 
    return a+b;
}

/** 
 **************************************************
 * @b prod
 * @param a QRCellIndexType number with INFTY
 * @param b QRCellIndexType number with INFTY
 * @return The product of a and b handling INFTY
 **************************************************/
QRCellIndexType prod(QRCellIndexType a, QRCellIndexType b) 
{
  if ((a==INFTY) || (b==INFTY)) 
    return INFTY;  
  else 
    return a*b;
}
/** 
 **************************************************
 * @b opp
 * @param a QRCellIndexType number with INFTY
 * @return The opposite of a  handling INFTY
 **************************************************/
QRCellIndexType opp (QRCellIndexType a) {
  if (a == INFTY) {
    return INFTY;
  }
  else {
    return -a;
  }
}

/** 
 **************************************************
 * @b intdivint
 * @param divid QRCellIndexType number with INFTY
 * @param divis QRCellIndexType number with INFTY
 * @return The division (integer) of divid out of divis handling INFTY
 **************************************************/
QRCellIndexType intdivint (QRCellIndexType divid, QRCellIndexType divis) {
  if (divis == 0) 
    return  INFTY;
  if (divid == INFTY) 
    return  INFTY;
  else 
    return  divid / divis;
}
//////////

/**
 =================================================
 * @file   operators.cc
 * @author David COEURJOLLY <David Coeurjolly <dcoeurjo@liris.cnrs.fr>>
 * @date   Thu Sep 30 09:22:19 2004
 * 
 * @brief  Basic implementation of arithmetical operations using INFTY numbers.
 * 
 * 
 =================================================*/
QRCellIndexType F(int x, int i, QRCellIndexType gi2)
{
  return sum((x-i)*(x-i), gi2);
}

QRCellIndexType Sep(int i, int u, QRCellIndexType gi2, QRCellIndexType gu2) {
  return intdivint(sum( sum((QRCellIndexType) (u*u - i*i),gu2), opp(gi2) ), 2*(u-i));
}