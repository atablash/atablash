#pragma once

namespace ab {



//
// check if class has valid clone() method
// * the method should be const
// * it should return pointer to a cloned object
// * the pointer can either be raw pointer or smart pointer (usually unique_ptr)
//
template<typename T>
concept bool HasClone = requires (const T x) {
	x.clone();
	requires std::is_base_of_v<
		std::remove_reference_t<decltype(*x.clone())>, // base
		T // derived
	>;
};




} // namespace ab

