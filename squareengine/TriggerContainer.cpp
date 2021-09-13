#include "TriggerContainer.h"

#if IS_ENGINE_MODE
#include "imgui\imgui.h"
#endif // IS_ENGINE_MODE

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"
#include "EngineFunctions.hpp"

#if IS_ENGINE_MODE
TriggerContainer::TriggerContainer(std::string path, Graphics& gfx, Rgph::RenderGraph& rg, AppLog* aLog)
	:
	path(path),
	applog(aLog),
	gfx(gfx),
	rg(rg)
{
	applog->AddLog(TRIGGER_LOG, "�������������\n");

	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � ��������� ����");
	}

	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 orientation;

	json j;
	dataFile >> j;

	for (json::iterator t = j.begin(); t != j.end(); ++t)
	{
		auto &d = t.key();

		for (const auto& obj : j.at(d))
		{
			/* ������ ��������� �������� */

			position.x = obj.at("pos-x");
			position.y = obj.at("pos-y");
			position.z = obj.at("pos-z");

			orientation.x = obj.at("roll");
			orientation.y = obj.at("pitch");
			orientation.z = obj.at("yaw");

			/***************************/

			/* ������ ������� �������� */
			
			size.x = obj.at("width");
			size.y = obj.at("height");

			/***************************/

			/* ������ ��������� � ����� �������� */

			std::string ptr = obj.at("ptr");

			std::string name = obj.at("name");

			/*****************************/ 

			triggers.emplace(
				ptr, 
				std::make_unique<Trigger>
				(
					name,
					gfx,
					size,
					position,
					orientation
				)
			);
		}
	}

	dataFile.close();
}
#else
TriggerContainer::TriggerContainer(std::string path)
	:
	path(path)
{
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � ��������� ����");
	}

	DirectX::XMFLOAT3 pos_lt;
	DirectX::XMFLOAT3 pos_rt;
	DirectX::XMFLOAT3 pos_lb;
	DirectX::XMFLOAT3 pos_rb;

	float roll;
	float pitch;
	float yaw;

	json j;
	dataFile >> j;

	for (json::iterator t = j.begin(); t != j.end(); ++t)
	{
		auto d = t.key();

		for (const auto& obj : j.at(d))
		{
			/* ������ ������� �������� */

			for (const auto& pos : obj.at("pos-lt"))
			{
				pos_lt = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}

			for (const auto& pos : obj.at("pos-rt"))
			{
				pos_rt = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}

			for (const auto& pos : obj.at("pos-lb"))
			{
				pos_lb = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}

			for (const auto& pos : obj.at("pos-rb"))
			{
				pos_rb = { pos.at("pos-x"), pos.at("pos-y"), pos.at("pos-z") };
			}


			roll = obj.at("roll");
			pitch = obj.at("pitch");
			yaw = obj.at("yaw");

			/***************************/

			/* ������ ��������� �������� */

			std::string ptr = obj.at("ptr");

			/*****************************/

			triggers.emplace(
				Ptr,
				std::make_unique<Trigger>
				(
					Name,
					TriggerStruct(pos_lt, pos_rt, pos_lb, pos_rb, roll, pitch, yaw)
				)
			);
		}
	}

	dataFile.close();
}
#endif // IS_ENGINE_MODE

TriggerContainer::~TriggerContainer()
{}

#if IS_ENGINE_MODE
void TriggerContainer::LinkTechniques()
{
	for (auto it = triggers.begin(); it != triggers.end(); ++it)
	{
		it->second->LinkTechniques(rg);
		it->second->SetDefault();

#if IS_ENGINE_MODE
		std::ostringstream oss;
		oss << "��������� � ������� [" << std::string(it->first) << "]\n";

		applog->AddLog(TRIGGER_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
	}
}

void TriggerContainer::Submit(size_t channels)
{
	for (auto it = triggers.begin(); it != triggers.end(); ++it)
	{
		it->second->Submit(channels);
	}
}
#endif // IS_ENGINE_MODE

std::pair<std::string, bool> TriggerContainer::Check(DirectX::XMFLOAT3 position)
{
	for (auto it = triggers.begin(); it != triggers.end(); it++)
	{
		if (it->second->Check(position))
		{
			auto HittedTriggerGoal = it->first;

#if IS_ENGINE_MODE
			std::ostringstream oss;
			oss << "������� [" << static_cast<std::string>(HittedTriggerGoal) << "]\n";

			applog->AddLog(TRIGGER_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE

			return std::make_pair(HittedTriggerGoal, true);
		}
	}

	return std::make_pair("NULL", false);
}

#if IS_ENGINE_MODE
void TriggerContainer::ShowLeftPanel()
{
	bool IsAdd = false;

	if (ImGui::Begin("��������", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& [key, value] : triggers)
		{
			char label[128];

			sprintf_s(label, value->GetName().c_str(), selected.c_str());

			ImGui::Bullet();
			if (ImGui::Selectable(label, selected == value->GetName().c_str()))
			{
				selected = std::move(value->GetName());
			}
		}

		if (ImGui::Button("��������"))
		{
			ImGui::OpenPopup("�������� ��������");
		}

		if (ImGui::BeginPopupModal("�������� ��������", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("������� ��������� ��������");
			ImGui::Separator();

			ImGui::InputText("���", name, sizeof(name));

			ImGui::InputText("����", goal, sizeof(goal));

			ImGui::InputFloat("������", &new_w, 1.0f, 30.0f, "%.2f");
			ImGui::InputFloat("������", &new_h, 1.0f, 30.0f, "%.2f");
			
			if (ImGui::Button("��������", ImVec2(120, 0)))
			{
				//AddTrigger(gfx, std::string(name), std::string(goal), trs, rg);

				IsAdd = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("������", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void TriggerContainer::ShowRightPanel()
{
	if (ImGui::Begin("����������", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (const auto& [key, value] : triggers)
		{
			if (value->GetName() == selected)
			{
				auto pos = value->GetPosition();

				value->SpawnControl();
				
				ImGui::NewLine();

				ImGui::Separator();
				ImGui::Text("������� ��������� ��: %s", key);
				
				ImGui::EndChild();
				
				break;
			}
		}
	}

	ImGui::End();
}

void TriggerContainer::AddTrigger(std::string name, std::string ptr)
{
	using std::to_string;

	// �������� ����� � ����������
	std::ifstream dataFile(path);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � ��������� ����");
	}

	// ������ �����
	json j;
	dataFile >> j;

	// �������� �����
	dataFile.close();

	// ����� ��������� � ����� + 1
	int triggerIndex = triggers.size() + 1;

	// ����� �������
	std::ostringstream newTrigger;
	/*newTrigger << "\"Trigger " << triggerIndex;
	
	newTrigger << "\": [{\"name\": \"" + name + "\",";

	newTrigger << "\"pos-lt\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosTopLeft.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosTopLeft.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosTopLeft.z << "}],";

	newTrigger << "\"pos-rt\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosTopRight.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosTopRight.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosTopRight.z << "}],";

	newTrigger << "\"pos-lb\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosBottomLeft.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosBottomLeft.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosBottomLeft.z << "}],";
	
	newTrigger << "\"pos-rb\" :";
	newTrigger << "[{\"pos-x\": " << trs.PosBottomRight.x << ",";
	newTrigger << "\"pos-y\" : " << trs.PosBottomRight.y << ",";
	newTrigger << "\"pos-z\" : " << trs.PosBottomRight.z << "}],";

	newTrigger << "\"ptr\": \"" << ptr << "\",";

	newTrigger << "\"roll\": " << trs.Roll << ",";
	newTrigger << "\"pitch\": " << trs.Pitch << ",";
	newTrigger << "\"yaw\": " << trs.Yaw << "}]";*/

	// ���������� � ������� � ����
	std::string json_str = j.dump();
	size_t pos_of_par = json_str.find_last_of('}');
	size_t pos_of_par2 = json_str.find_last_of(']');

	json_str.at(pos_of_par) = ' ';
	json_str.at(pos_of_par2 + 1) = ',';

	// ������ � ���� ������ ��������
	std::ofstream ostr(path);
	ostr << json_str << newTrigger.str() << '}';

	// �������� �����
	ostr.close();

	//trig_sc_container.emplace(ptr, std::make_unique<Trigger>(name, trs, gfx));

	for (auto& [key, value] : triggers)
	{
		if (value->GetName() == name)
		{
			value->LinkTechniques(rg);
			value->SetDefault();

#if IS_ENGINE_MODE
			std::ostringstream oss;
			oss << "��������� � ������� [" << name << "]\n";

			applog->AddLog(TRIGGER_LOG, oss.str().c_str());
#endif // IS_ENGINE_MODE
		}
	}
}
#endif // IS_ENGINE_MODE