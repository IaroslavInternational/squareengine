#pragma once

#include "ContainerBase.hpp"
#include "AliveObject2D.h"
#include "Window.h"

class FightSystem : public ContainerBase<AliveObject2D*>
{
public:
	FightSystem(std::shared_ptr<Window> wnd);
public:
	void SetFighter(AliveObject2D* fighter);
	void ProcessFight();
private:
	AliveObject2D* fighter = nullptr;
	char		   attack_btn = 'F';
private:
	std::shared_ptr<Window> wnd;
	float					attack_hb_offset = 50.0f;
	bool IsClicked = false;
};

