#pragma once

#include "ImguiManager.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"
#include "Scene.h"
#include "GUISystem.h"
#include "SceneTransition.h"

#include <map>

class App
{
public:
	App(const std::string& commandLine, const std::string& projectName);
public:
	int Go();											// Игровой цикл
private:
	void HandleInput(float dt);							// Обработка данных переферии
	void DoFrame(float dt);								// Отрисовка кадра
private:
	ImguiManager imgui;									// Инициализация интерфейса 
private:
	std::shared_ptr<Window>					wnd;		// Указатель на окно
	std::shared_ptr<Physics::PhysicsEngine> phEngine;	// Указатель на физический движок
	std::map<std::string, bool>				scenes;		// Контейнер контроля активности сцен
	std::unique_ptr<Scene>					scene;		// Текущая сцена
	std::shared_ptr<GUISystem>				gui;		// Указатель на интерфейс
	std::unique_ptr<SceneTransition>		transition;
private:
	std::string				   commandLine;						// Коммандная строка
	std::string				   projectName;						// Имя проекта
	ScriptCommander			   scriptCommander;					// Управление командными скриптами
	EngineTimer				   timer;								// Игровой таймер
	float					   speed_factor = 1.0f;				// Калибровочный параметр скорости
	std::optional<std::string> curTrigger = std::nullopt;
};