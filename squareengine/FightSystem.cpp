#include "FightSystem.h"

FightSystem::FightSystem(std::shared_ptr<Window> wnd)
	:
	ContainerBase(""),
	wnd(wnd)
{}

void FightSystem::SetFighter(AliveObject2D* fighter)
{
	this->fighter = fighter;
}

void FightSystem::ProcessFight()
{
	assert(fighter == nullptr);

	if (!IsClicked && wnd->kbd.KeyIsPressed(attack_btn))
	{
		for (auto& obj : elements)
		{
			auto fighter_hb = fighter->GetHitBox();
			int dir = fighter->GetDirection();
			
			if (dir == -1)
			{
				fighter_hb.UpdateLeftSide(fighter->GetHitBoxOffset());
			}
			else
			{
				fighter_hb.UpdateRightSide(fighter->GetHitBoxOffset());
			}

			if (fighter_hb.IsCollide(obj->GetHitBox()))
			{
				fighter->Attack(obj); 
			}
		}

		IsClicked = true;
	}
	else if(!wnd->kbd.KeyIsPressed(attack_btn))
	{
		IsClicked = false;
	}
}

void FightSystem::DrawDebug()
{
	if (IsShowHitBoxes)
	{
		auto fighter_hb = fighter->GetHitBox();
		int dir = fighter->GetDirection();

		if (dir == -1)
		{
			fighter_hb.UpdateLeftSide(fighter->GetHitBoxOffset());
		}
		else
		{
			fighter_hb.UpdateRightSide(fighter->GetHitBoxOffset());
		}

		wnd->Gfx().DrawHitBox(fighter_hb, Colors::Green);

		for (auto& obj : elements)
		{
			fighter_hb = obj->GetHitBox();
			dir = obj->GetDirection();

			if (dir == -1)
			{
				fighter_hb.UpdateLeftSide(obj->GetHitBoxOffset());
			}
			else
			{
				fighter_hb.UpdateRightSide(obj->GetHitBoxOffset());
			}

			wnd->Gfx().DrawHitBox(fighter_hb, Colors::Red);
		}
	}
}
