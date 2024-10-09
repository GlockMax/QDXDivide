// Файл bezier.h
// Структура данных для описания порций поверхностей и сегментов кривых

#pragma once

#include "Bmatr.h"

class view;
class segmt;
class uv_point;

class piece
{
public:
	point pc[4][4];
	piece set(point&,point&,point&,point&,point&,point&,point&,point&,
				point&,point&,point&,point&,point&,point&,point&,point&);
	void setp( int i, int j, const point & pp ) { pc[i][j] = pp; }
	point v(int,int) const;
	point pcpt(double,double) const;
	int meshinit(piece&,int,int);
	point mesh(point&);
	void draw(const matrix&,double);
	void drmesh(int,int);
	void bernst(piece &);
	int read(void *);
	int write(void *);
	int ispiece(void);
	int small(void);
	int normdirn(void);
	piece& t(void);
	struct gabar gabar() const;
	int is_visible() const;
	void get_n_steps(  double, int &, int & ) const;
	friend piece badpiece(void);
	segmt get_segmt( float, int ) const;
	void get_eight_points(uv_point *uv_dat, int l_r, point res[4][2]);
	void set_point(const point &p_dat, int i1, int i2);
	friend piece operator *(const piece&,const matrix&);
	friend piece operator *(const matrix&,const piece&);
	friend piece  operator %(const matrix&,const piece&);
	friend piece  operator %(const piece&,const matrix&);
	point * modif_lom(point *, double, double );
	point * modif_lom_bol(point *, double, double );
	int operator == ( const piece & p) const
	{
		int res;
		int s[4][4];
		for( int i1=0; i1<4; i1++)
		{
			for( int i2=0; i2<4; i2++)
			{
				if( pc[i1][i2] == p.pc[i1][i2])
					s[i1][i2] = 1;
				else
					s[i1][i2] = 0;
			}
		}
		res = 1;
		for( i1=0; i1<4; i1++)
		{
			for( int i2=0; i2<4; i2++)
			{
				if( s[i1][i2] != 1)
					res = 0;
			}
		}
		return res;
	}
	int get_elem_kas( uv_point &, point * );
};

class segmt
{
public:
	point sg[4];
	segmt set( const point &, const point &, const point &, const point & );
	int read(void *);
	int write(void *);
	point v(int) const;
	void draw(matrix&, double);
	void dr_apx_sg(int);
	struct gabar gabar(void) const;
	int is_visible() const;
	int issegmt(void);
	void bernst(segmt &);
	point pt(double);
	point operator [] (int) const;
	point get_point ( float t ) const;
	int get_n_steps( double, int & ) const;
	double sg_eps_hord( float * ) const;
	int divide( segmt &, segmt & ) const;
	int arc_kasat( const point &, const point &, const point &  );
	friend segmt badsegmt(void);
	friend segmt operator *(const segmt&,const matrix&);
};

class bazis
{
protected:
	double a1, a2, a3, a4;
public:
	 bazis()
	{
		a1=0.0; a2=0.0; a3=0.0; a4=0.0;
	}
	void set_b( double t=0. )
	{
			a1=(1.-t)*(1.-t);
			a2=2.*a1*t;
			a3=2.*t*t*(1.-t);
			a4=t*t;
	}
	double get_a1() const { return a1; }
	double get_a2() const { return a2; }
	double get_a3() const { return a3; }
	double get_a4() const { return a4; }
};

class kasat
{
protected:
	double a1, a2, a3, a4;
public:
	 kasat()
	{
		a1=0.0; a2=0.0; a3=0.0; a4=0.0;
	}
	void set_k( double t=0. )
	{
			a1=(-2.)*(1. - t);
			a2=2.*(1. - t)*(1. - 3.*t);
			a3=2.*t*(2. - 3.*t);
			a4=2.*t;
	}
	double get_a1() const { return a1; }
	double get_a2() const { return a2; }
	double get_a3() const { return a3; }
	double get_a4() const { return a4; }
};

