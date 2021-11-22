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
	void RenderNodes();	// Отрисовка нод
	void AddNode(size_t id, std::string cmd, int value); // Добавить ноду
	void ShowLeftPanel(ImVec2 size);	// Показать модели *Левая панель*
	void ShowRightPanel(ImVec2 size);	// Показать модели *Правая панель*
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
	imnodes::EditorContext*  context = nullptr;	// Контекст среды разработки
	std::vector<ScriptNode>  nodes;				// Ноды скриптов
	std::vector<std::string> cmd_list = 
	{
		"step_x",
		"step_y",
		"delay"
	};
private:
	bool		IsInit		= false; //
	bool		IsPopup		= false; // Статус модального окна подтверждения
	std::string cmdSelected = "";	 //
};