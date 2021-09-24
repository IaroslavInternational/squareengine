#pragma once

#include "MainPersonDataReader.h"
#include "Object2D.h"
#include "Animation.h"
#include "Window.h"

class MainPerson : public Object2D
{
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

	/**********************************************************/
private:
	/* ���������� �������� ��������� ��������� */

	std::string dataPath;

	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// ������ �������� ��������
	float			  speed;				// �������� �����������
	struct
	{
		float Duration;	// ����������������� �������
		float Time;		// ����� �������
		bool Active;	// ��������� ���������� �������
	} effect;			// ��������� �������

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
	};	// ������ ��������

	std::vector<Animation> animations;								// ������ ��������
	Sequence			   iCurSequence = Sequence::StandingDown;	// ������� ��������� ��������

	/*******************************************/
private:
	std::shared_ptr<Window> wnd;
};


