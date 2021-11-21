#pragma once

#include "imgui\imnodes.h"

#include "AppLog.h"

#include <algorithm>
#include <vector>
#include <string>
#include <DirectXMath.h>

// Blueprint
class NodeEditor
{
public:
	NodeEditor(AppLog* log);
	~NodeEditor();
public:
	// ������������� ������ �����
	void BeginFrame();

	// ���������� ������ �����
	void EndFrame();

	// �������� Editor
	void Show(bool *IsShown);

	// �������������
	void Init();
private:
	struct TransformStruct
	{
		TransformStruct(DirectX::XMFLOAT3 pos)
			:
			x(pos.x),
			y(pos.y),
			z(pos.z)
		{}

		float x;
		float y;
		float z;
	};
	
	// ��������� ����� ������
	struct CameraNode
	{
		CameraNode(const int i, std::string n, DirectX::XMFLOAT3 pos)
			:
			id(i),
			name(n),
			pos(pos),
			offset(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
			isOffset_CurrentPos(false)
		{}

		int id;
		std::string name;
		TransformStruct pos;
		TransformStruct offset;

		bool isOffset_CurrentPos;
	};

	// ��������� ����� ������
	struct ModelNode
	{
		ModelNode(const int i, std::string n, DirectX::XMFLOAT3 pos)
			:
			id(i),
			name(n),
			pos(pos)
		{}

		int id;
		std::string name;
		TransformStruct pos;
	};

	// ��������� ����� ������
	struct Link
	{
		int id;
		int start_attr;
		int end_attr;
	};
private:
	// �������� ����� ����������
	imnodes::EditorContext* context = nullptr;
	
	// nodes �����
	std::vector<CameraNode> cNodes;

	// nodes �������
	std::vector<ModelNode> mNodes;

	// ����� ����� node
	std::vector<Link> links;

	// ������� id � �������������
	int current_id = 0;

	// ������ ��� ������ ����� �����
	float current_delta_cam = 0.0f;

	// ������ ��� ������ ����� �������
	float current_delta_model = 0.0f;

	// ������ �������������
	bool IsInit = false;
private:
	/* ���������� */

	// ������ ��������� ������
	size_t activeCam = 0;
	
	// ������ ��������� ������
	size_t activeModel = 0;

	// id ����� ������ ��� ����������
	mutable int camIdToPopup = 0;

	// id ����� ������ ��� ����������
	mutable int modIdToPopup = 0;

	// ���� ������� ��������� ���� �������������
	bool isPopup = false;
	/**************/
private:
	// ��������� nodes
	void RenderNodes();

	// �������� ������ node
	void AddCameraNode(int id, std::string name);

	// �������� ������ node
	void AddModelNode(int id, std::string name);

	// �������� ������ *����� ������*
	void ShowLeftPanel(ImVec2 size);

	// �������� ������ *������ ������*
	void ShowRightPanel(ImVec2 size);

	CameraNode* FindCamNodeById(int id);

	ModelNode* FindModNodeById(int id);

	void ConncetCam2Model(int cam_id, int mod_id);
private:
	// ���
	AppLog* applog;
};