#pragma once




#if not defined __EXCEPTIONS and not defined FUNC_NO_EXCEPTIONS
#define FUNC_NO_EXCEPTIONS
#endif



namespace ab{


#ifndef FUNC_NO_EXCEPTIONS
	struct bad_function_call : std::exception
	{
		const char * what() const FUNC_NOEXCEPT override
		{
			return "Bad function call";
		}
	};
#endif



template<typename>
struct force_function_heap_allocation
	: std::false_type
{
};



}

