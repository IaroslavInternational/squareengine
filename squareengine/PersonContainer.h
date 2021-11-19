#pragma once

#include "Person.h"

class PersonContainer
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	PersonContainer(std::string dataPath);
public:
	void					 Process(float dt);
	void					 Draw(Graphics& gfx);
	void					 Translate(DirectX::XMFLOAT2 delta);
	void					 DeletePersonAt(size_t k);
	void					 DeletePersonAt(std::vector<std::unique_ptr<Person>>::iterator it);
	std::pair<bool, Person*> CheckCollision(HitBox& hb);
private:
	bool								 IsScriptsRunning = false;
	std::string							 dataPath;
	std::vector<std::unique_ptr<Person>> persons;
};
