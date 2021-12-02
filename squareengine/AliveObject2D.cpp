#include "AliveObject2D.h"

AliveObject2D::AliveObject2D(std::string name, DirectX::XMFLOAT2 position, float health, float damage, HitBox hitbox, size_t layer, std::string pathToSprite, Color key, int jump_height, float gravity)
	:
	PhysicObject2D(name, position, hitbox, layer, pathToSprite, key, jump_height, gravity),
	health(health),
	damage(damage)
{}

void AliveObject2D::Attack(AliveObject2D* enemy)
{
	enemy->Hit(damage);
}

void AliveObject2D::Hit(float damage)
{
	if (health >= 0.0f)
	{
		this->health -= damage;
	}
}

bool AliveObject2D::IsAlive()
{
	return health >= 0.0f;
}
