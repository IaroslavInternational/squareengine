#pragma once

#include "Object2D.h"
#include "Animation.h"

class Person : public Object2D
{
public:
	friend class GUISystem;
public:
	Person(std::string name,		 DirectX::XMFLOAT2 position,
		   std::string pathToSprite, HitBox			   hitbox,
		   float	   speed 		  =	1.0f, 
		   float	   effectDuration = 0.045f, 
		   float	   effectTime 	  =	0.0f, 
		   bool		   effectActive   =	false);
public:
	/* ������� ������ ��� ��������� ��������� */
	
	void Draw(Graphics& gfx);					// ���������� ���������
	void SetDirection(DirectX::XMFLOAT2 dir);	// ���������� ����������� ��������� 
	void Update(float dt);						// �������� ��������� ���������
	void ActivateEffect();						// ������������ ������

	/******************************************/
public:
	/* ������� ������ ��� �������������� � ���������� */
	
	void   Translate(DirectX::XMFLOAT2 delta);
	void   SetHitBox(HitBox hb);	// ���������� hitbox
	HitBox GetHitBox();				// �������� hitbox

	/**************************************************/
private:
	/* ���������� ������ */

	void CalculateDeltas();	// ���������� �������������� ������� hitbox �� ����� ������� ���������

	/*********************/
private:
	/* ���������� �������� ��������� ��������� */
	
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
	/* ���������� �������� ������ � hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// ��������� hitbox
	float  dx;							// �������������� ������ hitbox �� ����� ������� �� ��� x
	float  dy;							// �������������� ������ hitbox �� ����� ������� �� ��� y

	/***************************************/
};