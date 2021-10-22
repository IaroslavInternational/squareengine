#pragma once

#include "Window.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"

#include "ImguiManager.h"

#include "Scene.h"
#include "PhysicsEngine.h"

#include <map>

class App
{
public:
	App(const std::string& commandLine = "");
	~App();
public:
	int Go();											// Игровой цикл
private:
	void HandleInput(float dt);							// Обработка данных переферии
	void DoFrame(float dt);								// Отрисовка кадра
private:	
	std::string commandLine;							// Коммандная строка
	ImguiManager imgui;									//
	std::shared_ptr<Window> wnd;						// Указатель на окно
	std::shared_ptr<Physics::PhysicsEngine> phEngine;	//
	ScriptCommander scriptCommander;					// Управление скриптами
	EngineTimer timer;									// Игровой таймер
	float speed_factor = 1.0f;							// Скорость
	std::map<std::string, bool> scenes;					// Контейнер сцен
	std::unique_ptr<Scene> scene;						// Контейнер сцен
};