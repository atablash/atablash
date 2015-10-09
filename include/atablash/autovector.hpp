// a vector which resizes itself automatically when out-of-range element is requested
template<typename T> class AutoVector
{
public:
	T& operator[](const unsigned int index) const
	{
		if(mVector.size() <= index)
		{
			mVector.resize(index+1);
		}
		return mVector[index];
	}
private:
	std::vector<T> mVector;
};
