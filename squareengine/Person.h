#pragma once

#include "Object2D.h"
#include "Animation.h"
#include "Script.h"

struct AnimationData;

class Person : public Object2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	Person(std::string   name,		   DirectX::XMFLOAT2   position,
		   size_t        layer,
		   std::string   pathToSprite, HitBox			   hitbox,
		   AnimationData aData,
		   std::string   scriptPath,
		   float		 speed 			= 1.0f, 
		   float		 effectDuration	= 0.045f, 
		   float		 effectTime 	= 0.0f, 
		   bool			 effectActive	= false);
public:
	/* Главные методы для отрисовки персонажа */
	
	void Draw(Graphics& gfx) override;			// Отрисовать персонажа
	void Process(float dt);						// Игровый цикл персонажа
	void SetDirection(DirectX::XMFLOAT2 dir);	// Установить направление положения 
	void Update(float dt);						// Обновить состояние персонажа
	void ActivateEffect();						// Активировать эффект

	/******************************************/
public:
	/* Главные методы для взаимодействия с персонажем */
	
	void   Translate(DirectX::XMFLOAT2 delta);
	void   SetHitBox(HitBox hb);	// Установить hitbox
	HitBox GetHitBox();				// Получить hitbox
	void   SetAnimation(std::vector<Animation> anim);

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
	};																// Список анимаций
	std::vector<Animation> animations;								// Вектор анимаций
	Sequence			   iCurSequence = Sequence::StandingDown;	// Текущее состояние анимаций
	float				   holdTime;								// Задержка анимации

	/*******************************************/
private:
	/* Переменные описания работы с hitbox */

	HitBox hitbox;						// Hitbox
	float  dx;							// Корректирующий отступ hitbox от блока спрайта по оси x
	float  dy;							// Корректирующий отступ hitbox от блока спрайта по оси y

	/***************************************/
private:
	/* Переменные описания работы скрипта */
	
	std::string scriptPath;
	Script      script;

	/**************************************/
};