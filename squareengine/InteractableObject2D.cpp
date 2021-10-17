#include "InteractableObject2D.h"

InteractableObject2D::InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite, 
	HitBox hitbox, float gDeep, bool gAble)
	:
	Object2D(name, position, layer, pathToSprite),
	deep(gDeep),
	drawGhostable(gAble),
	hitbox(hitbox)
{
}

void InteractableObject2D::Draw(Graphics& gfx)
{
	if (!drawGhost)
	{
		gfx.DrawSprite((int)position.x, (int)position.y, image, Color(0, 0, 0));
	}
	else if(drawGhostable)
	{
		DrawTransparent(gfx);
	}

	if (hitbox_visability)
	{
		gfx.DrawHitBox(hitbox);
	}
}

void InteractableObject2D::DrawTransparent(Graphics& gfx)
{
	gfx.DrawSpriteGhost((int)position.x, (int)position.y, image, deep, Color(0, 0, 0));
}

void InteractableObject2D::Translate(DirectX::XMFLOAT2 delta)
{
	position.x += delta.x;
	position.y += delta.y;

	hitbox.Translate(delta);
}

void InteractableObject2D::SetGhostState(bool state)
{
	drawGhost = state;
}

void InteractableObject2D::SetGhostAble(bool state)
{
	drawGhostable = state;
}

bool InteractableObject2D::IsGhostable()
{
	return drawGhostable;
}

HitBox InteractableObject2D::GetHitBox()
{
	return hitbox;
}
