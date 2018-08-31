#pragma once

#include<atablash/templates.hpp>

namespace ab{

/////////////////////////////// STATIC QUEUE ///////////////////////////////////
//
// Copyright (c) 2013 by Adam Blaszkiewicz. All rights reserved.
//
template<typename T, int capacity>
class StaticQueue
{
public:
	StaticQueue() : _front(mData), _back(mData-1) {}
	inline void push_back(const T& t = T()) {++_back; *_back=t;}
	inline void pop_back() {--_back;}
	inline void pop_front() {++_front;}

	inline void push(const T& t = T()) {push_back(t);}
	inline void pop() {pop_front();}

	inline T& back() {return *_back;}
	inline const T& back() const {return *_back;}

	inline T& front() {return *_front;}
	inline const T& front() const {return *_front;}

	inline int size() const {return _back-_front+1;}
	inline void clear() {_front=mData; _back=mData-1;}
	inline bool empty() const {return _front > _back;}
	typedef T* iterator;
	inline iterator begin() const {return _front;}
	inline iterator end() const   {return _back+1;}
private:
	T mData[capacity];
	T* _front;
	T* _back;
};
// queue with at most n pushes
template<typename T, int n> struct _queue
{typedef typename IF<n==0,std::queue<T>,StaticQueue<T,n> >::R R;};
////////////////////////////////////////////////////////////////////////////////

}

