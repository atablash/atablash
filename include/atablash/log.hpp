#pragma once


#include<vector>
#include<unordered_map>


// ?
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<mutex>

// to_string fix for MinGW
#include<atablash/stlext.hpp>

//#define  LOG0(str) 			fprintf(stderr,"%s:%d - " str "\n",__FILE__,__LINE__)
//#define  LOG1(str,...)		fprintf(stderr,"%s:%d - " str "\n",__FILE__,__LINE__,__VA_ARGS__)
//#define LOG(...) VA_GTONE(LOG,__VA_ARGS__)


#define LOGD(str) if(ab::Log::global()) ab::Log::global()->debug(str,__FILE__,__LINE__)
#define LOGI(str) if(ab::Log::global()) ab::Log::global()->info(str,__FILE__,__LINE__)
#define LOGW(str) if(ab::Log::global()) ab::Log::global()->warning(str,__FILE__,__LINE__)
#define LOGE(str) if(ab::Log::global()) ab::Log::global()->error(str,__FILE__,__LINE__)
#define LOGF(str) if(ab::Log::global()) ab::Log::global()->fail(str,__FILE__,__LINE__)

	


namespace ab{


// TODO: czas logowania
// TODO: interfejs do dodawania nazw message levelom, ustawiania nazwy pliku z logiem, czy printowac na stderr, ...
//
// level >= DEBUG powoduje ignorowanie logowania w release
class Log{
public:
	enum{
		DEBUG = 5000,
		INFO = 4000, // default?
		WARNING = 3000,
		ERROR = 2000,
		FAIL = 1000};

public:
	void debug(const std::string& _message, const std::string& file="", int line=0){
		message(DEBUG, _message, file, line);
	}
	void info(const std::string& _message, const std::string& file = "", int line = 0){
		message(INFO, _message, file, line);
	}
	void warning(const std::string& _message, const std::string& file = "", int line = 0){
		message(WARNING, _message, file, line);
	}
	void error(const std::string& _message, const std::string& file = "", int line = 0){
		message(ERROR, _message, file, line);
	}
	void fail(const std::string& _message, const std::string& file = "", int line = 0){
		message(FAIL, _message, file, line);
	}

	// slow
	void flushConsole(){
		// avoid slow cout during lock
		// copy messages to print
		static const uint max_messages_flush = 128;
		std::vector<Message> messages_copy;
		{
			std::lock_guard<std::mutex> lock(mut);
			messages_copy.assign(messages.begin() + numPrinted, messages.begin() + numPrinted + std::min(max_messages_flush, (uint)messages.size() - numPrinted));
		}

		for (uint i = 0; i < messages_copy.size(); ++i){
			std::cout << messageToString(messages_copy[i]) << '\n';
		}
		std::cout.flush();

		numPrinted += messages_copy.size();
	}

	void message(int level, const std::string& messageText, const std::string& file = "", int line = 0){

		#if !defined DEBUG_BUILD
			if (level >= DEBUG) return;
		#endif
		
		std::lock_guard<std::mutex> lock(mut);

		messages.push_back(Message{ level, messageText, file, line });
		
		//if(autoFlush) flushConsole();

		#ifdef DEBUG_BUILD
			if (level <= ERROR){
				#ifdef _MSC_VER
					__debugbreak();
				#endif
				#ifdef __GNUC__
					__builtin_trap();
				#endif
			}
		#endif

	}


	// reference to pointer. client should create the log himself
	static Log*& global(){
		static Log* log = nullptr;
		return log;
	}

public:
	Log(){
		levelNames[DEBUG] = "DEBUG";
		levelNames[INFO] = "INFO";
		levelNames[WARNING] = "WARNING";
		levelNames[ERROR] = "ERROR";
		levelNames[FAIL] = "FAIL";
	}
	~Log(){
		if (filename != ""){
			std::ofstream os(filename);
			for (auto&m : messages){
				os << messageToString(m) << '\n';
			}
		}
	}
	
	//bool autoFlush = false;

private:
	struct Message{
		int level;
		std::string text;
		std::string file;
		int line;
	};
	
	static std::string shortenPath(const std::string& s){
		std::string result = s;
		std::string ss[] = {"include/", "include\\", "src/", "src\\"};
		for(uint i=0; i<sizeof(ss)/sizeof(std::string); ++i){
			auto pos = result.rfind(ss[i]);
			if(pos == std::string::npos) continue;
			
			result = result.substr(pos + ss[i].size(), std::string::npos);
		}
		return result;
	}

	std::string messageToString(const Message& m) const {
		std::string levelName;
		auto r = levelNames.find(m.level);
		if (r == levelNames.end()) levelName = std::to_string(m.level);
		else levelName = r->second;
		std::string result = levelName + ": " + m.text;
		if (m.file != "") result += " at " + shortenPath(m.file) + ":" + std::to_string(m.line);
		return result;
	}

	uint numPrinted = 0;
	std::vector<Message> messages;
	std::mutex mut; // TODO: spinlock

	std::unordered_map<int, std::string> levelNames;
	std::string filename = "atablash.log";

private:
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
};










}



