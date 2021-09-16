#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(std::string path);
	~SceneDataReader();
public:
	std::string GetPersonContainerPath() const;		// ���� � ����� � ������� � ���������� ��������
private:
	// 0 - ���� � ����������
	std::vector<std::string>& GetPaths();
private:
	std::vector<std::string> paths;
};

