#pragma once

#include "Window.h"

#if IS_ENGINE_MODE
#include "ImguiManager.h"
#endif

#include "EngineTimer.h"

//#include "SceneObjects.h"

#include "Character.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene(std::string sceneName,	   std::shared_ptr<Window> _wnd, 
		  std::string data);
	~Scene();

	/***** /Методы сцены\ *****/

	// Обработка входных данных
	void ProcessInput(float dt);

	// Отрисовка сцены
	void Render(float dt);

	std::pair<std::string, bool> IsOnTheSceneTrigger();

	// Возращение камеры на исходное положение
	void ResetPos();

	// Геттер имени сцены
	std::string GetName() const;

	/***** \Методы сцены/ *****/
private:
#if IS_ENGINE_MODE
	/* -- Интерфейс -- */

	/**** Панели ****/

	// Установка положения и размеров панели
	void SetPanelWidthAndPos(int corner, float width, float height, float x_offset = 0.0f, float y_offset = 0.0f);

	// Верхня панель меню
	void ShowMenu();

	// Левая панель
	void ShowLeftSide();

	// Правая панель
	void ShowRightSide();

	// Левая нижняя панель
	void ShowLeftBottomSide();

	// Нижняя панель
	void ShowBottomPanel();	

	/****************/

	// Отслеживание пересечения триггера
	void ShowTrigCheck();

	// Показать FPS и графические адаптеры *Левая нижняя панель*
	void ShowFPSAndGPU();

	// Показать лог *Нижняя панель*
	void ShowLog();

	// Показать редактор узлов *Отдельное окно*
	void ShowNodeEditor();

	// Отключить боковые панели
	void DisableSides();

	// Отключить все панели
	void DisableAll();

	// Показать интерфейс
	void ShowGui();

	// Цветовая гамма ImGui
	void SetGuiColors();

	/*******************/
	
	// Демо-интерфейс 
	void ShowImguiDemoWindow();

	/*******************/

#endif // IS_ENGINE_MODE
private:
	// Идентификаторы сцены
	std::string sceneName;

	// Настройки
#if IS_ENGINE_MODE
	bool savingDepth = false;
#endif // IS_ENGINE_MODE

	bool showDemoWindow = true;
	bool onTrigger = false; const char* triggerGoal = nullptr;
	bool cursorState = false;

#if IS_ENGINE_MODE
	/* Интерфейс */

	// Левая панель моделей на сцене
	bool ShowModelsList = true;

	// Правая панель управления настройками моделей
	bool ShowModelsSettings = true;

	// FPS и информация о графическом адаптере
	bool ShowHardwareInfo = true;

	// Лог
	bool ShowLogs = true;

	// Левая панель триггеров на сцене
	bool ShowTriggersList = false;

	// Правая панель триггеров на сцене
	bool ShowTriggersSettings = false;

	// Левая панель точечных источников света на сцене
	bool ShowPLightsList = false;

	// Правая панель управления настройками точечных источников света
	bool ShowPLightsSettings = false;

	// Левая панель камер на сцене с настройками
	bool ShowCamsList = false;

	// Редактор узлов
	bool ShowNodeEditorWnd = false;

	/*************/
#endif // IS_ENGINE_MODE
private:
	// Указатель на главное окно 
	std::shared_ptr<Window> wnd;

	// Рендер граф
	/********* -Объекты- *********/

	//SceneObjects objects;
	Character person;

	/*****************************/
};