#pragma once

#include "Object2D.h"
#include "HitBox.h"

class InteractableObject2D : public Object2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	InteractableObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite,
		HitBox hitbox, float gDeep = 2.0f, bool gAble = false);
public:
	void Draw(Graphics &gfx) override;
	void DrawTransparent(Graphics& gfx);
public:
	void   Translate(DirectX::XMFLOAT2 delta);
	void   SetGhostState(bool state);
	void   SetGhostAble(bool state);
	bool   IsGhostable();
	void   SetHitBox(HitBox hb);	// Установить hitbox
	HitBox GetHitBox();
private:
	float  deep;
	bool   drawGhost = false;
	bool   drawGhostable = false;
	HitBox hitbox;
};
