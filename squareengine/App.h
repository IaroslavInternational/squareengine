#pragma once

#include "Window.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"

#include "ImguiManager.h"

#include "Scene.h"
#include "PhysicsEngine.h"

#include <map>

class App
{
public:
	App(const std::string& commandLine = "");
	~App();

	// ������� ����
	int Go();
private:
	// ��������� ������ ���������
	void HandleInput(float dt);

	// ��������� �����
	void DoFrame(float dt);
private:
	// ���������� ������
	std::string commandLine;

	ImguiManager imgui;

	// ��������� �� ����
	std::shared_ptr<Window> wnd;
	std::shared_ptr<Physics::PhysicsEngine> phEngine;

	// ���������� ���������
	ScriptCommander scriptCommander;
	
	// ������� ������
	EngineTimer timer;

	// ��������
	float speed_factor = 1.0f;

	// ��������� ����
	std::map<std::unique_ptr<Scene>, bool> scenes;
};