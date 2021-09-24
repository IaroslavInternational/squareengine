#pragma once

#include "Window.h"

#include "SceneDataReader.h"
#include "PersonContainer.h"
#include "MainPerson.h"

#include "GUISystem.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene(std::string name,	std::shared_ptr<Window> wnd, 
		  std::string data);
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
	/* Вспомогательные переменные */
	
	std::shared_ptr<Window> wnd;	// Указатель на главное окно 
	GUISystem				gui;	// Интерфейс двжика
	SceneDataReader			sdr;	// Ридер данных сцены
	MainPersonDataReader	mdr;	// Ридер данных главного персонажа

	/******************************/
private:
	/* Объекты */
	
	PersonContainer pc;
	MainPerson		hero;
	
	/***********/
};