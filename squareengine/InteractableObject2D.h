#pragma once

#include "Object2D.h"

class InteractableObject2D : public Object2D
{
public:
	InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite);
public:
	void Draw();
	void DrawTransparent(float deep);
private:

};
