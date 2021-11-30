#pragma once

#include "ImguiManager.h"
#include "EngineTimer.h"
#include "ScriptCommander.h"
#include "Scene.h"
#include "GUISystem.h"
#include "SceneTransition.h"

#include <map>

class App
{
public:
	App(const std::string& commandLine, const std::string& projectName);
public:
	int Go();											// ������� ����
private:
	void HandleInput(float dt);							// ��������� ������ ���������
	void DoFrame(float dt);								// ��������� �����
private:
	ImguiManager imgui;									// ������������� ���������� 
private:
	std::shared_ptr<Window>					wnd;		// ��������� �� ����
	std::shared_ptr<Physics::PhysicsEngine> phEngine;	// ��������� �� ���������� ������
	std::map<std::string, bool>				scenes;		// ��������� �������� ���������� ����
	std::unique_ptr<Scene>					scene;		// ������� �����
	std::shared_ptr<GUISystem>				gui;		// ��������� �� ���������
	std::unique_ptr<SceneTransition>		transition;
private:
	std::string				   commandLine;						// ���������� ������
	std::string				   projectName;						// ��� �������
	ScriptCommander			   scriptCommander;					// ���������� ���������� ���������
	EngineTimer				   timer;								// ������� ������
	float					   speed_factor = 1.0f;				// ������������� �������� ��������
	std::optional<std::string> curTrigger = std::nullopt;
};