#pragma once

namespace ab {



template<typename T>
concept bool HasClone = requires (const T x) {
	x.clone();
	requires std::is_base_of_v<
		std::remove_reference_t<decltype(*x.clone())>, // base
		T // derived
	>;
};




} // namespace ab

