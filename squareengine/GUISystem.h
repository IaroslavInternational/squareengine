#pragma once

#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"
#include "Scene.h"
#include "AnimationData.h"
#include "Window.h"
#include "NodeEditor.h"

#include <map>

#define N_POINTS   100						// Максимальное кол-во точек на графике
#define MAX_POINTS (double)N_POINTS - 1.0;	// Максимальное кол-во точек на графике для корректного отображения	

struct IobjData
{
	size_t			  layer;
	std::string		  name;
	std::string		  pathToSprite;
	DirectX::XMFLOAT2 position;
};

struct PersonData
{
	size_t					   layer;
	std::string				   name;
	std::string				   pathToSprite;
	DirectX::XMFLOAT2		   position;
	std::vector<AnimationData> animPersonData;
};

class GUISystem
{
public:
	GUISystem(Scene* scene);	// Конструктор (принимает указатель на сцену)
public:
	/* Главные методы для отрисовки интерфейса */

	void						 Show(float dt);						// Показать интерфейс (принимает время одного кадра)
	void						 Hide();								// Скрыть интерфейс
	void						 BeginFrame();							// Начало кадра
	void						 EndFrame();							// Конец кадра
	void						 AddLog(const std::ostringstream& oss);	// Добавить лог
	void						 AddLog(std::string str);				// Добавить лог
	void						 AddLog(const char* text);				// Добавить лог
	void						 LoadScene(Scene* scene);				// Загрузить сцену
	std::pair<bool, std::string> UpdatingScene();						// Идёт обновление сцены
	bool						 AddingScene();							// Идёт добавление сцены
	void						 SetAddingSceneState(bool state);		// Установить статус добавления сцены

	/*******************************************/
private: 
	/* Методы настройки и отрисовки панелей */
	
	void SetGUIColors();														// Установка цветовой гаммы
	void SetPanelSizeAndPosition(int   corner,			float width,			// Установка положения и размеров панели
								 float height,			float x_offset = 0.0f, 
								 float y_offset = 0.0f); 	
	void ShowMenu(); 															// Верхняя панель меню
	void ShowLeftSide(float dt);														// Левая панель	
	void ShowRightSide(float dt);												// Правая панель
	void ShowLeftBottomSide();													// Левая нижняя панель
	void ShowBottomPanel();														// Нижняя панель
	void ShowOptionalPanel();													// Дополнительная панель
	void ShowMouseHelperPanel(std::string info);								// Панель под курсором
	void DisableSides();														// Отключить боковые панели

	/****************************************/
private: 
	/* Методы отрисовки конкретных интерфейсов */

	void   ShowMainPersonList();			// Показать левую панель главного персонажа в списке
	void   ShowPersonList(float dt);		// Показать левую панель списка персонажей 
	void   ShowIobjList();					// Показать	левую панель списка интерактивных объектов
	void   ShowTriggerList();				// Показать левую панель списка триггеров
	void   ShowPhysicsEngineObjList();		// Показать левую панель списка объектов в физическом движке		   

	void   ShowMainPersonControl(float dt); // Показать правую панель настроек для главного персонажа
	void   ShowPersonControl(float dt);		// Показать правую панель настроек для выбранного персонажа
	void   ShowIobjControl();				// Показать правую панель настроек для выбранного интерактивного объекта
	void   ShowTriggerControl();			// Показать правую панель настроек для выбранного триггера
	void   ShowPhysicsEngineObjControl();	// Показать правую панель настроек для выбранного объекта в физическом движке		   

	void   ShowLayersControl();				// Показать панель настроек слоёв
	void   ShowCameraControl();				// Показать панель настроек камеры
	void   ShowScenesControl();		   		// Показать панель настроек сцен
	void   SpawnCameraToHeroControl();		// Показать модуль настроек для взаимодейтсвия камеры и героя
	void   SpawnDefaultObject2DControl(
		   Object2D*   obj, 
		   std::string dataPath);			// Показать модуль настроек для объекта на базе Object2D
	void   ShowScriptsControl();			// Показать панель настроек для скриптов

	void   ShowLog();						// Показать панель лога
	void   ShowGPU();						// Показать панель с ифнормацией об FPS и графическом адаптере 
	void   ShowFPS();						// Показать панель с графиком изменения FPS		   
	void   ShowPhysicsEngineObjHelp();		// Показать подписи имён объектов физического движка
	void   ShowPhysicsEngineSettings();		// Показать панель настроек физического движка
	void   ShowGraphicsEngineSettings();	// Показать панель настроек графики
	void   ShowProjectControl();			// Показать панель настроек проекта
	void   ShowViewportControl();			// Показать панель настроек Viewport
	HitBox CreateNewHitBox();				// Создать HitBox с помощью интерфейса
	
	std::string					ShowLoadingSpriteDilaog();				// Показать диалоговое окно для загрузки спрайта
	std::optional<IobjData>		ShowAddingIobjDialog();					// Показать диалоговое окно для добавления интерактивного объекта
	std::optional<PersonData>	ShowAddingPersonDialog(float dt);		// Показать диалоговое окно для добавления персонажа
	std::vector<AnimationData>	ShowAnimationCreatingDialog(float dt);	// Показать диалоговое окно для создания анимации

	/*******************************************/
private:
	/* Методы сохранения данных */

	void SaveAll();					// Сохранить все настройки
	void SaveMainPersonData();		// Сохранить настройки для главного персонажа
	void SavePersonsData();			// Сохранить настройки для персонажей
	void SaveIobjData();			// Сохранить настройки для интерактивных объектов
	void SaveTriggersData();		// Сохранить настройки для триггеров
	void SaveLayersData();			// Сохранить настройки для слоёв
	void SaveCameraData();			// Сохранить настройки для камеры
	void SaveScenesData();			// Сохранить настройки для сцен
	void SavePhysicsEngineData();	// Сохранить настройки для физического движка
	void SaveGraphicsEngineData();	// Сохранить настройки для графического движка

	/****************************/
private:
	/* Переменные видимости панелей */
	
	bool ShowMainPersonEnum			  = true;	// Видимость левой панели списка главного персонажа
	bool ShowPersonEnum				  = false;	// Видимость левой панели списка персонажей
	bool ShowIobjEnum				  = false;	// Видимость левой панели списка интерактивных объектов
	bool ShowPhysicsEngineObjEnum	  = false;	// Видимость левой панели списка физических объектов
	bool ShowTriggersEnum			  = false;	// Видимость левой панели списка триггеров на сцене

	bool ShowMainPersonSettings		  = true;	// Видимость правой панели настроек главного персонажа
	bool ShowPersonSettings			  = false;	// Видимость правой панели настроек выбранного персонажа	
	bool ShowIobjSettings			  = false;	// Видимость правой панели настроек интерактивного объекта
	bool ShowPhysicsEngineObjSettings = false;	// Видимость правой панели настроек выбранного объекта в физическом движке
	bool ShowTriggersSettings		  = false;	// Видимость правой панели настроек выбранного триггера

	bool ShowLayersSettings			  = false;	// Видимость панели настроек слоёв
	bool ShowCameraSettings			  = false;	// Видимость панели настроек камеры
	bool ShowScenesSettings			  = false;	// Видимость панели настроек сцен

	bool ShowPhysicsEngineObjInfo	  = true;	// Видимость подписей имён объектов физического движка  
	bool ShowPhysicsSettings		  = false;	// Видимость панели настроек физического движка
	bool ShowGraphicsSettings		  = false;	// Видимость панели настроек графики
	bool ShowProjectSettings		  = false;	// Видимость панели настроек проекта
	bool ShowViewportSettings		  = false;	// Видимость панели настроек Viewport
	bool ShowLogs					  = true;	// Видимость лога
	bool ShowHardwareInfo 			  =	true;	// Видимость FPS и информации о графическом адаптере
	bool ShowFPSChart 				  =	false;	// Видимость графика изменения FPS
	bool ShowScriptsSettings		  = false;	// Видимость панели настроек скриптов

	/********************************/
private:
	/* Переменные для работы с настройками сцен */
	
	bool		SavingScenesSettings = false;	// Статус сохранения настроек сцен
	bool		IsAddingScene		 = false;	// Статус добавления сцены
	bool		IsUpdatingScene		 = false;	// Статус изменения сцены на другую
	std::string curSceneName;					// Имя текущей сцены

	/********************************************/
private:
	/* Системные переменные */

	AppLog									applog;		// Лог
	std::shared_ptr<Window>					wnd;		// Указатель на окно отрисовки
	std::shared_ptr<Camera>					camera;		// Указатель на камеру
	std::map<std::wstring, double>			gpu_desc;	// Контейнер описаний графических адаптеров [first - имя, second - объём памяти]
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;	// Указатель на физический движок
	NodeEditor								nEditor;	// Редактор узлов

	/************************/
private:
	/* Указатели на объекты */

	MainPerson*					   hero;		// Указатель на главного персонажа
	PersonContainer*			   persons;		// Указатель на	контейнер персонажей
	InteractableObject2DContainer* Iobjects;	// Указатель на	контейнер интерактивных объектов
	ObjectsQueue*				   objQueue;	// Указатель на	контейнер, определяющий очередь (слои) отрисовки
	TriggerContainer*			   triggers;	// Указатель на контейнер триггеров

	/************************/
private:
	/* Общие переменные для работы с интерфейсом */

	bool			  DrawingHitBox		= false;	// Статус отрисовки хитбокса
	bool			  DrawingLine		= false;	// Статус отрисовки линии
	bool			  SavingSettings    = false;	// Статус сохранения настроек
	bool			  CreatingScript    = false;	// Статус создания скрипта
	bool			  SettedFirstPoint  = false;	// Статус установки первой точки изменяемого объекта
	bool			  SettedSecondPoint = false;	// Статус установки второй точки изменяемого объекта
	DirectX::XMFLOAT2 firstPoint  = {0.0f, 0.0f};	// Координаты первой точки изменяемого объекта
	DirectX::XMFLOAT2 secondPoint = {0.0f, 0.0f};	// Координаты второй точки изменяемого объекта

	/*********************************************/
private:
	/* Вспомагательные переменные для работы с главным персонажем */

	std::string heroSelected = "mainperson";	// Имя выбранного главного персонажа

	/**************************************************************/
private:
	/* Вспомагательные переменные для работы с контейнером персонажей */
	
	bool		IsAddingPerson = false;	// Статус добавления персонажа
	bool		GotPersonData  = false;	// Статус получения данных о новом персонаже
	bool		ChoosingPerson = false;	// Статус выбора спрайта персонажа
	std::string personSelected = "";	// Имя выбранного персонажа
	std::string PersonPath	   = "";	// Путь к выбранному спрайту персонажа
	std::optional<PersonData> optPdata;	// Вспомогательная переменная с даннами о новом персонаже

	/******************************************************************/
private:
	/* Вспомагательные переменные для работы с физическим движком */

	bool		AddingObject   = false;	// Статус добавления объекта в физический движок
	float		sq_l		   = 0.0f;	// Сторона квадрата для нормирования выбранного hitbox в физическом движке
	std::string objectSelected = "";	// Имя выбранного объекта из физического движка

	/**************************************************************/
private:
	/* Вспомагательные переменные для работы с контейнером интерактивных объектов */

	bool			  AddingIobj		 = false;			// Статус добавления интерактивного объекта
	bool			  ChosingIobj		 = false;			// Статус выбора интерактивного объекта
	bool			  IsCaclulatedDeltas = false;			// Статус перерасчёта корректирующих отступов для hitbox интерактивного объекта
	std::string		  IobjSelected		 = "";				// Имя выбранного интерактивного объекта
	std::string		  IobjPath			 = "";				// Путь при добавлении интерактивного объекта
	DirectX::XMFLOAT2 hb_delta			 = { 0.0f, 0.0f };	// Корректирующий отступ для hitbox интерактивного объекта при перемещении с помощью интерфейса

	/******************************************************************************/
private:
	/* Вспомогательные переменные для работы с контейнером триггеров */

	bool		AddingTrigger	  = false; // Статус добавления триггера
	std::string trigScenesPreview = "";	   // Выбранная цель триггера (сцена)
	std::string triggerSelected   = "";	   // Выбранный триггер
	std::string trigType	      = "";	   // Выбранный тип триггера
	size_t		trigTypeId	      = 0;	   // Выбранный тип триггера (id)

	/*****************************************************************/
private:
	/* Вспомагательные переменные для работы с объектом (база - Object2D) */
	
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ImageResource;	// Объявления нового типа для простоты применения

	int           sprite_width  = 0;		// Ширина спрайта
	int           sprite_height = 0;		// Высота спрайта
	bool          LoadingSprite = false;	// Статус загрузки спрайта
	bool          LoadedPreview = false;	// Статус загрузки превью спрайта
	float         scaleObj      = 1.0f;		// Множитель размера превью спрайта
	ImageResource loadedSprite  = NULL;		// Спрайт для внутренних операций в интерфейсе

	/***********************************************************/
private:
	/* Вспомагательные переменные для работы с анимацией */
	
	int			curFrame			   = 0;				 // Текущий кадр анимации во всех превью
	int			maxFrames			   = 4;				 // Максимальное число кадров в анимации при создании анимации
	bool        CreatingAnimation	   = false;			 // Статус создания анимации
	bool        ChoosingAnimation	   = false;			 // Статус редактирования анимации
	bool        CreatingAnimtionLoaded = false;			 // Статус загрузки нового спрайта анимации
	float       scaleFrame			   = 1.0f;			 // Множитель размера превью спрайта
	float		newFrameWidth		   = 90.0f;			 // Ширина кадра новой анимации
	float		newFrameHeight		   = 90.0f;			 // Высота кадра новой анимации
	float		curAnimW			   = 0.0f;			 // Ширина кадра превью
	float		curAnimH			   = 0.0f;			 // Высота кадра превью
	float		framesHoldTime		   = 0.2f;			 // Задержка проигрывания анимаций 
	size_t		animSelectedId		   = 0;				 // Номер выбранного типа анимации
	size_t		animPlayingId		   = 0;				 // Номер выбранного типа анимации при создании анимации
	ImVec2		previewSize			   = { 0.0f, 0.0f }; // Размер превью анимации
	ImVec2		ltNormPixel			   = { 0.0f, 0.0f }; // Нормированый пиксель (левый верхний) для масштабирования кадра
	ImVec2		rtNormPixel			   = { 0.0f, 0.0f }; // Нормированый пиксель (правый нижний) для масштабирования кадра
	Surface2D   animSpritePreview;						 // Временный реусрс для созданных анимаций
	std::string animPath			   = "";			 // Путь к спрайту при создании анимации
	std::string animSelected		   = "";			 // Имя выбранной анимации
	std::string newAnimNameSelected	   = "";			 // Имя выбранной анимации при создании

	std::vector<std::string>   animationNames;		// Списко доступных имён анимаций
	std::vector<Animation>     animationsPreview;	// Анимации в превью при создании
	std::vector<AnimationData> animationsData;		// Созданые анимации

	/*****************************************************/
private:
	/* Вспомагательные переменные для отображения FPS */

	double sum     = 0.0;		// Сумма всех элементов массива
	double average = 0.0;		// Среднее значение FPS
	double pXMin   = 0.0;		// Минимальное значение для масштабирования графика по оси X
	double pXMax   = MAX_POINTS	// Максимальное значение для масштабирования графика по оси X
	double pYMin   = 0.0;		// Минимальное значение для масштабирования графика по оси Y
	double pYMax   = 0.0;		// Максимальное значение для масштабирования графика по оси Y
	float  arr[N_POINTS];		// Массив значений fps
	float  counters[N_POINTS];	// Массив отчётов
	size_t counter = 0;			// Счётчик итерации

	/**************************************************/
private:
	/* Вспомагательные переменные для ряботы с Viewport */

	int viewportWidth;	// Ширина viewport
	int viewportHeight;	// Высота viewport

	/****************************************************/
private:
	/* Вспомагательные переменные для интерфейса */
	
	std::string mouseHelpInfo = "";	  // Подпись под курсором
	bool		IsShow		  = true; // Состояние видимости интерфейса
	int			draggingObjId = -1;	  // Индекс выбранного интеративного объекта для перемещения мышкой

	/*********************************************/
};