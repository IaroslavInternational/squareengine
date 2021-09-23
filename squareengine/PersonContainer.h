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
	void Process();
	bool TestCollision()
	{
		return persons[0]->GetHitBox().IsCollide(persons[1]->GetHitBox());
	}
private:
	std::string dataPath;
	std::vector<std::unique_ptr<Person>> persons;
};
