#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"

#include <map>
#include <string>

#include "PersonContainer.h"

class GUISystem
{
public:
	GUISystem(std::shared_ptr<Window> wnd, std::vector<std::unique_ptr<Person>>* pv_ptr);
public: // Главные методы
	// Показать интерфейс
	void Show();

	// Скрыть интерфейс
	void Hide();

	void AddLog(const char* text);
private: // Методы настройки и отрисовки панелей
	void SetGUIColors();

	// Установка положения и размеров панели
	void SetPanelWidthAndPosition(int corner, float width, float height, float x_offset = 0.0f, float y_offset = 0.0f);

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

	// Отключить боковые панели
	void DisableSides();
private: // Методы отрисовки конкретных интерфейсов
	// Показать панель с ифнормацией об FPS и графическом адаптере 
	void ShowFPSAndGPU();

	void ShowMouseCoordinates();

	void ShowLog();

	void ShowPersonList();
	void ShowPersonControl();
private: // Переменные видимости панелей
	// Левая панель объектов на сцене
	bool ShowPersonEnum = true;

	// Правая панель управления настройками объектов
	bool ShowPersonSettings = true;

	// FPS и информация о графическом адаптере
	bool ShowHardwareInfo = true;

	// Лог
	bool ShowLogs = true;

	// Левая панель триггеров на сцене
	bool ShowTriggersList = false;

	// Правая панель триггеров на сцене
	bool ShowTriggersSettings = false;
private:
	std::shared_ptr<Window> wnd;

	// Лог
	AppLog applog;
	std::map<std::wstring, double> gpu_desc;
	std::pair<bool, std::pair<int, int>> pointSet = { false, {0, 0} };
	bool IsDrawingHitBox = false;
private:
	std::string personSelected = "";
	std::vector<std::unique_ptr<Person>>* pv_ptr;
};

