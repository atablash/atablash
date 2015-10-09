#pragma once

// external - atablash
#include <atablash/debug.hpp>
#include <atablash/log.hpp>
#include <atablash/functional/function-movable.hpp>
#define FUN_TYPE ab::FunctionMovable

// external - std
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <map>




#ifdef _MSC_VER
#define  thread_local __declspec(thread)
#endif


namespace ab{



// priority >= 0 will be executed
// priority <  0 will be dropped if object is destructed
class DispatchThread {
public:

	DispatchThread()
		: th(loop, this) {
		LOGD("Creating a DispatchThread.");
	}
	
	DispatchThread(const std::string& _name) : DispatchThread(){
		name = _name;
	}

	~DispatchThread(){
		LOGD("DispatchThread destructor.");
		{
			std::unique_lock<std::mutex> lock(queuesMut); // required for notify
			
			
			// purge priority<0 from scheduledForLater
			if(!scheduled.empty() && scheduled.top().first < 0) scheduled = decltype(scheduled)();
			
			for(auto itr = scheduledForLater.begin(); itr != scheduledForLater.end();)
				if(itr->second.first < 0) scheduledForLater.erase(itr++);
				else ++itr;
			
			
			std::unique_lock<std::mutex> startStopLock(startStopMut); // required for notify
			
			exit = true;
			
			startStopEvent.notify_one(); // if thread is waiting for start(), let it know we're exiting
			newJobEvent.notify_one();    // if thread is waiting for a job, let it know we're exiting
		}		
		th.join();
	}
	
	/*
	void cancelAll(){
		//kill = true;
		//start_stop.notify_one();
		//th.join();
	}*/

	void start(){ // non-blocking
		if(running) return;

		std::unique_lock<std::mutex> lock(startStopMut); // required
		running = true;
		startStopEvent.notify_one();
	}

	template<bool block = true>
	void stop(){
		if(!running) return;

		if(block){
			justStopped = false;
			std::unique_lock<std::mutex> lock(startStopMut);
			{
				// notify if thread is waiting for job
				std::unique_lock<std::mutex> queuesLock(queuesMut);
				running = false;
				newJobEvent.notify_one();
			}
			while(!justStopped){
				startStopEvent.wait(lock);
				if(!justStopped) LOGD("Spurious wakeup!");
			}
		}
		else running = false;
	}
	
	// async
	void dispatch(FUN_TYPE<void()>&& fun, int priority = 1000){
		std::unique_lock<std::mutex> lock(queuesMut);
		scheduled.push(std::make_pair(priority, std::forward<FUN_TYPE<void()>>(fun)));
		newJobEvent.notify_one();
	}
	
	template<class FUN, class Rep, class Period>
	void dispatchDelayed(FUN&& fun, const std::chrono::duration<Rep,Period>& dur, int priority = 1000){
		std::unique_lock<std::mutex> lock(queuesMut);
		auto destTimePoint = std::chrono::high_resolution_clock::now() + dur;
		scheduledForLater.insert(std::make_pair(std::chrono::time_point_cast<Duration>(destTimePoint), std::make_pair(priority, std::forward<FUN>(fun))));
		newJobEvent.notify_one();
	}
	
	// sync
	void dispatchSync(FUN_TYPE<void()>&& fun, int priority = 2000){
		std::mutex mut;
		std::condition_variable cvar;
		
		std::unique_lock<std::mutex> lock(mut);
		bool dispatchRunning = true;
		
		dispatch([&](){
			fun();
			std::unique_lock<std::mutex> innerLock(mut);
			dispatchRunning = false;
			cvar.notify_one();
		}, priority);
		
		while(dispatchRunning){
			cvar.wait(lock);
			if(dispatchRunning) LOGD("Spurious wakeup!");
		}
	}
	
	std::thread::id id()const{return th.get_id();}

private:
	DispatchThread(const DispatchThread&) = delete;
	DispatchThread& operator=(const DispatchThread&) = delete;


private:
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;
	typedef std::chrono::high_resolution_clock::duration Duration;

	// start/stop
	bool running = true;
	bool justStopped = false;
	std::mutex startStopMut;
	std::condition_variable startStopEvent;
	
	// queues
	std::mutex queuesMut;
	std::condition_variable newJobEvent;
	struct _scheduled_compare{
		bool operator()(const std::pair<int, FUN_TYPE<void()> >& a, const std::pair<int, FUN_TYPE<void()> >& b)const{return a.first < b.first;}
	};
	std::priority_queue<std::pair<int, FUN_TYPE<void()> >, std::vector<std::pair<int, FUN_TYPE<void()>>>, _scheduled_compare > scheduled;
	std::multimap<TimePoint, std::pair<int, FUN_TYPE<void()> > > scheduledForLater;

	static void loop(DispatchThread* lt){
		const_cast<DispatchThread*&>(thisThread()) = lt;
		LOGD("DispatchThread " + lt->name + " is starting.");
		for(;;){
			// start/stop synchronization
			if(!lt->running){
				std::unique_lock<std::mutex> lock(lt->startStopMut);
				
				// notify stop() that execution stopped
				lt->justStopped = true;   lt->startStopEvent.notify_one();
				
				// wait for start() or exit
				while(!lt->running){
					
					// the end
					if(lt->exit && lt->scheduled.empty() && lt->scheduledForLater.empty()) break; // TODO: lock queues?
					
					lt->startStopEvent.wait(lock);
					
					
					if(!lt->running && !(lt->exit && lt->scheduled.empty() && lt->scheduledForLater.empty()))
						LOGD("Spurious wakeup or exit called with jobs left on paused thread");
				}

				// the end
				// Break immediately. Otherwise would do one more iteration.
				// Even worse: thread might be stopped() now
				if(lt->exit && lt->scheduled.empty() && lt->scheduledForLater.empty()) break; // TODO: lock queues?
			}

			// process queues
			FUN_TYPE<void()> fun;
			// queues locked:
			{
				std::unique_lock<std::mutex> lock(lt->queuesMut);
				for(;;){
					// move scheduledForLater to scheduled
					auto ctime = std::chrono::high_resolution_clock::now();
					while(!lt->scheduledForLater.empty() && lt->scheduledForLater.begin()->first <= ctime){
						lt->scheduled.push(std::move(lt->scheduledForLater.begin()->second));
						lt->scheduledForLater.erase(lt->scheduledForLater.begin());
					}
					
					// if exiting and highest priority is less than 0, purge all
					if(lt->exit && !lt->scheduled.empty() && lt->scheduled.top().first < 0) lt->scheduled = decltype(lt->scheduled)();
					
					if(!lt->scheduled.empty() || !lt->running) break;
					
					if(lt->exit && lt->scheduled.empty() && lt->scheduledForLater.empty()) break;
					
					// wait for new job, scheduled job, exit or stop()
					
					if(lt->scheduledForLater.empty()) lt->newJobEvent.wait(lock);
					else lt->newJobEvent.wait_until(lock, lt->scheduledForLater.begin()->first);
					
					// does not detect all spurious wakeups:
					if(lt->running && lt->scheduled.empty() && lt->scheduledForLater.empty()) LOGD("Spurious wakeup or exit() called!");
				}
				
				// the end
				if(lt->exit && lt->scheduled.empty() && lt->scheduledForLater.empty()) break;
				
				// thread is stopping
				if(!lt->running) continue;
				
				
				ASS(!lt->scheduled.empty());
				
				if(lt->scheduled.empty()) continue;
				
				// warning! const_cast! pop() immediately!
				fun = std::move(const_cast<FUN_TYPE<void()>&>(lt->scheduled.top().second));
				lt->scheduled.pop();
			}
			
			// do something
			fun();
		}
		LOGD("DispatchThread " + lt->name + " is exiting.");
	}



public:
	std::string name;


private:
	bool exit = false;
	std::thread th;

public:
	static DispatchThread* const& thisThread(){
		static thread_local DispatchThread* _thisThread;
		return _thisThread;
	}
private:
};





#undef FUN_TYPE




}
