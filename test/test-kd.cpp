#include <gtest/gtest.h>

#include <atablash/kd.hpp>


using namespace ab;



using Vec3 = Eigen::Matrix<double, 3, 1>;
using Vec3_noalign = Eigen::Matrix<double, 3, 1, Eigen::DontAlign>;



template<class KD>
void insert_grid(KD& kd) {

	std::vector<Vec3_noalign> v;

	for(int x=0; x<10; ++x) {
		for(int y=0; y<10; ++y) {
			for(int z=0; z<10; ++z) {
				v.emplace_back(x,y,z);
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

	std::vector<std::pair<Vec3_noalign,int>> v;

	for(int x=0; x<10; ++x) {
		for(int y=0; y<10; ++y) {
			for(int z=0; z<10; ++z) {
				v.emplace_back(Vec3_noalign{1.0*x, 1.0*y, 1.0*z}, 100*x + 10*y + z);
			}
		}
	}

	std::random_shuffle(v.begin(), v.end());

	for(auto& p : v) {
		kd.insert(p);
	}
}


TEST(Kd, FindClosest) {
	Kd<double, 3> kd;

	insert_grid(kd);

	auto v = kd.find_closest({5.49, 7.88, 1.12});
	ASSERT_NE(nullptr, v);
	EXPECT_EQ(Vec3(5,8,1), *v);
}


TEST(Kd, FindClosestIf) {
	Kd<double, 3> kd;

	insert_grid(kd);

	auto v = kd.find_closest_if({5.49, 7.88, 1.12}, [](auto& x){return x[1] < 7.9;});
	ASSERT_NE(nullptr, v);
	EXPECT_EQ(Vec3(5,7,1), *v);
}



TEST(Kd, FindClosestVal) {
	Kd<double, 3, int> kd;

	insert_grid_keyval(kd);

	auto v = kd.find_closest({1.12, 2.9, 6.9});
	ASSERT_NE(nullptr, v);
	EXPECT_EQ(100*1 + 10*3 + 7, v->second);
}


TEST(Kd, FindKClosest) {
	Kd<double, 3> kd;

	insert_grid(kd);

	auto v = kd.find_k_closest(Vec3{3.4, 5.7, 1.3}, 8);
	ASSERT_EQ(8, v.size());

	Eigen::Matrix<double, 3, 1> r = {0,0,0};
	for(auto& e : v) {
		r += *e;
	}
	r /= v.size();

	EXPECT_FLOAT_EQ(3.5, r[0]);
	EXPECT_FLOAT_EQ(5.5, r[1]);
	EXPECT_FLOAT_EQ(1.5, r[2]);
}


TEST(Kd, FindKClosestIf) {
	Kd<double, 3> kd;

	insert_grid(kd);

	auto v = kd.find_k_closest_if(Vec3{3.4, 5.7, 1.3}, 4, [](auto& x){return x[1] < 5.9;});
	ASSERT_EQ(4, v.size());

	Eigen::Matrix<double, 3, 1> r = {0,0,0};
	for(auto& e : v) {
		r += *e;
	}
	r /= v.size();

	EXPECT_FLOAT_EQ(3.5, r[0]);
	EXPECT_FLOAT_EQ(5.0, r[1]);
	EXPECT_FLOAT_EQ(1.5, r[2]);
}


TEST(Kd, EachAabb) {
	Kd<double, 3> kd;

	insert_grid(kd);

	std::vector<Vec3_noalign> v;

	kd.each_intersect(Eigen::AlignedBox<double, 3>(Vec3{5.5, 5.5, 5.5}, Vec3{7.5, 7.5, 7.5}), [&v](auto& e){
		v.emplace_back(e);
	});

	ASSERT_EQ(8, v.size());

	Eigen::Matrix<double, 3, 1> r = {0,0,0};
	for(auto& e : v) {
		r += e;
	}
	r /= v.size();

	EXPECT_FLOAT_EQ(6.5, r[0]);
	EXPECT_FLOAT_EQ(6.5, r[1]);
	EXPECT_FLOAT_EQ(6.5, r[2]);
}




int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


