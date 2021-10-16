#include "InteractableObject2D.h"

InteractableObject2D::InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite)
	:
	Object2D(name, position, layer, pathToSprite)
{
}

void InteractableObject2D::Draw(Graphics& gfx)
{
	gfx.DrawSprite((int)position.x, (int)position.y, image, Color(0, 0, 0));
}

void InteractableObject2D::DrawTransparent(float deep)
{
}
