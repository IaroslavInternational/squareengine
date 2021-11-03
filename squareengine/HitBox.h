#pragma once

#include <DirectXMath.h>
#include <string>

class HitBox
{
public:
	friend class GUISystem;
public:
	HitBox(std::string name, DirectX::XMFLOAT4 coordinates);
	HitBox(std::string name, float leftTopX,     float leftTopY,
							 float rightBottomX, float rightBottomY);
public:
	/* ��������� */
	
	void   operator=(HitBox hb);
	HitBox operator-(DirectX::XMFLOAT2 delta);
	HitBox operator+(DirectX::XMFLOAT2 delta);

	/*************/
public:
	/* �������� ������ ��� ������ � hitbox */
	
	void			   Translate(DirectX::XMFLOAT2 delta);
	bool			   IsVisible();
	void			   SetVisibility(bool state);
	void			   Update(DirectX::XMFLOAT2 delta);	// �������� 4 ���������� ����� hitbox ��������������� ���������
	void			   Update(float dx, float dy);		// �������� 4 ���������� ����� hitbox ��������������� ���������
	void			   UpdateX(float dx);				// �������� 2 ���������� ����� hitbox ��������������� ���������
	void			   UpdateY(float dy);				// �������� 2 ���������� ����� hitbox ��������������� ���������
	std::string		   GetName();						// �������� ���
	DirectX::XMFLOAT4& GetCoordinates();				// �������� 4 ���������� ����� hitbox 

	/***************************************/
public:
	/* ������ �������� � hitbox */
	
	bool IsCollide(HitBox hb);					// �������� ������������ � ������ hitbox
	bool IsOverlap(DirectX::XMFLOAT2 point);	// �������� ��������� �����

	/****************************/
private:
	/* ���������� �������� hitbox */
	
	std::string		  name;
	DirectX::XMFLOAT4 coordinates;		  // 4 ���������� ����� hitbox
	bool			  visability = true;  // ���������

	/******************************/
};