#pragma once

#include "Object2D.h"
#include "HitBox.h"

class PhysicObject2D : public Object2D
{
public:
	PhysicObject2D(std::string name, DirectX::XMFLOAT2 position, HitBox	hitbox, size_t layer, std::string pathToSprite,
				   int jump_height, float gravity);
public:
	void AllowMoveUp();
	void AllowMoveDown();
	void AllowMoveLeft();
	void AllowMoveRight();
	void DisAllowMoveUp();
	void DisAllowMoveDown();
	void DisAllowMoveLeft();
	void DisAllowMoveRight();
public:
	/* ���������� ������ */
	
	void   SetHitBox(HitBox hb); // ���������� hitbox
	HitBox GetHitBox();			 // �������� hitbox
	void   CalculateDeltas();	 // ���������� �������������� ������� hitbox �� ����� ������� ���������

	/*********************/
protected:
	int   jump_height;		     // ������ ������
	int	  jump_count;			 // �������������� ���������� ��� ������
	bool  IsOnJump     = false;	 // ��������� ������
	bool  IsMovingDown = false;	 // ��������� �������� ����� ������
	float gravity;				 // ���������� ����������
protected:
	bool AllowedMovingUp    = true; // ��������� ����������� �������� �����
	bool AllowedMovingDown  = true;	// ��������� ����������� �������� ����
	bool AllowedMovingLeft  = true;	// ��������� ����������� �������� �����
	bool AllowedMovingRight = true;	// ��������� ����������� �������� ������
protected:
	/* ���������� �������� ������ � hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// ��������� hitbox
	float  dx;							// �������������� ������ hitbox �� ����� ������� �� ��� x
	float  dy;							// �������������� ������ hitbox �� ����� ������� �� ��� y

	/***************************************/
};

