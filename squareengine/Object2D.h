#pragma once

#include <DirectXMath.h>
#include <string>

#include "HitBox.h"
#include "Surface2D.h"

// ������� ����� ��� ���� �������� �� �����. 
// �� ����� ������ ����������� ��������� ����������� �������.
class Object2D
{
public:
	// ����������� ������� ������� ��������� ����:
	// name - ��� �������
	// position - ������� �������
	// hitbox - ������� �������
	Object2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite, HitBox hitbox);
public:
	/* ������� ������ ��� �������� ������� */
	
	void			  SetName(std::string name);				// ���������� ��� �������
	void			  SetPosition(DirectX::XMFLOAT2 position);	// ���������� ������� �������
	void			  SetSurface(Surface2D sprite);			// ���������� ��������� ����������� �������
	void			  SetHitBox(HitBox hb);						// ���������� hitbox
	std::string		  GetName() const;							// �������� ��� �������
	DirectX::XMFLOAT2 GetPosition();							// �������� ������� �������
	Surface2D&		  GetSurface();								// �������� ��������� ����������� �������
	HitBox			  GetHitBox();								// �������� hitbox

	/***************************************/
protected:
	/* ���������� ������ */

	void CalculateDeltas();	// ���������� �������������� ������� hitbox �� ����� ������� ���������

	/*********************/
protected:
	/* ���������� �������� ������� */
	
	std::string		  name;		// ��� �������
	DirectX::XMFLOAT2 position;	// ������� �������
	Surface2D		  sprite;	// ��������� ����������� �������

	/*******************************/
protected:
	/* ���������� �������� ������ � hitbox */

	HitBox hitbox;						// Hitbox
	bool   hitbox_visability = true;	// ��������� hitbox
	float  dx;							// �������������� ������ hitbox �� ����� ������� �� ��� x
	float  dy;							// �������������� ������ hitbox �� ����� ������� �� ��� y

	/***************************************/
};
