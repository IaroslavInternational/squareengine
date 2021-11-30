#include "InteractableObject2D.h"

InteractableObject2D::InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite, 
										   Color key, bool chromaKeyAble, HitBox hitbox, float gDeep, bool gAble)
	:
	Object2D(name, position, layer, pathToSprite, key),
	chromaKeyAble(chromaKeyAble),
	deep(gDeep),
	drawGhostable(gAble),
	hitbox(hitbox)
{
}

void InteractableObject2D::Draw(Graphics& gfx)
{
	if (!drawGhost)
	{
		if (chromaKeyAble)
		{
			gfx.DrawSprite((int)position.x, (int)position.y, image, chromaKey);
		}
		else
		{
			gfx.DrawSpriteNonChroma((int)position.x, (int)position.y, image);
		}
	}
	else if(drawGhostable)
	{
		DrawTransparent(gfx);
	}

	if (hitbox.IsVisible())
	{
		gfx.DrawHitBox(hitbox);
	}
}

void InteractableObject2D::DrawTransparent(Graphics& gfx)
{
	if (chromaKeyAble)
	{
		gfx.DrawSpriteGhost((int)position.x, (int)position.y, image, deep, chromaKey);
	}
	else
	{
		gfx.DrawSpriteGhostNonChroma((int)position.x, (int)position.y, image, deep);
	}
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

void InteractableObject2D::SetHitBox(HitBox hb)
{
	hitbox = hb;
}

HitBox InteractableObject2D::GetHitBox()
{
	return hitbox;
}
