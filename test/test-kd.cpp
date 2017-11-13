#include <gtest/gtest.h>

#include <atablash/kd.hpp>


using namespace ab;



using Vec4 = Eigen::Matrix<double, 4, 1>;
using Vec4_noalign = Eigen::Matrix<double, 4, 1, Eigen::DontAlign>;



template<class KD>
void insert_grid(KD& kd) {

	std::vector<Vec4_noalign> v;

	for(int x=0; x<10; ++x) {
		for(int y=0; y<10; ++y) {
			for(int z=0; z<10; ++z) {
				for(int w=0; w<10; ++w) {
					v.emplace_back(x,y,z,w);
				}
			}
		}
	}

	std::random_shuffle(v.begin(), v.end());

	for(auto& p : v) {
		kd.insert(p);
	}
}

template<class KD>
void insert_grid_keyval(KD& kd) {

	std::vector<std::pair<Vec4_noalign,int>> v;

	for(int x=0; x<5; ++x) {
		for(int y=0; y<5; ++y) {
			for(int z=0; z<5; ++z) {
				for(int w=0; w<5; ++w) {
					v.emplace_back(Vec4_noalign{1.0*x, 1.0*y, 1.0*z, 1.0*w}, 1000*x + 100*y + 10*z + w);
				}
			}
		}
	}

	std::random_shuffle(v.begin(), v.end());

	for(auto& p : v) {
		kd.insert(p);
	}
}


TEST(Kd, FindClosest) {
	Kd<double, 4> kd;

	insert_grid(kd);

	auto v = kd.find_closest({1.49, 3.88, 0.12, 2.7});
	ASSERT_NE(nullptr, v);
	EXPECT_EQ(Vec4(1,4,0,3), *v);
}


TEST(Kd, FindClosestIf) {
	Kd<double, 4> kd;

	insert_grid(kd);

	auto v = kd.find_closest_if({0.49, 3.88, 1.12, 2.8}, [](auto& x){return x[1] < 3.9;});
	ASSERT_NE(nullptr, v);
	EXPECT_EQ(Vec4(0,3,1,3), *v);
}



TEST(Kd, FindClosestVal) {
	Kd<double, 4, int> kd;

	insert_grid_keyval(kd);

	auto v = kd.find_closest({1.12, 2.9, 3.9, 0.8});
	ASSERT_NE(nullptr, v);
	EXPECT_EQ(1000*1 + 100*3 + 10*4 + 1, v->second);
}


TEST(Kd, FindKClosest) {
	Kd<double, 4> kd;

	insert_grid(kd);

	auto v = kd.find_k_closest(Vec4{3.4, 3.6, 1.4, 0.1}, 8);
	ASSERT_EQ(8, v.size());

	Eigen::Matrix<double, 4, 1> r = {0,0,0,0};
	for(auto& e : v) {
		r += *e;
	}
	r /= v.size();

	EXPECT_FLOAT_EQ(3.5, r[0]);
	EXPECT_FLOAT_EQ(3.5, r[1]);
	EXPECT_FLOAT_EQ(1.5, r[2]);
	EXPECT_FLOAT_EQ(0.0, r[3]);
}


TEST(Kd, FindKClosestIf) {
	Kd<double, 4> kd;

	insert_grid(kd);

	auto v = kd.find_k_closest_if(Vec4{3.4, 3.7, 1.3, 0.3}, 8, [](auto& x){return x[1] < 3.9;});
	ASSERT_EQ(8, v.size());

	Eigen::Matrix<double, 4, 1> r = {0,0,0,0};
	for(auto& e : v) {
		r += *e;
	}
	r /= v.size();

	EXPECT_FLOAT_EQ(3.5, r[0]);
	EXPECT_FLOAT_EQ(3.0, r[1]);
	EXPECT_FLOAT_EQ(1.5, r[2]);
	EXPECT_FLOAT_EQ(0.5, r[3]);
}


TEST(Kd, EachAabb) {
	Kd<double, 4> kd;

	insert_grid(kd);

	std::vector<Vec4_noalign> v;

	kd.each_intersect(Eigen::AlignedBox<double, 4>(Vec4{1.5, 1.5, 1.5, 1.5}, Vec4{3.5, 3.5, 3.5, 3.5}), [&v](auto& e){
		v.emplace_back(e);
	});

	ASSERT_EQ(16, v.size());

	Eigen::Matrix<double, 4, 1> r = {0,0,0,0};
	for(auto& e : v) {
		r += e;
	}
	r /= v.size();

	EXPECT_FLOAT_EQ(2.5, r[0]);
	EXPECT_FLOAT_EQ(2.5, r[1]);
	EXPECT_FLOAT_EQ(2.5, r[2]);
	EXPECT_FLOAT_EQ(2.5, r[3]);
}




int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


