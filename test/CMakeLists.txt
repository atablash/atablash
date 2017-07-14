


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



# make #include<atablash/...> work
include_directories( SYSTEM
	../include
)




add_executable(
	test-dispatch-thread
	test-dispatch-thread.cpp
)

target_link_libraries(
	test-dispatch-thread
	${CMAKE_THREAD_LIBS_INIT} # required by GoogleTest
	${GTEST_LIBRARIES}
)

add_test(test-dispatch-thread test-dispatch-thread)

