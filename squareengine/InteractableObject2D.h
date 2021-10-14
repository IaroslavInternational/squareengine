#pragma once

#include "Object2D.h"

class InteractableObject2D : public Object2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite);
public:
	void Draw(Graphics &gfx) override;
	void DrawTransparent(float deep);
private:

};
