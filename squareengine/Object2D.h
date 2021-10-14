#pragma once

#include <DirectXMath.h>
#include <string>

#include "HitBox.h"
#include "Surface2D.h"
#include "Graphics.h"

// ������� ����� ��� ���� �������� �� �����. 
// �� ����� ������ ����������� ��������� ����������� �������.
class Object2D
{
public:
	// ����������� ������� ������� ��������� ����:
	// name - ��� �������
	// position - ������� �������
	Object2D(std::string name, DirectX::XMFLOAT2 position, size_t layer, std::string pathToSprite);
	virtual ~Object2D() {};
public:
	/* ������� ������ ��� �������� ������� */

	virtual void	  Draw(Graphics& gfx) {};
	void			  Translate(DirectX::XMFLOAT2 delta);		//
	void			  SetName(std::string name);				// ���������� ��� �������
	void			  SetPosition(DirectX::XMFLOAT2 position);	// ���������� ������� �������
	void			  SetSurface(Surface2D sprite);				// ���������� ��������� ����������� �������
	void			  LayerUp();								// 
	void			  LayerDown();								//
	std::string		  GetName() const;							// �������� ��� �������
	DirectX::XMFLOAT2 GetPosition();							// �������� ������� �������
	size_t			  GetLayer();								// �������� ����� ����
	Surface2D&		  GetSurface();								// �������� ��������� ����������� �������

	/***************************************/
protected:
	/* ���������� �������� ������� */
	
	std::string		  name;		// ��� �������
	DirectX::XMFLOAT2 position;	// ������� �������
	size_t			  layer;	// ����� ����
	Surface2D		  image;	// ��������� ����������� �������

	/*******************************/
};
