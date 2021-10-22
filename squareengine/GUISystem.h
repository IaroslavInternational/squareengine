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

#define N_POINTS 100	// ������������ ���-�� ����� �� �������

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
	/* ������� ������ ��� ��������� ���������� */
	
	void Show(float dt);			// �������� ���������
	void Hide();					// ������ ���������
	void AddLog(const char* text);	// �������� ���
	void LoadScene(Scene* scene);

	/*******************************************/
private: 
	/* ������ ��������� � ��������� ������� */
	
	void SetGUIColors();														// ��������� �������� �����
	void SetPanelSizeAndPosition(int   corner,			float width,			// ��������� ��������� � �������� ������
								 float height,			float x_offset = 0.0f, 
								 float y_offset = 0.0f); 	
	void ShowMenu(); 															// ������� ������ ����
	void ShowLeftSide();														// ����� ������	
	void ShowRightSide(float dt);												// ������ ������
	void ShowLeftBottomSide();													// ����� ������ ������
	void ShowBottomPanel();														// ������ ������
	void ShowOptionalPanel();													// �������������� ������
	void ShowMouseHelperPanel(std::string info);								// ������ ��� ��������
	void DisableSides();														// ��������� ������� ������

	/****************************************/
private: 
	/* ������ ��������� ���������� ����������� */
	
	void					ShowLog();						// �������� ������ ����
	void					ShowGPU();						// �������� ������ � ����������� �� FPS � ����������� �������� 
	void					ShowFPS();						// �������� ������ � �������� ��������� FPS
	void					ShowPhysicsEngineObjHelp();		// �������� ������� ��� �������� ����������� ������
	void					ShowPhysicsEngineSettings();	// �������� ������ �������� ����������� ������
	void					ShowPersonList();				// �������� ������ ���������� 
	void					ShowPersonControl();			// �������� ��������� ��� ���������� ���������
	void					ShowMainPersonList();			// �������� �������� ��������� � ������
	void					ShowMainPersonControl(float dt);// �������� ��������� ��� �������� ���������
	void					ShowPhysicsEngineObjList();		// �������� ������ �������� � ���������� ������
	void					ShowPhysicsEngineObjControl();	// �������� ��������� ��� ���������� ������� � ���������� ������
	void					SpawnCameraToHeroControl();		// �������� ��������� ��� ������
	void					ShowLayersControl();			// �������� ������ �������� ����
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
	/* ���������� ��������� ������� */
	
	bool ShowPersonEnum 			  =	false;	// ����� ������ ���������� �� �����
	bool ShowPersonSettings			  =	false;	// ������ ������ ���������� ����������� ����������	
	bool ShowMainPersonEnum 		  =	true;	// ����� ������ �������� ��������� �� �����
	bool ShowMainPersonSettings		  =	true;	// ������ ������ ���������� ����������� �������� ���������
	bool ShowPhysicsEngineObjEnum	  = false;	// ����� ������ ���������� �������� �� �����
	bool ShowPhysicsEngineObjSettings = false;	// ������ ������ ���������� ����������� ���������� �������
	bool ShowPhysicsEngineObjInfo	  = true;	// ������� ����� ������� ����������� ������  
	bool ShowPhysicsSettings		  = false;	// ������ �������� ����������� ������
	bool ShowHardwareInfo 			  =	true;	// FPS � ���������� � ����������� ��������
	bool ShowFPSGraph 				  =	false;	// ������ ��������� FPS
	bool ShowLogs 					  =	true;	// ���
	bool ShowTriggersList 			  =	false;	// ����� ������ ��������� �� �����
	bool ShowTriggersSettings		  =	false;	// ������ ������ ��������� �� �����
	bool ShowLayersSettings			  = false;	// �������� ������ �������� ����
	bool ShowIobjEnum				  = false;	//
	bool ShowIobjSettings			  = false;	//
	bool ShowCameraSettings			  = true;

	/********************************/
private:
	/* ��������� ���������� */
	
	std::shared_ptr<Window>					wnd;		// ��������� �� ���� ���������
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;	// ��������� �� ���������� ������
	std::shared_ptr<Camera>					camera;
	std::map<std::wstring, double>			gpu_desc;	// �������� ����������� ��������� [first - ���, second - ����� ������]
	AppLog									applog;		// ���

	/************************/
private:
	/* ��������� �� ������� */

	MainPerson*					   hero;
	PersonContainer*			   persCon;
	InteractableObject2DContainer* IobjCon;
	ObjectsQueue*				   objQueue;

	/************************/
private:
	/* ����� ���������� ��� ������ � ����������� */

	bool			  DrawingHitBox		= false;	// ��� ��������� ��������
	bool			  DrawingLine		= false;	// ��� ��������� �����
	bool			  SavingSettings    = false;	// ��� ���������� ��������
	bool			  SettedFirstPoint  = false;	// ���������� ������ ����� ����������� �������
	bool			  SettedSecondPoint = false;	// ���������� ������ ����� ����������� �������
	DirectX::XMFLOAT2 firstPoint;					// ���������� ������ ����� ����������� �������
	DirectX::XMFLOAT2 secondPoint;					// ���������� ������ ����� ����������� �������

	/*********************************************/
private:
	/* ��������������� ���������� ��� ������ � ������� ���������� */

	std::string heroSelected = "";	// ��� ���������� �������� ���������

	/**************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ����������� ���������� */
	
	std::string personSelected = "";	// ��� ���������� ���������

	/******************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ���������� ������� */

	std::string objectSelected = "";	// ��� ���������� ������� �� ����������� ������
	bool		AddingObject   = false;	// ��� ���������� �������
	float		sq_l		   = 0.0f;	// ������� �������� ��� hitbox

	/**************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ����������� �������� */

	std::string		  IobjSelected		 = "";		// ��� ���������� �������
	std::string		  IobjPath			 = "";		//
	bool			  AddingIobj		 = false;	//
	bool			  ChosingIobj		 = false;	//
	bool			  isCaclulatedDeltas = false;
	DirectX::XMFLOAT2 hb_delta			 = { 0.0f, 0.0f };

	/****************************************************************/
private:
	/* ��������������� ���������� ��� ������ �� ������ */

	bool SavingLayersSettings = false;

	/***************************************************/
private:
	/* ��������������� ���������� ��� ������ � �������� (����) */
	
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ImageResource;

	float         scaleObj        = 1.0f;	// ��������� ������� ������ �������
	bool          LoadingSprite   = false;
	bool          LoadedPreview   = false;
	int           sprite_width  = 0;
	int           sprite_height = 0;
	ImageResource loadedSprite = NULL;

	/***********************************************************/
private:
	/* ��������������� ���������� ��� ������ � ��������� */
	
	float       scaleFrame				= 1.0f;	// ��������� ������� ������ �������
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
	/* ��������������� ���������� ��� ����������� FPS */

	double sum     = 0.0;						// ����� ���� ��������� �������
	double average = 0.0;						// ������� �������� FPS
	double pXMin   = 0.0;						// ����������� �������� ��� ��������������� ������� �� ��� X
	double pXMax   = (double)N_POINTS - 1.0;	// ������������ �������� ��� ��������������� ������� �� ��� X
	double pYMin   = 0.0;						// ����������� �������� ��� ��������������� ������� �� ��� Y
	double pYMax   = 0.0;						// ������������ �������� ��� ��������������� ������� �� ��� Y
	float  arr[N_POINTS];						// ������ �������� fps
	float  counters[N_POINTS];					// ������ �������
	size_t counter = 0;							// ������� ��������

	/**************************************************/

	std::string mouseHelpInfo = "";	// ������� ��� ��������
};
