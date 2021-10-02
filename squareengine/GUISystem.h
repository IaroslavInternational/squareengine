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

#define N_POINTS 100	// Максимальное кол-во точек на графике

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
	void ShowOptionalPanel();													// Дополнительная панель
	void ShowMouseHelperPanel(std::string info);								// Панель под курсором
	void DisableSides();														// Отключить боковые панели

	/****************************************/
private: 
	/* Методы отрисовки конкретных интерфейсов */
	
	void ShowLog();						// Показать панель лога
	void ShowGPU();						// Показать панель с ифнормацией об FPS и графическом адаптере 
	void ShowFPS();						// Показать панель с графиком изменения FPS
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
	bool ShowFPSGraph 				  =	false;	// График изменения FPS
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
	/* Общие переменные для работы с интерфейсом */

	bool			  DrawingHitBox		= false;	// Идёт отрисовка хитбокса
	bool			  DrawingLine		= false;	// Идёт отрисовка линии
	bool			  SavingSettings    = false;	// Идёт сохранение настроек
	bool			  SettedFirstPoint  = false;	// Поставлена первая точка изменяемого объекта
	bool			  SettedSecondPoint = false;	// Поставлена вторая точка изменяемого объекта
	DirectX::XMFLOAT2 firstPoint;					// Координаты первой точки изменяемого объекта
	DirectX::XMFLOAT2 secondPoint;					// Координаты второй точки изменяемого объекта

	/*********************************************/

	/* Вспомагательные переменные для работы с контейнером персонажей */
	
	std::string personSelected = "";	// Имя выбранного персонажа

	/******************************************************************/

	/* Вспомагательные переменные для работы с физическим движком */

	std::string objectSelected = "";	// Имя выбранного объекта из физического движка

	/**************************************************************/

	/* Вспомагательные переменные для отображения FPS */

	float	arr[N_POINTS];		// Массив значений fps
	float	counters[N_POINTS];	// Массив отчётов
	size_t	counter = 0;		// Счётчик итерации

	/**************************************************/

	std::string mouseHelpInfo = "";	// Подпись под курсором
};
