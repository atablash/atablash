#pragma once


#include <atablash/common.hpp>

#include<map>
#include<iostream>



#if __cplusplus >= 201103L or defined _MSC_VER
#include <unordered_map>
#include <unordered_set>
namespace std{
#else
#include <ext/hash_map>
#include <ext/hash_set>
namespace __gnu_cxx{
#define unordered_map hash_map
#define unordered_multimap hash_multimap
#define unordered_set hash_set
#define unordered_multiset hash_multiset
/*
template<class K, class V> class unordered_map : public hash_map<K,V>{
public:
	typename hash_map<K,V>::iterator erase(const typename hash_map<K,V>::iterator& it){
		typename hash_map<K,V>::iterator r = it;
		++r;
		if(r!=hash_map<K,V>::end()){
			K key = r->first;
			hash_map<K,V>::erase(it);
			return hash_map<K,V>::find(key);
		}
		else{
			hash_map<K,V>::erase(it);
			return r;
		}
	}
	typename hash_map<K,V>::size_type erase(const K& key){
		return hash_map<K,V>::erase(key);
	}
};
*/
#endif

// 1,2,last (!!!!!)
/*template<>*/ template<class A,class B>struct hash<std::map<A,B> >{
    size_t operator()(const std::map<A,B>& m) const{
        size_t r = m.size();
        if(!r)return 0;
        __typeof(m.begin()) i=m.be;
        r^=hash<__typeof(i)>()(i);
        ++i;
        r^=cbitl(hash<__typeof(i)>()(*i),sizeof(r)*8/4);
        i=m.end();
        --i;
        r^=cbitl(hash<__typeof(i)>()(*i),sizeof(r)*8/2);
        return r;
    }
};



// hash pairs:
/*template<>*/ template<class A, class B>struct hash<pair<A,B> >{
    size_t operator()(const pair<A,B>& p) const{
        return hash<A>()(p.first) ^ ab::cbitl(hash<B>()(p.second),sizeof(size_t)*8/2);
    }
};



// hash enums:
//
// TODO: fix for msvc
#if defined __GNUC__ and MIN_GCC(4,8,4)
template<class E> struct hash {
    using sfinae = typename std::enable_if<std::is_enum<E>::value, E>::type;
public:
    size_t operator()(const E&e) const {
      return std::hash<typename std::underlying_type<E>::type>()((typename std::underlying_type<E>::type)e);
    }
};
#endif
  
} // namespace std / __gnu_cxx








// component-wise pair operators
template<class A, class B, class C, class D>
inline std::pair<A,B>& operator+=(std::pair<A,B>& p, const std::pair<C,D>& o){
	p.first+=o.first;
	p.second+=o.second;
	return p;
}
template<class A, class B, class C, class D>
inline std::pair<A,B>& operator-=(std::pair<A,B>& p, const std::pair<C,D>& o){
	p.first-=o.first;
	p.second-=o.second;
	return p;
}
template<class A, class B, class C, class D>
inline std::pair<A,B>& operator*=(std::pair<A,B>& p, const std::pair<C,D>& o){
	p.first*=o.first;
	p.second*=o.second;
	return p;
}
template<class A, class B, class C, class D>
inline std::pair<A,B>& operator/=(std::pair<A,B>& p, const std::pair<C,D>& o){
	p.first/=o.first;
	p.second/=o.second;
	return p;
}

template<class Afi, class Ase, class Bfi, class Bse>
inline std::pair<decltype(Afi() + Bfi()), decltype(Ase() + Bse())>&
operator+(const std::pair<Afi,Ase>& a, const std::pair<Bfi,Bse>& b){
	return std::make_pair(a.fi+b.fi, a.se+b.se);
}
template<class Afi, class Ase, class Bfi, class Bse>
inline std::pair<decltype(Afi() - Bfi()), decltype(Ase() - Bse())>&
operator-(const std::pair<Afi,Ase>& a, const std::pair<Bfi,Bse>& b){
	return std::make_pair(a.fi-b.fi, a.se-b.se);
}
template<class Afi, class Ase, class Bfi, class Bse>
inline std::pair<decltype(Afi()*Bfi()), decltype(Ase()*Bse())>&
operator*(const std::pair<Afi,Ase>& a, const std::pair<Bfi,Bse>& b){
	return std::make_pair(a.fi*b.fi, a.se*b.se);
}
template<class Afi, class Ase, class Bfi, class Bse>
inline std::pair<decltype(Afi() / Bfi()), decltype(Ase() / Bse())>&
operator/(const std::pair<Afi,Ase>& a, const std::pair<Bfi,Bse>& b){
	return std::make_pair(a.fi/b.fi, a.se/b.se);
}








// vector operators
template<class T, class U>
inline std::vector<T>& operator+=(std::vector<T>& v, const std::vector<U>& o){
	ASS(v.siz == o.siz);
	FOR(i,v.siz) v[i] += o[i];
	return v;
}


template<class T>
inline std::vector<T>& operator*=(std::vector<T>& v, const T& a){
	FOR(i,v.siz) v[i] *= a;
	return v;
}





// serialization
template<class A,class B>inline std::ostream&operator<<(std::ostream&o,const std::pair<A,B>&p){o<<'<'<<p.first<<", "<<p.second<<'>';return o;}
template<class A,class B>inline std::ostream&operator<<(std::ostream&o,const std::map<A,B>&m){o<<'\n';FE(k,m)o<<"map["<<k->first<<"] -> "<<k->se<<'\n';return o;}
template<class A,class B>inline std::ostream&operator<<(std::ostream&o,const umap<A,B>&m){o<<'\n';FE(k,m)o<<"umap["<<k->first<<"] -> "<<k->se<<'\n';return o;}
template<class A,class B>inline std::ostream&operator<<(std::ostream&o,const uset<A,B>&_m){o<<"uset{\n";FE(k,_m)o<<*k<<'\n';o<<'\n';return o;}
template<class A>inline std::ostream&operator<<(std::ostream&o,const std::vector<A>&_v){o<<'\n';FOR(i,_v.siz)o<<"vector["<<i<<"] = "<<_v[i]<<"}\n";return o;}





// fix for MinGW: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52015
#ifdef _GLIBCXX_HAVE_BROKEN_VSWPRINTF
namespace std{
	template<class T>
	inline string to_string(const T& t){
		stringstream ss;
		ss << t;
		return ss.str();
	}
}
#endif




