#ifndef BP_PROXYLIST_H
#define BP_PROXYLIST_H

#include <assert.h>

#include <vector>
#include <algorithm>
#include <utility>

class BP_Proxy;

typedef std::pair<BP_Proxy *, unsigned int> BP_ProxyEntry; 

inline bool operator<(const BP_ProxyEntry& a, const BP_ProxyEntry& b) 
{
	return a.first < b.first;
}

class BP_ProxyList : public std::vector<BP_ProxyEntry> {
public:
   BP_ProxyList(size_t n = 20)
   {
      reserve(n); 
   }      

	iterator add(BP_Proxy *proxy) 
	{
		BP_ProxyEntry entry = std::make_pair(proxy, 0);
		iterator it = std::lower_bound(begin(), end(), entry);
		if (it == end() || (*it).first != proxy) 
		{
			it = insert(it, entry);
		}
		++(*it).second;
      assert((*it).second <= 3);
		return it;
	}

   unsigned int operator[](BP_Proxy *proxy)
   {
      BP_ProxyEntry entry = std::make_pair(proxy, 0);
      iterator it = std::lower_bound(begin(), end(), entry);
      return (it == end() || (*it).first != proxy)  ? 0 : (*it).second;
   }

	void remove(BP_Proxy *proxy) 
	{
		BP_ProxyEntry entry = std::make_pair(proxy, 0);
		iterator it = std::lower_bound(begin(), end(), entry);
		if (it != end() && (*it).first == proxy && --(*it).second == 0) 
		{
			erase(it);	
		}	
	}
};

#endif
