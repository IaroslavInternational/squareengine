#pragma once

#include "Person.h"
#include "AppLog.h"

class PersonContainer
{
public:
	PersonContainer(std::string dataPath);
public:
	void Draw(Graphics& gfx);
	void DrawLog();

	std::vector<std::unique_ptr<Person>>* GetPersons();
private:
	// Ћог
	AppLog applog;
private:
	std::vector<std::unique_ptr<Person>> persons;
};
