#pragma once

#include "Object2D.h"
#include "Animation.h"

class Person : public Object2D
{
public:
	friend class GUISystem;
public:
	Person(std::string name,		 DirectX::XMFLOAT2 position,
		   std::string pathToSprite, HitBox			   hitbox,
		   float	   speed 		  =	1.0f, 
		   float	   effectDuration = 0.045f, 
		   float	   effectTime 	  =	0.0f, 
		   bool		   effectActive   =	false);
public:
	/* Главные методы для отрисовки персонажа */
	
	void Draw(Graphics& gfx);					// Отрисовать персонажа
	void SetDirection(DirectX::XMFLOAT2 dir);	// Установить направление положения 
	void Update(float dt);						// Обновить состояние персонажа
	void ActivateEffect();						// Активировать эффект

	/******************************************/
public:
	/* Главные методы для взаимодействия с персонажем */
	
	void   Translate(DirectX::XMFLOAT2 delta);
	void   SetHitBox(HitBox hb);	// Установить hitbox
	HitBox GetHitBox();				// Получить hitbox

	/**************************************************/
private:
	/* Внутренние методы */

	void CalculateDeltas();	// Рассчитать корректирующие отступы hitbox от блока спрайта персонажа

	/*********************/
private:
	/* Переменные описания состояния персонажа */
	
	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// Вектор скорости движения
	float			  speed;				// Скорость перемещения
	struct
	{
		float Duration;	// Продолжительность эффекта
		float Time;		// Время эффекта
		bool Active;	// Состояние активности эффекта
	} effect;			// Параметры эффекта

	/*******************************************/
private:
	/* Переменные описания работы с анимациями */

	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};	// Список анимаций

	std::vector<Animation> animations;								// Вектор анимаций
	Sequence			   iCurSequence = Sequence::StandingDown;	// Текущее состояние анимаций

	/*******************************************/
private:
	/* Переменные описания работы с hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// Видимость hitbox
	float  dx;							// Корректирующий отступ hitbox от блока спрайта по оси x
	float  dy;							// Корректирующий отступ hitbox от блока спрайта по оси y

	/***************************************/
};