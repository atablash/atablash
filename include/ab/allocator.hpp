#pragma once

namespace ab
{


class SimpleAllocator
{
	public: static inline void* Alloc(unsigned int size)
	{
		return malloc(size);
	}

	public: static inline void Free(void *p)
	{
		free(p);
	}
};

#ifdef AB_SIMPLE_ALLOCATOR_SIZE
const unsigned int FastAllocatorSize = AB_SIMPLE_ALLOCATOR_SIZE;


class FastAllocator
{
	private: static unsigned char data[FastAllocatorSize];
	private: static unsigned int dataPos;

	public: static inline void* Alloc(unsigned int size)
	{
		void *r = data + dataPos;
		dataPos += size;
		return r;
	}

	public: static inline void Free(void *p)
	{
	}
};

unsigned int FastAllocator::dataPos = 0;
unsigned char FastAllocator::data[FastAllocatorSize];

#endif

}