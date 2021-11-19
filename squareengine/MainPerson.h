#pragma once

#include "MainPersonDataReader.h"
#include "PhysicObject2D.h"
#include "Animation.h"
#include "Window.h"
#include "Camera.h"

class MainPerson : public PhysicObject2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera);
public:
	/* ������� ������ ��� ��������� �������� ��������� */

	void Draw(Graphics& gfx) override;			// ���������� �������� ���������
	void ProcessMoving(float dt);
	void SetDirection(DirectX::XMFLOAT2 dir);	// ���������� ����������� ��������� 
	void Update(float dt);						// �������� ��������� �������� ���������
	void ActivateEffect();						// ������������ ������

	/***************************************************/
public:
	/* ������� ������ ��� �������������� � ������� ���������� */
	
	void   Translate(DirectX::XMFLOAT2 delta);
	void   SetHitBox(HitBox hb);	// ���������� hitbox
	HitBox GetHitBox();				// �������� hitbox
	void   SetAnimation(std::vector<Animation> anim);

	/**********************************************************/
private:
	/* ���������� ������ */

	void CalculateDeltas();	// ���������� �������������� ������� hitbox �� ����� ������� ���������

	/*********************/
private:
	/* ���������� �������� ��������� ��������� */

	std::string		  dataPath;				// ���� � ������ � ������� ���������
	float			  speed;
	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// ������ �������� ��������
	struct
	{
		float Duration;						// ����������������� �������
		float Time;							// ����� �������
		bool Active;						// ��������� ���������� �������
	} effect;								// ��������� �������

	/*******************************************/
private:
	/* ���������� �������� ������ � ���������� */

	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		StandingLeft,
		StandingRight,
		Count
	};																// ������ ��������
	std::vector<Animation> animations;								// ������ ��������
	Sequence			   iCurSequence = Sequence::StandingRight;	// ������� ��������� ��������
	float				   holdTime;								// �������� ��������

	/*******************************************/
private:
	/* ���������� �������� ������ � hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// ��������� hitbox
	float  dx;							// �������������� ������ hitbox �� ����� ������� �� ��� x
	float  dy;							// �������������� ������ hitbox �� ����� ������� �� ��� y

	/***************************************/
private:
	enum class CameraMode
	{
		SteadyPerson,
		SteadyWorld
	};									// ������ ������� ������
	std::shared_ptr<Window> wnd;		// ��������� �� ������� ����
	std::shared_ptr<Camera> camera;		// ��������� �� ������
	CameraMode				cameraMode;	// ����� ������
};
