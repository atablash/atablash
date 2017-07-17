#include <gtest/gtest.h>

#include <atablash/clone.hpp>

#include <vector>

using namespace ab;


template<class T>
struct Tester {
	static bool has_clone() requires ClonableUsingClone<T> {
		return true;
	}
	
	static bool has_clone() requires !ClonableUsingClone<T> {
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








struct U {
	virtual std::unique_ptr<U> clone() const;
};

struct V : U {
	std::unique_ptr<U> clone() const override;
};

TEST(HasClone, SmartPointer) {
	EXPECT_TRUE(Tester<U>::has_clone());
	EXPECT_TRUE(Tester<V>::has_clone());
}









class With_clone_base {
public:
	virtual std::unique_ptr<With_clone_base> clone() const = 0;
	virtual int& x_ref() = 0;
};


class With_clone final : public With_clone_base {
public:
	With_clone() : x(1) {}
	With_clone(int new_x) : x(new_x) {}
	
	std::unique_ptr<With_clone_base> clone() const override final {
		return std::make_unique<With_clone>(x);
	}
	
	int& x_ref() override {
		return x;
	}
	
	int x = 0;
};


TEST(Clone, UsingCloneMethod) {
	std::unique_ptr<With_clone_base> p1 = std::make_unique<With_clone>(69);
	
	EXPECT_EQ(69, p1->x_ref());
	
	auto p2 = clone(*p1);
	
	p1->x_ref() = 100;
	
	EXPECT_EQ(100, p1->x_ref());
	EXPECT_EQ(69, p2->x_ref());
}


TEST(Clone, Ambiguity) {
	auto p1 = std::make_unique<With_clone>(69);
	
	EXPECT_EQ(69, p1->x);
	
	// this should invoke copy constructor...
	auto p2 = clone(*p1);
	
	p1->x = 100;
	
	// ...so p2 is pointer to derived
	EXPECT_EQ(69, p2->x);
}





TEST(Clone, CloneInt) {
	int a = 69;
	auto ptr_b = clone(a);
	a = 100;
	
	EXPECT_EQ(69, *ptr_b);
}





int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


