#pragma once

#include "Object2D.h"
#include "Animation.h"
#include "Window.h"
#include "HitBox.h"

class Person : public Object2D
{
public:
	Person(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite, HitBox hitbox,
		float speed = 1.0f, float effectDuration = 0.045f, float effectTime = 0.0f,
		bool effectActive = false);
public:
	void Draw(Graphics& gfx);
	void SetDirection(DirectX::XMFLOAT2 dir);
	void Update(float dt);
	void ActivateEffect();
	Surface2D& GetSurface();
	float* GetEffectDuration();
	float* GetEffectTime();
	bool* GetEffectActive();
	bool* GetHitBoxVisability();
private:
	Surface2D sprite;
	DirectX::XMFLOAT2 vel = { 0.0f,0.0f };
private:
	float speed;
	struct
	{
		float Duration;
		float Time;
		bool Active;
	} effect;
	HitBox hitbox;
	bool hitbox_visability = false;
	int dx, dy;
private:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};

	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingDown;
};

