#pragma once

#include "Window.h"

#include "SceneDataReader.h"
#include "ObjectsQueue.h"
#include "PhysicsEngine.h"
#include "Camera.h"

#include "GUISystem.h"

class Scene
{
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd,		
		  std::string							  scData, 
		  std::shared_ptr<Physics::PhysicsEngine> phEngine);
	~Scene();
public:
	/* Методы сцены */

	void						 ProcessInput(float dt);	// Обработка входных данных переферии
	void						 Render(float dt);			// Отрисовка сцены
	std::pair<std::string, bool> IsOnTheSceneTrigger();		// Состояние вызова триггера перехода на сцене 
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
	
	std::shared_ptr<Window> wnd;	// Указатель на главное окно 
	std::shared_ptr<Camera> camera;	// Указатель на камеру 
	GUISystem				gui;	// Интерфейс двжика
	SceneDataReader			sdr;	// Ридер данных сцены
	MainPersonDataReader	mdr;	// Ридер данных главного персонажа

	/******************************/
private:
	/* Объекты */

	MainPerson					  hero;
	PersonContainer				  persCon;
	InteractableObject2DContainer Iobj;
	ObjectsQueue				  objQueue;

	/***********/
};