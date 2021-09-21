#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"
#include "PersonContainer.h"

#include <map>
#include <string>

class GUISystem
{
public:
	GUISystem(std::shared_ptr<Window> wnd, PersonContainer* persons);
public:
	/* Главные методы для отрисовки интерфейса */
	
	void Show();					// Показать интерфейс
	void Hide();					// Скрыть интерфейс
	void AddLog(const char* text);	// Добавить лог

	/*******************************************/
private: 
	/* Методы настройки и отрисовки панелей */
	
	void SetGUIColors();														// Установка цветовой гаммы
	void SetPanelWidthAndPosition(int corner,			float width,			// Установка положения и размеров панели
								  float height,			float x_offset = 0.0f, 
								  float y_offset = 0.0f); 	
	void ShowMenu(); 															// Верхняя панель меню
	void ShowLeftSide();														// Левая панель	
	void ShowRightSide();														// Правая панель
	void ShowLeftBottomSide();													// Левая нижняя панель
	void ShowBottomPanel();														// Нижняя панель
	void DisableSides();														// Отключить боковые панели

	/****************************************/
private: 
	/* Методы отрисовки конкретных интерфейсов */
	
	void ShowFPSAndGPU();			// Показать панель с ифнормацией об FPS и графическом адаптере 
	void ShowMouseCoordinates();	// Показать подпись координат под курсором
	void ShowLog();					// Показать панель лога
	void ShowPersonList();			// Показать список персонажей 
	void ShowPersonControl();		// Показать настройки для выбранного персонажа

	/*******************************************/
private:
	/* Переменные видимости панелей */
	
	bool ShowPersonEnum 	  =	true;	// Левая панель объектов на сцене
	bool ShowPersonSettings   =	true;	// Правая панель управления настройками объектов
	bool ShowHardwareInfo 	  =	true;	// FPS и информация о графическом адаптере
	bool ShowLogs 			  =	true;	// Лог
	bool ShowTriggersList 	  =	false;	// Левая панель триггеров на сцене
	bool ShowTriggersSettings = false;	// Правая панель триггеров на сцене

	/********************************/
private:
	/* Системные переменные */
	
	std::shared_ptr<Window>		   wnd;			// Указатель на окно отрисовки
	std::map<std::wstring, double> gpu_desc;	// Описание графических адаптеров [first - имя, second - объём памяти]
	AppLog						   applog;		// Лог

	/************************/
private:
	/* Указатели на контейнеры */

	PersonContainer* persConPtr;

	/***************************/
private:
	/* Вспомагательные переменные для работы с контейнером персонажей */
	
	bool			DrawingHitBox	  = false;	// Идёт изменение хитбокса персонажа
	bool			SettedFirstPoint  = false;	// Поставлена первая точка
	bool			SettedSecondPoint = false;	// Поставлена вторая точка
	DirectX::XMINT2 firstPoint;					// Координаты первой точки
	DirectX::XMINT2 secondPoint;				// Координаты второй точки
	std::string		personSelected	  = "";		// Имя выбранного персонажа

	/******************************************************************/
};

