#pragma once


namespace ab {


//
// singletonize any class
//
// if you want to forbid other instances, make T's constructor private and set Singleton<T> as a friend class
//
template<class T>
class Singleton {
public:

	//
	// get instance and optionally pass arguments for creation
	//
	template<class... Args>
	static T& instance(Args... args) {
		static T t(std::forward<Args>(args)...);
		return t;
	}
};



} // namespace ab

