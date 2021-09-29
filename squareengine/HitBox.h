#pragma once

#include <DirectXMath.h>

class HitBox
{
public:
	HitBox(DirectX::XMFLOAT4 coordinates);
	HitBox(float leftTopX,     float leftTopY,
		   float rightBottomX, float rightBottomY);
public:
	/* ��������� */
	
	HitBox operator-(DirectX::XMFLOAT2 delta);
	HitBox operator+(DirectX::XMFLOAT2 delta);

	/*************/
public:
	/* �������� ������ ��� ������ � hitbox */
	
	void			   Update(DirectX::XMFLOAT2 delta);	// �������� 4 ���������� ����� hitbox ��������������� ���������
	void			   Update(float dx, float dy);		// �������� 4 ���������� ����� hitbox ��������������� ���������
	void			   UpdateX(float dx);				// �������� 2 ���������� ����� hitbox ��������������� ���������
	void			   UpdateY(float dy);				// �������� 2 ���������� ����� hitbox ��������������� ���������
	DirectX::XMFLOAT4& GetCoordinates();				// �������� 4 ���������� ����� hitbox 

	/***************************************/
public:
	/* ������ �������� � hitbox */
	
	bool IsCollide(HitBox hb);	// �������� ������������ � ������ hitbox

	/****************************/
private:
	/* ���������� �������� hitbox */
	
	DirectX::XMFLOAT4 coordinates; // 4 ���������� ����� hitbox

	/******************************/
};