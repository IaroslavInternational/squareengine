#include "Camera.h"

#include "MainPerson.h"
#include "PersonContainer.h"
#include "InteractableObject2DContainer.h"

Camera::Camera(MainPerson* hero, PersonContainer* pc, InteractableObject2DContainer* Iobj, std::shared_ptr<Physics::PhysicsEngine> phEngPtr)
	:
	hero(hero),
	pc(pc),
	Iobj(Iobj),
	phEngPtr(phEngPtr)
{}

void Camera::Init()
{
	position = initPosition;

	Translate(initPosition);
	hero->Translate(initPosition);
}

void Camera::Translate(DirectX::XMFLOAT2 delta)
{
	position.x -= delta.x;
	position.y -= delta.y;

	phEngPtr->TranslateObjects(delta);
	Iobj->Translate(delta);
	pc->Translate(delta);
}

void Camera::TranslateAll(DirectX::XMFLOAT2 delta)
{
	delta.x *= noclipSpeed;
	delta.y *= noclipSpeed;

	Translate(delta);
	hero->Translate(delta);
}

bool Camera::GetNoClipState()
{
	return noclip;
}

void Camera::SetPosition(DirectX::XMFLOAT2 pos)
{
	DirectX::XMFLOAT2 dif;
	dif.x = position.x - pos.x;
	dif.y = position.y - pos.y;

	position = pos;

	phEngPtr->TranslateObjects(dif);
	Iobj->Translate(dif);
	pc->Translate(dif);
	hero->Translate(dif);
}

void Camera::SetInitPosition(DirectX::XMFLOAT2 pos)
{
	initPosition = pos;
}
