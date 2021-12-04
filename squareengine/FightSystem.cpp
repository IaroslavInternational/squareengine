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
	if (!IsClicked && wnd->kbd.KeyIsPressed(attack_btn))
	{
		for (auto& obj : elements)
		{
			auto fighter_hb = fighter->GetHitBox();
			int dir = fighter->GetDirection();
			
			if (dir == -1)
			{
				fighter_hb.UpdateLeftSide(attack_hb_offset);
			}
			else
			{
				fighter_hb.UpdateRightSide(attack_hb_offset);
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
