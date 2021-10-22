#pragma once

#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"
#include "Scene.h"
#include "PhysicsEngine.h"
#include "AnimationData.h"
#include "Window.h"

#include <map>

#define N_POINTS 100	// Максимальное кол-во точек на графике

struct IobjData
{
	std::string		  name;
	DirectX::XMFLOAT2 position;
	size_t			  layer;
	std::string		  pathToSprite;
};

class GUISystem
{
public:
	GUISystem(Scene* scene);
public:
	/* Главные методы для отрисовки интерфейса */
	
	void Show(float dt);			// Показать интерфейс
	void Hide();					// Скрыть интерфейс
	void AddLog(const char* text);	// Добавить лог
	void LoadScene(Scene* scene);

	/*******************************************/
private: 
	/* Методы настройки и отрисовки панелей */
	
	void SetGUIColors();														// Установка цветовой гаммы
	void SetPanelSizeAndPosition(int   corner,			float width,			// Установка положения и размеров панели
								 float height,			float x_offset = 0.0f, 
								 float y_offset = 0.0f); 	
	void ShowMenu(); 															// Верхняя панель меню
	void ShowLeftSide();														// Левая панель	
	void ShowRightSide(float dt);												// Правая панель
	void ShowLeftBottomSide();													// Левая нижняя панель
	void ShowBottomPanel();														// Нижняя панель
	void ShowOptionalPanel();													// Дополнительная панель
	void ShowMouseHelperPanel(std::string info);								// Панель под курсором
	void DisableSides();														// Отключить боковые панели

	/****************************************/
private: 
	/* Методы отрисовки конкретных интерфейсов */
	
	void					ShowLog();						// Показать панель лога
	void					ShowGPU();						// Показать панель с ифнормацией об FPS и графическом адаптере 
	void					ShowFPS();						// Показать панель с графиком изменения FPS
	void					ShowPhysicsEngineObjHelp();		// Показать подписи имён объектов физического движка
	void					ShowPhysicsEngineSettings();	// Показать панель настроек физического движка
	void					ShowPersonList();				// Показать список персонажей 
	void					ShowPersonControl();			// Показать настройки для выбранного персонажа
	void					ShowMainPersonList();			// Показать главного персонажа в списке
	void					ShowMainPersonControl(float dt);// Показать настройки для главного персонажа
	void					ShowPhysicsEngineObjList();		// Показать список объектов в физическом движке
	void					ShowPhysicsEngineObjControl();	// Показать настройки для выбранного объекта в физическом движке
	void					SpawnCameraToHeroControl();		// Показать настройки для камеры
	void					ShowLayersControl();			// Показать панель настроек слоёв
	void					ShowIobjList();
	void					ShowIobjControl();
	void					SpawnDefaultObject2DControl(Object2D* obj, std::string dataPath);
	std::string				ShowLoadingSpriteDilaog();
	std::optional<IobjData> ShowAddingIobjDialog();
	HitBox					CreateNewHitBox();
	void					ShowCameraControl();
	std::vector<AnimationData>	ShowAnimationCreatingDialog(float dt);
	
	/*******************************************/
private:
	/* Переменные видимости панелей */
	
	bool ShowPersonEnum 			  =	false;	// Левая панель персонажей на сцене
	bool ShowPersonSettings			  =	false;	// Правая панель управления настройками персонажей	
	bool ShowMainPersonEnum 		  =	true;	// Левая панель главного персонажа на сцене
	bool ShowMainPersonSettings		  =	true;	// Правая панель управления настройками главного персонажа
	bool ShowPhysicsEngineObjEnum	  = false;	// Левая панель физических объектов на сцене
	bool ShowPhysicsEngineObjSettings = false;	// Правая панель управления настройками выбранного объекта
	bool ShowPhysicsEngineObjInfo	  = true;	// Подпись имени объекта физического движка  
	bool ShowPhysicsSettings		  = false;	// Панель настроек физического движка
	bool ShowHardwareInfo 			  =	true;	// FPS и информация о графическом адаптере
	bool ShowFPSGraph 				  =	false;	// График изменения FPS
	bool ShowLogs 					  =	true;	// Лог
	bool ShowTriggersList 			  =	false;	// Левая панель триггеров на сцене
	bool ShowTriggersSettings		  =	false;	// Правая панель триггеров на сцене
	bool ShowLayersSettings			  = false;	// Показать панель настроек слоёв
	bool ShowIobjEnum				  = false;	//
	bool ShowIobjSettings			  = false;	//
	bool ShowCameraSettings			  = true;

	/********************************/
private:
	/* Системные переменные */
	
	std::shared_ptr<Window>					wnd;		// Указатель на окно отрисовки
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;	// Указатель на физический движок
	std::shared_ptr<Camera>					camera;
	std::map<std::wstring, double>			gpu_desc;	// Описание графических адаптеров [first - имя, second - объём памяти]
	AppLog									applog;		// Лог

	/************************/
private:
	/* Указатели на объекты */

	MainPerson*					   hero;
	PersonContainer*			   persCon;
	InteractableObject2DContainer* IobjCon;
	ObjectsQueue*				   objQueue;

	/************************/
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
private:
	/* Вспомагательные переменные для работы с главным персонажем */

	std::string heroSelected = "";	// Имя выбранного главного персонажа

	/**************************************************************/
private:
	/* Вспомагательные переменные для работы с контейнером персонажей */
	
	std::string personSelected = "";	// Имя выбранного персонажа

	/******************************************************************/
private:
	/* Вспомагательные переменные для работы с физическим движком */

	std::string objectSelected = "";	// Имя выбранного объекта из физического движка
	bool		AddingObject   = false;	// Идёт добавление объекта
	float		sq_l		   = 0.0f;	// Сторона квадрата для hitbox

	/**************************************************************/
private:
	/* Вспомагательные переменные для работы с контейнером объектов */

	std::string		  IobjSelected		 = "";		// Имя выбранного объекта
	std::string		  IobjPath			 = "";		//
	bool			  AddingIobj		 = false;	//
	bool			  ChosingIobj		 = false;	//
	bool			  isCaclulatedDeltas = false;
	DirectX::XMFLOAT2 hb_delta			 = { 0.0f, 0.0f };

	/****************************************************************/
private:
	/* Вспомагательные переменные для работы со слоями */

	bool SavingLayersSettings = false;

	/***************************************************/
private:
	/* Вспомагательные переменные для работы с объектом (база) */
	
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ImageResource;

	float         scaleObj        = 1.0f;	// Множитель размера превью спрайта
	bool          LoadingSprite   = false;
	bool          LoadedPreview   = false;
	int           sprite_width  = 0;
	int           sprite_height = 0;
	ImageResource loadedSprite = NULL;

	/***********************************************************/
private:
	/* Вспомагательные переменные для работы с анимацией */
	
	float       scaleFrame				= 1.0f;	// Множитель размера превью спрайта
	std::string animSelected			= "";
	size_t		animSelectedId			= 0;
	int			curFrame				= 0;
	int			animFrameSize[2];
	float		curAnimW				= 0.0f;
	float		curAnimH				= 0.0f;
	ImVec2		previewSize				= {0.0f, 0.0f};
	ImVec2		ltNormPixel				= {0.0f, 0.0f};
	ImVec2		rtNormPixel				= {0.0f, 0.0f};
	std::string animPath				= "";
	bool        CreatingAnimation	    = false;
	bool        ChoosingAnimation	    = false;
	bool        CreatingAnimtionLoaded  = false;
	float		newFrameWidth		    = 90.0f;
	float		newFrameHeight		    = 90.0f;
	int			maxFrames				= 4;
	float		framesHoldTime			= 0.2f;
	size_t		animPlayingId			= 0;
	std::string newAnimNameSelected		= "";
	Surface2D   animSpritePreview;
	std::vector<std::string> animationNames;
	std::vector<Animation>   animationsPreview;
	std::vector<AnimationData>  animationsData;

	/*****************************************************/
private:
	/* Вспомагательные переменные для отображения FPS */

	double sum     = 0.0;						// Сумма всех элементов массива
	double average = 0.0;						// Среднее значение FPS
	double pXMin   = 0.0;						// Минимальное значение для масштабирования графика по оси X
	double pXMax   = (double)N_POINTS - 1.0;	// Максимальное значение для масштабирования графика по оси X
	double pYMin   = 0.0;						// Минимальное значение для масштабирования графика по оси Y
	double pYMax   = 0.0;						// Максимальное значение для масштабирования графика по оси Y
	float  arr[N_POINTS];						// Массив значений fps
	float  counters[N_POINTS];					// Массив отчётов
	size_t counter = 0;							// Счётчик итерации

	/**************************************************/

	std::string mouseHelpInfo = "";	// Подпись под курсором
};
