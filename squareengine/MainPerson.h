#pragma once

#include "MainPersonDataReader.h"
#include "AliveObject2D.h"
#include "Animation.h"
#include "Window.h"
#include "Camera.h"

class MainPerson : public AliveObject2D
{
public:
	friend class GUISystem;
	friend class ObjectsQueue;
public:
	MainPerson(MainPersonDataReader data, std::shared_ptr<Window> wnd, std::shared_ptr<Camera> camera);
public:
	/* ������� ������ ��� ��������� �������� ��������� */

	void Draw(Graphics& gfx) override;			// ���������� �������� ���������
	void Process(float dt);
	void SetDirection(DirectX::XMFLOAT2 dir);	// ���������� ����������� ��������� 
	void Update(float dt);						// �������� ��������� �������� ���������
	void ActivateEffect();						// ������������ ������

	/***************************************************/
public:
	/* ������� ������ ��� �������������� � ������� ���������� */
	
	void Translate(DirectX::XMFLOAT2 delta);
	void SetAnimation(std::vector<Animation> anim);

	/**********************************************************/
private:
	/* ���������� �������� ��������� ��������� */

	std::string		  dataPath;				// ���� � ������ � ������� ���������
	float			  speed;
	DirectX::XMFLOAT2 vel = { 0.0f, 0.0f };	// ������ �������� ��������
	float			  inertion = 0.0f;

	struct
	{
		float Duration;						// ����������������� �������
		float Time;							// ����� �������
		bool Active;						// ��������� ���������� �������
	} effect;								// ��������� �������

	/*******************************************/
private:
	/* ���������� �������� ������ � ���������� */

	std::vector<Animation> animations;	// ������ ��������
	float				   holdTime;	// �������� ��������

	/*******************************************/
private:
	enum class CameraMode
	{
		SteadyPerson,
		SteadyWorld,
		Hybrid
	};									// ������ ������� ������
	std::shared_ptr<Window> wnd;		// ��������� �� ������� ����
	std::shared_ptr<Camera> camera;		// ��������� �� ������
	CameraMode				cameraMode;	// ����� ������
};
