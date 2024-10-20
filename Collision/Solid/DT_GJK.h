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

#ifndef DT_GJK_H
#define DT_GJK_H

//#define USE_BACKUP_PROCEDURE
//#define JOHNSON_ROBUST
#define FAST_CLOSEST
#define UNROLL_LOOPS


#include "MT_Point3.h"
#include "MT_Vector3.h"
#include "GEN_MinMax.h"
#include "DT_Accuracy.h"

class DT_GJK {
private:
	typedef unsigned int T_Bits;
	inline static bool subseteq(T_Bits a, T_Bits b) { return (a & b) == a; }
	inline static bool contains(T_Bits a, T_Bits b) { return (a & b) != 0x0; }

public:
	DT_GJK() :
		m_bits(0x0),
		m_all_bits(0x0)
	{}

	bool emptySimplex() const { return m_bits == 0x0; }
	bool fullSimplex() const { return m_bits == 0xf; }

	void reset() 
	{
		m_bits = 0x0;
		m_all_bits = 0x0;	
	}

	bool inSimplex(const MT_Vector3& w) const
	{
       		int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1)
		{
            if (contains(m_all_bits, bit) && w == m_y[i])
			{
				return true;
			}
		}
		return false;
	}

    bool isAffinelyDependent() const 
	{
        MT_Scalar sum(0.0);
       	int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1)
		{
            if (contains(m_all_bits, bit))
            {
                sum += m_det[m_all_bits][i];
			}
		}
		return sum <= MT_Scalar(0.0);
	}


	void addVertex(const MT_Vector3& w) 
	{
		assert(!fullSimplex());
		m_last = 0;
        m_last_bit = 0x1;
        while (contains(m_bits, m_last_bit)) 
		{ 
			++m_last; 
			m_last_bit <<= 1; 
		}
		m_y[m_last] = w;
		m_ylen2[m_last] = w.length2();
        m_all_bits = m_bits | m_last_bit;

		update_cache();
		compute_det();
	}

	void addVertex(const MT_Vector3& w, const MT_Point3& p, const MT_Point3& q)
	{
		addVertex(w);
		m_p[m_last] = p;
		m_q[m_last] = q;
	}

	int getSimplex(MT_Point3 *pBuf, MT_Point3 *qBuf, MT_Vector3 *yBuf) const 
	{
		int num_verts = 0;
		int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1) 
		{
			if (contains(m_bits, bit)) 
			{
				pBuf[num_verts] = m_p[i];
				qBuf[num_verts] = m_q[i];
				yBuf[num_verts] = m_y[i];
				
#ifdef DEBUG
				std::cout << "Point " << i << " = " << m_y[i] << std::endl;
#endif
				
				++num_verts;
			}
		}
		return num_verts;
    }

	void compute_points(MT_Point3& p1, MT_Point3& p2) 
	{
		MT_Scalar sum = MT_Scalar(0.0);
		p1.setValue(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));
		p2.setValue(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));
		int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1) 
		{
			if (contains(m_bits, bit))
			{
				sum += m_det[m_bits][i];
				p1 += m_p[i] * m_det[m_bits][i];
				p2 += m_q[i] * m_det[m_bits][i];
			}
		}

		assert(sum > MT_Scalar(0.0));
		MT_Scalar s = MT_Scalar(1.0) / sum;
		p1 *= s;
		p2 *= s;
	}

	bool closest(MT_Vector3& v) 
	{
#ifdef FAST_CLOSEST
		T_Bits s;
		for (s = m_bits; s != 0x0; --s)
		{
			if (subseteq(s, m_bits) && valid(s | m_last_bit)) 
			{
				m_bits = s | m_last_bit;
				v = compute_vector(m_bits);
				return true;
			}
		}
		if (valid(m_last_bit)) 
		{
			m_bits = m_last_bit;
			m_maxlen2 = m_ylen2[m_last];
			v = m_y[m_last];
			return true;
		}
#else
		T_Bits s;
		for (s = m_all_bits; s != 0x0; --s)
		{
			if (subseteq(s, m_all_bits) && valid(s)) 
			{
				m_bits = s;
				v = compute_vector(m_bits);
				return true;
			}
		}
#endif
		
		// Original GJK calls the backup procedure at this point.
#ifdef USE_BACKUP_PROCEDURE
        backup_closest(v); 
#endif
		return false;  
	}

	void backup_closest(MT_Vector3& v)
	{ 		
		MT_Scalar min_dist2 = MT_INFINITY;
		
      T_Bits s;
		for (s = m_all_bits; s != 0x0; --s) 
		{
			if (subseteq(s, m_all_bits) && proper(s))
			{	
				MT_Vector3 u = compute_vector(s);
				MT_Scalar dist2 = u.length2();
				if (dist2 < min_dist2)
				{
					min_dist2 = dist2;
					m_bits = s;
					v = u;
				}
			}
		}
	}
	
	MT_Scalar maxVertex() const { return m_maxlen2; }


private:
	void update_cache();
	void compute_det();

	bool valid(T_Bits s) 
	{
		int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1)
		{
			if (contains(m_all_bits, bit)) 
			{
				if (contains(s, bit)) 
				{
					if (m_det[s][i] <= MT_Scalar(0.0)) 
					{
						return false; 
					}
				}
				else if (m_det[s | bit][i] > MT_Scalar(0.0))
				{ 
					return false;
				}
			}
		}
		return true;
	}

	bool proper(T_Bits s)
	{ 
		int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1)
		{
			if (contains(s, bit) && m_det[s][i] <= MT_Scalar(0.0))
			{
				return false; 
			}
		}
		return true;
	}

	MT_Vector3 compute_vector(T_Bits s) 
	{
        MT_Vector3 v(MT_Scalar(0.0), MT_Scalar(0.0), MT_Scalar(0.0));
		m_maxlen2 = MT_Scalar(0.0);

        MT_Scalar sum = MT_Scalar(0.0);
		int i;
		T_Bits bit;
		for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1) 
		{
			if (contains(s, bit))
			{
				sum += m_det[s][i];
				GEN_set_max(m_maxlen2, m_ylen2[i]);
				v += m_y[i] * m_det[s][i];
			}
		}
		
		assert(sum > MT_Scalar(0.0));

		return v / sum;
	}
 
private:
	MT_Scalar	m_det[16][4]; // cached sub-determinants
    MT_Vector3	m_edge[4][4];

#ifdef JOHNSON_ROBUST
    MT_Scalar	m_norm[4][4];
#endif

	MT_Point3	m_p[4];    // support points of object A in local coordinates 
	MT_Point3	m_q[4];    // support points of object B in local coordinates 
	MT_Vector3	m_y[4];   // support points of A - B in world coordinates
	MT_Scalar	m_ylen2[4];   // Squared lengths support points y

	MT_Scalar	m_maxlen2; // Maximum squared length to a vertex of the current 
	                      // simplex

	T_Bits		m_bits;      // identifies current simplex
	T_Bits		m_last;      // identifies last found support point
	T_Bits		m_last_bit;  // m_last_bit == 0x1 << last
	T_Bits		m_all_bits;  // m_all_bits == m_bits  | m_last_bit 
};




inline void DT_GJK::update_cache() 
{
	int i;
	T_Bits bit;
    for (i = 0, bit = 0x1; i < 4; ++i, bit <<= 1)
	{
        if (contains(m_bits, bit)) 
		{
			m_edge[i][m_last] = m_y[i] - m_y[m_last];
			m_edge[m_last][i] = -m_edge[i][m_last];

#ifdef JOHNSON_ROBUST
			m_norm[i][m_last] = m_norm[m_last][i] = m_edge[i][m_last].length2();
#endif

		}
	}
}

#ifdef JOHNSON_ROBUST

inline void DT_GJK::compute_det() 
{
    m_det[m_last_bit][m_last] = MT_Scalar(1.0);

    if (m_bits != 0x0)
    {
	int i;
	T_Bits si;
    for (i = 0, si = 0x1; i < 4; ++i, si <<= 1) 
	{
        if (contains(m_bits, si)) 
		{
            T_Bits s2 = si | m_last_bit;
            m_det[s2][i] = m_edge[m_last][i].dot(m_y[m_last]); 
            m_det[s2][m_last] = m_edge[i][m_last].dot(m_y[i]);

			int j;
			T_Bits sj;
            for (j = 0, sj = 0x1; j < i; ++j, sj <<= 1) 
			{
                if (contains(m_bits, sj)) 
				{
					int k;
                    T_Bits s3 = sj | s2;			
					
					k = m_norm[i][j] < m_norm[m_last][j] ? i : m_last;
                    m_det[s3][j] = m_det[s2][i] * m_edge[k][j].dot(m_y[i]) + 
                                   m_det[s2][m_last] * m_edge[k][j].dot(m_y[m_last]);
					k = m_norm[j][i] < m_norm[m_last][i] ? j : m_last;
                    m_det[s3][i] = m_det[sj|m_last_bit][j] * m_edge[k][i].dot(m_y[j]) +  
                                   m_det[sj|m_last_bit][m_last] * m_edge[k][i].dot(m_y[m_last]);
					k = m_norm[i][m_last] < m_norm[j][m_last] ? i : j;
                    m_det[s3][m_last] = m_det[sj|si][j] * m_edge[k][m_last].dot(m_y[j]) + 
                                        m_det[sj|si][i] * m_edge[k][m_last].dot(m_y[i]);
                }
            }
        }
    }

    if (m_all_bits == 0xf) 
	{
		int k;

		k = m_norm[1][0] < m_norm[2][0] ? (m_norm[1][0] < m_norm[3][0] ? 1 : 3) : (m_norm[2][0] < m_norm[3][0] ? 2 : 3);
		
        m_det[0xf][0] = m_det[0xe][1] * m_edge[k][0].dot(m_y[1]) + 
                        m_det[0xe][2] * m_edge[k][0].dot(m_y[2]) + 
                        m_det[0xe][3] * m_edge[k][0].dot(m_y[3]);

		k = m_norm[0][1] < m_norm[2][1] ? (m_norm[0][1] < m_norm[3][1] ? 0 : 3) : (m_norm[2][1] < m_norm[3][1] ? 2 : 3);
		
        m_det[0xf][1] = m_det[0xd][0] * m_edge[k][1].dot(m_y[0]) + 
                        m_det[0xd][2] * m_edge[k][1].dot(m_y[2]) +
                        m_det[0xd][3] * m_edge[k][1].dot(m_y[3]);

		k = m_norm[0][2] < m_norm[1][2] ? (m_norm[0][2] < m_norm[3][2] ? 0 : 3) : (m_norm[1][2] < m_norm[3][2] ? 1 : 3);
		
        m_det[0xf][2] = m_det[0xb][0] * m_edge[k][2].dot(m_y[0]) + 
                        m_det[0xb][1] * m_edge[k][2].dot(m_y[1]) +  
                        m_det[0xb][3] * m_edge[k][2].dot(m_y[3]);

		k = m_norm[0][3] < m_norm[1][3] ? (m_norm[0][3] < m_norm[2][3] ? 0 : 2) : (m_norm[1][3] < m_norm[2][3] ? 1 : 2);
		
        m_det[0xf][3] = m_det[0x7][0] * m_edge[k][3].dot(m_y[0]) + 
                        m_det[0x7][1] * m_edge[k][3].dot(m_y[1]) + 
                        m_det[0x7][2] * m_edge[k][3].dot(m_y[2]);
    }
}
}

#else

inline void DT_GJK::compute_det() 
{
    m_det[m_last_bit][m_last] = MT_Scalar(1.0);

    if (m_bits != 0x0)
    {

#ifdef UNROLL_LOOPS

    if (contains(m_bits, 0x1)) 
    {
        T_Bits s2 = 0x1 | m_last_bit;
        m_det[s2][0] = m_edge[m_last][0].dot(m_y[m_last]); 
        m_det[s2][m_last] = m_edge[0][m_last].dot(m_y[0]);
    }
    
    if (contains(m_bits, 0x2)) 
    {
        T_Bits s2 = 0x2 | m_last_bit;
        m_det[s2][1] = m_edge[m_last][1].dot(m_y[m_last]); 
        m_det[s2][m_last] = m_edge[1][m_last].dot(m_y[1]);
     
        if (contains(m_bits, 0x1)) 
        {
            T_Bits s3 = 0x1 | s2;
            T_Bits t2 = 0x1 | m_last_bit;
            m_det[s3][0] = m_det[s2][1] * m_edge[1][0].dot(m_y[1]) + m_det[s2][m_last] * m_edge[1][0].dot(m_y[m_last]);
            m_det[s3][1] = m_det[t2][0] * m_edge[0][1].dot(m_y[0]) + m_det[t2][m_last] * m_edge[0][1].dot(m_y[m_last]);
            m_det[s3][m_last] = m_det[0x3][0] * m_edge[0][m_last].dot(m_y[0]) + m_det[0x3][1] * m_edge[0][m_last].dot(m_y[1]);
        }
    }

    if (contains(m_bits, 0x4)) 
    {
        T_Bits s2 = 0x4 | m_last_bit;
        m_det[s2][2] = m_edge[m_last][2].dot(m_y[m_last]); 
        m_det[s2][m_last] = m_edge[2][m_last].dot(m_y[2]);
       
        if (contains(m_bits, 0x1)) 
        {
            T_Bits s3 = 0x1 | s2;
            T_Bits t2 = 0x1 | m_last_bit;
            m_det[s3][0] = m_det[s2][2] * m_edge[2][0].dot(m_y[2]) + m_det[s2][m_last] * m_edge[2][0].dot(m_y[m_last]);
            m_det[s3][2] = m_det[t2][0] * m_edge[0][2].dot(m_y[0]) + m_det[t2][m_last] * m_edge[0][2].dot(m_y[m_last]);
            m_det[s3][m_last] = m_det[0x5][0] * m_edge[0][m_last].dot(m_y[0]) + m_det[0x5][2] * m_edge[0][m_last].dot(m_y[2]);
        }

        if (contains(m_bits, 0x2)) 
        {
            T_Bits s3 = 0x2 | s2;
            T_Bits t2 = 0x2 | m_last_bit;
            m_det[s3][1] = m_det[s2][2] * m_edge[2][1].dot(m_y[2]) + m_det[s2][m_last] * m_edge[2][1].dot(m_y[m_last]);
            m_det[s3][2] = m_det[t2][1] * m_edge[1][2].dot(m_y[1]) + m_det[t2][m_last] * m_edge[1][2].dot(m_y[m_last]);
            m_det[s3][m_last] = m_det[0x6][1] * m_edge[1][m_last].dot(m_y[1]) + m_det[0x6][2] * m_edge[1][m_last].dot(m_y[2]);
        }
    }

    if (contains(m_bits, 0x8)) 
    {
        T_Bits s2 = 0x8 | m_last_bit;
        m_det[s2][3] = m_edge[m_last][3].dot(m_y[m_last]); 
        m_det[s2][m_last] = m_edge[3][m_last].dot(m_y[3]);
        
        if (contains(m_bits, 0x1)) 
        {
            T_Bits s3 = 0x1 | s2;
            T_Bits t2 = 0x1 | m_last_bit;
            m_det[s3][0] = m_det[s2][3] * m_edge[3][0].dot(m_y[3]) + m_det[s2][m_last] * m_edge[3][0].dot(m_y[m_last]);
            m_det[s3][3] = m_det[t2][0] * m_edge[0][3].dot(m_y[0]) + m_det[t2][m_last] * m_edge[0][3].dot(m_y[m_last]);
            m_det[s3][m_last] = m_det[0x9][0] * m_edge[0][m_last].dot(m_y[0]) + m_det[0x9][3] * m_edge[0][m_last].dot(m_y[3]);
        }

        if (contains(m_bits, 0x2)) 
        {
            T_Bits s3 = 0x2 | s2;
            T_Bits t2 = 0x2 | m_last_bit;
            m_det[s3][1] = m_det[s2][3] * m_edge[3][1].dot(m_y[3]) + m_det[s2][m_last] * m_edge[3][1].dot(m_y[m_last]);
            m_det[s3][3] = m_det[t2][1] * m_edge[1][3].dot(m_y[1]) + m_det[t2][m_last] * m_edge[1][3].dot(m_y[m_last]);
            m_det[s3][m_last] = m_det[0xa][1] * m_edge[1][m_last].dot(m_y[1]) + m_det[0xa][3] * m_edge[1][m_last].dot(m_y[3]);
        }

        if (contains(m_bits, 0x4)) 
        {
            T_Bits s3 = 0x4 | s2;
            T_Bits t2 = 0x4 | m_last_bit;
            m_det[s3][2] = m_det[s2][3] * m_edge[3][2].dot(m_y[3]) + m_det[s2][m_last] * m_edge[3][2].dot(m_y[m_last]);
            m_det[s3][3] = m_det[t2][2] * m_edge[2][3].dot(m_y[2]) + m_det[t2][m_last] * m_edge[2][3].dot(m_y[m_last]);
            m_det[s3][m_last] = m_det[0xc][2] * m_edge[2][m_last].dot(m_y[2]) + m_det[0xc][3] * m_edge[2][m_last].dot(m_y[3]);
        }
        
    }
   

#else
    
	int i;
	T_Bits si;
    for (i = 0, si = 0x1; i < 4; ++i, si <<= 1) 
	{
        if (contains(m_bits, si)) 
		{
            T_Bits s2 = si | m_last_bit;
            m_det[s2][i] = m_edge[m_last][i].dot(m_y[m_last]); 
            m_det[s2][m_last] = m_edge[i][m_last].dot(m_y[i]);

			int j;
			T_Bits sj;
            for (j = 0, sj = 0x1; j < i; ++j, sj <<= 1)
			{
                if (contains(m_bits, sj)) 
				{
                    T_Bits s3 = sj | s2;
                    m_det[s3][j] = m_det[s2][i] * m_edge[i][j].dot(m_y[i]) + 
                                   m_det[s2][m_last] * m_edge[i][j].dot(m_y[m_last]);
                    m_det[s3][i] = m_det[sj|m_last_bit][j] * m_edge[j][i].dot(m_y[j]) +  
                                   m_det[sj|m_last_bit][m_last] * m_edge[j][i].dot(m_y[m_last]);
                    m_det[s3][m_last] = m_det[sj|si][j] * m_edge[j][m_last].dot(m_y[j]) + 
                                        m_det[sj|si][i] * m_edge[j][m_last].dot(m_y[i]);
                }
            }
        }
    }

#endif

    if (m_all_bits == 0xf) 
	{
        m_det[0xf][0] = m_det[0xe][1] * m_edge[1][0].dot(m_y[1]) + 
                        m_det[0xe][2] * m_edge[1][0].dot(m_y[2]) + 
                        m_det[0xe][3] * m_edge[1][0].dot(m_y[3]);
        m_det[0xf][1] = m_det[0xd][0] * m_edge[0][1].dot(m_y[0]) + 
                        m_det[0xd][2] * m_edge[0][1].dot(m_y[2]) +
                        m_det[0xd][3] * m_edge[0][1].dot(m_y[3]);
        m_det[0xf][2] = m_det[0xb][0] * m_edge[0][2].dot(m_y[0]) + 
                        m_det[0xb][1] * m_edge[0][2].dot(m_y[1]) +  
                        m_det[0xb][3] * m_edge[0][2].dot(m_y[3]);
        m_det[0xf][3] = m_det[0x7][0] * m_edge[0][3].dot(m_y[0]) + 
                        m_det[0x7][1] * m_edge[0][3].dot(m_y[1]) + 
                        m_det[0x7][2] * m_edge[0][3].dot(m_y[2]);
    }
}
}

#endif

#endif
