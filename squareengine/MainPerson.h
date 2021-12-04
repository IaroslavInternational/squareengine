#pragma once

#include "MainPersonDataReader.h"
#include "AliveObject2D.h"
#include "Animation.h"
#include "Window.h"
#include "Camera.h"

class MainPerson : public AliveObject2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera);
public:
	/* Главные методы для отрисовки главного персонажа */

	void Draw(Graphics& gfx) override;			// Отрисовать главного персонажа
	void Process(float dt);
	void SetDirection(DirectX::XMFLOAT2 dir);	// Установить направление положения 
	void Update(float dt);						// Обновить состояние главного персонажа
	void ActivateEffect();						// Активировать эффект

	/***************************************************/
public:
	/* Главные методы для взаимодействия с главным персонажем */
	
	void Translate(DirectX::XMFLOAT2 delta);
	void SetAnimation(std::vector<Animation> anim);

	/**********************************************************/
private:
	/* Переменные описания состояния персонажа */

	std::string		  dataPath;				// Путь к данным о главном персонаже
	float			  speed;
	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// Вектор скорости движения
	float			  inertion = 0.0f;

	struct
	{
		float Duration;						// Продолжительность эффекта
		float Time;							// Время эффекта
		bool Active;						// Состояние активности эффекта
	} effect;								// Параметры эффекта

	/*******************************************/
private:
	/* Переменные описания работы с анимациями */

	std::vector<Animation> animations;	// Вектор анимаций
	float				   holdTime;	// Задержка анимации

	/*******************************************/
private:
	enum class CameraMode
	{
		SteadyPerson,
		SteadyWorld,
		Hybrid
	};									// Список режимов камеры
	std::shared_ptr<Window> wnd;		// Указатель на главное окно
	std::shared_ptr<Camera> camera;		// Указатель на камеру
	CameraMode				cameraMode;	// Режим камеры
};
