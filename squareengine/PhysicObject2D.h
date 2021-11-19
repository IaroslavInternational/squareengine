#pragma once

#include "Object2D.h"

class PhysicObject2D : public Object2D
{
public:
	PhysicObject2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite,
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
};

