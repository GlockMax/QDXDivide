
#ifndef BP_PROXY_H
#define BP_PROXY_H

#include "BP_Endpoint.h"
#include "BP_ProxyList.h"

class BP_Interval {
public:
	BP_Interval() {}
	BP_Interval(BP_Proxy *proxy) :
		m_min(proxy),
		m_max(proxy) 
	{}

	BP_Link m_min;
	BP_Link m_max;
};

class BP_Scene;

class BP_Proxy {
public:
    BP_Proxy(void *object, BP_Scene& scene);

	void add(const DT_Vector3 min,
			 const DT_Vector3 max,
			 BP_ProxyList& proxies);
	
    void remove(BP_ProxyList& proxies);
	
	void setBBox(const DT_Vector3 min, const DT_Vector3 max);
    
    void *getObject() { return m_object; }

	DT_Scalar getMin(int i) const;
	DT_Scalar getMax(int i) const;

private:
	BP_Interval  m_interval[3];
    void        *m_object;
	BP_Scene&    m_scene;
};

inline bool BP_overlap(const BP_Proxy *a, const BP_Proxy *b)
{
	return a->getMin(0) <= b->getMax(0) && b->getMin(0) <= a->getMax(0) && 
		   a->getMin(1) <= b->getMax(1) && b->getMin(1) <= a->getMax(1) &&
		   a->getMin(2) <= b->getMax(2) && b->getMin(2) <= a->getMax(2);
}

#endif




