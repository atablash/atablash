#pragma once


namespace ab{


template<class Deri>
struct Singleton{
	static Deri& singleton(){
		static Deri obj;
		return obj;
	}
	
protected:
	Singleton(){}
};

}



