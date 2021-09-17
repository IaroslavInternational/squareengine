#pragma once

#include <DirectXMath.h>
#include <string>

// ������� ����� ��� ���� �������� �� �����. �� ����� ������ ����������� ��������� ����������� �������.
class Object2D
{
public:
	// ����������� ������� ������� ��������� ����:
	// name - ��� �������
	// position - ������� �������
	Object2D(std::string name, DirectX::XMFLOAT2 position);
public:
	// �������� ��� �������
	std::string		  GetName() const;

	// �������� ������� �������
	DirectX::XMFLOAT2 GetPosition();
	DirectX::XMFLOAT2* GetPositionPtr();

	// ���������� ��� �������
	void			  SetName(std::string name);

	// ���������� ������� �������
	void			  SetPosition(DirectX::XMFLOAT2 position);
protected:
	// ��� �������
	std::string		  name;
	
	// ������� �������
	DirectX::XMFLOAT2 position;
};
