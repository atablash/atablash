// REPLACED BY LoopThreadTimed

#pragma once


// external
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace ab{




// loops given function until the object is destroyed
// blocking call to 'join' on destruction
class LoopThreadEx {
public:

	template<class FUN, class... ARGS>
	LoopThreadEx(const FUN&& fun, ARGS&&... args)
		: th(iteration<FUN,ARGS...>, this, fun, args...) {
		printf("Creating a LoopThreadEx.\n");
	}

	~LoopThreadEx(){
		kill = true;
		start_stop.notify_one();
		th.join();

		puts("LoopThreadEx destructor.");
	}

	std::chrono::duration<float> desiredPeriod = 1.0f / 60.0f;
	mutable std::chrono::duration<float> actualPeriod = 0; // read only

	void start(){ // non-blocking
		if(running) return;

		std::unique_lock<std::mutex> lock(mut);
		running = true;
		start_stop.notify_one();
	}

	template<bool block = true>
	void stop(){
		if(!running) return;

		if(block){
			std::unique_lock<std::mutex> lock(mut);
			running = false;
			while(!justStopped){
				start_stop.wait(lock);
				if(!justStopped) LOGD("Spurious wakeup!");
			}
			justStopped = false;
		}
		else running = false;
	}

private:
	LoopThreadEx(const LoopThreadEx&) = delete;
	LoopThreadEx& operator=(const LoopThreadEx&) = delete;


private:
	mutable std::mutex startStopMutex;
	mutable std::condition_variable startStopEvent;
	bool running = false;
	mutable bool justStopped = false;
	mutable std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();

	template<class FUN, class... ARGS>
	static void iteration(const LoopThreadEx* lt, const FUN fun, ARGS... args){

		while(not lt->kill){

			// synchronization
			if(!lt->running){
				std::unique_lock<std::mutex> lock(lt->mut);
				
				// notify stop()
				justStopped = true;   lt->start_stop.notify_one();
				
				// wait for start()
				while(!lt->running){
					lt->start_stop.wait(lock);
					if(!lt->running) LOGD("Spurious wakeup!");
				}

				if(lt->kill)break;

				lt->t = std::chrono::high_resolution_clock::now();
			}

			std::bind(fun,args...)();

			auto t1 = std::chrono::high_resolution_clock::now();
			lt->actualPeriod = t1 - lt->t;
			if(lt->actualPeriod < lt->desiredPeriod){
				auto dur = lt->desiredPeriod - lt->actualPeriod;

				#ifdef _MSC_VER
				std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::nanoseconds>(dur));
				#else
				std::this_thread::sleep_for(dur);
				#endif
			}
			lt->t = t1;
		}
	}



public:
	std::string name;


private:
	bool kill = false;
	std::thread th;
};







// Dlaczego to nie działa dla skomplikowanych przypadków?

/*
#include <atablash/threads/loop-thread.hpp>

// loops given function until the object is destroyed
// blocking call to 'join' on destruction
class LoopThreadEx : public LoopThread{
public:

	template<class FUN, class... ARGS>
	LoopThreadEx(const FUN&& fun, ARGS&&... args)
		: LoopThread(iteration<FUN,ARGS...>, this, fun, args...) {
	}

	float desiredPeriod = 1.0f / 60.0f;
	mutable float actualPeriod = 0; // read only

	void start(){ // non-blocking
		if(running) return;

		std::unique_lock<std::mutex> lock(mut);
		running = true;
		start_stop.notify_one();
	}

	void stop(){ // blocking: waits for iteration to finish
		if(!running) return;

		std::unique_lock<std::mutex> lock(mut);
		running = false;
		start_stop.wait(lock);
	}

private:
	LoopThreadEx(const LoopThreadEx&) = delete;
	LoopThreadEx& operator=(const LoopThreadEx&) = delete;


private:
	mutable std::mutex mut;
	mutable std::condition_variable start_stop;
	bool running = false;
	mutable std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now();
	template<class FUN, class... ARGS>
	static void iteration(const LoopThreadEx* lt, const FUN fun, ARGS... args){

		// synchronization
		if(!lt->running){
			std::unique_lock<std::mutex> lock(lt->mut);
			lt->start_stop.notify_one();
			if(!lt->running) lt->start_stop.wait(lock);

			lt->t = std::chrono::high_resolution_clock::now();
		}

		std::bind(fun,args...)();

		auto t1 = std::chrono::high_resolution_clock::now();
		lt->actualPeriod = std::chrono::duration<float>(t1-lt->t).count();
		if(lt->actualPeriod < lt->desiredPeriod){
			std::this_thread::sleep_for(std::chrono::duration<float>(lt->desiredPeriod - lt->actualPeriod));
		}
		lt->t = t1;
	}

};
*/




}
