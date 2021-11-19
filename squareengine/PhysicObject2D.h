#pragma once

#include "Object2D.h"

class PhysicObject2D : public Object2D
{
public:
	PhysicObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite,
				   int jump_height, float gravity);
public:
	void AllowMoveUp();
	void AllowMoveDown();
	void AllowMoveLeft();
	void AllowMoveRight();
	void DisAllowMoveUp();
	void DisAllowMoveDown();
	void DisAllowMoveLeft();
	void DisAllowMoveRight();
protected:
	int   jump_height;		     // Высота прыжка
	int	  jump_count;			 // Вспомогательня переменная для прыжка
	bool  IsOnJump     = false;	 // Состояние прыжка
	bool  IsMovingDown = false;	 // Состояние движения после прыжка
	float gravity;				 // Коэффицент притяжения
protected:
	bool AllowedMovingUp    = true; // Состояние ограничения движения вверх
	bool AllowedMovingDown  = true;	// Состояние ограничения движения вниз
	bool AllowedMovingLeft  = true;	// Состояние ограничения движения влево
	bool AllowedMovingRight = true;	// Состояние ограничения движения вправо
};

