#pragma once

#include "Animation.h"
#include "Object.h"

class Character : public Object
{
public:
	Character(const std::string& path, DirectX::XMFLOAT2 coordinates, 
		float speed = 110.0f, float effectDuration = 0.045f, float effectTime = 0.0f, 
		bool effectActive = false);
public:
	void Draw(Graphics& gfx);
	void SetDirection(DirectX::XMFLOAT2 dir);
	void Update( float dt );
	// activates a damage visual effect
	void ActivateEffect();
private:
	Surface2D sprite;
	DirectX::XMFLOAT2 vel = { 0.0f,0.0f };
private:
	float speed = 110.0f;
	float effectDuration = 0.045f;
	float effectTime = 0.0f;
	bool effectActive = false;
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