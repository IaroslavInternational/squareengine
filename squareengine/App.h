#pragma once

#include "Window.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"

#include "ImguiManager.h"

#include "Scene.h"
#include "PhysicsEngine.h"
#include "GUISystem.h"

#include <map>

class App
{
public:
	App(const std::string& commandLine, const std::string& projectName);
	~App();
public:
	int Go();											// ������� ����
private:
	void HandleInput(float dt);							// ��������� ������ ���������
	void DoFrame(float dt);								// ��������� �����
private:
	std::string commandLine;							// ���������� ������
	std::string projectName;							// 
	ImguiManager imgui;									//
	std::shared_ptr<Window> wnd;						// ��������� �� ����
	std::shared_ptr<Physics::PhysicsEngine> phEngine;	//
	ScriptCommander scriptCommander;					// ���������� ���������
	EngineTimer timer;									// ������� ������
	std::shared_ptr<GUISystem> gui;
	float speed_factor = 1.0f;							// ��������
	std::map<std::string, bool> scenes;					// ��������� ����
	std::unique_ptr<Scene> scene;						// ��������� ����
};