#pragma once

// external glog
#include <glog/logging.h>

// external - std
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <map>
#include <functional>
#include <atomic>




namespace ab{



// (requires c++11)
// Handler thread that supports following operations:
//
// dispatch(<function>, priority = 1000) - run function asynchronously
// 				with given priority (higher priority processed first)
//
// dispatch_delayed(<function>, delay, priority = 1000) - run function asynchronously
// 				with given delay and priority
//
// dispatch_sync(<function>, priority = 2000) - run function synchronously
// 				with given priority (blocks until completion)
//
// dispatch_repeating(<function>, ...) - run at intervals (and try to catch up)
// 
// start() - starts thread, causing it to process incoming jobs
// stop() - pauses thread. this function blocks until current job (posted function) is finished
//
// Dispatch_thread runs functions as a whole, can't break in the middle.
// If Dispatch_thread is idle, it takes all scheduled functions that are already delayed,
// and all functions posted for immediate execution,
// and selects the one with the highest priority among them for execution
//
// force - means to run job even if object is being destructed
//
// this object itself is not protected internally from concurrent access
//
//
class Dispatch_thread {
	
	
private:
	using Clock = std::chrono::steady_clock;
	
	using Time_point = std::chrono::time_point<Clock>;
	using Duration = Clock::duration;
	
	
	
	
	
	
	
private:
	//
	// thread state
	//
	
	enum class State{
		STOPPED,
		RUNNING,
		EXITING
	};
	
	// only the inner thread can write this
	std::atomic<State> state = State::RUNNING;
	
	// inner thread can't write this
	std::atomic<State> requested_state = State::RUNNING;

	
	// just lock everything with this one big mutex for now
	std::mutex control_mutex;
	
	//
	// (this object itself is not protected by mutex internally)
	//
	// also protect against multi-threaded use of this object
	//std::mutex access_mutex;
	
	
	// guarded by control_mutex
	// waited on by worker thread to receive `start` or `exit`
	// waited on by this object to receive `just_stopped`
	std::condition_variable state_event;
	
	// guarded by control_mutex
	// waited on by worker thread to receive new job, stop, or exit
	std::condition_variable new_job_event;
	
	
	
	
	
	
public:
	Dispatch_thread()
		: Dispatch_thread("<no-name>") {
	}
	
	
	Dispatch_thread(const std::string& name_) : th(loop, this), name(name_) {
		//LOGD("Creating a Dispatch_thread.");
	}


	~Dispatch_thread(){
		{
			auto lock = std::lock_guard(control_mutex);
		
			// remove force==false from scheduled_for_later
			for(auto itr = scheduled_for_later.begin(); itr != scheduled_for_later.end();){
				if(!itr->second.force) scheduled_for_later.erase(itr++);
				else ++itr;
			}
			
			requested_state = State::EXITING;
			state_event.notify_one();
			new_job_event.notify_one();
		}
		

		th.join();
	}
	

	void start(){
		//lock_guard access_lock(access_mutex);
		
		auto lock = std::lock_guard(control_mutex);
			
		if(state == State::RUNNING) return;
		
		requested_state = State::RUNNING;
		state_event.notify_one();
	}

	
	void stop(bool wait_until_stopped = true){
		//lock_guard access_lock(access_mutex);
		
		auto lock = std::unique_lock(control_mutex);
		
		if(state == State::STOPPED) return;
		
		requested_state = State::STOPPED;
		new_job_event.notify_one();
		

		if(wait_until_stopped){
			state_event.wait(lock, [this](){return state == State::STOPPED;});
		}
	}
	
	
	// async dispatch immediately with priority
	template<class FUN>
	void dispatch(FUN&& fun, bool force = true, int priority = 1000){
		
		Job job;
		job.priority = priority;
		job.fun = std::forward<FUN>(fun);
		job.force = force;
		
		auto lock = std::unique_lock(control_mutex);
		
		scheduled.push(std::move(job));
		new_job_event.notify_one();
	}
	
	
	// async dispatch delayed
	template<class FUN, class DUR>
	void dispatch_delayed(FUN&& fun, DUR&& duration, bool force = false, int priority = 1000){
		auto dest_time_point = Clock::now() + duration;
		
		Job job;
		job.priority = priority;
		job.fun = std::forward<FUN>(fun);
		job.force = force;
		
		job.scheduled_for = dest_time_point;
	
		auto lock = std::lock_guard(control_mutex);
		
		scheduled_for_later.insert(std::make_pair(
			dest_time_point, std::move(job)));
			
		new_job_event.notify_one();
	}
	
	
	// synchronous dispatch with priority
	template<class FUN>
	void dispatch_sync(FUN&& fun, int priority = 2000){
		auto mut = std::mutex();
		auto cvar = std::condition_variable();
		
		auto lock = std::unique_lock(mut);
		auto dispatch_running = std::atomic(true);
		
		dispatch([&](){
		
			fun();
			
			auto inner_lock = std::lock_guard(mut);
			dispatch_running = false;
			cvar.notify_one();
		}, priority);
		
		cvar.wait(lock, [&dispatch_running](){return !dispatch_running;});
	}
	
	
	template<class FUN, class DUR>
	void dispatch_repeating(FUN&& fun, DUR&& interval, int priority = 1000){
		
		Job job;
		job.priority = priority;
		job.fun = std::forward(fun);
		job.force = false;
		
		job.repeating = true;
		//job.catch_up = catch_up;
		job.interval = std::forward(interval);
		
		auto lock = std::unique_lock(control_mutex);
		
		scheduled.push(std::move(job));
		new_job_event.notify_one();
	}
	
	
	std::thread::id id() const { return th.get_id(); }

private:
	Dispatch_thread(const Dispatch_thread&) = delete;
	Dispatch_thread& operator=(const Dispatch_thread&) = delete;


	
	
	
	
	
	//
	// queues
	//
	
	struct Job{
		int priority;
		std::function<void()> fun;
		
		bool force = false; // run even when object is being destructed
		
		// repeating jobs
		bool repeating = false;
		//bool catch_up = true; // similar to Unity's FixedUpdate behavior
		Duration interval;
		
		Time_point scheduled_for;
		
		// compare by priority
		inline bool operator<(const Job& o) const {
			return priority < o.priority;
		}
	};
	
	std::priority_queue<Job> scheduled;
	
	std::multimap<Time_point, Job> scheduled_for_later;




	//
	// thread function
	//
	static void loop(Dispatch_thread* lt){
	
		const_cast<Dispatch_thread*&>(this_thread()) = lt;
		
		DLOG(INFO) << "Dispatch_thread " << lt->name << " is starting.";
		
		// main loop
		for(;;){
		
			// function to be called next
			std::function<void()> fun;
		
			// lock control_mutex
			{
				auto lock = std::unique_lock(lt->control_mutex);
				
				
				//
				// are we stopped?
				//
				if(lt->requested_state == State::STOPPED){
					lt->state = State::STOPPED;
					
					// notify stop() that execution just stopped
					lt->state_event.notify_all();
					
					lt->state_event.wait(lock, [lt](){return lt->requested_state != State::STOPPED;});
				}
				
				
				lt->state = lt->requested_state.load();
			
			
				//
				// wait for new job or scheduled job
				// continue from beginning if requested state has changed (to stopped or exiting)
				//
				{
					bool state_change = false;
					bool ready_to_exit = false;
					for(;;){
						
						// if exiting, ignore jobs with force == false
						if(lt->requested_state == State::EXITING){
							while(!lt->scheduled.empty() && !lt->scheduled.top().force){
								lt->scheduled.pop();
							}
						}
						
						// got scheduled job!
						if(!lt->scheduled.empty()) break;
						
						// got scheduled_for_later job!
						auto ctime = Clock::now();
						if(!lt->scheduled_for_later.empty() && lt->scheduled_for_later.begin()->first <= ctime) break;
						
						// if no more jobs, we can exit
						if(lt->requested_state == State::EXITING && lt->scheduled_for_later.empty()){
							ready_to_exit = true;
							break;
						}
						
						if(lt->scheduled_for_later.empty()){
							lt->new_job_event.wait(lock);
						}
						else{
							lt->new_job_event.wait_until(lock, lt->scheduled_for_later.begin()->first);
						}
						
						// here we have (at least) one of:
						// * requested_state changed
						// * got new job
						// * spurious wakeup
						
						if(lt->requested_state != lt->state){
							state_change = true;
							break;
						}
					}
					
					if(state_change){
						continue;
					}
					
					if(ready_to_exit){
						break;
					}
				}
				
				// at this point we should have job in either `scheduled` or `scheduled_for_later` ready to run
				
				// move scheduled_for_later to scheduled
				{
					static_assert(Clock::is_steady); // we need this in current implementation!
					auto ctime = Clock::now();
					while(!lt->scheduled_for_later.empty() && lt->scheduled_for_later.begin()->first <= ctime){
						lt->scheduled.push(std::move(lt->scheduled_for_later.begin()->second));
						lt->scheduled_for_later.erase(lt->scheduled_for_later.begin());
					}
				}
				
				// at this point we should have job in `scheduled` ready to run
				DCHECK(!lt->scheduled.empty());
				
				
				if(lt->requested_state == State::EXITING){
					DCHECK(lt->scheduled.top().force) << "if we're exiting, this job should have force == true";
				}
				
				
				
				
				//
				// have next job
				//
				
				
				// const-cast to move out priority_queue element
				auto job = std::move(const_cast<Job&>(lt->scheduled.top()));
				lt->scheduled.pop();
				
				
				if(job.repeating){
					
					// schedule next invocation
					
					job.scheduled_for += job.interval;
					
					auto scheduled_for = job.scheduled_for;
					
					lt->scheduled_for_later.emplace(
						scheduled_for,
						std::move(job)
					);
				}
				else{
					fun = std::move(job.fun);
				}
					
				
				
			} // lock control_mutex
			
			fun(); // do job
		}
		
		DLOG(INFO) << "Dispatch_thread " << lt->name << " is exiting.";
	}



public:
	std::string name;


private:
	std::thread th;

public:
	static Dispatch_thread* const& this_thread(){
		static thread_local Dispatch_thread* t = nullptr;
		return t;
	}
};





}


