#pragma once

#include "Person.h"
#include "ContainerBase.hpp"

class PersonContainer : public ContainerBase<std::unique_ptr<Person>>
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
	std::unique_ptr<Person>& Get(size_t i);
private:
	bool IsScriptsRunning = false;
};
