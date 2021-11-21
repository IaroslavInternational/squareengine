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
	// Инициализация нового кадра
	void BeginFrame();

	// Завершение нового кадра
	void EndFrame();

	// Показать Editor
	void Show(bool *IsShown);

	// Инициализация
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
	
	// Структура блока камеры
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

	// Структура блока модели
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

	// Структура связи блоков
	struct Link
	{
		int id;
		int start_attr;
		int end_attr;
	};
private:
	// Контекст среды разработки
	imnodes::EditorContext* context = nullptr;
	
	// nodes камер
	std::vector<CameraNode> cNodes;

	// nodes моделей
	std::vector<ModelNode> mNodes;

	// Связи между node
	std::vector<Link> links;

	// Текущий id в инициализации
	int current_id = 0;

	// Отступ для нового блока камер
	float current_delta_cam = 0.0f;

	// Отступ для нового блока моделей
	float current_delta_model = 0.0f;

	// Статус инициализации
	bool IsInit = false;
private:
	/* Контейнеры */

	// Индекс выбранной камеры
	size_t activeCam = 0;
	
	// Индекс выбранной модели
	size_t activeModel = 0;

	// id блока камеры при перезаписи
	mutable int camIdToPopup = 0;

	// id блока модели при перезаписи
	mutable int modIdToPopup = 0;

	// Если вызвано модальное окно подтверждения
	bool isPopup = false;
	/**************/
private:
	// Отрисовка nodes
	void RenderNodes();

	// Добавить камера node
	void AddCameraNode(int id, std::string name);

	// Добавить модель node
	void AddModelNode(int id, std::string name);

	// Показать модели *Левая панель*
	void ShowLeftPanel(ImVec2 size);

	// Показать модели *Правая панель*
	void ShowRightPanel(ImVec2 size);

	CameraNode* FindCamNodeById(int id);

	ModelNode* FindModNodeById(int id);

	void ConncetCam2Model(int cam_id, int mod_id);
private:
	// Лог
	AppLog* applog;
};