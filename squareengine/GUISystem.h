#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"
#include "PersonContainer.h"
#include "MainPerson.h"
#include "PhysicsEngine.h"
#include "Window.h"

#include <map>

class GUISystem
{
public:
	GUISystem(std::shared_ptr<Window>				  wnd,
			  PersonContainer*						  persons, 
			  MainPerson*							  mPersPtr,
			  std::shared_ptr<Physics::PhysicsEngine> phEngPtr);
public:
	/* Главные методы для отрисовки интерфейса */
	
	void Show();					// Показать интерфейс
	void Hide();					// Скрыть интерфейс
	void AddLog(const char* text);	// Добавить лог

	/*******************************************/
private: 
	/* Методы настройки и отрисовки панелей */
	
	void SetGUIColors();														// Установка цветовой гаммы
	void SetPanelWidthAndPosition(int   corner,			float width,			// Установка положения и размеров панели
								  float height,			float x_offset = 0.0f, 
								  float y_offset = 0.0f); 	
	void ShowMenu(); 															// Верхняя панель меню
	void ShowLeftSide();														// Левая панель	
	void ShowRightSide();														// Правая панель
	void ShowLeftBottomSide();													// Левая нижняя панель
	void ShowBottomPanel();														// Нижняя панель
	void ShowMouseHelperPanel(std::string info);								// Панель под курсором
	void DisableSides();														// Отключить боковые панели

	/****************************************/
private: 
	/* Методы отрисовки конкретных интерфейсов */
	
	void ShowLog();						// Показать панель лога
	void ShowFPSAndGPU();				// Показать панель с ифнормацией об FPS и графическом адаптере 
	void ShowPersonList();				// Показать список персонажей 
	void ShowPersonControl();			// Показать настройки для выбранного персонажа
	void ShowMainPersonList();			// Показать главного персонажа в списке
	void ShowMainPersonControl();		// Показать настройки для главного персонажа
	void ShowPhysicsEngineObjList();	// Показать список объектов в физическом движке
	void ShowPhysicsEngineObjControl();	// Показать настройки для выбранного объекта в физическом движке

	/*******************************************/
private:
	/* Переменные видимости панелей */
	
	bool ShowPersonEnum 			  =	false;	// Левая панель объектов на сцене
	bool ShowPersonSettings			  =	false;	// Правая панель управления настройками объектов	
	bool ShowMainPersonEnum 		  =	false;	// Левая панель главного персонажа на сцене
	bool ShowMainPersonSettings		  =	false;	// Правая панель управления настройками главного персонажа
	bool ShowPhysicsEngineObjEnum	  = true;	// Левая панель
	bool ShowPhysicsEngineObjSettings = true;	// Правая панель 
	bool ShowHardwareInfo 			  =	true;	// FPS и информация о графическом адаптере
	bool ShowLogs 					  =	true;	// Лог
	bool ShowTriggersList 			  =	false;	// Левая панель триггеров на сцене
	bool ShowTriggersSettings		  =	false;	// Правая панель триггеров на сцене

	/********************************/
private:
	/* Системные переменные */
	
	std::shared_ptr<Window>					wnd;		// Указатель на окно отрисовки
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;	// Указатель на физический движок
	std::map<std::wstring, double>			gpu_desc;	// Описание графических адаптеров [first - имя, second - объём памяти]
	AppLog									applog;		// Лог

	/************************/
private:
	/* Указатели на объекты */

	PersonContainer* persConPtr;	// Указатель на контейнер персонажей
	MainPerson*		 mPersPtr;		// Указатель на главного персонажа

	/***************************/
private:
	/* Вспомагательные переменные для работы с контейнером персонажей */
	
	bool			  DrawingHitBox	    = false;	// Идёт изменение хитбокса персонажа
	bool			  SavingSettings	= false;	// Идёт сохранение настроек персонажа
	bool			  SettedFirstPoint  = false;	// Поставлена первая точка hitbox
	bool			  SettedSecondPoint = false;	// Поставлена вторая точка hitbox
	DirectX::XMFLOAT2 firstPoint;					// Координаты первой точки hitbox
	DirectX::XMFLOAT2 secondPoint;					// Координаты второй точки hitbox
	std::string		  personSelected    = "";		// Имя выбранного персонажа

	/******************************************************************/

	/* Вспомагательные переменные для работы с физическим движком */

	std::string objectSelected = "";	// Имя выбранного объекта из физического движка

	/**************************************************************/

	std::string mouseHelpInfo = "";	// Подпись под курсором
};
