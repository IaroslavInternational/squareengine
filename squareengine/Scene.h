#pragma once

#include "Window.h"

#include "EngineTimer.h"
#include "SceneDataReader.h"
#include "PersonContainer.h"
#include "MainPerson.h"

#include "Font.h"
#include "GUISystem.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene(std::string name,	std::shared_ptr<Window> _wnd, 
		  std::string data);
	~Scene();

	/***** /Методы сцены\ *****/

	// Обработка входных данных
	void ProcessInput(float dt);

	// Отрисовка сцены
	void Render(float dt);

	std::pair<std::string, bool> IsOnTheSceneTrigger();

	// Геттер имени сцены
	std::string GetName() const;

	/***** \Методы сцены/ *****/
private:
	// Идентификаторы сцены
	std::string name;

	// Настройки
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;
private:
	// Указатель на главное окно 
	std::shared_ptr<Window> wnd;

	// Интерфейс
	GUISystem gui;

	// Ридер данных сцены
	SceneDataReader sdr;
	MainPersonDataReader mdr;

	/********* -Объекты- *********/

	PersonContainer pc;
	MainPerson hero;
	/*****************************/
};