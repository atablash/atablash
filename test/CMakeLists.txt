


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





add_executable(
	test-cache
	test-cache.cpp
)
target_link_libraries(
	test-cache
	${CMAKE_THREAD_LIBS_INIT} # required by GoogleTest
	${GTEST_LIBRARIES}
)
add_test(test-cache test-cache)

