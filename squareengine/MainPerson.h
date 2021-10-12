#pragma once

#include "MainPersonDataReader.h"
#include "Object2D.h"
#include "Animation.h"
#include "Window.h"
#include "Camera.h"

class MainPerson : public Object2D
{
public:
	friend class GUISystem;
public:
	MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera);
public:
	/* Главные методы для отрисовки главного персонажа */

	void Draw();								// Отрисовать главного персонажа
	void ProcessMoving(float dt);
	void SetDirection(DirectX::XMFLOAT2 dir);	// Установить направление положения 
	void Update(float dt);						// Обновить состояние главного персонажа
	void ActivateEffect();						// Активировать эффект

	/***************************************************/
public:
	/* Главные методы для взаимодействия с главным персонажем */
	
	void   AllowMoveUp();
	void   AllowMoveDown();
	void   AllowMoveLeft();
	void   AllowMoveRight();
	void   DisAllowMoveUp();
	void   DisAllowMoveDown();
	void   DisAllowMoveLeft();
	void   DisAllowMoveRight();
	void   SetHitBox(HitBox hb);	// Установить hitbox
	HitBox GetHitBox();				// Получить hitbox

	/**********************************************************/
private:
	/* Внутренние методы */

	void CalculateDeltas();	// Рассчитать корректирующие отступы hitbox от блока спрайта персонажа

	/*********************/
private:
	/* Переменные описания состояния персонажа */

	std::string		  dataPath;				// Путь к данным о главном персонаже
	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// Вектор скорости движения
	float			  speed;				// Скорость перемещения
	struct
	{
		float Duration;	// Продолжительность эффекта
		float Time;		// Время эффекта
		bool Active;	// Состояние активности эффекта
	} effect;			// Параметры эффекта
	bool AllowedMovingUp    = true;
	bool AllowedMovingDown  = true;	
	bool AllowedMovingLeft  = true;
	bool AllowedMovingRight = true;

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

	/*******************************************/
private:
	/* Переменные описания работы с hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// Видимость hitbox
	float  dx;							// Корректирующий отступ hitbox от блока спрайта по оси x
	float  dy;							// Корректирующий отступ hitbox от блока спрайта по оси y

	/***************************************/
private:
	enum class CameraMode
	{
		SteadyPerson,
		SteadyWorld
	};

	std::shared_ptr<Window> wnd;	// Указатель на главное окно
	std::shared_ptr<Camera> camera;	// Указатель на камеру
	CameraMode				cameraMode = CameraMode::SteadyPerson;
};
