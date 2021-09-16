#pragma once

#include "Person.h"
#include "AppLog.h"

class PersonContainer
{
public:
	PersonContainer(std::string dataPath);
public:
	void Draw(Graphics& gfx);

#if IS_ENGINE_MODE
	void DrawLog();
#endif // IS_ENGINE_MODE
private:
#if IS_ENGINE_MODE
	// Ћог
	AppLog applog;
#endif // IS_ENGINE_MODE
private:
	std::vector<std::unique_ptr<Person>> persons;
};
