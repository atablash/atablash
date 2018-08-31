#pragma once




namespace ab{




template<class Derived>
class SafeBool{

private:
    typedef void (SafeBool::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}

public:
	operator bool_type() const {
		return static_cast<const Derived*>(this)->test()
			? &this_type_does_not_support_comparisons : 0;
	}

};





}




