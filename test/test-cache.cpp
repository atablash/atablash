#include <atablash/cache.hpp>

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace ab;

using namespace std::string_literals;


TEST(TestCache, DefaultLoadFunction) {
	Cache<std::string, std::string> cache;
	
	auto pstr = cache.get_or_load("test69");
	EXPECT_EQ("test69"s, *pstr);
}




class Test_object {
public:
	template<class STR>
	Test_object(STR&& str) {
		names.emplace_back(std::forward<STR>(str));
	}
	
	const std::string& get_name() const {
		if(names.empty()) throw std::logic_error("object has not loaded");
		return names.back();
	}
	
	int get_number_of_loads() const {
		return names.size();
	}
	
private:
	std::vector<std::string> names;
};



TEST(TestCache, DoesNotReload) {
	Cache<std::string, Test_object> cache;
	
	cache.get_or_load("test699");
	auto obj = cache.get_or_load("test699");
	
	EXPECT_EQ(1, obj->get_number_of_loads());
}



TEST(TestCache, CustomLoadFunction) {
	Cache<std::string, int> cache([](const std::string& uri){return (int)uri.size();});
	
	auto obj = cache.get_or_load("1234567");
	
	EXPECT_EQ(7, *obj);
}




int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

