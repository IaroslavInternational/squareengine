#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
	~SceneDataReader();
public:
	std::string GetPersonContainerPath() const;	// ���� � ����� � ������� � ���������� ��������
	std::string GetMainPersonDataPath() const;	// ���� � ����� � ������� � ������� ���������
	std::string GetPhysicsDataPath() const;		// ���� � ����� � ������� � ������ �����
private:
	// 0 - ���� � ����������
	// 1 - ���� � �������� �����
	// 2 - ���� � ������
	std::vector<std::string>& GetPaths();	
private:
	std::vector<std::string> paths;
};

