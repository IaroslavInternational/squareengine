#include "Camera.h"

Camera::Camera(std::shared_ptr<Physics::PhysicsEngine> phEngPtr, PersonContainer* pcPtr)
	:
	phEngPtr(phEngPtr),
	pcPtr(pcPtr)
{}

void Camera::Translate(DirectX::XMFLOAT2 delta)
{
	phEngPtr->TranslateObjects(delta);
	pcPtr->Translate(delta);
}
