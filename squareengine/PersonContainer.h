#pragma once

#include "Person.h"

class PersonContainer
{
public:
	PersonContainer(std::string dataPath);
public:
	void Draw(Graphics& gfx);

	std::vector<std::unique_ptr<Person>>* GetPersons();
private:
	std::vector<std::unique_ptr<Person>> persons;
};
