#pragma once



namespace ab{

class Timer{
public:
	long double seconds() const {
		auto deltaTime = std::chrono::high_resolution_clock::now() - t0;
		return std::chrono::duration_cast<std::chrono::duration<long double>>(deltaTime).count();
	}
	
private:
	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
};






}


namespace std{
	inline string to_string(const ::ab::Timer& timer){
		return to_string(timer.seconds()) + "s";
	}
}

