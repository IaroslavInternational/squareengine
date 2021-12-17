#pragma once

#include "AliveObject2D.h"
#include "Animation.h"
#include "Script.h"

struct AnimationData;

class Person : public AliveObject2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	Person(std::string   name,		   DirectX::XMFLOAT2   position,
		   float		 health,	   float			   damage,
		   size_t        layer,		   std::string		   pathToSprite, 
		   Color		 key,		   bool				   chromaKeyAble, 
		   HitBox		 hitbox,
		   AnimationData aData,		   std::string		   scriptPath,
		   int			 jumpHeight		= 12,
		   int			 gravity		= 300,
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

	struct
	{
		float Duration;	// ����������������� �������
		float Time;		// ����� �������
		bool Active;	// ��������� ���������� �������
	} effect;			// ��������� �������

	/*******************************************/
private:
	/* ���������� �������� ������ � ���������� */

	std::vector<Animation> animations;	// ������ ��������
	float				   holdTime;	// �������� ��������
	
	/*******************************************/
private:
	/* ���������� �������� ������ ������� */
	
	std::string scriptPath;			 // ���� � ������
	Script      script;				 // ������
	float		integralTime = 0.0f; // ��������� �����

	/**************************************/
};