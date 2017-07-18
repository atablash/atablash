#pragma once

#include <unordered_map>
#include <functional>
#include <memory>


namespace ab {



//
// A cache that caches objects of a given type T
// * Elements are identified using URI identifier object (string?)
// * An optional load_function can be specified, which loads T given URI
// * It returns const T* - the idea is not to modify its objects (it's a cache after all)
// * It doesn't move around Ts once created
// * currently modified to return shared_ptrs - it gives more possibilities,
//   and performance is irrevelant in case of a cache
//
template<class URI, class T>
class Cache {
public:

	//
	// construct a cache
	// default loading function uses T(const KEY&) constructor
	//
	Cache() : load_function([](const URI& uri){return T(uri);}) {}
	
	template<class FUN>
	Cache(FUN&& load_function_) :
		load_function(std::forward<FUN>(load_function_)) {}
	
	
	template<class FILE_NAME>
	const std::shared_ptr<const T>& get(FILE_NAME&& file_name) {
		auto itr = objects.find(file_name);
		if(itr == objects.end()) {
			itr = objects.emplace(file_name, std::make_shared<const T>(load_function(file_name))).first;
		}
		return itr->second;
	}
	

	// no copies
	Cache(const Cache&) = delete;
	Cache& operator=(const Cache&) = delete;

private:
	std::unordered_map<URI, std::shared_ptr<const T>> objects;
	std::function<T(const URI&)> load_function;
};



}


