#pragma once

#include<thread>



namespace ab{


// loops given function until the object is destroyed
// blocking call to 'join' on destruction
class LoopThread{
public:

	template<class FUN, class... ARGS>
	LoopThread(const FUN&& fun, ARGS&&... args)
		: th(loop<FUN,ARGS...>, this, fun, args...) {
		printf("Creating a LoopThread.\n");
	}

	~LoopThread(){
		kill = true;
		th.join();
		puts("LoopThread destructor.");
	}


private:
	LoopThread(const LoopThread&) = delete;
	LoopThread& operator=(const LoopThread&) = delete;


private:
	template<class FUN, class... ARGS>
	static void loop(LoopThread* loopThread, FUN fun, ARGS... args){
		while(not loopThread->kill) std::bind(fun,args...)();
	}


public:
	std::string name;


private:
	bool kill = false;
	std::thread th;
};




}


