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
	friend class ObjectsQueue;
public:
	MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera);
public:
	/* Главные методы для отрисовки главного персонажа */

	void Draw(Graphics& gfx) override;			// Отрисовать главного персонажа
	void ProcessMoving(float dt);
	void SetDirection(DirectX::XMFLOAT2 dir);	// Установить направление положения 
	void Update(float dt);						// Обновить состояние главного персонажа
	void ActivateEffect();						// Активировать эффект

	/***************************************************/
public:
	/* Главные методы для взаимодействия с главным персонажем */
	
	void   Translate(DirectX::XMFLOAT2 delta);
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
	void   SetAnimation(std::vector<Animation> anim);

	/**********************************************************/
private:
	/* Внутренние методы */

	void CalculateDeltas();	// Рассчитать корректирующие отступы hitbox от блока спрайта персонажа

	/*********************/
private:
	/* Переменные описания состояния персонажа */

	std::string		  dataPath;							// Путь к данным о главном персонаже
	DirectX::XMFLOAT2 vel		   = { 0.0f, 0.0f };	// Вектор скорости движения
	float			  speed;							// Скорость перемещения
	int				  jump_height  = 8;					// Высота прыжка
	int				  jump_count;						// Вспомогательня переменная для прыжка
	bool			  IsMovingDown = false;				// Состояние движения после прыжка
	float			  gravity	   = 300.0f;			// Коэффицент притяжения
	bool			  IsOnJump	   = false;				// Состояние прыжка
	struct
	{
		float Duration;									// Продолжительность эффекта
		float Time;										// Время эффекта
		bool Active;									// Состояние активности эффекта
	} effect;											// Параметры эффекта
	bool AllowedMovingUp    = true;						// Состояние ограничения движения вверх
	bool AllowedMovingDown  = true;						// Состояние ограничения движения вниз
	bool AllowedMovingLeft  = true;						// Состояние ограничения движения влево
	bool AllowedMovingRight = true;						// Состояние ограничения движения вправо

	/*******************************************/
private:
	/* Переменные описания работы с анимациями */

	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		StandingLeft,
		StandingRight,
		Count
	};																// Список анимаций
	std::vector<Animation> animations;								// Вектор анимаций
	Sequence			   iCurSequence = Sequence::StandingRight;	// Текущее состояние анимаций
	float				   holdTime;								// Задержка анимации

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
	};									// Список режимов камеры
	std::shared_ptr<Window> wnd;		// Указатель на главное окно
	std::shared_ptr<Camera> camera;		// Указатель на камеру
	CameraMode				cameraMode;	// Режим камеры
};
