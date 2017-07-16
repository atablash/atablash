#include <atablash/threads/dispatch-thread.hpp>

#include <gtest/gtest.h>


using namespace ab;


TEST(TestDispatchThread, NoOp) {
	Dispatch_thread dt;
}


TEST(TestDispatchThread, SimpleSyncJob) {
	Dispatch_thread dt;
	int value = 0;
	dt.dispatch_sync([&value](){value = 69;});
	EXPECT_EQ(69, value);
}


TEST(TestDispatchThread, WeakJobAndKill) {

	int value = 0;
		
	{
		Dispatch_thread dt;
		dt.stop();
	
		dt.dispatch([&value](){value = 69;}, false);
	}
	
	EXPECT_EQ(0, value);
}


TEST(TestDispatchThread, StrongJobAndKill) {

	int value = 0;
		
	{
		Dispatch_thread dt;
		dt.stop();
	
		dt.dispatch([&value](){value = 69;}, true);
	}
	
	EXPECT_EQ(69, value);
}


TEST(TestDispatchThread, Priority1) {

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
		
	{
		Dispatch_thread dt;
		dt.stop();
	
		dt.dispatch([&value1](){value1 = 69;}, true, 1003);
		dt.dispatch([&value1,&value2](){value2 = value1;}, true, 1002);
		dt.dispatch([&value2,&value3](){value3 = value2;}, true, 1001);
	}
	
	EXPECT_EQ(69, value3);
}


TEST(TestDispatchThread, Priority2) {

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
		
	{
		Dispatch_thread dt;
		dt.stop();
	
		dt.dispatch([&value2,&value3](){value3 = value2;}, true, 1001);
		dt.dispatch([&value1](){value1 = 69;}, true, 1003);
		dt.dispatch([&value1,&value2](){value2 = value1;}, true, 1002);
	}
	
	EXPECT_EQ(69, value3);
}


TEST(TestDispatchThread, Priority3) {

	int value1 = 0;
	int value2 = 0;
	int value3 = 0;
		
	{
		Dispatch_thread dt;
		dt.stop();
	
		dt.dispatch([&value2,&value3](){value3 = value2;}, true, 1001);
		dt.dispatch([&value1,&value2](){value2 = value1;}, true, 1002);
		dt.dispatch([&value1](){value1 = 69;}, true, 1003);
	}
	
	EXPECT_EQ(69, value3);
}


TEST(TestDispatchThread, Delay) {

	int value = 0;
	
	using namespace std::chrono;
	using clock = steady_clock;
		
	auto t0 = clock::time_point();
	
	const auto delay = 100ms;
	
	{
		Dispatch_thread dt;
		
		t0 = clock::now();
	
		dt.dispatch_after([&value](){value = 69;}, delay, true);
		
	}
	
	auto time_diff = clock::now() - t0;
	
	EXPECT_EQ(69, value);
	EXPECT_GE(time_diff, delay);
}


//
// TODO: test dispatch_repeating
//


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


