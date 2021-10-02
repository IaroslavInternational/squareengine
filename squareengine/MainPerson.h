#pragma once

#include "MainPersonDataReader.h"
#include "Object2D.h"
#include "Animation.h"
#include "Window.h"

class MainPerson : public Object2D
{
public:
	friend class GUISystem;
public:
	MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd);
public:
	/* ������� ������ ��� ��������� �������� ��������� */

	void Draw();								// ���������� �������� ���������
	void SetDirection(DirectX::XMFLOAT2 dir);	// ���������� ����������� ��������� 
	void Update(float dt);						// �������� ��������� �������� ���������
	void ActivateEffect();						// ������������ ������

	/***************************************************/
public:
	/* ������� ������ ��� �������������� � ������� ���������� */
	
	void ProcessMoving(float dt);
	void AllowMoveUp();
	void AllowMoveDown();
	void AllowMoveLeft();
	void AllowMoveRight();
	void DisAllowMoveUp();
	void DisAllowMoveDown();
	void DisAllowMoveLeft();
	void DisAllowMoveRight();

	/**********************************************************/
private:
	/* ���������� �������� ��������� ��������� */

	std::string		  dataPath;				// ���� � ������ � ������� ���������
	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// ������ �������� ��������
	float			  speed;				// �������� �����������
	struct
	{
		float Duration;	// ����������������� �������
		float Time;		// ����� �������
		bool Active;	// ��������� ���������� �������
	} effect;			// ��������� �������
	bool AllowedMovingUp    = true;
	bool AllowedMovingDown  = true;	
	bool AllowedMovingLeft  = true;
	bool AllowedMovingRight = true;

	/*******************************************/
private:
	/* ���������� �������� ������ � ���������� */

	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};																// ������ ��������
	std::vector<Animation> animations;								// ������ ��������
	Sequence			   iCurSequence = Sequence::StandingDown;	// ������� ��������� ��������

	/*******************************************/
private:
	std::shared_ptr<Window> wnd;	// ��������� �� ������� ����
};


