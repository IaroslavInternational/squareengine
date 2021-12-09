#pragma once

#include "Object2D.h"
#include "HitBox.h"

class PhysicObject2D : public Object2D
{
public:
	friend class GUISystem;
public:
	PhysicObject2D(std::string name, DirectX::XMFLOAT2 position, HitBox	hitbox, size_t layer, std::string pathToSprite,
				   Color key, float speed, int jump_height, float gravity);
public:
	void AllowMoveUp();
	void AllowMoveDown();
	void AllowMoveLeft();
	void AllowMoveRight();
	void DisAllowMoveUp();
	void DisAllowMoveDown();
	void DisAllowMoveLeft();
	void DisAllowMoveRight();
public:
	/* Внутренние методы */
	
	void   SetHitBox(HitBox hb); // Установить hitbox
	HitBox GetHitBox();			 // Получить hitbox
	void   CalculateDeltas();	 // Рассчитать корректирующие отступы hitbox от блока спрайта персонажа
	int    GetDirection();		 // Получить направление

	/*********************/
protected:
	int				  jump_height;					 // Высота прыжка
	int				  jump_count;					 // Вспомогательня переменная для прыжка
	bool			  IsOnJump     = false;			 // Состояние прыжка
	bool			  IsMovingDown = false;			 // Состояние движения после прыжка
	float			  gravity;						 // Коэффицент притяжения
	float			  speed;						 // Скорость
	DirectX::XMFLOAT2 vel		   = { 0.0f, 0.0f }; // Вектор скорости движения
	float			  inertion	   = 0.0f;			 // Инерция
protected:
	bool AllowedMovingUp    = true; // Состояние ограничения движения вверх
	bool AllowedMovingDown  = true;	// Состояние ограничения движения вниз
	bool AllowedMovingLeft  = true;	// Состояние ограничения движения влево
	bool AllowedMovingRight = true;	// Состояние ограничения движения вправо
protected:
	/* Переменные описания работы с hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// Видимость hitbox
	float  dx;							// Корректирующий отступ hitbox от блока спрайта по оси x
	float  dy;							// Корректирующий отступ hitbox от блока спрайта по оси y

	/***************************************/
protected:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		StandingLeft,
		StandingRight,
		Count
	};
	Sequence iCurSequence = Sequence::StandingRight;	// Текущее состояние анимаций
};

