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

#define N_POINTS 100	// ������������ ���-�� ����� �� �������

class GUISystem
{
public:
	GUISystem(std::shared_ptr<Window>				  wnd,
			  PersonContainer*						  persons, 
			  MainPerson*							  mPersPtr,
			  std::shared_ptr<Physics::PhysicsEngine> phEngPtr);
public:
	/* ������� ������ ��� ��������� ���������� */
	
	void Show();					// �������� ���������
	void Hide();					// ������ ���������
	void AddLog(const char* text);	// �������� ���

	/*******************************************/
private: 
	/* ������ ��������� � ��������� ������� */
	
	void SetGUIColors();														// ��������� �������� �����
	void SetPanelSizeAndPosition(int   corner,			float width,			// ��������� ��������� � �������� ������
								 float height,			float x_offset = 0.0f, 
								 float y_offset = 0.0f); 	
	void ShowMenu(); 															// ������� ������ ����
	void ShowLeftSide();														// ����� ������	
	void ShowRightSide();														// ������ ������
	void ShowLeftBottomSide();													// ����� ������ ������
	void ShowBottomPanel();														// ������ ������
	void ShowOptionalPanel();													// �������������� ������
	void ShowMouseHelperPanel(std::string info);								// ������ ��� ��������
	void DisableSides();														// ��������� ������� ������

	/****************************************/
private: 
	/* ������ ��������� ���������� ����������� */
	
	void ShowLog();						// �������� ������ ����
	void ShowGPU();						// �������� ������ � ����������� �� FPS � ����������� �������� 
	void ShowFPS();						// �������� ������ � �������� ��������� FPS
	void ShowPhysicsEngineObjHelp();	// �������� ������� ��� �������� ����������� ������
	void ShowPhysicsEngineSettings();	// �������� ������ �������� ����������� ������
	void ShowPersonList();				// �������� ������ ���������� 
	void ShowPersonControl();			// �������� ��������� ��� ���������� ���������
	void ShowMainPersonList();			// �������� �������� ��������� � ������
	void ShowMainPersonControl();		// �������� ��������� ��� �������� ���������
	void ShowPhysicsEngineObjList();	// �������� ������ �������� � ���������� ������
	void ShowPhysicsEngineObjControl();	// �������� ��������� ��� ���������� ������� � ���������� ������

	/*******************************************/
private:
	/* ���������� ��������� ������� */
	
	bool ShowPersonEnum 			  =	false;	// ����� ������ ���������� �� �����
	bool ShowPersonSettings			  =	false;	// ������ ������ ���������� ����������� ����������	
	bool ShowMainPersonEnum 		  =	false;	// ����� ������ �������� ��������� �� �����
	bool ShowMainPersonSettings		  =	false;	// ������ ������ ���������� ����������� �������� ���������
	bool ShowPhysicsEngineObjEnum	  = true;	// ����� ������ ���������� �������� �� �����
	bool ShowPhysicsEngineObjSettings = false;	// ������ ������ ���������� ����������� ���������� �������
	bool ShowPhysicsEngineObjInfo	  = true;	// ������� ����� ������� ����������� ������  
	bool ShowPhysicsSettings		  = true;	// ������ �������� ����������� ������
	bool ShowHardwareInfo 			  =	true;	// FPS � ���������� � ����������� ��������
	bool ShowFPSGraph 				  =	false;	// ������ ��������� FPS
	bool ShowLogs 					  =	true;	// ���
	bool ShowTriggersList 			  =	false;	// ����� ������ ��������� �� �����
	bool ShowTriggersSettings		  =	false;	// ������ ������ ��������� �� �����

	/********************************/
private:
	/* ��������� ���������� */
	
	std::shared_ptr<Window>					wnd;		// ��������� �� ���� ���������
	std::shared_ptr<Physics::PhysicsEngine> phEngPtr;	// ��������� �� ���������� ������
	std::map<std::wstring, double>			gpu_desc;	// �������� ����������� ��������� [first - ���, second - ����� ������]
	AppLog									applog;		// ���

	/************************/
private:
	/* ��������� �� ������� */

	PersonContainer* persConPtr;	// ��������� �� ��������� ����������
	MainPerson*		 mPersPtr;		// ��������� �� �������� ���������

	/***************************/
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

	/* ��������������� ���������� ��� ������ � ����������� ���������� */
	
	std::string personSelected = "";	// ��� ���������� ���������

	/******************************************************************/

	/* ��������������� ���������� ��� ������ � ���������� ������� */

	std::string objectSelected = "";	// ��� ���������� ������� �� ����������� ������
	bool		AddingObject   = false;	// ��� ���������� �������

	/* ���������� ��� �������� ����������� ������ */
	
	float sq_l = 0.0f;	// ������� �������� ��� hitbox

	/**********************************************/

	/**************************************************************/

	/* ��������������� ���������� ��� ����������� FPS */

	double	sum     = 0.0;						// ����� ���� ��������� �������
	double  average = 0.0;						// ������� �������� FPS
	double	pXMin	= 0.0;						// ����������� �������� ��� ��������������� ������� �� ��� X
	double	pXMax	= (double)N_POINTS - 1.0;	// ������������ �������� ��� ��������������� ������� �� ��� X
	double	pYMin	= 0.0;						// ����������� �������� ��� ��������������� ������� �� ��� Y
	double	pYMax	= 0.0;						// ������������ �������� ��� ��������������� ������� �� ��� Y
	float	arr[N_POINTS];						// ������ �������� fps
	float	counters[N_POINTS];					// ������ �������
	size_t	counter = 0;						// ������� ��������

	/**************************************************/

	std::string mouseHelpInfo = "";	// ������� ��� ��������
};
