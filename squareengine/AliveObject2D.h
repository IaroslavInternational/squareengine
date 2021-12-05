#pragma once

#include "PhysicObject2D.h"

class AliveObject2D : public PhysicObject2D
{
public:
	friend class GUISystem;
public:
	AliveObject2D(std::string name, DirectX::XMFLOAT2 position, float health, float damage, 
				  HitBox hitbox, size_t layer, std::string pathToSprite, Color key, int jump_height, float gravity);
public:
	void  Attack(AliveObject2D* enemy);
	void  Hit(float damage);
	bool  IsAlive();
	float GetHitBoxOffset();
protected:
	float health;
	float damage;
	float attack_hb_offset = 50.0f;
};
