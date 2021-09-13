#pragma once

#include "Window.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"

#include "ImguiManager.h"

#include "Scene.h"

#include <map>

class App
{
public:
	App(const std::string& commandLine = "");
	~App();

	// Игровой цикл
	int Go();
private:
	// Обработка данных переферии
	void HandleInput(float dt);

	// Отрисовка кадра
	void DoFrame(float dt);
private:
	// Коммандная строка
	std::string commandLine;

	ImguiManager imgui;

	// Указатель на окно
	std::shared_ptr<Window> wnd;

	// Управление скриптами
	ScriptCommander scriptCommander;
	
	// Игровой таймер
	EngineTimer timer;

	// Скорость
	float speed_factor = 1.0f;

	// Контейнер сцен
	std::map<std::unique_ptr<Scene>, bool> scenes;
};