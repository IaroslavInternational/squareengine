#pragma once

#include "Window.h"

#include "SceneDataReader.h"
#include "Layers.h"
#include "PhysicsEngine.h"
#include "Camera.h"

#include "GUISystem.h"

class Scene
{
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd,		std::string scData, 
		  std::shared_ptr<Physics::PhysicsEngine> phEngine);
	~Scene();
public:
	/* ������ ����� */

	void						 ProcessInput(float dt);	// ��������� ������� ������ ���������
	void						 Render(float dt);			// ��������� �����
	std::pair<std::string, bool> IsOnTheSceneTrigger();		// ��������� ������ �������� �������� �� ����� 
	const std::string&			 GetName() const;			// �������� ��� �����

	/****************/
private:
	/* ���������� �������� ����� */
	
	std::string name;	// ��� �����

	/*****************************/
private:
	/* ��������������� ���������� */
	
	std::shared_ptr<Window> wnd;	// ��������� �� ������� ���� 
	std::shared_ptr<Camera> camera;	// ��������� �� ������ 
	GUISystem				gui;	// ��������� ������
	SceneDataReader			sdr;	// ����� ������ �����
	MainPersonDataReader	mdr;	// ����� ������ �������� ���������

	/******************************/
private:
	/* ������ ������ */

	std::shared_ptr<Physics::PhysicsEngine> phEngine;

	/*****************/
private:
	/* ������� */
	
	PersonContainer pc;
	MainPerson		hero;
	InteractableObject2DContainer Iobj;
	Layers			layers;

	/***********/
};