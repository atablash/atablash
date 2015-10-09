#pragma once


template<typename T>
class p
{
	// IMPORTANT:
	// newObject should not be constructed yet!
	// oldObject shouldn't be destructed after this call!
	public: static inline void MoveConstructor(const p& newObject, const p& oldObject)
	{
		newObject.mpObject = oldObject.mpObject;
	}
	public: inline p() : mpObject(new T) {}
	public: inline p(T* pObject) : mpObject(pObject)
	{
		ASSERT(pObject != NULL);
	}

	private: p(const p& o);
	private: p& operator=(const p& o);

	public: inline ~p()
	{
		delete mpObject;
	}

	public: inline operator T*() const
	{
		ASSERT(mpObject);
		return mpObject;
	}

	public: inline T* operator->() const
	{
		ASSERT(mpObject);
		return mpObject;
	}

	public: inline T& operator*() const
	{
		ASSERT(mpObject);
		return *mpObject;
	}

	public: inline T* get() const
	{
		ASSERT(mpObject);
		return mpObject;
	}

	private: T* mpObject;
};

// maksymalnie uproszczona wersja - ma³o ifów i du¿a wydajnoœæ
// nie moze wskazywac na NULL
// przez cale zycie wskazuje tylko na 1 obiekt, który mo¿e byæ ewentualnie przemieszczony
template<typename T>
class ref
{
public:
	//p() : pNode(new Node(NULL)) {}

	public: ~ref()
	{
		if(pNode->count == 1)
		{
			delete pNode;
		}
		else --pNode->count;
	}

	public: ref(const ref& o)
	{
		pNode = o.pNode;
		++pNode->count;
	}

	public: ref(T *pObject) : pNode(new Node(pObject)) { ASSERT(pObject); }

	private: ref& operator=(const ref& o);
	//p& operator=(T *pObject)
	//{
	//	ASSERT(pObject);
	//	Free();
	//	pNode = new Node(pObject);
	//	return *this;
	//}

	/*
	p& operator=(const p& o)
	{
		ASSERT(this != &o);
		//if(this == &o) return *this;

		Free();
		pNode = o.pNode;

		if(pNode)
		{
			ASSERT(pNode->count >= 1);
			++pNode->count;
		}

		return *this;
	}
	*/

	operator T*() const
	{
		ASSERT(pNode);
		return pNode->pObject;
	}

	T* operator->() const
	{
		ASSERT(pNode);
		return pNode->pObject;
	}

	T& operator*() const
	{
		ASSERT(pNode);
		return *pNode->pObject;
	}

	T* get() const
	{
		ASSERT(pNode);
		return pNode->pObject;
	}

private:

	struct Node
	{
	public:
		inline Node(T* pObject) : pObject(pObject), count(1) {}
		inline ~Node()
		{
			delete pObject;
		}

	private:
		T *pObject;
		unsigned int count;
	};

	Node *pNode;
};
