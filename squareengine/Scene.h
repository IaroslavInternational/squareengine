#pragma once

#include "Window.h"

#include "PhysicsEngine.h"
#include "SceneDataReader.h"
#include "Map.h"

class Scene
{
public:
	friend class GUISystem;
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd,		
		  std::string							  scData, 
		  std::shared_ptr<Physics::PhysicsEngine> phEngine);
public:
	/* ������ ����� */

	void						 ProcessInput(float dt);	// ��������� ������� ������ ���������
	void						 Render(float dt);			// ��������� �����
	std::optional<std::string>   IsOnTheSceneTrigger();		// ��������� ������ �������� �������� �� ����� 
	const std::string&			 GetName() const;			// �������� ��� �����

	/****************/
private:
	/* ���������� �������� ����� */
	
	std::string name;	// ��� �����

	/*****************************/
private:
	/* ������ ������ */

	std::shared_ptr<Physics::PhysicsEngine> phEngine;

	/*****************/
private:
	/* ��������������� ���������� */
	
	SceneDataReader			sdr;	// ����� ������ �����
	std::shared_ptr<Window> wnd;	// ��������� �� ������� ���� 
	std::shared_ptr<Camera> camera;	// ��������� �� ������ 

	/******************************/
private:
	Map world;
};