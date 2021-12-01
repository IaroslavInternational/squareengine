#pragma once

#include "Window.h"

#include "PhysicsEngine.h"
#include "SceneDataReader.h"
#include "Map.h"

class Scene
{
public:
	friend class GUISystem;
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd,		
		  std::string							  scData, 
		  std::shared_ptr<Physics::PhysicsEngine> phEngine);
public:
	/* Методы сцены */

	void						 ProcessInput(float dt);	// Обработка входных данных переферии
	void						 Render(float dt);			// Отрисовка сцены
	std::optional<std::string>   IsOnTheSceneTrigger();		// Состояние вызова триггера перехода на сцене 
	const std::string&			 GetName() const;			// Получить имя сцены

	/****************/
private:
	/* Переменные описания сцены */
	
	std::string name;	// Имя сцены

	/*****************************/
private:
	/* Модули движка */

	std::shared_ptr<Physics::PhysicsEngine> phEngine;

	/*****************/
private:
	/* Вспомогательные переменные */
	
	SceneDataReader			sdr;	// Ридер данных сцены
	std::shared_ptr<Window> wnd;	// Указатель на главное окно 
	std::shared_ptr<Camera> camera;	// Указатель на камеру 

	/******************************/
private:
	Map world;
};