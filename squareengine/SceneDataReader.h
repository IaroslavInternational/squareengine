#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
	~SceneDataReader();
public:
	std::string GetCameraContainerPath() const;		// ���� � ����� � ������� � ���������� �����
	std::string GetPointLightContainerPath() const;	// ���� � ����� � ������� � ���������� ���. ���. ���������
	std::string GetModelContainerPath() const;		// ���� � ����� � ������� � ���������� �������
	std::string GetTriggerContainerPath() const;	// ���� � ����� � ������� � ���������� ���������
private:
	// 0 - ���� � �������
	// 1 - ���� � ��������� 
	// 2 - ���� � �������� ���������� �����
	// 3 - ���� � �������
	std::vector<std::string>& GetPaths();
private:
	std::vector<std::string> paths;
};

