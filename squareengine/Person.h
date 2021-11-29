#pragma once

#include "PhysicObject2D.h"
#include "Animation.h"
#include "Script.h"

struct AnimationData;

class Person : public PhysicObject2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	Person(std::string   name,		   DirectX::XMFLOAT2   position,
		   size_t        layer,		   std::string		   pathToSprite, 
		   Color		 key,		   HitBox			   hitbox,
		   AnimationData aData,		   std::string		   scriptPath,
		   float		 speed 			= 1.0f, 
		   float		 effectDuration	= 0.045f, 
		   float		 effectTime 	= 0.0f, 
		   bool			 effectActive	= false);
public:
	/* ������� ������ ��� ��������� ��������� */
	
	void Draw(Graphics& gfx) override;			// ���������� ���������
	void Process(float dt);						// ������� ���� ���������
	void SetDirection(DirectX::XMFLOAT2 dir);	// ���������� ����������� ��������� 
	void Update(float dt);						// �������� ��������� ���������
	void ActivateEffect();						// ������������ ������

	/******************************************/
public:
	/* ������� ������ ��� �������������� � ���������� */
	
	void   Translate(DirectX::XMFLOAT2 delta);
	void   SetAnimation(std::vector<Animation> anim);
	void   SetScript(std::string path);

	/**************************************************/
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
	};																// ������ ��������
	std::vector<Animation> animations;								// ������ ��������
	Sequence			   iCurSequence = Sequence::StandingDown;	// ������� ��������� ��������
	float				   holdTime;								// �������� ��������
	
	/*******************************************/
private:
	/* ���������� �������� ������ ������� */
	
	std::string scriptPath;			 // ���� � ������
	Script      script;				 // ������
	float		integralTime = 0.0f; // ��������� �����

	/**************************************/
};