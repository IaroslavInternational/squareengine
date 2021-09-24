#pragma once

#include "Window.h"

#include "SceneDataReader.h"
#include "PersonContainer.h"
#include "MainPerson.h"

#include "GUISystem.h"

namespace dx = DirectX;

class Scene
{
public:
	Scene(std::string name,	std::shared_ptr<Window> wnd, 
		  std::string data);
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
	GUISystem				gui;	// ��������� ������
	SceneDataReader			sdr;	// ����� ������ �����
	MainPersonDataReader	mdr;	// ����� ������ �������� ���������

	/******************************/
private:
	/* ������� */
	
	PersonContainer pc;
	MainPerson		hero;
	
	/***********/
};