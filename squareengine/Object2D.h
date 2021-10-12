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
	Object2D(std::string name, DirectX::XMFLOAT2 position, std::string pathToSprite);
public:
	/* ������� ������ ��� �������� ������� */
	
	void			  Translate(DirectX::XMFLOAT2 delta);		//
	void			  SetName(std::string name);				// ���������� ��� �������
	void			  SetPosition(DirectX::XMFLOAT2 position);	// ���������� ������� �������
	void			  SetSurface(Surface2D sprite);				// ���������� ��������� ����������� �������
	std::string		  GetName() const;							// �������� ��� �������
	DirectX::XMFLOAT2 GetPosition();							// �������� ������� �������
	Surface2D&		  GetSurface();								// �������� ��������� ����������� �������

	/***************************************/
protected:
	/* ���������� �������� ������� */
	
	std::string		  name;		// ��� �������
	DirectX::XMFLOAT2 position;	// ������� �������
	Surface2D		  image;	// ��������� ����������� �������

	/*******************************/
};
