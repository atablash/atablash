#pragma once


namespace ab
{

template<typename Container, typename _Predicate>
class Subcontainer
{
	typedef _Predicate Predicate;

	public: Subcontainer(Container& container) : mContainer(container)
	{
	}

	public: class iterator
	{
		public: bool isValid(typename Container::iterator& itr)
		{
			return Predicate(*itr);
		}

		public: iterator(Container& container, typename Container::iterator& itr) : mContainer(container)
		{
			realIterator = itr;
			firstValid(realIterator);
		}

		public: bool operator==(const iterator& o) const
		{
			return realIterator == o.realIterator;
		}

		public: bool operator!=(const iterator& o) const
		{
			return realIterator != o.realIterator;
		}

		private: inline void firstValid(typename Container::iterator& itr) const
		{
			for(;;)
			{
				if(itr == mContainer.end()) return;
				if(Predicate(*itr)) return;
				++itr;
			}
		}

		public: iterator& operator++()
		{
			++realIterator;
			firstValid(realIterator);
		}

		public: typename Container::value_type& operator*()
		{
			return realIterator.operator*();
		}

		public: typename Container::value_type* operator->()
		{
			return realIterator.operator->();
		}

		private: typename Container::iterator realIterator;
		private: Container& mContainer;
	};

	//void erase(iterator& where)
	//{
	//	container.erase(where.realIterator);
	//}

	public: iterator begin()
	{
		return iterator(*this, mContainer.begin());
	}

	public: iterator end()
	{
		return iterator(*this, mContainer.end());
	}
public:

private:
	Container& mContainer;
};

}	// namespace ab
