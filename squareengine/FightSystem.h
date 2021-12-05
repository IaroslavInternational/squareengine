#pragma once

#include "ContainerBase.hpp"
#include "AliveObject2D.h"
#include "Window.h"

class FightSystem : public ContainerBase<AliveObject2D*>
{
public:
	friend class GUISystem;
public:
	FightSystem(std::shared_ptr<Window> wnd);
public:
	void SetFighter(AliveObject2D* fighter);
	void ProcessFight();
	void DrawDebug();
private:
	AliveObject2D* fighter = nullptr;
	char		   attack_btn = 'F';
private:
	std::shared_ptr<Window> wnd;
	bool					IsClicked	   = false;
	bool				    IsShowHitBoxes = true;
};

