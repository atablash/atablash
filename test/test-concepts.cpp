#include <gtest/gtest.h>

#include <atablash/concepts.hpp>

#include <vector>

using namespace ab;


template<class T>
struct Tester {
	static bool has_clone() requires HasClone<T> {
		return true;
	}
	
	static bool has_clone() requires !HasClone<T> {
		return false;
	}
};




struct X {
	X* clone() const;
};


TEST(HasClone, SimpleTrue) {
	EXPECT_TRUE(Tester<X>::has_clone());
}





struct Y {
	X* Clone() const;
};

TEST(HasClone, SimpleFalse) {
	EXPECT_FALSE(Tester<Y>::has_clone());
}




//
// in this case B has clone, even though it should override
//
struct A {
	A* clone() const;
};

struct B : A {
};

TEST(HasClone, SimpleDerived) {
	EXPECT_TRUE(Tester<B>::has_clone());
}




//
// require const clone method
//
struct Z {
	Z* clone(); // non-const
};
struct ZZ {
	const ZZ* clone() const; // const result - is it wrong?
};

TEST(HasClone, NonConst) {
	EXPECT_FALSE(Tester<Z>::has_clone());
	//EXPECT_FALSE(Tester<ZZ>::has_clone());
}








struct AA {
	virtual AA* clone() const = 0;
};

struct BB : AA {
};

struct CC : BB {
	AA* clone() const override;
};

struct DD : CC {
};

TEST(HasClone, Virtual) {
	EXPECT_TRUE(Tester<AA>::has_clone());
	EXPECT_TRUE(Tester<BB>::has_clone());
	EXPECT_TRUE(Tester<CC>::has_clone());
	EXPECT_TRUE(Tester<DD>::has_clone());
}






struct R {
	void* clone() const;
};

TEST(HasClone, BadReturnType1) {
	EXPECT_FALSE(Tester<R>::has_clone());
}








struct T;

struct S {
	T* clone() const;
};

struct T : S {
};

TEST(HasClone, BadReturnType2) {
	EXPECT_FALSE(Tester<S>::has_clone());
}









int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


