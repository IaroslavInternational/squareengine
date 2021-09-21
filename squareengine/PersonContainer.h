#pragma once

#include "Person.h"

class PersonContainer
{
public:
	friend class GUISystem;
public:
	PersonContainer(std::string dataPath);
public:
	void Draw(Graphics& gfx);
	std::string GetDataPath();
private:
	std::string dataPath;
	std::vector<std::unique_ptr<Person>> persons;
};
