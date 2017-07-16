#pragma once

#include <unordered_map>
#include <functional>


namespace ab {



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
	T* get_or_load(FILE_NAME&& file_name) {
		auto itr = objects.find(file_name);
		if(itr == objects.end()) {
			itr = objects.emplace(file_name, load_function(file_name)).first;
		}
		return &itr->second;
	}
	

	// no copies
	Cache(const Cache&) = delete;
	Cache& operator=(const Cache&) = delete;

private:
	std::unordered_map<URI, T> objects;
	std::function<T(const URI&)> load_function;
};



}


