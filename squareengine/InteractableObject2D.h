#pragma once

#include "Object2D.h"
#include "HitBox.h"

class InteractableObject2D : public Object2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite, HitBox hitbox);
public:
	void Draw(Graphics &gfx) override;
	void DrawTransparent(Graphics& gfx);
public:
	void Translate(DirectX::XMFLOAT2 delta);
private:
	float  deep = 2.0f;
	bool   hitbox_visability = true;
	HitBox hitbox;
};
