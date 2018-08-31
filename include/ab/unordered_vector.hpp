#pragma once

#include<cstdlib>
#include<atablash/debug.hpp>
#include<atablash/allocator.hpp>
#include<atablash/templates.hpp>

namespace ab
{

// UWAGA: oparte o realloc, czyli nie wywo³uje kontruktorów kopiuj¹cych, tylko przenosi na chama
// usuwany element jest zastepowany ostatnim
// pointer sie nie uniewaznia
template<typename T, bool UseMoveConstructor = false, typename Allocator = SimpleAllocator > class UnorderedVector
{
	public: class iterator;
	public: class magic_pointer;
	private: class Node;
	typedef unsigned int size_type;

    public:	UnorderedVector() : pData((T*)malloc(1 * sizeof(T))), mSize(0), mReserved(1)
    {
    }

	private: UnorderedVector(const UnorderedVector& o) : pData((T*)malloc(o.mReserved*sizeof(T))), mSize(o.mSize), mReserved(o.mReserved)
	{
	}

	public: UnorderedVector(unsigned int _size) : pData((T*)malloc(_size*sizeof(T))), mSize(_size), mReserved(_size)
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			new(&pData[i]) Node();
		}
	}
	public: UnorderedVector(unsigned int _size, const T&) : pData((T*)malloc(_size*sizeof(T))), mSize(_size), mReserved(_size)
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			new(&pData[i]) Node();
		}
	}

	virtual ~UnorderedVector()
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			pData[i].~T();
		}
		free(pData);
	}

	inline T& operator[](const unsigned int index) const
	{
		ASS(index < mSize);
		return pData[index];
	}

	inline const T& back() const
	{
		ASS(mSize >= 1);
		return pData[mSize-1];
	}

	inline const T& front() const
	{
		ASS(mSize >= 1);
		return pData[0];
	}

	inline T& back()
	{
		ASS(mSize >= 1);
		return pData[mSize-1];
	}

	inline T& front()
	{
		ASS(mSize >= 1);
		return pData[0];
	}

	public: inline iterator begin() const
	{
		return iterator(pData[0]);
	}

	public: inline iterator end() const
	{
		return iterator(pData[mSize]);
	}

	public: void erase(size_type index)
	{
		_erase(&pData[index]);
	}

	public: void erase(iterator& itr)
	{
		_erase(itr.pNode);
	}

	private: void _erase(T* pNode)
	{
		ASS(mSize > 0);
		ASS(pNode <  &pData[mSize]);
		ASS(pNode >= &pData[0]);

		//unsigned int index = pT.pElement-pData;
		pNode->~T();
		--mSize;
		if(pNode == &pData[mSize]) return;

		MoveConstructor<void>(*pNode, pData[mSize]);

		//pData[mSize].~Node(); // brak destruktora!
	}

	//private: typedef void (*sign1)(T& out, const T& in);
	//HAS_MEMBER_SIGNATURE(MoveConstructor, sign1);

	//HAS_MEMBER(MOVE_CONSTRUCTOR,MoveConstructor);

	private: template<typename TTT>
	inline void MoveConstructor(T& to, const T& from, typename DISABLE_IF<UseMoveConstructor,TTT>::RET* dummy = 0)
	{
		//to = from;
		new(&to) T(from);
		from.~T();
	}

	private: template<typename TTT>
	inline void MoveConstructor(const T& to, const T& from, typename ENABLE_IF<UseMoveConstructor,TTT>::RET* dummy = 0)
	{
		T::MoveConstructor(to, from);
	}

	public: inline size_type size() const
	{
		return mSize;
	}

	public: inline void push_back(const T& e = T())
	{
		if(mSize == mReserved)
		{
			mReserved = (mReserved*3 + 1) / 2;
			pData = (T *)realloc(pData, mReserved * sizeof(T));
		}
		new(&pData[mSize]) T(e);
		++mSize;
	}

private:

	public: class iterator
	{
		friend class UnorderedVector;

		public: iterator(T& node) : pNode(&node) {}

		public: inline bool operator==(const iterator& o) const
		{
			return pNode==o.pNode;
		}

		public: inline bool operator!=(const iterator& o) const
		{
			return pNode!=o.pNode;
		}

		public: inline unsigned int operator-(const iterator& o) const
		{
			return pNode - o.pNode;
		}

		public: inline iterator& operator++()
		{
			++pNode;
			return *this;
		}

		public:
			const T& operator*() const
			{
				return *pNode;
			}

			T& operator*()
			{
				return *pNode;
			}

			T* operator->() const
			{
				return pNode;
			}
		private:
			T *pNode;
	};
private:
	T* pData;
	unsigned int mSize;
	unsigned int mReserved;
};


}	// namespace abs
