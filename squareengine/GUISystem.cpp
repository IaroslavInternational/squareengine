#include "GUISystem.h"
#include <sstream>

GUISystem::GUISystem(std::vector<std::unique_ptr<Person>>* pv_ptr)
	:
	pv_ptr(pv_ptr)
{
	SetGUIColors();

	auto GPU_Data = AdapterReader::GetAdapterData();

	for (auto& d : GPU_Data)
	{
		gpu_desc.emplace(std::wstring(d.desc.Description), round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
	}
}

void GUISystem::Show()
{
	ShowMenu();
	ShowLeftSide();
	ShowRightSide();
	ShowLeftBottomSide();
	ShowBottomPanel();

	ImGui::SetNextWindowPos({ ImGui::GetMousePos().x, ImGui::GetMousePos().y + 30 }, ImGuiCond_Always);
	if (ImGui::Begin("mouse coord", (bool*)0,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
		| ImGuiWindowFlags_NoBackground))
	{
		std::ostringstream pos_str;
		pos_str << "x: " << ImGui::GetMousePos().x
			<< "\ny: " << ImGui::GetMousePos().y;

		ImGui::Text(pos_str.str().c_str());

		ImGui::End();
	}
}

void GUISystem::Hide()
{
	DisableSides();

	ShowHardwareInfo = false;
	ShowLogs = false;
}

void GUISystem::AddLog(const char* text)
{
	applog.AddLog(text);
}

void GUISystem::SetGUIColors()
{
	/*����� ����������*/

	ImGui::GetStyle().FrameRounding = 4.0f;									// ����������� �����������
	ImGui::GetStyle().WindowBorderSize = 0.0f;								// ������ �������
	ImGui::GetStyle().WindowRounding = 0.0f;								// ����������� ����

	// �����
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.36f, 0.39f, 1.00f);		// ������� ����
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.84f);			// ��� ����
	colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.00f, 0.20f, 0.73f);			// ���� ����
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.00f, 0.07f, 0.73f);	// ��������� �� ���� ����
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.50f, 0.38f, 0.54f);			// ����������
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.18f, 0.15f, 0.40f);	// ��������� �� ����������
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.48f, 0.45f, 0.67f);	// �������� ����������
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);		// �������
	colors[ImGuiCol_SliderGrab] = ImVec4(0.37f, 0.70f, 0.00f, 1.00f);		// �������� ��������
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.82f, 0.19f, 1.00f);	// �������� �������� ��������
	colors[ImGuiCol_Button] = ImVec4(0.56f, 0.05f, 0.05f, 0.59f);			// ������
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.01f, 0.17f, 1.00f);	// ��������� �� ������
	colors[ImGuiCol_ButtonActive] = ImVec4(0.03f, 0.55f, 0.48f, 1.00f);		// �������� ������
	colors[ImGuiCol_Separator] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// �����������
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.08f, 0.27f, 0.86f);				// ������
	colors[ImGuiCol_TabHovered] = ImVec4(0.01f, 0.43f, 0.63f, 0.80f);		// ��������� �� ������
	colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// �������� ������
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.19f, 0.67f, 0.65f, 1.00f);	// ��������
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.31f, 0.04f, 0.04f, 0.81f);	// ���� ��������� �������
	colors[ImGuiCol_Header] = ImVec4(0.50f, 0.09f, 0.70f, 0.31f);			// ���������
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.57f, 0.49f, 1.00f);		// �������� ���������
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.22f, 0.59f, 0.80f);	// ��������� �� ��������

	/******************/
}

void GUISystem::SetPanelWidthAndPosition(int corner, float width, float height, float x_offset, float y_offset)
{
	ImGuiIO& io = ImGui::GetIO();

	float MenuHeight = ImGui::GetMenuHeight();
	ImVec2 DispSize = io.DisplaySize;

	float PanelW = round(DispSize.x * width);
	float PanelH = DispSize.y * height;

	ImVec2 PanelSize = ImVec2(
		PanelW,
		PanelH
	);

	ImVec2 PanelPos = ImVec2(
		(corner & 1) ? DispSize.x + round(DispSize.x * x_offset) : round(DispSize.x * x_offset),
		(corner & 2) ? DispSize.y + MenuHeight + DispSize.y * y_offset : MenuHeight + DispSize.y * y_offset
	);

	ImVec2 PanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(PanelPos, ImGuiCond_Always, PanelPivot);
	ImGui::SetNextWindowSize(PanelSize);
}

void GUISystem::ShowMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("����"))
		{
			if (ImGui::MenuItem("�����"))
			{
				exit(0);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("����"))
		{
			if (ImGui::BeginMenu("�������"))
			{
				if (ImGui::MenuItem("���������"))
				{
					if (ShowPersonEnum && ShowPersonSettings)
					{
						DisableSides();

						ShowPersonEnum = false;
						ShowPersonSettings = false;
					}
					else
					{
						DisableSides();

						ShowPersonEnum = true;
						ShowPersonSettings = true;
					}
				}

				if (ImGui::MenuItem("��������"))
				{
					if (ShowTriggersList && ShowTriggersSettings)
					{
						DisableSides();

						ShowTriggersList = false;
						ShowTriggersSettings = false;
					}
					else
					{
						DisableSides();

						ShowTriggersList = true;
						ShowTriggersSettings = true;
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("FPS & GPU"))
			{
				ShowHardwareInfo ? ShowHardwareInfo = false : ShowHardwareInfo = true;
			}

			if (ImGui::MenuItem("���"))
			{
				ShowLogs ? ShowLogs = false : ShowLogs = true;
			}

			if (ImGui::MenuItem("��������� ������"))
			{
				Hide();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void GUISystem::ShowLeftSide()
{
	/* ����� ������� */

	SetPanelWidthAndPosition(0, 0.2f, 0.75f);

	/* ���������� */

	if (ShowPersonEnum)
	{
		ShowPersonList();
	}
	else if (ShowTriggersList)
	{
		//objects.triggers.ShowLeftPanel();
	}

	/**************/

	ImGui::GetStyle().DisplayWindowPadding = { 0.0f, 0.0f };
	ImGui::GetStyle().DisplaySafeAreaPadding = { 0.0f, 0.0f };

	/* ����� ����� ������� */
}

void GUISystem::ShowRightSide()
{
	/* ������ ������� */

	ImGuiIO& io = ImGui::GetIO();

	int corner = 1;

	float MenuHeight = ImGui::GetMenuHeight();
	float RightPanelW = io.DisplaySize.x * 0.2f;

	ImVec2 RightPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	SetPanelWidthAndPosition(corner, 0.2f, 1.0f);

	/* ���������� */

	if (ShowPersonEnum)
	{
		ShowPersonControl();

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowTriggersSettings)
	{
		//objects.triggers.ShowRightPanel();

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f }, ImGuiCond_FirstUseEver);
		//ShowTrigCheck();
	}

	/**************/

	/* ����� ������ ������� */
}

void GUISystem::ShowLeftBottomSide()
{
	/* ����� ������ ������� */

	SetPanelWidthAndPosition(2, 0.2f, 0.25f);

	/* ���������� */

	if (ShowHardwareInfo)
	{
		ShowFPSAndGPU();
	}

	/**************/

	/* ����� ����� ������ ������� */
}

void GUISystem::ShowBottomPanel()
{
	/* ������ ������� */

	SetPanelWidthAndPosition(3, 0.6f, 0.25f, -0.2f);

	/* ���������� */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* ����� ������ ������� */
}

void GUISystem::DisableSides()
{
	ShowPersonEnum = false;
	ShowPersonSettings = false;
	ShowTriggersList = false;
	ShowTriggersSettings = false;
}

void GUISystem::ShowFPSAndGPU()
{
	if (ImGui::Begin("�������������", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text("FPS:");
		ImGui::Text("%.3f ��/���� (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Separator();

		ImGui::Text("����������� ������������:");
		for (auto& d : gpu_desc)
		{
			char name_gpu[256];
			sprintf_s(name_gpu, "%ws", reinterpret_cast<const char*>(d.first.c_str()));

			if (ImGui::TreeNode(name_gpu))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(133.0f / 255.0f, 219.0f / 255.0f, 15.0f / 255, 1.0f));
				ImGui::Text("������: ~%.1f ��", d.second);
				ImGui::PopStyleColor();
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowLog()
{
	ImGui::Begin("���", NULL, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("���", NULL);
	ImGui::End();
}

void GUISystem::ShowPersonList()
{
	if (ImGui::Begin("���������", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto p = pv_ptr->begin(); p != pv_ptr->end(); p++)
		{
			char label[128];
			sprintf_s(label, p->get()->GetName().c_str(), personSelected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, personSelected == p->get()->GetName().c_str()))
			{
				personSelected = p->get()->GetName();
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("��������"))
		{

		}
	}

	ImGui::End();
}

void GUISystem::ShowPersonControl()
{
	if (ImGui::Begin("�����", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < pv_ptr->size(); k++)
		{
			if (pv_ptr->at(k)->GetName() == personSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					bool posDirty = false;
					bool effDirty = false;

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

					ImGui::Text("�������:");
					dcheck(ImGui::SliderFloat("X", &pv_ptr->at(k)->GetPositionPtr()->x, -1000.0f, 1000.0f, "%.1f"), posDirty);
					dcheck(ImGui::SliderFloat("Y", &pv_ptr->at(k)->GetPositionPtr()->y, -1000.0f, 1000.0f, "%.1f"), posDirty);

					ImGui::Text("������:");
					dcheck(ImGui::SliderFloat("������������.", pv_ptr->at(k)->GetEffectDuration(), 0.0f, 100.0f, "%.1f"), effDirty);
					dcheck(ImGui::SliderFloat("�����", pv_ptr->at(k)->GetEffectTime(), 0.0f, 100.0f, "%.1f"), effDirty);
					
					ImGui::Checkbox("�������", pv_ptr->at(k)->GetEffectActive());

					if (ImGui::Button("�������", ImVec2(100, 20)))
					{
						//IsDelete = true;
					}

					ImGui::SameLine();

					if (ImGui::Button("���������", ImVec2(100, 20)))
					{
						//IsSave = true;
					}

					ImGui::EndChild();
				}

				break;
			}
		}
	}

	ImGui::End();
}
