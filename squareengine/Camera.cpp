#include "Camera.h"

#include "PersonContainer.h"
#include "InteractableObject2DContainer.h"

Camera::Camera(PersonContainer* pc, InteractableObject2DContainer* Iobj, std::shared_ptr<Physics::PhysicsEngine> phEngPtr)
	:
	pc(pc),
	Iobj(Iobj),
	phEngPtr(phEngPtr)
{}

void Camera::Translate(DirectX::XMFLOAT2 delta)
{
	phEngPtr->TranslateObjects(delta);
	Iobj->Translate(delta);
	pc->Translate(delta);
}
