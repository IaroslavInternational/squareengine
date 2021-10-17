#include "InteractableObject2D.h"

InteractableObject2D::InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite, HitBox hitbox)
	:
	Object2D(name, position, layer, pathToSprite),
	hitbox(hitbox)
{
}

void InteractableObject2D::Draw(Graphics& gfx)
{
	gfx.DrawSprite((int)position.x, (int)position.y, image, Color(0, 0, 0));

	if (hitbox_visability)
	{
		gfx.DrawHitBox(hitbox);
	}
}

void InteractableObject2D::DrawTransparent(Graphics& gfx)
{
	gfx.DrawSpriteGhost((int)position.x, (int)position.y, image, deep, Color(0, 0, 0));

	if (hitbox_visability)
	{
		gfx.DrawHitBox(hitbox);
	}
}

void InteractableObject2D::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	hitbox.Translate(delta);
}
