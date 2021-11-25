#pragma once

#include <vector>
#include <string>

template <typename Type>
class ContainerBase
{
public:
	ContainerBase(std::string dataPath)
		:
		dataPath(dataPath)
	{}
public:
	inline void Add(Type element) 
	{
		elements.push_back(std::move(element));
	}
	inline void DeleteAt(size_t element_idx)
	{
		elements.erase(elements.begin() + element_idx);
	}
	inline void DeleteAt(typename std::vector<Type>::iterator iterator)
	{
		elements.erase(it);
	}
protected:
	std::string dataPath;
	std::vector<Type> elements;
};