# flags
CXXFLAGS_COMMON = -Wall -Wfatal-errors -Werror -Wshadow -std=c++14 -pedantic -isystem /usr/local/include -Iinclude -fno-exceptions -fno-rtti
#CXXFLAGS += -I/usr/local/include
LDLIBS += -lz

CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -DNDEBUG -O2 -mmmx -mavx -msse -msse2 -msse3 -s
CXXFLAGS_DEBUG   = $(CXXFLAGS_COMMON) -DDEBUG -g -ggdb

CXXFLAGS = $(CXXFLAGS_RELEASE)




all: atablash.dll libatablash.a


SRC_ALL = $(wildcard src/**/*.cpp)


libatablash.a: $(SRC_ALL:cpp=o)
	$(AR) rcs $@ $(filter %.o,$^)



atablash.dll: $(SRC_ALL:cpp=o)
	$(CXX) $(CXXFLAGS) -shared -o atablash.dll  $(SRC_ALL:cpp=o) $(LDLIBS) -Wl,--out-implib,libatablash.dll.a


	
debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: all


install:
	cp atablash.dll /usr/local/bin/
	cp libatablash.dll.a /usr/local/lib/
	cp libatablash.a /usr/local/lib/


clean:
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	find . -name "*.so" -delete
	find . -name "*.so.*" -delete
	find . -name "*.a" -delete
	find . -name "*.gch" -delete






