#pragma once

#include "Window.h"

#include "SceneDataReader.h"
#include "ObjectsQueue.h"
#include "PhysicsEngine.h"
#include "Camera.h"

#include "GUISystem.h"

class Scene
{
public:
	Scene(std::string							  name,	
		  std::shared_ptr<Window>				  wnd,		
		  std::string							  scData, 
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
	/* ������ ������ */

	std::shared_ptr<Physics::PhysicsEngine> phEngine;

	/*****************/
private:
	/* ��������������� ���������� */
	
	std::shared_ptr<Window> wnd;	// ��������� �� ������� ���� 
	std::shared_ptr<Camera> camera;	// ��������� �� ������ 
	GUISystem				gui;	// ��������� ������
	SceneDataReader			sdr;	// ����� ������ �����
	MainPersonDataReader	mdr;	// ����� ������ �������� ���������

	/******************************/
private:
	/* ������� */

	MainPerson					  hero;
	PersonContainer				  persCon;
	InteractableObject2DContainer Iobj;
	ObjectsQueue				  objQueue;

	/***********/
};