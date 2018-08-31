#pragma once

namespace ab {




//
// check if class has valid clone() method
// * the method should be const
// * it should return pointer to a cloned object
// * the pointer can either be raw pointer or smart pointer (usually unique_ptr)
//
template<typename T>
concept bool ClonableUsingClone = requires (const T x) {
	x.clone();
	requires std::is_base_of_v<
		std::remove_reference_t<decltype(*x.clone())>, // base
		T // derived
	>;
};





//
// check if it's ok to use class' copy constructor for cloning
//
template<typename T>
concept bool ClonableUsingConstructor =
	(!std::is_class_v<T> || std::is_final_v<T>)
	&& std::is_copy_constructible_v<T>;






//
// clone object
// * use copy constructor only if class is final
// * if both copy constructor and clone() are applicable, prefer copy contructor
//

// use clone() method
template<class T>
inline auto clone(const T& t)
requires ClonableUsingClone<T> && !ClonableUsingConstructor<T>
{
	return t.clone();
}

// use copy contructor
template<class T>
inline auto clone(const T& t)
requires ClonableUsingConstructor<T>
{
	return std::make_unique<T>(t);
}





} // namespace ab

