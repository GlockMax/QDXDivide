
#ifndef BP_ENDPOINTLIST_H
#define BP_ENDPOINTLIST_H

//#define PARANOID

#include <vector>

#include "BP_Endpoint.h"
#include "BP_ProxyList.h"

class BP_Scene;

typedef bool (*T_Overlap)(const BP_Proxy& a, const BP_Proxy& b);

class BP_EndpointList : public std::vector<BP_Endpoint> {
public:
	BP_EndpointList() {}
	
	DT_Index stab(const BP_Endpoint& pos, BP_ProxyList& proxies) const;
	
	DT_Index stab(DT_Scalar pos, BP_ProxyList& proxies) const
	{
		return stab(BP_Endpoint(pos, BP_Endpoint::MINIMUM, 0), proxies);
	}
	

   void range(const BP_Endpoint& min, const BP_Endpoint& max, 
			  DT_Index& first, DT_Index& last, BP_ProxyList& proxies) const;
	
	void range(DT_Scalar lb, DT_Scalar ub, DT_Index& first, DT_Index& last, BP_ProxyList& proxies) const 
	{
		range(BP_Endpoint(lb, BP_Endpoint::MINIMUM, 0), 
			  BP_Endpoint(ub, BP_Endpoint::MAXIMUM, 0),
			  first, last, proxies);
	}
	
	void addInterval(const BP_Endpoint& min, const BP_Endpoint& max, BP_ProxyList& proxies);
	void removeInterval(DT_Index first, DT_Index last, BP_ProxyList& proxies);

	void move(DT_Index index, DT_Scalar pos, Uint32 type, BP_Scene& scene, T_Overlap overlap);	
   
   DT_Scalar nextLambda(DT_Index& index, DT_Scalar source, DT_Scalar target) const;
	

private:
	void encounters(const BP_Endpoint& a, const BP_Endpoint& b,
					    BP_Scene& scene, T_Overlap overlap);


#ifdef PARANOID
	bool invariant() const 
	{
		DT_Count count = 0;
		DT_Index i;
		for (i = 0; i != size(); ++i) 
		{
         const BP_Endpoint& endpoint = (*this)[i];

			if (endpoint.getType() == BP_Endpoint::MINIMUM) 
			{
				++count;
			}
			else 
			{
				--count;
			}
			if (endpoint.getCount() != count)
			{
				return false;
			}
			if (endpoint.getIndex() != i) 
			{
				return false;
			}
		}
		return true;
	}
#else
	bool invariant() const { return true; }
#endif

};



#endif
