#pragma once

#include<cstdlib>
#include<atablash/debug.h>
#include<atablash/allocator.h>

namespace ab
{

// usuwany element jest zastepowany ostatnim
// T jest niejawnie przenoszony podczas realloc(), bez użycia kontruktorów i destruktorów!
template<typename T, typename Allocator = SimpleAllocator > class Vector
{
	public: class iterator;
	typedef unsigned int size_type;

    public:	Vector() : pData((T*)malloc(1 * sizeof(T))), mSize(0), mReserved(1)
    {
    }

	private: Vector(const Vector& o) : pData((T*)malloc(o.mReserved*sizeof(T))), mSize(o.mSize), mReserved(o.mReserved)
	{
	}
	
	// nie wywołuje konstruktorów!
	public: inline void resize(unsigned int size)
	{
		if(size > mReserved)
		{
			_realloc(size);
		}
		mSize = size;
	}

	// nie wywołuje kontruktorów!
	public: Vector(unsigned int size) : pData((T*)malloc(size*sizeof(T))), mSize(size), mReserved(size)
	{
		/*
		for(unsigned int i=0; i<mSize; ++i)
		{
			new(&pData[i]) T();
		}
		*/
	}
	public: Vector(unsigned int size, const T& val) : pData((T*)malloc(size*sizeof(T))), mSize(size), mReserved(size)
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			new(&pData[i]) T(val);
		}
	}

	virtual ~Vector()
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			pData[i].~T();
		}
		free(pData);
	}

	public: inline bool empty() const
	{
		return !mSize;
	}

	inline T& operator[](const unsigned int index)
	{
		ASSERT(index < mSize);
		return pData[index];
	}
	
	inline const T& operator[](const unsigned int index) const
	{
		ASSERT(index < mSize);
		return pData[index];
	}

	inline const T& back() const
	{
		ASSERT(mSize >= 1);
		return pData[mSize-1];
	}

	inline const T& front() const
	{
		ASSERT(mSize >= 1);
		return pData[0];
	}

	inline T& back()
	{
		ASSERT(mSize >= 1);
		return pData[mSize-1];
	}

	inline T& front()
	{
		ASSERT(mSize >= 1);
		return pData[0].data;
	}

	public: inline iterator begin() const
	{
		return iterator(pData[0]);
	}

	public: inline iterator end() const
	{
		return iterator(pData[mSize]);
	}

	public: inline size_type size() const
	{
		return mSize;
	}

	public: inline void pop_back()
	{
		ASSERT(mSize);
		--mSize;
		pData[mSize].~T();
	}

	// przenoszenie bez użycia konstruktorów!
	private: inline void _realloc(unsigned int nsize)
	{
		mReserved = nsize;
		pData = (T *)realloc(pData, mReserved * sizeof(T));
	}
	
	public: inline void push_back(T e = T())
	{
		_realloc((mReserved*3 + 1) / 2);
		new(&pData[mSize]) T(e);
		++mSize;
	}


	public: class iterator
	{
		public: iterator(T& node) : pNode(&node) {}

		public: inline bool operator==(const iterator& o) const
		{
			return pNode==o.pNode;
		}

		public: inline bool operator!=(const iterator& o) const
		{
			return pNode!=o.pNode;
		}

		public: inline iterator& operator++()
		{
			++pNode;
			return *this;
		}

		public:
			inline const T& operator*() const
			{
				return pNode->data;
			}

			inline T& operator*()
			{
				return pNode->data;
			}

			inline T* operator->() const
			{
				return &pNode->data;
			}
		private:
			T *pNode;
	};
	
private:
	T* pData;
	unsigned int mSize;
	unsigned int mReserved;
};


}	// namespace ab
