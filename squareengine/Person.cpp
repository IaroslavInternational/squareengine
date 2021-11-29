#include "Person.h"

#include "AnimationData.h"
#include "ScriptCompiler.h"
#include "EngineFunctions.hpp"

Person::Person(std::string	 name,		   DirectX::XMFLOAT2 position, size_t layer,
			   std::string	 pathToSprite, Color key,				   HitBox hitbox,
			   AnimationData aData,		   std::string       scriptPath,
			   float	     speed,		   float			 effectDuration,
			   float	     effectTime,   bool			     effectActive)
	:
	PhysicObject2D(name, position, hitbox, layer, pathToSprite, key, 8, 300),
	holdTime(aData.ft),
	speed(speed),
	scriptPath(scriptPath)
{
	effect.Duration = effectDuration;
	effect.Active	= effectTime;
	effect.Time		= effectActive;

	std::vector<std::string> animationNames =
	{
		"Ходьба влево",
		"Ходьба вправо",
		"Ходьба вверх",
		"Ходьба вниз",
		"Покой влево",
		"Покой вправо",
		"Покой вверх",
		"Покой вниз",
	};

	for (int i = 0; i < (int)Sequence::StandingLeft; i++)
	{
		animations.emplace_back(Animation(aData.pStart, aData.pEnd * i, aData.width, aData.height, aData.frames, image, aData.ft, animationNames[i], chromaKey));
	}
	for (int i = (int)Sequence::StandingLeft; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(0, aData.pEnd * (i - (int)Sequence::StandingLeft), aData.width, aData.height, 1, image, aData.ft, animationNames[i], chromaKey));
	}

	if (scriptPath != "")
	{
		ScriptCompiler sCompiler(scriptPath);
		script = sCompiler.GetScript();
	}
	else
	{
		script = Script();
	}
}

/* Главные методы для отрисовки персонажа */

void Person::Draw(Graphics& gfx)
{
	if (effect.Active)
	{
		animations[(int)iCurSequence].DrawColor(DirectX::XMFLOAT2({ position.x - dx, position.y - dy }), gfx, Colors::Red);
	}
	else
	{
		animations[(int)iCurSequence].Draw(DirectX::XMFLOAT2({ position.x - dx, position.y - dy}), gfx);
	}

	if (hitbox.IsVisible())
	{
		gfx.DrawHitBox(hitbox - DirectX::XMFLOAT2(dx, dy));
	}
}

void Person::Process(float dt)
{
	if (!script.IsEmpty())
	{
		auto current_cmd = script.GetCommand();

		if (current_cmd.first == "step_x")
		{
			if (current_cmd.second > 0 && AllowedMovingRight)
			{
				script.SetGoal((int)position.x + current_cmd.second);
		
				if ((int)position.x >= script.GetGoal())
				{
					script.NextCommand();
					return;
				}
				else
				{
					SetDirection({ 1.0f, 0.0f });
					Update(dt);
				}
			}
			else if (AllowedMovingLeft)
			{
				script.SetGoal((int)position.x + current_cmd.second);

				if ((int)position.x <= script.GetGoal())
				{
					script.NextCommand();
					return;
				}
				else
				{
					SetDirection({ -1.0f, 0.0f });
					Update(dt);
				}
			}
			else
			{
				script.NextCommand();
				return;
			}
		}
		else if (current_cmd.first == "step_y")
		{
			if (current_cmd.second > 0 && AllowedMovingDown)
			{
				script.SetGoal((int)position.y + current_cmd.second);

				if ((int)position.y >= script.GetGoal())
				{
					script.NextCommand();
					return;
				}
				else
				{
					SetDirection({ 0.0f, 1.0f });
					Update(dt);
				}
			}
			else if (AllowedMovingUp)
			{
				script.SetGoal((int)position.y + current_cmd.second);

				if ((int)position.y <= script.GetGoal())
				{
					script.NextCommand();
					return;
				}
				else
				{
					SetDirection({ 0.0f, -1.0f });
					Update(dt);
				}
			}
			else
			{
				script.NextCommand();
				return;
			}
		}
		else if (current_cmd.first == "delay")
		{
			script.SetGoal(current_cmd.second);

			if (integralTime >= (float)script.GetGoal() / 1000.0f)
			{
				script.NextCommand();
				integralTime = 0.0f;

				return;
			}
			else
			{
				SetDirection({ 0.0f, 0.0f });
				integralTime += dt;
			}
		}
	}

	if (AllowedMovingDown && !IsOnJump)
	{
		float d = gravity * dt;
		
		position.y += d;
		hitbox.UpdateY(d);
	}
}

void Person::SetDirection(DirectX::XMFLOAT2 dir)
{
	if (dir.x > 0.0f)
	{
		iCurSequence = Sequence::WalkingRight;
	}
	else if (dir.x < 0.0f)
	{
		iCurSequence = Sequence::WalkingLeft;
	}
	else if (dir.y < 0.0f)
	{
		iCurSequence = Sequence::WalkingUp;
	}
	else if (dir.y > 0.0f)
	{
		iCurSequence = Sequence::WalkingDown;
	}
	else
	{
		if (vel.x > 0.0f)
		{
			iCurSequence = Sequence::StandingRight;
		}
		else if (vel.x < 0.0f)
		{
			iCurSequence = Sequence::StandingLeft;
		}
		else if (vel.y < 0.0f)
		{
			iCurSequence = Sequence::StandingUp;
		}
		else if (vel.y > 0.0f)
		{
			iCurSequence = Sequence::StandingDown;
		}
	}

	vel.x = dir.x * speed;
	vel.y = dir.y * speed;
}

void Person::Update(float dt)
{
	vel.x *= dt;
	vel.y *= dt;

	position.x += vel.x;
	position.y += vel.y;

	hitbox.Update(vel.x, vel.y);
	CalculateDeltas();

	animations[(int)iCurSequence].Update(dt);

	if (effect.Active)
	{
		effect.Time += dt;

		if (effect.Time >= effect.Duration)
		{
			effect.Active = false;
		}
	}
}

void Person::ActivateEffect()
{
	effect.Active = true;
	effect.Time = 0.0f;
}

/******************************************/

/* Главные методы для взаимодействия с персонажем */

void Person::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	if (!script.IsEmpty())
	{
		auto current_cmd = script.GetCommand();

		if (current_cmd.first == "step_x")
		{
			script.ForceGoalTranslate(delta.x);
		}
		else if (current_cmd.first == "step_y")
		{
			script.ForceGoalTranslate(delta.y);
		}
	}

	hitbox.Translate(delta);
}

void Person::SetAnimation(std::vector<Animation> anim)
{
	animations.clear();

	for (auto& a : anim)
	{
		animations.emplace_back(a);
	}
}

void Person::SetScript(std::string path)
{
	if (path != "")
	{
		ScriptCompiler sCompiler(path);
		script = sCompiler.GetScript();
	}
	else
	{
		script = Script();
	}

	scriptPath = path;
}

/**************************************************/