#pragma once

#include "imgui\imnodes.h"

#include <vector>
#include <string>
#include <DirectXMath.h>

// Blueprint
class NodeEditor
{
public:
	NodeEditor();
	~NodeEditor();
public:
	void Show();
	void Init();
	void BeginFrame();
	void EndFrame();
private:
	void RenderNodes();	// ��������� ���
	void AddNode(size_t id, std::string cmd, int value); // �������� ����
	void ShowLeftPanel(ImVec2 size);	// �������� ������ *����� ������*
	void ShowRightPanel(ImVec2 size);	// �������� ������ *������ ������*
	void ConncetCam2Model(int cam_id, int mod_id); /*/**//*/*/
private:
	struct ScriptNode
	{
		ScriptNode(size_t id, std::string cmd, int value, DirectX::XMFLOAT2 position)
			:
			id(id),
			cmd(cmd),
			value(value),
			position(position)
		{}

		bool operator == (const ScriptNode& rhs)
		{
			return id == rhs.id && cmd == rhs.cmd && value == rhs.value && 
				   position.x == rhs.position.x && position.y == rhs.position.y;
		}

		static float NextSize(size_t nodes_size)
		{
			return height * (float)nodes_size;
		}

		size_t			  id;
		std::string		  cmd;
		int				  value;
		DirectX::XMFLOAT2 position;

		static constexpr float height = 90.0f;
	};

	struct Link
	{
		int id;
		int start_attr;
		int end_attr;
	};
private:
	imnodes::EditorContext*  context = nullptr;	// �������� ����� ����������
	std::vector<ScriptNode>  nodes;				// ���� ��������
	std::vector<std::string> cmd_list = 
	{
		"step_x",
		"step_y",
		"delay"
	};
private:
	bool		IsInit		= false; //
	bool		IsPopup		= false; // ������ ���������� ���� �������������
	std::string cmdSelected = "";	 //
};