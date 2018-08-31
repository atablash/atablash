#pragma once

#include<atablash/debug.hpp>
#include<atablash/templates.hpp>

namespace ab{

/////////////////////////////// STATIC ARRAY ///////////////////////////////////
// requires: TEMPLATES
//
// Copyright (c) 2013 by Adam Blaszkiewicz. All rights reserved.
//
template<typename T, int _capacity>
class StaticArray
{
public:
	StaticArray() : mSize(0) {}
	StaticArray(unsigned int _size) : mSize(_size) {ASS(_size<=_capacity);}
	inline void push_back(const T& t) {new(&mData()[mSize]) T(t); ++mSize;}
	inline void push_back() {new(&mData()[mSize]) T; ++mSize;}
	inline void pop_back() {ASS(mSize>0); --mSize; mData()[mSize].~T();}
	inline T& operator[](const int idx) {return mData()[idx];}
	inline const T& operator[](const int idx) const {ASS(idx<mSize); return mData()[idx];}
	inline T& back() {ASS(mSize>0); return mData()[mSize-1];}
	inline const T& back() const {ASS(mSize>0); return mData()[mSize-1];}
	inline T& front() {ASS(mSize>0); return mData()[0];}
	inline const T& front() const {ASS(mSize>0); return mData()[0];}
	inline int size() const {return mSize;}
	inline unsigned int capacity() const {return _capacity;}
	inline void resize(int newSize)
	{
		ASS(newSize <= _capacity);
		if(newSize > mSize) new (&mData()[mSize]) T[newSize-mSize];
		else for(int i=newSize; i<mSize; ++i) mData()[i].~T();
		mSize = newSize;
	}
	inline void reserve(int howMuch) const {ASS(howMuch <= _capacity);}
	inline void clear() {for(int i=0; i<mSize; ++i) mData()[i].~T(); mSize=0;}
	typedef T* iterator;
	inline iterator begin() {return mData;}
	inline iterator end()   {return mData+mSize;}
private:
	T* mData() {return reinterpret_cast<T*>(_mData);}
	char _mData[_capacity*sizeof(T)];
	int mSize;
};

// vector of at most n elements
template<typename T, int n> struct _vector
{typedef typename IF<n==0,std::vector<T>,StaticArray<T,n> >::R R;};
////////////////////////////////////////////////////////////////////////////////

}

