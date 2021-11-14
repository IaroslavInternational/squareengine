#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
public:
	std::string GetPersonContainerPath() const;				// ���� � ����� � ������� � ���������� ��������
	std::string GetMainPersonDataPath() const;				// ���� � ����� � ������� � ������� ���������
	std::string GetPhysicsDataPath() const;					// ���� � ����� � ������� � ������ �����
	std::string GetInteractableObjectsDataPath() const;		// ���� � ����� � ������� �� ������������� ��������
	std::string GetCameraDataPath() const;					// ���� � ����� � ������� � ������
	std::string GetTriggerContainerDataPath() const;		// ���� � ����� � ������� � ���������
private:
	// 0 - ���� � ����������
	// 1 - ���� � �������� �����
	// 2 - ���� � ������
	// 3 - ���� � ������������� ��������
	// 4 - ���� � ������
	// 5 - ���� � ���������
	std::vector<std::string>& GetPaths();	
private:
	std::vector<std::string> paths;
};

