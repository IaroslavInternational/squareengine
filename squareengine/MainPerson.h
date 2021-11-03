#pragma once

#include "MainPersonDataReader.h"
#include "Object2D.h"
#include "Animation.h"
#include "Window.h"
#include "Camera.h"

class MainPerson : public Object2D
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
	void   AllowMoveUp();
	void   AllowMoveDown();
	void   AllowMoveLeft();
	void   AllowMoveRight();
	void   DisAllowMoveUp();
	void   DisAllowMoveDown();
	void   DisAllowMoveLeft();
	void   DisAllowMoveRight();
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

	std::string		  dataPath;							// ���� � ������ � ������� ���������
	DirectX::XMFLOAT2 vel		   = { 0.0f, 0.0f };	// ������ �������� ��������
	float			  speed;							// �������� �����������
	int				  jump_height  = 8;					// ������ ������
	int				  jump_count;						// �������������� ���������� ��� ������
	bool			  IsMovingDown = false;				// ��������� �������� ����� ������
	float			  gravity	   = 300.0f;			// ���������� ����������
	bool			  IsOnJump	   = false;				// ��������� ������
	struct
	{
		float Duration;									// ����������������� �������
		float Time;										// ����� �������
		bool Active;									// ��������� ���������� �������
	} effect;											// ��������� �������
	bool AllowedMovingUp    = true;						// ��������� ����������� �������� �����
	bool AllowedMovingDown  = true;						// ��������� ����������� �������� ����
	bool AllowedMovingLeft  = true;						// ��������� ����������� �������� �����
	bool AllowedMovingRight = true;						// ��������� ����������� �������� ������

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
