#pragma once

#include<cstdlib>
#include<atablash/debug.hpp>
#include<atablash/allocator.hpp>
#include<atablash/templates.hpp>

namespace ab
{

// usuwany element jest zastepowany ostatnim
// pointer sie nie uniewaznia
template<typename T, bool UseMoveConstructor = false, typename Allocator = SimpleAllocator > class UnorderedSmartVector
{
	public: class iterator;
	public: class magic_pointer;
	private: class Node;
	typedef unsigned int size_type;

    public:	UnorderedSmartVector() : pData((Node*)malloc(1 * sizeof(Node))), mSize(0), mReserved(1)
    {
    }

	private: UnorderedSmartVector(const UnorderedSmartVector& o) : pData((Node*)malloc(o.mReserved*sizeof(Node))), mSize(o.mSize), mReserved(o.mReserved)
	{
	}

	public: UnorderedSmartVector(unsigned int _size) : pData((Node*)malloc(_size*sizeof(Node))), mSize(_size), mReserved(_size)
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			new(&pData[i]) Node();
		}
	}
	public: UnorderedSmartVector(unsigned int _size, const T&) : pData((Node*)malloc(_size*sizeof(Node))), mSize(_size), mReserved(_size)
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			new(&pData[i]) Node();
		}
	}

	virtual ~UnorderedSmartVector()
	{
		for(unsigned int i=0; i<mSize; ++i)
		{
			pData[i].~Node();
		}
		free(pData);
	}

	inline T& operator[](const unsigned int index) const
	{
		ASS(index < mSize);
		return pData[index].data;
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
		return pData[mSize-1].data;
	}

	inline T& front()
	{
		ASS(mSize >= 1);
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

	public: void erase(size_type index)
	{
		erase(&pData[index]);
	}

	public: void erase(iterator& itr)
	{
		erase(itr.pNode);
	}

	public: void erase(const magic_pointer& ptr)
	{
		erase(*ptr.ppNode);
	}

	private: void erase(Node* pNode)
	{
		ASS(mSize > 0);
		ASS(pNode <  &pData[mSize]);
		ASS(pNode >= &pData[0]);

		//unsigned int index = pT.pElement-pData;
		pNode->~Node();
		--mSize;
		if(pNode == &pData[mSize]) return;


		//UnorderedVector<pointer*>& users(*pData[mSize].pUsers);
		//for(unsigned int i=0; i<users.size();)
		//{
		//	if(users[i]->pElement != &pData[mSize])
		//	{
		//		users.erase(i);
		//		continue;
		//	}
		//	users[i]->pElement = pNode;
		//	++i;
		//}

		//new(pNode) Node(pData[mSize]);
		//pData[mSize].data.~T();
		pNode->ppThis = pData[mSize].ppThis;
		*pNode->ppThis = pNode;

		MoveConstructor<void>(pNode->data, pData[mSize].data);

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
			Node* pNewData = (Node *)realloc(pData, mReserved * sizeof(Node));
			if(pNewData != pData)
			{
				//printf("realloc has moved the data\n");
				for(unsigned int i=0; i<mSize; ++i)
				{
					*pNewData[i].ppThis = &pNewData[i];
				}
				pData = pNewData;
			}
			//else
			//	printf("realloc has not moved the data\n");
		}
		new(&pData[mSize]) Node(e);
		++mSize;
	}

	public: inline magic_pointer getMagicPointer(unsigned int index)
	{
		return magic_pointer(pData[index]);
	}

	public: inline magic_pointer getMagicPointer(const iterator& itr)
	{
		magic_pointer r(&*itr);
		registerPointer(r);
		return r;
	}

private:

	// nie uniewaznia sie
	public: class magic_pointer
	{
		friend class UnorderedSmartVector;
		friend class iterator;

		private:
			magic_pointer(const Node& node) : ppNode(node.ppThis) {}

		public: magic_pointer(){}

		public: inline unsigned int operator-(const magic_pointer& o) const
		{
			return *ppNode - *o.ppNode;
		}

		public: inline unsigned int operator-(const iterator& o) const
		{
			return *ppNode - o.pNode;
		}

		public:
			const T& operator*() const
			{
				ASSERT(*ppNode);
				return (**ppNode).data;
			}

			T& operator*()
			{
				ASSERT(*ppNode);
				return (**ppNode).data;
			}

			T* operator->() const
			{
				ASSERT(*ppNode);
				return &(**ppNode).data;
			}

		private:
			Node **ppNode;
	};

	public: class iterator
	{
		friend class UnorderedSmartVector;
		friend class magic_pointer;

		public: iterator(Node& node) : pNode(&node) {}

		public: inline bool operator==(const iterator& o) const
		{
			return pNode==o.pNode;
		}

		public: inline bool operator!=(const iterator& o) const
		{
			return pNode!=o.pNode;
		}

		public: inline unsigned int operator-(const magic_pointer& o) const
		{
			return pNode - *o.ppNode;
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
				return pNode->data;
			}

			T& operator*()
			{
				return pNode->data;
			}

			T* operator->() const
			{
				return &pNode->data;
			}
		private:
			Node *pNode;
	};
private:
	struct Node
	{
		//Node() : ppThis(new Node*(this)) {}
		//Node(const Node& o) : pUsers(o.pUsers), data(o.data) {}
		Node()
		{
			ppThis = (Node**)Allocator::Alloc(sizeof(Node*));
			*ppThis = this;
		}
		Node(const T& t) : data(t)
		{
			ppThis = (Node**)Allocator::Alloc(sizeof(Node*));
			*ppThis = this;
		}
		~Node()
		{
			DE
			{
				*ppThis = NULL;
			}
			Allocator::Free(ppThis);
			//delete ppThis;
		}
		T data;
		Node** ppThis;
	};

	Node* pData;
	unsigned int mSize;
	unsigned int mReserved;
};


}	// namespace abs
