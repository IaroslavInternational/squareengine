#pragma once

#include "imgui/imgui.h"
#include "AdapterData.h"
#include "AppLog.h"
#include "Scene.h"
#include "AnimationData.h"
#include "Window.h"
#include "NodeEditor.h"

#include <map>

#define N_POINTS   100						// ������������ ���-�� ����� �� �������
#define MAX_POINTS (double)N_POINTS - 1.0;	// ������������ ���-�� ����� �� ������� ��� ����������� �����������	

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
	GUISystem(Scene* scene);	// ����������� (��������� ��������� �� �����)
public:
	/* ������� ������ ��� ��������� ���������� */

	void						 Show(float dt);						// �������� ��������� (��������� ����� ������ �����)
	void						 Hide();								// ������ ���������
	void						 BeginFrame();							// ������ �����
	void						 EndFrame();							// ����� �����
	void						 AddLog(const std::ostringstream& oss);	// �������� ���
	void						 AddLog(std::string str);				// �������� ���
	void						 AddLog(const char* text);				// �������� ���
	void						 LoadScene(Scene* scene);				// ��������� �����
	std::pair<bool, std::string> UpdatingScene();						// ��� ���������� �����
	bool						 AddingScene();							// ��� ���������� �����
	void						 SetAddingSceneState(bool state);		// ���������� ������ ���������� �����

	/*******************************************/
private: 
	/* ������ ��������� � ��������� ������� */
	
	void SetGUIColors();														// ��������� �������� �����
	void SetPanelSizeAndPosition(int   corner,			float width,			// ��������� ��������� � �������� ������
								 float height,			float x_offset = 0.0f, 
								 float y_offset = 0.0f); 	
	void ShowMenu(); 															// ������� ������ ����
	void ShowLeftSide(float dt);														// ����� ������	
	void ShowRightSide(float dt);												// ������ ������
	void ShowLeftBottomSide();													// ����� ������ ������
	void ShowBottomPanel();														// ������ ������
	void ShowOptionalPanel();													// �������������� ������
	void ShowMouseHelperPanel(std::string info);								// ������ ��� ��������
	void DisableSides();														// ��������� ������� ������

	/****************************************/
private: 
	/* ������ ��������� ���������� ����������� */

	void   ShowMainPersonList();			// �������� ����� ������ �������� ��������� � ������
	void   ShowPersonList(float dt);		// �������� ����� ������ ������ ���������� 
	void   ShowIobjList();					// ��������	����� ������ ������ ������������� ��������
	void   ShowTriggerList();				// �������� ����� ������ ������ ���������
	void   ShowPhysicsEngineObjList();		// �������� ����� ������ ������ �������� � ���������� ������		   

	void   ShowMainPersonControl(float dt); // �������� ������ ������ �������� ��� �������� ���������
	void   ShowPersonControl(float dt);		// �������� ������ ������ �������� ��� ���������� ���������
	void   ShowIobjControl();				// �������� ������ ������ �������� ��� ���������� �������������� �������
	void   ShowTriggerControl();			// �������� ������ ������ �������� ��� ���������� ��������
	void   ShowPhysicsEngineObjControl();	// �������� ������ ������ �������� ��� ���������� ������� � ���������� ������		   

	void   ShowLayersControl();				// �������� ������ �������� ����
	void   ShowCameraControl();				// �������� ������ �������� ������
	void   ShowScenesControl();		   		// �������� ������ �������� ����
	void   SpawnCameraToHeroControl();		// �������� ������ �������� ��� �������������� ������ � �����
	void   SpawnDefaultObject2DControl(
		   Object2D*   obj, 
		   std::string dataPath);			// �������� ������ �������� ��� ������� �� ���� Object2D
	void   ShowScriptsControl();			// �������� ������ �������� ��� ��������

	void   ShowLog();						// �������� ������ ����
	void   ShowGPU();						// �������� ������ � ����������� �� FPS � ����������� �������� 
	void   ShowFPS();						// �������� ������ � �������� ��������� FPS		   
	void   ShowPhysicsEngineObjHelp();		// �������� ������� ��� �������� ����������� ������
	void   ShowPhysicsEngineSettings();		// �������� ������ �������� ����������� ������
	void   ShowGraphicsEngineSettings();	// �������� ������ �������� �������
	void   ShowProjectControl();			// �������� ������ �������� �������
	void   ShowViewportControl();			// �������� ������ �������� Viewport
	HitBox CreateNewHitBox();				// ������� HitBox � ������� ����������
	
	std::string					ShowLoadingSpriteDilaog();				// �������� ���������� ���� ��� �������� �������
	std::optional<IobjData>		ShowAddingIobjDialog();					// �������� ���������� ���� ��� ���������� �������������� �������
	std::optional<PersonData>	ShowAddingPersonDialog(float dt);		// �������� ���������� ���� ��� ���������� ���������
	std::vector<AnimationData>	ShowAnimationCreatingDialog(float dt);	// �������� ���������� ���� ��� �������� ��������

	/*******************************************/
private:
	/* ������ ���������� ������ */

	void SaveAll();					// ��������� ��� ���������
	void SaveMainPersonData();		// ��������� ��������� ��� �������� ���������
	void SavePersonsData();			// ��������� ��������� ��� ����������
	void SaveIobjData();			// ��������� ��������� ��� ������������� ��������
	void SaveTriggersData();		// ��������� ��������� ��� ���������
	void SaveLayersData();			// ��������� ��������� ��� ����
	void SaveCameraData();			// ��������� ��������� ��� ������
	void SaveScenesData();			// ��������� ��������� ��� ����
	void SavePhysicsEngineData();	// ��������� ��������� ��� ����������� ������
	void SaveGraphicsEngineData();	// ��������� ��������� ��� ������������ ������

	/****************************/
private:
	/* ���������� ��������� ������� */
	
	bool ShowMainPersonEnum			  = true;	// ��������� ����� ������ ������ �������� ���������
	bool ShowPersonEnum				  = false;	// ��������� ����� ������ ������ ����������
	bool ShowIobjEnum				  = false;	// ��������� ����� ������ ������ ������������� ��������
	bool ShowPhysicsEngineObjEnum	  = false;	// ��������� ����� ������ ������ ���������� ��������
	bool ShowTriggersEnum			  = false;	// ��������� ����� ������ ������ ��������� �� �����

	bool ShowMainPersonSettings		  = true;	// ��������� ������ ������ �������� �������� ���������
	bool ShowPersonSettings			  = false;	// ��������� ������ ������ �������� ���������� ���������	
	bool ShowIobjSettings			  = false;	// ��������� ������ ������ �������� �������������� �������
	bool ShowPhysicsEngineObjSettings = false;	// ��������� ������ ������ �������� ���������� ������� � ���������� ������
	bool ShowTriggersSettings		  = false;	// ��������� ������ ������ �������� ���������� ��������

	bool ShowLayersSettings			  = false;	// ��������� ������ �������� ����
	bool ShowCameraSettings			  = false;	// ��������� ������ �������� ������
	bool ShowScenesSettings			  = false;	// ��������� ������ �������� ����

	bool ShowPhysicsEngineObjInfo	  = true;	// ��������� �������� ��� �������� ����������� ������  
	bool ShowPhysicsSettings		  = false;	// ��������� ������ �������� ����������� ������
	bool ShowGraphicsSettings		  = false;	// ��������� ������ �������� �������
	bool ShowProjectSettings		  = false;	// ��������� ������ �������� �������
	bool ShowViewportSettings		  = false;	// ��������� ������ �������� Viewport
	bool ShowLogs					  = true;	// ��������� ����
	bool ShowHardwareInfo 			  =	true;	// ��������� FPS � ���������� � ����������� ��������
	bool ShowFPSChart 				  =	false;	// ��������� ������� ��������� FPS
	bool ShowScriptsSettings		  = false;	// ��������� ������ �������� ��������

	/********************************/
private:
	/* ���������� ��� ������ � ����������� ���� */
	
	bool		SavingScenesSettings = false;	// ������ ���������� �������� ����
	bool		IsAddingScene		 = false;	// ������ ���������� �����
	bool		IsUpdatingScene		 = false;	// ������ ��������� ����� �� ������
	std::string curSceneName;					// ��� ������� �����

	/********************************************/
private:
	/* ��������� ���������� */

	AppLog									applog;		// ���
	std::shared_ptr<Window>					wnd;		// ��������� �� ���� ���������
	std::shared_ptr<Camera>					camera;		// ��������� �� ������
	std::map<std::wstring, double>			gpu_desc;	// ��������� �������� ����������� ��������� [first - ���, second - ����� ������]
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;	// ��������� �� ���������� ������
	NodeEditor								nEditor;	// �������� �����

	/************************/
private:
	/* ��������� �� ������� */

	MainPerson*					   hero;		// ��������� �� �������� ���������
	PersonContainer*			   persons;		// ��������� ��	��������� ����������
	InteractableObject2DContainer* Iobjects;	// ��������� ��	��������� ������������� ��������
	ObjectsQueue*				   objQueue;	// ��������� ��	���������, ������������ ������� (����) ���������
	TriggerContainer*			   triggers;	// ��������� �� ��������� ���������

	/************************/
private:
	/* ����� ���������� ��� ������ � ����������� */

	bool			  DrawingHitBox		= false;	// ������ ��������� ��������
	bool			  DrawingLine		= false;	// ������ ��������� �����
	bool			  SavingSettings    = false;	// ������ ���������� ��������
	bool			  CreatingScript    = false;	// ������ �������� �������
	bool			  SettedFirstPoint  = false;	// ������ ��������� ������ ����� ����������� �������
	bool			  SettedSecondPoint = false;	// ������ ��������� ������ ����� ����������� �������
	DirectX::XMFLOAT2 firstPoint  = {0.0f, 0.0f};	// ���������� ������ ����� ����������� �������
	DirectX::XMFLOAT2 secondPoint = {0.0f, 0.0f};	// ���������� ������ ����� ����������� �������

	/*********************************************/
private:
	/* ��������������� ���������� ��� ������ � ������� ���������� */

	std::string heroSelected = "mainperson";	// ��� ���������� �������� ���������

	/**************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ����������� ���������� */
	
	bool		IsAddingPerson = false;	// ������ ���������� ���������
	bool		GotPersonData  = false;	// ������ ��������� ������ � ����� ���������
	bool		ChoosingPerson = false;	// ������ ������ ������� ���������
	std::string personSelected = "";	// ��� ���������� ���������
	std::string PersonPath	   = "";	// ���� � ���������� ������� ���������
	std::optional<PersonData> optPdata;	// ��������������� ���������� � ������� � ����� ���������

	/******************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ���������� ������� */

	bool		AddingObject   = false;	// ������ ���������� ������� � ���������� ������
	float		sq_l		   = 0.0f;	// ������� �������� ��� ������������ ���������� hitbox � ���������� ������
	std::string objectSelected = "";	// ��� ���������� ������� �� ����������� ������

	/**************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ����������� ������������� �������� */

	bool			  AddingIobj		 = false;			// ������ ���������� �������������� �������
	bool			  ChosingIobj		 = false;			// ������ ������ �������������� �������
	bool			  IsCaclulatedDeltas = false;			// ������ ����������� �������������� �������� ��� hitbox �������������� �������
	std::string		  IobjSelected		 = "";				// ��� ���������� �������������� �������
	std::string		  IobjPath			 = "";				// ���� ��� ���������� �������������� �������
	DirectX::XMFLOAT2 hb_delta			 = { 0.0f, 0.0f };	// �������������� ������ ��� hitbox �������������� ������� ��� ����������� � ������� ����������

	/******************************************************************************/
private:
	/* ��������������� ���������� ��� ������ � ����������� ��������� */

	bool		AddingTrigger	  = false; // ������ ���������� ��������
	std::string trigScenesPreview = "";	   // ��������� ���� �������� (�����)
	std::string triggerSelected   = "";	   // ��������� �������
	std::string trigType	      = "";	   // ��������� ��� ��������
	size_t		trigTypeId	      = 0;	   // ��������� ��� �������� (id)

	/*****************************************************************/
private:
	/* ��������������� ���������� ��� ������ � �������� (���� - Object2D) */
	
	typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ImageResource;	// ���������� ������ ���� ��� �������� ����������

	int           sprite_width  = 0;		// ������ �������
	int           sprite_height = 0;		// ������ �������
	bool          LoadingSprite = false;	// ������ �������� �������
	bool          LoadedPreview = false;	// ������ �������� ������ �������
	float         scaleObj      = 1.0f;		// ��������� ������� ������ �������
	ImageResource loadedSprite  = NULL;		// ������ ��� ���������� �������� � ����������

	/***********************************************************/
private:
	/* ��������������� ���������� ��� ������ � ��������� */
	
	int			curFrame			   = 0;				 // ������� ���� �������� �� ���� ������
	int			maxFrames			   = 4;				 // ������������ ����� ������ � �������� ��� �������� ��������
	bool        CreatingAnimation	   = false;			 // ������ �������� ��������
	bool        ChoosingAnimation	   = false;			 // ������ �������������� ��������
	bool        CreatingAnimtionLoaded = false;			 // ������ �������� ������ ������� ��������
	float       scaleFrame			   = 1.0f;			 // ��������� ������� ������ �������
	float		newFrameWidth		   = 90.0f;			 // ������ ����� ����� ��������
	float		newFrameHeight		   = 90.0f;			 // ������ ����� ����� ��������
	float		curAnimW			   = 0.0f;			 // ������ ����� ������
	float		curAnimH			   = 0.0f;			 // ������ ����� ������
	float		framesHoldTime		   = 0.2f;			 // �������� ������������ �������� 
	size_t		animSelectedId		   = 0;				 // ����� ���������� ���� ��������
	size_t		animPlayingId		   = 0;				 // ����� ���������� ���� �������� ��� �������� ��������
	ImVec2		previewSize			   = { 0.0f, 0.0f }; // ������ ������ ��������
	ImVec2		ltNormPixel			   = { 0.0f, 0.0f }; // ������������ ������� (����� �������) ��� ��������������� �����
	ImVec2		rtNormPixel			   = { 0.0f, 0.0f }; // ������������ ������� (������ ������) ��� ��������������� �����
	Surface2D   animSpritePreview;						 // ��������� ������ ��� ��������� ��������
	std::string animPath			   = "";			 // ���� � ������� ��� �������� ��������
	std::string animSelected		   = "";			 // ��� ��������� ��������
	std::string newAnimNameSelected	   = "";			 // ��� ��������� �������� ��� ��������

	std::vector<std::string>   animationNames;		// ������ ��������� ��� ��������
	std::vector<Animation>     animationsPreview;	// �������� � ������ ��� ��������
	std::vector<AnimationData> animationsData;		// �������� ��������

	/*****************************************************/
private:
	/* ��������������� ���������� ��� ����������� FPS */

	double sum     = 0.0;		// ����� ���� ��������� �������
	double average = 0.0;		// ������� �������� FPS
	double pXMin   = 0.0;		// ����������� �������� ��� ��������������� ������� �� ��� X
	double pXMax   = MAX_POINTS	// ������������ �������� ��� ��������������� ������� �� ��� X
	double pYMin   = 0.0;		// ����������� �������� ��� ��������������� ������� �� ��� Y
	double pYMax   = 0.0;		// ������������ �������� ��� ��������������� ������� �� ��� Y
	float  arr[N_POINTS];		// ������ �������� fps
	float  counters[N_POINTS];	// ������ �������
	size_t counter = 0;			// ������� ��������

	/**************************************************/
private:
	/* ��������������� ���������� ��� ������ � Viewport */

	int viewportWidth;	// ������ viewport
	int viewportHeight;	// ������ viewport

	/****************************************************/
private:
	/* ��������������� ���������� ��� ���������� */
	
	std::string mouseHelpInfo = "";	  // ������� ��� ��������
	bool		IsShow		  = true; // ��������� ��������� ����������
	int			draggingObjId = -1;	  // ������ ���������� ������������� ������� ��� ����������� ������

	/*********************************************/
};