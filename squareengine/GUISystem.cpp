#include "GUISystem.h"

#include "imgui/implot.h"
#include "EngineFunctions.hpp"
#include "HitBox.h"
#include "Line.h"

GUISystem::GUISystem(std::shared_ptr<Window>				 wnd,
					 MainPerson*							 hero,
					 PersonContainer*						 persCon,
					 InteractableObject2DContainer*			 Iobj,
					 ObjectsQueue*							 objectsPtr,
					 std::shared_ptr<Physics::PhysicsEngine> phEngPtr)
	:
	wnd(wnd),
	hero(hero),
	persCon(persCon),
	Iobj(Iobj),
	objectsPtr(objectsPtr),
	phEngPtr(phEngPtr)
{
	SetGUIColors();

	for (size_t i = 0; i < N_POINTS; i++)
	{
		counters[i] = float(i);
	}

	auto GPU_Data = AdapterReader::GetAdapterData();

	for (auto& d : GPU_Data)
	{
		if (d.desc.DedicatedVideoMemory != 0)
		{
			gpu_desc.emplace(std::wstring(d.desc.Description), round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
		}
	}
}

/* ������� ������ ��� ��������� ���������� */

void GUISystem::Show()
{
	ShowMenu();
	ShowLeftSide();
	ShowRightSide();
	ShowLeftBottomSide();
	ShowBottomPanel();
	ShowOptionalPanel();
}

void GUISystem::Hide()
{
	DisableSides();

	ShowPhysicsSettings = false;
	ShowLogs = false;
}

void GUISystem::AddLog(const char* text)
{
	applog.AddLog(text);
}

/*******************************************/

/* ������ ��������� � ��������� ������� */

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

void GUISystem::SetPanelSizeAndPosition(int corner, float width, float height, float x_offset, float y_offset)
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
				if (ImGui::MenuItem("�������"))
				{
				}

				if (ImGui::BeginMenu("������"))
				{
					if (ImGui::MenuItem("�������"))
					{
						if (ShowPhysicsEngineObjEnum && ShowPhysicsEngineObjSettings)
						{
							DisableSides();

							ShowPhysicsEngineObjEnum = false;
							ShowPhysicsEngineObjSettings = false;
						}
						else
						{
							DisableSides();

							ShowPhysicsEngineObjEnum = true;
							ShowPhysicsEngineObjSettings = true;
						}
					}
					
					if (ImGui::MenuItem("���������"))
					{
						ShowPhysicsSettings ? ShowPhysicsSettings = false : ShowPhysicsSettings = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("����"))
				{
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("�������"))
			{
				if (ImGui::MenuItem("������� ��������"))
				{
					if (ShowMainPersonEnum && ShowMainPersonSettings)
					{
						DisableSides();

						ShowMainPersonEnum = false;
						ShowMainPersonSettings = false;
					}
					else
					{
						DisableSides();

						ShowMainPersonEnum = true;
						ShowMainPersonSettings = true;
					}
				}

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

			if (ImGui::MenuItem("GPU"))
			{
				ShowHardwareInfo ? ShowHardwareInfo = false : ShowHardwareInfo = true;
			}

			if (ImGui::MenuItem("������ FPS"))
			{
				ShowFPSGraph ? ShowFPSGraph = false : ShowFPSGraph = true;
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

	SetPanelSizeAndPosition(0, 0.2f, 0.75f);

	/* ���������� */

	if (ShowPersonEnum)
	{
		ShowPersonList();
	}
	else if (ShowMainPersonEnum)
	{
		ShowMainPersonList();
	}
	else if (ShowPhysicsEngineObjEnum)
	{
		ShowPhysicsEngineObjList();
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

	SetPanelSizeAndPosition(corner, 0.2f, 1.0f);

	/* ���������� */

	if (ShowPersonEnum)
	{
		ShowPersonControl();

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowMainPersonSettings)
	{
		ShowMainPersonControl();

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowPhysicsEngineObjSettings)
	{
		ShowPhysicsEngineObjControl();

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

	SetPanelSizeAndPosition(2, 0.2f, 0.25f);

	/* ���������� */

	if (ShowHardwareInfo)
	{
		ShowGPU();
	}

	/**************/

	/* ����� ����� ������ ������� */
}

void GUISystem::ShowBottomPanel()
{
	/* ������ ������� */

	SetPanelSizeAndPosition(3, 0.6f, 0.25f, -0.2f);

	/* ���������� */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* ����� ������ ������� */
}

void GUISystem::ShowOptionalPanel()
{
	if (ShowPhysicsSettings)
	{	
		SetPanelSizeAndPosition(0, 0.80f, 0.80f, 0.1f, 0.1f);	
		ShowPhysicsEngineSettings();
	}
	else if (ShowPhysicsEngineObjInfo)
	{
		ShowPhysicsEngineObjHelp();
	}

	if (ShowFPSGraph)
	{
		ShowFPS();
	}

	if (ShowObjectsSettings)
	{
		ShowObjectsControl();
	}

	if (mouseHelpInfo == "")
	{
		std::ostringstream pos;
		pos << "x: "   << ImGui::GetMousePos().x
			<< "\ny: " << ImGui::GetMousePos().y;

		ShowMouseHelperPanel(pos.str().c_str());
	}
	else
	{
		ShowMouseHelperPanel(mouseHelpInfo);
	}
}

void GUISystem::ShowMouseHelperPanel(std::string info)
{
	bool IsBlending = false;
	if (ImGui::GetStyle().Alpha == 0.1f)
	{
		ImGui::GetStyle().Alpha = 1.0f;
		IsBlending = true;
	}

	ImGui::SetNextWindowPos({ ImGui::GetMousePos().x, ImGui::GetMousePos().y + 30 }, ImGuiCond_Always);
	if (ImGui::Begin("mouse info helper", (bool*)0,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
		| ImGuiWindowFlags_NoBackground))
	{
		ImGui::Text(info.c_str());
		ImGui::End();
	}

	if (IsBlending)
	{
		ImGui::GetStyle().Alpha = 0.1f;
	}
}

void GUISystem::DisableSides()
{
	ShowPersonEnum = false;
	ShowPersonSettings = false;
	ShowMainPersonEnum = false;
	ShowMainPersonSettings = false;
	ShowPhysicsEngineObjEnum = false;
	ShowPhysicsEngineObjSettings = false;
	ShowTriggersList = false;
	ShowTriggersSettings = false;
	ShowPhysicsSettings = false;
	ShowFPSGraph = false;
}

/****************************************/

/* ������ ��������� ���������� ����������� */

void GUISystem::ShowLog()
{
	ImGui::Begin("���", NULL, 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("���", NULL);
	ImGui::End();
}

void GUISystem::ShowGPU()
{
	if (ImGui::Begin("�������������", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text("%.3f ��/���� (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		std::ostringstream s;
		s << sizeof(*this);

		ImGui::Text("��� GUI: %s ����", s.str().c_str());

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

void GUISystem::ShowFPS()
{
	if (ImGui::Begin("������ FPS", &ShowFPSGraph))
	{
		if (counter != N_POINTS)
		{
			arr[counter] = ImGui::GetIO().Framerate;
			counter++;
		}
		else
		{
			counter = 0;
			for (size_t i = 0; i < N_POINTS; i++)
			{
				sum += (double)arr[i];
			}

			average = sum / N_POINTS;
			sum = 0.0;

			pYMin = average - 1.0;
			pYMax = average + 1.0;
		}

		ImPlot::LinkNextPlotLimits(&pXMin, &pXMax, &pYMin, &pYMax);
		if (ImPlot::BeginPlot("FPS", "", "FPS"))
		{
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
			ImPlot::PlotShaded("FPS", counters, arr, N_POINTS);
			ImPlot::PopStyleVar();
			ImPlot::PlotLine("", counters, arr, N_POINTS);

			ImPlot::EndPlot();
		}
	}

	ImGui::End();
}

void GUISystem::ShowPhysicsEngineObjHelp()
{
	std::string objName;
	ImVec2 pos;

	for (auto& l : phEngPtr->lines)
	{
		std::ostringstream oss;
		oss << "Physics engine info for " << l.name;

		float offset = -30.0f;

		if (l.start.y == l.end.y)
		{
			pos = { (l.start.x + l.end.x) / 2.0f, l.start.y + offset };
		}
		else if (l.start.y != l.end.y)
		{
			pos = { (l.start.x + l.end.x) / 2.0f, (l.start.y + l.end.y) / 2.0f + offset };
		}

		ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
		if (ImGui::Begin(oss.str().c_str(), (bool*)0,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoBackground))
		{
			ImGui::Text(l.name.c_str());
			ImGui::End();
		}
	}

	for (auto& hb : phEngPtr->hitboxes)
	{
		std::ostringstream oss;
		oss << "Physics engine info for " << hb.name;

		float offset = 7.0f;
		ImVec2 pos;

		pos = { hb.coordinates.x + offset, hb.coordinates.y + offset };

		ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
		if (ImGui::Begin(oss.str().c_str(), (bool*)0,
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoBackground))
		{
			ImGui::Text(hb.name.c_str());
			ImGui::End();
		}
	}
}

void GUISystem::ShowPhysicsEngineSettings()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 0.95f));
	if (ImGui::Begin("��������� ����������� ������", &ShowPhysicsSettings,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		using namespace Physics;

		if (ImGui::CollapsingHeader("����� ����������", ImGuiTreeNodeFlags_DefaultOpen))
		{

			ImGui::TextWrapped(
				"���������� ������ ��������� ����������� �������� �������� � �� ���������� ���������. "
				"��������� ����������� ��� �������� �������� ������� ����� � �������������� (Hit-Box).\n");
			ImGui::Text("���������� ����� ������ ������� � ������ %d ���� (%.1f ��������).\n", sizeof(phEngPtr->lines) + sizeof(Line) * phEngPtr->lines.size(), float(sizeof(phEngPtr->lines) + sizeof(Line) * phEngPtr->lines.size()) / (1024.0f));
			ImGui::Text("���������� ����� ������ ������� � ��������������� %d ���� (%.1f ��������).\n", sizeof(phEngPtr->hitboxes) + sizeof(HitBox) * phEngPtr->hitboxes.size(), float(sizeof(phEngPtr->hitboxes) + sizeof(HitBox) * phEngPtr->hitboxes.size()) / (1024.0f));

			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("�����"))
		{
			ImGui::ColorEdit3("���� �����", phEngPtr->lineColor);
			ImGui::ColorEdit3("���� ���������������", phEngPtr->hbColor);

			if (ImGui::Button("���������"))
			{
				EngineFunctions::SetNewValue<float>(
					"settings",
					"l-clr-r",
					phEngPtr->lineColor[0],
					phEngPtr->dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"l-clr-g",
					phEngPtr->lineColor[1],
					phEngPtr->dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"l-clr-b",
					phEngPtr->lineColor[2],
					phEngPtr->dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"h-clr-r",
					phEngPtr->hbColor[0],
					phEngPtr->dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"h-clr-g",
					phEngPtr->hbColor[1],
					phEngPtr->dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"h-clr-b",
					phEngPtr->hbColor[2],
					phEngPtr->dataPath,
					&applog
					);
			}

			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("��������� �������� � ��������"))
		{
			ImGui::SliderFloat("������ ����� ���������� ��������", &phEngPtr->deltaCollision, -20.0f, 20.0f);
			ImGui::SliderFloat("������� ��������", &sq_l, 0.0f, 100.0f);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("����� ������� ��������, ���������� �� Hit-Box");
			}

			ImGui::Checkbox("�������� ��� �������", &phEngPtr->objVisability);
			ImGui::Checkbox("�������� ������� ��������", &ShowPhysicsEngineObjInfo);
			if (ImGui::Button("���������"))
			{
				EngineFunctions::SetNewValue<float>(
					"settings",
					"delta-collision",
					phEngPtr->deltaCollision,
					phEngPtr->dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<bool>(
					"settings",
					"obj-vis",
					phEngPtr->objVisability,
					phEngPtr->dataPath,
					&applog
					);
			}

			ImGui::Separator();
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

void GUISystem::ShowPersonList()
{
	if (ImGui::Begin("���������", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto p = persCon->persons.begin(); p != persCon->persons.end(); p++)
		{
			char label[128];
			sprintf_s(label, p->get()->name.c_str(), personSelected);

			std::string contextMenuId = "Context Menu for " + p->get()->name;

			ImGui::Bullet();
			if (ImGui::Selectable(label, personSelected == p->get()->name.c_str()))
			{
				personSelected = p->get()->name;
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("�������"))
				{
					std::string deletedPersonName = p->get()->name;
					AddLog("�������� ");
					AddLog(deletedPersonName.c_str());
					AddLog("...\n");

					persCon->DeletePersonAt(p);
					EngineFunctions::DeleteJsonObject(deletedPersonName, persCon->dataPath);

					AddLog("�������� ");
					AddLog(deletedPersonName.c_str());
					AddLog(" �����\n");

					ImGui::EndPopup();

					break;
				}

				ImGui::EndPopup();
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
		// ���� �� ����������
		for (int k = 0; k < persCon->persons.size(); k++)
		{
			// ����� ���������� ���������
			if (persCon->persons.at(k)->name == personSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					/* ���������� ���������� ������� ���������� */

					bool posDirty   = false;	// �������� �������
					bool effDirty   = false;	// �������� �������
					bool speedDirty = false;	// �������� ��������

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // ���������

					/********************************************/

					/* �������� ���������� �������� � �������� ��������� */

					if (ImGui::CollapsingHeader("���������", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("�������:");
						dcheck(ImGui::SliderFloat("X", &persCon->persons.at(k)->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
						dcheck(ImGui::SliderFloat("Y", &persCon->persons.at(k)->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

						ImGui::Text("��������:");
						dcheck(ImGui::SliderFloat("", &persCon->persons.at(k)->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);
					
						ImGui::Separator(); // �����������
					}

					/******************************************/

					/* �������� ���������� �������� ��������� */

					if (ImGui::CollapsingHeader("�������"))
					{						
						dcheck(ImGui::SliderFloat("������������.", &persCon->persons.at(k)->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
						dcheck(ImGui::SliderFloat("�����", &persCon->persons.at(k)->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

						ImGui::Checkbox("�������", &persCon->persons.at(k)->effect.Active);

						ImGui::Separator();	// �����������
					}

					/******************************************/

					/* �������� ���������� ��������� ��������� */

					if (ImGui::CollapsingHeader("Hit-box"))
					{
						ImGui::Checkbox("��������", &persCon->persons.at(k)->hitbox_visability);

						/* ���� ������ ������ �������� HitBox */
						{
							if (ImGui::Button("��������", ImVec2(100, 20)))
							{
								AddLog("��������� Hit-box ���:");
								AddLog(personSelected.c_str());
								AddLog("\n");

								DrawingHitBox = true;
								persCon->persons.at(k)->hitbox_visability = false;

								ImGui::GetStyle().Alpha = 0.1f;
							}

							if (DrawingHitBox)
							{
								if (!SettedFirstPoint)
								{
									mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

									if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
									{
										auto pos = wnd->mouse.GetPos();
										firstPoint = { (float)pos.first, (float)pos.second };

										SettedFirstPoint = true;
										std::ostringstream oss;
										oss << "���������� ������ �����:\n" <<
											"[x: " << firstPoint.x <<
											"; y: " << firstPoint.y << "]\n";

										AddLog(oss.str().c_str());
									}
								}
								else if (!SettedSecondPoint)
								{
									mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

									int ms_posX = wnd->mouse.GetPosX();
									int ms_posY = wnd->mouse.GetPosY();

									HitBox hb(std::string("Drown hitbox"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
									wnd->Gfx().DrawHitBox(hb);

									if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
									{
										auto pos = wnd->mouse.GetPos();
										secondPoint = { (float)pos.first, (float)pos.second };

										HitBox hb_new(persCon->persons.at(k)->name + std::string(" hitbox"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
										persCon->persons.at(k)->SetHitBox(hb);
										persCon->persons.at(k)->hitbox_visability = true;

										std::ostringstream oss;
										oss << "���������� ������ �����:\n" <<
											"[x: " << secondPoint.x <<
											"; y: " << secondPoint.y << "]\n";

										AddLog(oss.str().c_str());

										AddLog("���������� Hit-box:\n");

										auto actual_hb = persCon->persons.at(k)->hitbox;

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-ltx", actual_hb.GetCoordinates().x,
											persCon->dataPath,
											&applog
											);

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-lty", actual_hb.GetCoordinates().y,
											persCon->dataPath,
											&applog
											);

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-rbx", actual_hb.GetCoordinates().z,
											persCon->dataPath,
											&applog
											);

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-rby", actual_hb.GetCoordinates().w,
											persCon->dataPath,
											&applog
											);

										mouseHelpInfo = "";
										SettedSecondPoint = true;
									}
								}
								else if (SettedFirstPoint && SettedSecondPoint)
								{
									DrawingHitBox = false;
									SettedFirstPoint = false;
									SettedSecondPoint = false;

									ImGui::GetStyle().Alpha = 1.0f;
								}
							}
						}
						/**************************************/

						ImGui::Separator(); // �����������
					}
					/*******************************************/

					ImGui::NewLine();
					ImGui::NewLine();

					/* ���� ������ ������ ��������� ������� ��������� ��������� */
					{
						if (ImGui::Button("���������", ImVec2(100, 20)))
						{
							AddLog("���������� �������� ���: ");
							AddLog(personSelected.c_str());
							AddLog("\n");

							SavingSettings = true;
						}

						if (SavingSettings)
						{
							/* ���������� ������� � �������� */
							
							EngineFunctions::SetNewValue<float>(
								personSelected,
								"pos-x", persCon->persons.at(k)->position.x,
								persCon->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"pos-y", persCon->persons.at(k)->position.y,
								persCon->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"speed", persCon->persons.at(k)->speed,
								persCon->dataPath,
								&applog
								);

							/**********************/

							/* ���������� �������� ������� */
							
							EngineFunctions::SetNewValue<bool>(
								personSelected,
								"eff-a", persCon->persons.at(k)->effect.Active,
								persCon->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"eff-d", persCon->persons.at(k)->effect.Duration,
								persCon->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"eff-t", persCon->persons.at(k)->effect.Time,
								persCon->dataPath,
								&applog
								);

							/*******************************/

							SavingSettings = false;
						}
					}
					/************************************************************/

					ImGui::SameLine();

					/* ���� ������ ������ ������� ��������� */
					{
						if (ImGui::Button("�������", ImVec2(100, 20)))
						{
							for (auto i = persCon->persons.begin(); i != persCon->persons.end(); i++)
							{
								if (i->get()->name == personSelected)
								{
									persCon->persons.erase(i);
									break;
								}
							}
							
							EngineFunctions::DeleteJsonObject(personSelected, persCon->dataPath);
						}
					}
					/****************************************/

					ImGui::EndChild();
				}

				break;
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowMainPersonList()
{
	if (ImGui::Begin("���������", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text(hero->name.c_str());
	}

	ImGui::End();
}

void GUISystem::ShowMainPersonControl()
{
	if (ImGui::Begin("�����", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginChild(""))
		{
			/* ���������� ���������� ������� ���������� */

			bool posDirty = false;		// �������� �������
			bool effDirty = false;		// �������� �������
			bool speedDirty = false;	// �������� ��������

			const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // ���������

			/********************************************/

			/* �������� ���������� �������� � �������� �������� ��������� */

			if (ImGui::CollapsingHeader("���������", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("�������:");
				dcheck(ImGui::SliderFloat("X", &hero->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
				dcheck(ImGui::SliderFloat("Y", &hero->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

				ImGui::Text("��������:");
				dcheck(ImGui::SliderFloat("", &hero->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);

				ImGui::Separator();	// �����������
			}
			
			/**************************************************************/

			/* �������� ���������� �������� �������� ��������� */

			if (ImGui::CollapsingHeader("�������"))
			{
				dcheck(ImGui::SliderFloat("������������.", &hero->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
				dcheck(ImGui::SliderFloat("�����", &hero->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

				ImGui::Checkbox("�������", &hero->effect.Active);

				ImGui::Separator();	// �����������
			}

			/***************************************************/

			/* �������� ���������� ��������� �������� ��������� */
			
			if (ImGui::CollapsingHeader("Hit-box"))
			{
				ImGui::Checkbox("��������", &hero->hitbox_visability);

				/* ���� ������ ������ �������� HitBox */
				{
					if (ImGui::Button("��������", ImVec2(100, 20)))
					{
						AddLog("��������� Hit-box ���: ");
						AddLog(hero->name.c_str());
						AddLog("\n");

						DrawingHitBox = true;
						hero->hitbox_visability = false;

						ImGui::GetStyle().Alpha = 0.1f;
					}

					if (DrawingHitBox)
					{
						if (!SettedFirstPoint)
						{
							mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

							if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								firstPoint = { (float)pos.first, (float)pos.second };

								SettedFirstPoint = true;
								std::ostringstream oss;
								oss << "���������� ������ �����:\n" <<
									"[x: " << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss.str().c_str());
							}
						}
						else if (!SettedSecondPoint)
						{
							mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

							int ms_posX = wnd->mouse.GetPosX();
							int ms_posY = wnd->mouse.GetPosY();

							HitBox hb(std::string("Drown hitbox"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
							wnd->Gfx().DrawHitBox(hb);

							if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								secondPoint = { (float)pos.first, (float)pos.second };

								HitBox hb_new(hero->name + std::string(" hitbox"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
								hero->SetHitBox(hb);
								hero->hitbox_visability = true;

								std::ostringstream oss;
								oss << "���������� ������ �����:\n" <<
									"[x: " << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss.str().c_str());

								AddLog("���������� Hit-box:\n");

								auto actual_hb = hero->hitbox;

								EngineFunctions::SetNewValue<float>(
									hero->name,
									"hb-ltx", actual_hb.GetCoordinates().x,
									hero->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									hero->name,
									"hb-lty", actual_hb.GetCoordinates().y,
									hero->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									hero->name,
									"hb-rbx", actual_hb.GetCoordinates().z,
									hero->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									hero->name,
									"hb-rby", actual_hb.GetCoordinates().w,
									hero->dataPath,
									&applog
									);

								mouseHelpInfo = "";
								SettedSecondPoint = true;
							}
						}
						else if (SettedFirstPoint && SettedSecondPoint)
						{
							DrawingHitBox = false;
							SettedFirstPoint = false;
							SettedSecondPoint = false;

							ImGui::GetStyle().Alpha = 1.0f;
						}
					}
				}		

				ImGui::Separator(); // �����������
			}

			/**************************************/

			/****************************************************/

			if (ImGui::CollapsingHeader("������"))
			{
				ShowCameraControl();
			}

			ImGui::NewLine();
			ImGui::NewLine();

			/* ���� ������ ������ ��������� ������� ��������� �������� ��������� */
			{
				if (ImGui::Button("���������", ImVec2(100, 20)))
				{
					AddLog("���������� �������� ���: ");
					AddLog(hero->name.c_str());
					AddLog("\n");

					SavingSettings = true;
				}

				if (SavingSettings)
				{
					/* ���������� ������� � �������� */

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"pos-x", hero->position.x,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"pos-y", hero->position.y,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"speed", hero->speed,
						hero->dataPath,
						&applog
						);

					/**********************/

					/* ���������� �������� ������� */

					EngineFunctions::SetNewValue<bool>(
						hero->name,
						"eff-a", hero->effect.Active,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"eff-d", hero->effect.Duration,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"eff-t", hero->effect.Time,
						hero->dataPath,
						&applog
						);

					/*******************************/

					/* �������������� hitbox */

					auto actual_hb = hero->hitbox;

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"hb-ltx", actual_hb.GetCoordinates().x,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"hb-lty", actual_hb.GetCoordinates().y,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"hb-rbx", actual_hb.GetCoordinates().z,
						hero->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						hero->name,
						"hb-rby", actual_hb.GetCoordinates().w,
						hero->dataPath,
						&applog
						);

					/*************************/

					SavingSettings = false;
				}
			}
			/*********************************************************************/

			ImGui::EndChild();
		}
	}

	ImGui::End();
}

void GUISystem::ShowPhysicsEngineObjList()
{
	using namespace Physics;

	if (ImGui::Begin("�������", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginTabBar("PhEngTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("�����"))
			{
				for (auto l = phEngPtr->lines.begin(); l != phEngPtr->lines.end(); l++)
				{
					char label[128];
					sprintf_s(label, l->GetName().c_str(), objectSelected);

					std::string contextMenuId = "Context Menu for " + l->name;

					ImGui::Bullet();
					if (ImGui::Selectable(label, objectSelected == l->GetName().c_str()))
					{
						objectSelected = l->GetName();
					}
					if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
					{
						if(ImGui::Button("�������"))
						{
							std::string deletedLineName = l->name;
							AddLog("�������� ");
							AddLog(deletedLineName.c_str());
							AddLog("...\n");

							phEngPtr->DeleteLineAt(l);
							EngineFunctions::DeleteJsonObject(deletedLineName, phEngPtr->dataPath);

							AddLog("����� ");
							AddLog(deletedLineName.c_str());
							AddLog(" �������\n");

							ImGui::EndPopup();
							
							break;
						}

						ImGui::EndPopup();
					}
				}
				
				ImGui::NewLine();

				// ���� ������ ������ �������� �����
				if (ImGui::Button("��������"))
				{
					AddLog("���������� �����...");
					AddLog("\n");

					AddingObject = true;

					ImGui::GetStyle().Alpha = 0.1f;
				}

				/* ���� ��� ���������� ����� */
				{
					if (AddingObject)
					{
						if (!SettedFirstPoint)
						{
							mouseHelpInfo = "��� ���������� �����.\n������� ���, ����� ���������\n������ �����.";

							if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								firstPoint = { (float)pos.first, (float)pos.second };

								SettedFirstPoint = true;
								std::ostringstream oss;
								oss << "���������� ������ �����:\n" <<
									"[x: "  << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss.str().c_str());
							}
						}
						else if (!SettedSecondPoint)
						{
							mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

							int ms_posX = wnd->mouse.GetPosX();
							int ms_posY = wnd->mouse.GetPosY();

							Line line(std::string("Drown line"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
							wnd->Gfx().DrawLine(line.start, line.end);

							if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								secondPoint = { (float)pos.first, (float)pos.second };

								std::ostringstream line_name;
								line_name << "line " << phEngPtr->GetLinesAmount();

								Line line_new(line_name.str(), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);								
								phEngPtr->AddLine(line_new);

								std::ostringstream oss;
								oss << "���������� ������ �����:\n" <<
									"[x: "  << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss.str().c_str());

								AddLog("���������� �����:\n");

								using std::to_string;

								// �������� ����� � ������� � ������ �����
								std::ifstream dataFile(phEngPtr->dataPath);
								if (!dataFile.is_open())
								{
									throw ("�� ������ ������� ���� � ������� � ������ �����");
								}

								// ������ �����
								json j;
								dataFile >> j;

								// �������� �����
								dataFile.close();

								// ����� �����
								std::ostringstream newLine;
								newLine << "\"" << line_name.str() << "\":[{";

								newLine << "\"start-x\": "	<< firstPoint.x  << ",";
								newLine << "\"start-y\" : " << firstPoint.y  << ",";
								newLine << "\"end-x\" : "	<< secondPoint.x << ",";
								newLine << "\"end-y\" : "	<< secondPoint.y << "}]";
								
								// ���������� � ������� � ����
								std::string json_str = j.dump();
								size_t pos_of_par = json_str.find_last_of('}');
								size_t pos_of_par2 = json_str.find_last_of(']');

								json_str.at(pos_of_par) = ' ';
								json_str.at(pos_of_par2 + 1) = ',';

								// ������ � ���� ������ ����� �����
								std::ofstream ostream(phEngPtr->dataPath);
								ostream << json_str + newLine.str() + '}';

								// �������� �����
								ostream.close();

								std::ostringstream oss_l;
								oss_l << "��������� [" << line_name.str() << "]\n";

								AddLog(oss_l.str().c_str());

								mouseHelpInfo = "";
								SettedSecondPoint = true;
							}
						}
						else if (SettedFirstPoint && SettedSecondPoint)
						{
							AddingObject = false;
							SettedFirstPoint = false;
							SettedSecondPoint = false;

							ImGui::GetStyle().Alpha = 1.0f;
						}
					}
				}
				/******************************/

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Hit-Box'�"))
			{
				for (auto hb = phEngPtr->hitboxes.begin(); hb != phEngPtr->hitboxes.end(); hb++)
				{
					char label[128];
					sprintf_s(label, hb->GetName().c_str(), objectSelected);

					std::string contextMenuId = "Context Menu for " + hb->name;

					ImGui::Bullet();
					if (ImGui::Selectable(label, objectSelected == hb->GetName().c_str()))
					{
						objectSelected = hb->GetName();
					}
					if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
					{
						if (ImGui::Button("�������"))
						{
							std::string deletedHitBoxName = hb->name;
							AddLog("�������� ");
							AddLog(deletedHitBoxName.c_str());
							AddLog("...\n");

							phEngPtr->DeleteHitBoxAt(hb);
							EngineFunctions::DeleteJsonObject(deletedHitBoxName, phEngPtr->dataPath);

							AddLog("Hit-Box ");
							AddLog(deletedHitBoxName.c_str());
							AddLog(" �����\n");

							ImGui::EndPopup();

							break;
						}

						ImGui::EndPopup();
					}
				}

				ImGui::NewLine();

				// ���� ������ ������ �������� hitbox
				if (ImGui::Button("��������"))
				{
					AddLog("���������� Hit-Box'�...");
					AddLog("\n");

					AddingObject = true;

					ImGui::GetStyle().Alpha = 0.1f;
				}

				/* ���� ��� ���������� hitbox */
				{
					if (AddingObject)
					{
						if (!SettedFirstPoint)
						{
							mouseHelpInfo = "��� ���������� Hit-Box'�.\n������� ���, ����� ���������\n������ �����.";

							if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								firstPoint = { (float)pos.first, (float)pos.second };

								SettedFirstPoint = true;
								std::ostringstream oss;
								oss << "���������� ������ �����:\n" <<
									"[x: "  << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss.str().c_str());
							}
						}
						else if (!SettedSecondPoint)
						{
							mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

							int ms_posX = wnd->mouse.GetPosX();
							int ms_posY = wnd->mouse.GetPosY();

							HitBox hb(std::string("Drown hitbox"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
							wnd->Gfx().DrawHitBox(hb);

							if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								secondPoint = { (float)pos.first, (float)pos.second };

								std::ostringstream hb_name;
								hb_name << "hb " << phEngPtr->GetHitBoxAmount();

								HitBox hb_new(hb_name.str(), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
								phEngPtr->AddHitBox(hb_new);

								std::ostringstream oss;
								oss << "���������� ������ �����:\n" <<
									"[x: "  << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss.str().c_str());

								AddLog("���������� Hit-Box'�:\n");

								using std::to_string;

								// �������� ����� � ������� � ������ �����
								std::ifstream dataFile(phEngPtr->dataPath);
								if (!dataFile.is_open())
								{
									throw ("�� ������ ������� ���� � ������� � ������ �����");
								}

								// ������ �����
								json j;
								dataFile >> j;

								// �������� �����
								dataFile.close();

								// ����� hitbox
								std::ostringstream newLine;
								newLine << "\"" << hb_name.str() << "\":[{";

								newLine << "\"lt-x\": " <<  firstPoint.x << ",";
								newLine << "\"lt-y\" : " << firstPoint.y << ",";
								newLine << "\"rb-x\" : " << secondPoint.x << ",";
								newLine << "\"rb-y\" : " << secondPoint.y << "}]";

								// ���������� � ������� � ����
								std::string json_str = j.dump();
								size_t pos_of_par = json_str.find_last_of('}');
								size_t pos_of_par2 = json_str.find_last_of(']');

								json_str.at(pos_of_par) = ' ';
								json_str.at(pos_of_par2 + 1) = ',';

								// ������ � ���� ������ ������ hitbox
								std::ofstream ostream(phEngPtr->dataPath);
								ostream << json_str + newLine.str() + '}';

								// �������� �����
								ostream.close();

								std::ostringstream oss_l;
								oss_l << "��������� [" << hb_name.str() << "]\n";

								AddLog(oss_l.str().c_str());

								mouseHelpInfo = "";
								SettedSecondPoint = true;
							}
						}
						else if (SettedFirstPoint && SettedSecondPoint)
						{
							AddingObject = false;
							SettedFirstPoint = false;
							SettedSecondPoint = false;

							ImGui::GetStyle().Alpha = 1.0f;
						}
					}
				}
				/******************************/

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}

	ImGui::End();
}

void GUISystem::ShowPhysicsEngineObjControl()
{
	using namespace Physics;

	if (ImGui::Begin("�����", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (objectSelected.find("line") != objectSelected.npos)
		{
			// ���� �� ������
			for (size_t k = 0; k < phEngPtr->lines.size(); k++)
			{
				// ����� ��������� �����
				if (phEngPtr->lines.at(k).name == objectSelected)
				{
					if (ImGui::BeginChild(""))
					{
						/* ���������� ���������� ������� ���������� */

						bool posDirty = false;		// �������� �������

						const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // ���������					

						/********************************************/

						/* �������� ���������� ��������� ����� ����� */
						if (ImGui::CollapsingHeader("���������", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("������� ��������� �����:");
							dcheck(ImGui::SliderFloat("Xs", &phEngPtr->lines.at(k).start.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &phEngPtr->lines.at(k).start.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

							ImGui::Text("������� �������� �����:");
							dcheck(ImGui::SliderFloat("Xe", &phEngPtr->lines.at(k).end.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &phEngPtr->lines.at(k).end.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

							if (ImGui::Button("����������� X", ImVec2(100, 20)))
							{
								AddLog("������������ �� Xs ���: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								phEngPtr->lines.at(k).end.x = phEngPtr->lines.at(k).start.x;

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"start-x", phEngPtr->lines.at(k).start.x,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"end-x", phEngPtr->lines.at(k).end.x,
									phEngPtr->dataPath,
									&applog
									);
							}

							ImGui::SameLine();

							if (ImGui::Button("����������� Y", ImVec2(100, 20)))
							{
								AddLog("������������ �� Ys ���: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								phEngPtr->lines.at(k).end.y = phEngPtr->lines.at(k).start.y;

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"start-y", phEngPtr->lines.at(k).start.y,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"end-y", phEngPtr->lines.at(k).end.y,
									phEngPtr->dataPath,
									&applog
									);
							}

							ImGui::Separator();	// �����������
						}
						/*********************************************/

						/* �������� ���������� ������ */
						if (ImGui::CollapsingHeader("���������", ImGuiTreeNodeFlags_DefaultOpen))
						{
							/* ���� ������ ������ ������������ ����� */
							{
								if (ImGui::Button("������������", ImVec2(100, 20)))
								{
									AddLog("��������� �����: ");
									AddLog(objectSelected.c_str());
									AddLog("\n");

									DrawingLine = true;
									phEngPtr->lines.at(k).visability = false;

									ImGui::GetStyle().Alpha = 0.1f;
								}

								if (DrawingLine)
								{
									if (!SettedFirstPoint)
									{
										mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

										if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
										{
											auto pos = wnd->mouse.GetPos();
											firstPoint = { (float)pos.first, (float)pos.second };

											SettedFirstPoint = true;
											std::ostringstream oss;
											oss << "���������� ������ �����:\n" <<
												"[x: " << firstPoint.x <<
												"; y: " << firstPoint.y << "]\n";

											AddLog(oss.str().c_str());
										}
									}
									else if (!SettedSecondPoint)
									{
										mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

										int ms_posX = wnd->mouse.GetPosX();
										int ms_posY = wnd->mouse.GetPosY();

										Line line(std::string("Drown line"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
										wnd->Gfx().DrawLine(line.start, line.end);

										if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
										{
											auto pos = wnd->mouse.GetPos();
											secondPoint = { (float)pos.first, (float)pos.second };

											Line line_new(objectSelected, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
											phEngPtr->UpdateLineAt(k, line_new);

											std::ostringstream oss;
											oss << "���������� ������ �����:\n" <<
												"[x: " << secondPoint.x <<
												"; y: " << secondPoint.y << "]\n";

											AddLog(oss.str().c_str());

											AddLog("���������� �����:\n");

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"start-x", line_new.start.x,
												phEngPtr->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"start-y", line_new.start.y,
												phEngPtr->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"end-x", line_new.end.x,
												phEngPtr->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"end-y", line_new.end.y,
												phEngPtr->dataPath,
												&applog
												);

											mouseHelpInfo = "";
											SettedSecondPoint = true;
										}
									}
									else if (SettedFirstPoint && SettedSecondPoint)
									{
										DrawingLine = false;
										SettedFirstPoint = false;
										SettedSecondPoint = false;

										ImGui::GetStyle().Alpha = 1.0f;
									}
								}
							}
							/*****************************************/

							ImGui::Separator();	// �����������
						}
						/******************************/

						ImGui::NewLine();
						ImGui::NewLine();

						/* ���� ������ ������ ��������� ������� ��������� ����� */
						{
							if (ImGui::Button("���������", ImVec2(100, 20)))
							{
								AddLog("���������� �������� ���: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								SavingSettings = true;
							}

							if (SavingSettings)
							{
								/* ���������� ��������� */

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"start-x", phEngPtr->lines.at(k).start.x,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"start-y", phEngPtr->lines.at(k).start.y,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"end-x", phEngPtr->lines.at(k).end.x,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"end-y", phEngPtr->lines.at(k).end.y,
									phEngPtr->dataPath,
									&applog
									);

								/************************/

								SavingSettings = false;
							}
						}
						/********************************************************/

						ImGui::EndChild();
					}
				}
			}
		}
		else if (objectSelected.find("hb") != objectSelected.npos)
		{
			// ���� �� ���������
			for (size_t k = 0; k < phEngPtr->hitboxes.size(); k++)
			{
				// ����� ���������� hitbox
				if (phEngPtr->hitboxes.at(k).name == objectSelected)
				{
					if (ImGui::BeginChild(""))
					{
						/* ���������� ���������� ������� ���������� */

						bool posDirty = false;		// �������� �������
						bool boxDirty = false;		// �������� �������

						const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // ���������					

						/********************************************/

						/* �������� ���������� ��������� ����� ����� */
						if (ImGui::CollapsingHeader("���������", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("������� ����� ������� �����:");
							dcheck(ImGui::SliderFloat("Xs", &phEngPtr->hitboxes.at(k).coordinates.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &phEngPtr->hitboxes.at(k).coordinates.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

							ImGui::Text("������� ������ ������ �����:");
							dcheck(ImGui::SliderFloat("Xe", &phEngPtr->hitboxes.at(k).coordinates.z, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &phEngPtr->hitboxes.at(k).coordinates.w, -1000.0f, 1000.0f, "%.2f"), posDirty);

							ImGui::NewLine();

							dcheck(ImGui::SliderFloat("����� ������� ��������", &sq_l, 0.0f, 100.0f), boxDirty);

							if (ImGui::Button("������� �������", ImVec2(100, 21)))
							{
								AddLog("�������� �������� ��: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								phEngPtr->hitboxes.at(k).coordinates.z = phEngPtr->hitboxes.at(k).coordinates.x + sq_l;
								phEngPtr->hitboxes.at(k).coordinates.w = phEngPtr->hitboxes.at(k).coordinates.y + sq_l;

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"rb-x", phEngPtr->hitboxes.at(k).coordinates.z,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"rb-y", phEngPtr->hitboxes.at(k).coordinates.w,
									phEngPtr->dataPath,
									&applog
									);
							}

							ImGui::Separator();	// �����������
						}
						/*********************************************/

						/* �������� ���������� hitbox */
						if (ImGui::CollapsingHeader("���������", ImGuiTreeNodeFlags_DefaultOpen))
						{
							/* ���� ������ ������ ������������ hitbox */
							{
								if (ImGui::Button("������������", ImVec2(100, 20)))
								{
									AddLog("��������� Hit-Box'�: ");
									AddLog(objectSelected.c_str());
									AddLog("\n");

									DrawingHitBox = true;
									phEngPtr->hitboxes.at(k).visability = false;

									ImGui::GetStyle().Alpha = 0.1f;
								}

								if (DrawingHitBox)
								{
									if (!SettedFirstPoint)
									{
										mouseHelpInfo = "��� ��������� Hit-Box'�.\n������� ���, ����� ���������\n������ �����.";

										if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
										{
											auto pos = wnd->mouse.GetPos();
											firstPoint = { (float)pos.first, (float)pos.second };

											SettedFirstPoint = true;
											std::ostringstream oss;
											oss << "���������� ������ �����:\n" <<
												"[x: "  << firstPoint.x <<
												"; y: " << firstPoint.y << "]\n";

											AddLog(oss.str().c_str());
										}
									}
									else if (!SettedSecondPoint)
									{
										mouseHelpInfo = "������� ���, ����� ���������\n������ �����.";

										int ms_posX = wnd->mouse.GetPosX();
										int ms_posY = wnd->mouse.GetPosY();

										HitBox hb(std::string("Drown hitbox"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
										wnd->Gfx().DrawHitBox(hb);

										if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
										{
											auto pos = wnd->mouse.GetPos();
											secondPoint = { (float)pos.first, (float)pos.second };

											HitBox hb_new(objectSelected, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
											phEngPtr->UpdateHitBoxAt(k, hb_new);

											std::ostringstream oss;
											oss << "���������� ������ �����:\n" <<
												"[x: "  << secondPoint.x <<
												"; y: " << secondPoint.y << "]\n";

											AddLog(oss.str().c_str());

											AddLog("���������� Hit-Box'�:\n");

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"lt-x", hb_new.coordinates.x,
												phEngPtr->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"lt-y", hb_new.coordinates.y,
												phEngPtr->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"rb-x", hb_new.coordinates.z,
												phEngPtr->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												objectSelected,
												"rb-y", hb_new.coordinates.w,
												phEngPtr->dataPath,
												&applog
												);

											mouseHelpInfo = "";
											SettedSecondPoint = true;
										}
									}
									else if (SettedFirstPoint && SettedSecondPoint)
									{
										DrawingHitBox = false;
										SettedFirstPoint = false;
										SettedSecondPoint = false;

										ImGui::GetStyle().Alpha = 1.0f;
									}
								}
							}
							/*****************************************/

							ImGui::Separator();	// �����������
						}
						/******************************/

						ImGui::NewLine();
						ImGui::NewLine();

						/* ���� ������ ������ ��������� ������� ��������� hitbox */
						{
							if (ImGui::Button("���������", ImVec2(100, 20)))
							{
								AddLog("���������� �������� ���: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								SavingSettings = true;
							}

							if (SavingSettings)
							{
								/* ���������� ��������� */

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"lt-x", phEngPtr->hitboxes.at(k).coordinates.x,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"lt-y", phEngPtr->hitboxes.at(k).coordinates.y,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"rb-x", phEngPtr->hitboxes.at(k).coordinates.z,
									phEngPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									objectSelected,
									"rb-y", phEngPtr->hitboxes.at(k).coordinates.w,
									phEngPtr->dataPath,
									&applog
									);

								/************************/

								SavingSettings = false;
							}
						}
						/********************************************************/

						ImGui::EndChild();
					}
				}
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowObjectsControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.2f),
		DispSize.y * 0.6f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("����", &ShowObjectsSettings))
	{
		for (size_t i = 0; i < objectsPtr->queue.size(); i++)
		{
			char label[128];
			sprintf_s(label, objectsPtr->queue[i]->GetName().c_str(), objectSelected);

			std::string contextMenuId = "Context Menu for " + objectsPtr->queue[i]->GetName();

			ImGui::Bullet();
			if (ImGui::Selectable(label, objectSelected == objectsPtr->queue[i]->GetName().c_str()))
			{
				objectSelected = objectsPtr->queue[i]->GetName();
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("�� ������ ����"))
				{
					objectsPtr->MoveDown(i);
				}

				if (ImGui::Button("�� �������� ����"))
				{
					objectsPtr->MoveUp(i);
				}

				ImGui::EndPopup();
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowCameraControl()
{
	if (ImGui::Button("����������� ���"))
	{
		hero->cameraMode = MainPerson::CameraMode::SteadyWorld;
	}

	if (ImGui::Button("����������� ������"))
	{
		hero->cameraMode = MainPerson::CameraMode::SteadyPerson;
	}
}

/*******************************************/