#pragma once


// atablash
#include <atablash/threads/dispatch-thread.hpp>

// external
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>


namespace ab{




// loops given function until the object is destroyed
// blocking call to 'join' on destruction
class LoopThreadTimed : public DispatchThread {
public:

	LoopThreadTimed(const std::function<void()>&& _fun) : fun(_fun){
		printf("Creating a LoopThreadTimed.\n");
	}

	~LoopThreadTimed(){
		stopLoop();
	}

	std::chrono::duration<double> desiredPeriod = 1.0 / 60.0;
	std::chrono::duration<double> actualPeriod = 0; // read only

	// when thread lags, attempt to make up losses
	bool fixed = true;
	
	void startLoop(){
		if(loopRunning) return;
		
		loopRunning = true;
		lastTime = std::chrono::high_resolution_clock::now();
		dispatch([this](){
			iteration();
		},-1000);
	}
	
	template<bool block = true>
	void stopLoop(){
		if(!loopRunning)return;
		
		if(block){
			iterationJustStopped = false;
			std::unique_lock<std::mutex> lock(loopMutex);
			loopRunning = false;
			while(!iterationJustStopped){
				loopEvent.wait(lock);
				if(!iterationJustStopped) LOGD("Spurious wakeup!");
			}
		}
		else loopRunning = false;
	}

private:
	LoopThreadTimed(const LoopThreadTimed&) = delete;
	LoopThreadTimed& operator=(const LoopThreadTimed&) = delete;


private:
	mutable std::mutex loopMutex;
	mutable std::condition_variable loopEvent;
	bool loopRunning = false;
	mutable bool iterationJustStopped;
	mutable std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;

	bool shouldStop() const {
		if(loopRunning) return false;
		
		std::unique_lock<std::mutex> lock(loopMutex);
		iterationJustStopped = true;
		loopEvent.notify_one();
		return true;
	}

	void iteration() {
		
		if(shouldStop())return;
			
		fun();
		
		if(shouldStop())return;
		
		auto ctime = std::chrono::high_resolution_clock::now();
		actualPeriod = ctime - lastTime;
		if(actualPeriod < desiredPeriod){
			auto dur = desiredPeriod - actualPeriod;

			dispatchDelayed([this](){
				iteration();
			}, dur, -1000);
		}
		else{
			//LOGI("LoopThreadTimed lags! Actual period: " + std::to_string(actualPeriod.count()) + " / " + std::to_string(desiredPeriod.count()));
			dispatch([this](){
				iteration();
			}, -1000);
		}

		if (fixed) lastTime += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(desiredPeriod);
		else lastTime = ctime;
	}
	
private:
	std::function<void()> fun;
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
