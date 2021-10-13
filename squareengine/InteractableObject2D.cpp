#include "InteractableObject2D.h"

InteractableObject2D::InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite)
	:
	Object2D(name, position, pathToSprite)
{
}

void InteractableObject2D::Draw(Graphics& gfx)
{
	gfx.DrawSprite((int)position.x, (int)position.y, image);
}

void InteractableObject2D::DrawTransparent(float deep)
{
}
