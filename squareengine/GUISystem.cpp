#include "GUISystem.h"

#include "imgui/implot.h"
#include "EngineFunctions.hpp"
#include "HitBox.h"
#include "Line.h"

GUISystem::GUISystem(std::shared_ptr<Window>				 wnd,
					 PersonContainer*						 persons,
					 MainPerson*							 mPersPtr,
					 Layers*								 layers,
					 std::shared_ptr<Physics::PhysicsEngine> phEngPtr)
	:
	wnd(wnd),
	phEngPtr(phEngPtr),
	persConPtr(persons),
	mPersPtr(mPersPtr),
	layersPtr(layers)
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

/* Главные методы для отрисовки интерфейса */

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

/* Методы настройки и отрисовки панелей */

void GUISystem::SetGUIColors()
{
	/*Стиль интерфейса*/

	ImGui::GetStyle().FrameRounding = 4.0f;									// Закругление компонентов
	ImGui::GetStyle().WindowBorderSize = 0.0f;								// Размер границы
	ImGui::GetStyle().WindowRounding = 0.0f;								// Закругление окон
	
	// Цвета
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.36f, 0.39f, 1.00f);		// Главное меню
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.84f);			// Фон окна
	colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.00f, 0.20f, 0.73f);			// Меню окна
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.00f, 0.07f, 0.73f);	// Наведение на меню окна
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.50f, 0.38f, 0.54f);			// Компонента
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.18f, 0.15f, 0.40f);	// Наведение на компоненту
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.48f, 0.45f, 0.67f);	// Активные компоненты
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);		// Галочка
	colors[ImGuiCol_SliderGrab] = ImVec4(0.37f, 0.70f, 0.00f, 1.00f);		// Ползунок слайдера
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.82f, 0.19f, 1.00f);	// Активный ползунок слайдера
	colors[ImGuiCol_Button] = ImVec4(0.56f, 0.05f, 0.05f, 0.59f);			// Кнопка
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.01f, 0.17f, 1.00f);	// Наведение на кнопку
	colors[ImGuiCol_ButtonActive] = ImVec4(0.03f, 0.55f, 0.48f, 1.00f);		// Активная кнопка
	colors[ImGuiCol_Separator] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// Разделитель
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.08f, 0.27f, 0.86f);				// Раздел
	colors[ImGuiCol_TabHovered] = ImVec4(0.01f, 0.43f, 0.63f, 0.80f);		// Наведение на раздел
	colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// Активный раздел
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.19f, 0.67f, 0.65f, 1.00f);	// Ползунок
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.31f, 0.04f, 0.04f, 0.81f);	// Блок заголовка таблицы
	colors[ImGuiCol_Header] = ImVec4(0.50f, 0.09f, 0.70f, 0.31f);			// Заголовок
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.57f, 0.49f, 1.00f);		// Активный заголовок
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.22f, 0.59f, 0.80f);	// Наведение на заголовк

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
		if (ImGui::BeginMenu("Файл"))
		{
			if (ImGui::MenuItem("Выход"))
			{
				exit(0);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Окна"))
		{
			if (ImGui::BeginMenu("Система"))
			{
				if (ImGui::MenuItem("Графика"))
				{
				}

				if (ImGui::BeginMenu("Физика"))
				{
					if (ImGui::MenuItem("Объекты"))
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
					
					if (ImGui::MenuItem("Настройки"))
					{
						ShowPhysicsSettings ? ShowPhysicsSettings = false : ShowPhysicsSettings = true;
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Звук"))
				{
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Объекты"))
			{
				if (ImGui::MenuItem("Главный персонаж"))
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

				if (ImGui::MenuItem("Персонажи"))
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

				if (ImGui::MenuItem("Триггеры"))
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

			if (ImGui::MenuItem("График FPS"))
			{
				ShowFPSGraph ? ShowFPSGraph = false : ShowFPSGraph = true;
			}

			if (ImGui::MenuItem("Лог"))
			{
				ShowLogs ? ShowLogs = false : ShowLogs = true;
			}

			if (ImGui::MenuItem("Отключить панели"))
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
	/* Левая сторона */

	SetPanelSizeAndPosition(0, 0.2f, 0.75f);

	/* Содержимое */

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

	/* Конец левой стороны */
}

void GUISystem::ShowRightSide()
{
	/* Правая сторона */

	ImGuiIO& io = ImGui::GetIO();

	int corner = 1;

	float MenuHeight = ImGui::GetMenuHeight();
	float RightPanelW = io.DisplaySize.x * 0.2f;

	ImVec2 RightPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	SetPanelSizeAndPosition(corner, 0.2f, 1.0f);

	/* Содержимое */

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

	/* Конец правой стороны */
}

void GUISystem::ShowLeftBottomSide()
{
	/* Левая нижняя сторона */

	SetPanelSizeAndPosition(2, 0.2f, 0.25f);

	/* Содержимое */

	if (ShowHardwareInfo)
	{
		ShowGPU();
	}

	/**************/

	/* Конец левой нижней стороны */
}

void GUISystem::ShowBottomPanel()
{
	/* Нижняя сторона */

	SetPanelSizeAndPosition(3, 0.6f, 0.25f, -0.2f);

	/* Содержимое */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* Конец нижней стороны */
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

	if (ShowLayersSettings)
	{
		ShowLayersControl();
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

/* Методы отрисовки конкретных интерфейсов */

void GUISystem::ShowLog()
{
	ImGui::Begin("Лог", NULL, 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("Лог", NULL);
	ImGui::End();
}

void GUISystem::ShowGPU()
{
	if (ImGui::Begin("Представление", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text("%.3f мс/кадр (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		std::ostringstream s;
		s << sizeof(*this);

		ImGui::Text("Вес GUI: %s байт", s.str().c_str());

		ImGui::Separator();

		ImGui::Text("Графическое оборудование:");
		for (auto& d : gpu_desc)
		{
			char name_gpu[256];
			sprintf_s(name_gpu, "%ws", reinterpret_cast<const char*>(d.first.c_str()));

			if (ImGui::TreeNode(name_gpu))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(133.0f / 255.0f, 219.0f / 255.0f, 15.0f / 255, 1.0f));
				ImGui::Text("Память: ~%.1f ГБ", d.second);
				ImGui::PopStyleColor();
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowFPS()
{
	if (ImGui::Begin("График FPS", &ShowFPSGraph))
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
	if (ImGui::Begin("Настройки физического движка", &ShowPhysicsSettings,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		using namespace Physics;

		if (ImGui::CollapsingHeader("Общая информация", ImGuiTreeNodeFlags_DefaultOpen))
		{

			ImGui::TextWrapped(
				"Физический движок позволяет настраивать коллизию объектов и их физические параметры. "
				"Основными примитивами для создания объектов являтся линии и прямоугольники (Hit-Box).\n");
			ImGui::Text("Занимаемый объём памяти данными о линиях %d байт (%.1f килобайт).\n", sizeof(phEngPtr->lines) + sizeof(Line) * phEngPtr->lines.size(), float(sizeof(phEngPtr->lines) + sizeof(Line) * phEngPtr->lines.size()) / (1024.0f));
			ImGui::Text("Занимаемый объём памяти данными о прямоугольниках %d байт (%.1f килобайт).\n", sizeof(phEngPtr->hitboxes) + sizeof(HitBox) * phEngPtr->hitboxes.size(), float(sizeof(phEngPtr->hitboxes) + sizeof(HitBox) * phEngPtr->hitboxes.size()) / (1024.0f));

			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Цвета"))
		{
			ImGui::ColorEdit3("Цвет линий", phEngPtr->lineColor);
			ImGui::ColorEdit3("Цвет прямоугольников", phEngPtr->hbColor);

			if (ImGui::Button("Сохранить"))
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

		if (ImGui::CollapsingHeader("Параметры объектов и коллизий"))
		{
			ImGui::SliderFloat("Отступ между коллизиями объектов", &phEngPtr->deltaCollision, -20.0f, 20.0f);
			ImGui::SliderFloat("Сторона квадрата", &sq_l, 0.0f, 100.0f);
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Длина стороны квадрата, созданного из Hit-Box");
			}

			ImGui::Checkbox("Показать все объекты", &phEngPtr->objVisability);
			ImGui::Checkbox("Показать подписи объектов", &ShowPhysicsEngineObjInfo);
			if (ImGui::Button("Сохранить"))
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
	if (ImGui::Begin("Персонажи", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto p = persConPtr->persons.begin(); p != persConPtr->persons.end(); p++)
		{
			char label[128];
			sprintf_s(label, p->get()->GetName().c_str(), personSelected);

			std::string contextMenuId = "Context Menu for " + p->get()->name;

			ImGui::Bullet();
			if (ImGui::Selectable(label, personSelected == p->get()->GetName().c_str()))
			{
				personSelected = p->get()->GetName();
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("Удалить"))
				{
					std::string deletedPersonName = p->get()->name;
					AddLog("Удаление ");
					AddLog(deletedPersonName.c_str());
					AddLog("...\n");

					persConPtr->DeletePersonAt(p);
					EngineFunctions::DeleteJsonObject(deletedPersonName, persConPtr->dataPath);

					AddLog("Персонаж ");
					AddLog(deletedPersonName.c_str());
					AddLog(" удалён\n");

					ImGui::EndPopup();

					break;
				}

				ImGui::EndPopup();
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Добавить"))
		{

		}
	}

	ImGui::End();
}

void GUISystem::ShowPersonControl()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		// Цикл по персонажам
		for (int k = 0; k < persConPtr->persons.size(); k++)
		{
			// Поиск выбранного персонажа
			if (persConPtr->persons.at(k)->name == personSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					/* Переменные управления сбросом интерфейса */

					bool posDirty   = false;	// Контроль позиции
					bool effDirty   = false;	// Котнроль эффекта
					bool speedDirty = false;	// Котнроль скорости

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

					/********************************************/

					/* Элементы управления позицией и скорости персонажа */

					if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("Позиция:");
						dcheck(ImGui::SliderFloat("X", &persConPtr->persons.at(k)->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
						dcheck(ImGui::SliderFloat("Y", &persConPtr->persons.at(k)->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

						ImGui::Text("Скорость:");
						dcheck(ImGui::SliderFloat("", &persConPtr->persons.at(k)->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);
					
						ImGui::Separator(); // Разделитель
					}

					/******************************************/

					/* Элементы управления эффектом персонажа */

					if (ImGui::CollapsingHeader("Эффекты"))
					{						
						dcheck(ImGui::SliderFloat("Продолжитель.", &persConPtr->persons.at(k)->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
						dcheck(ImGui::SliderFloat("Время", &persConPtr->persons.at(k)->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

						ImGui::Checkbox("Активен", &persConPtr->persons.at(k)->effect.Active);

						ImGui::Separator();	// Разделитель
					}

					/******************************************/

					/* Элементы управления хитбоксом персонажа */

					if (ImGui::CollapsingHeader("Hit-box"))
					{
						ImGui::Checkbox("Показать", &persConPtr->persons.at(k)->hitbox_visability);

						/* Если нажата кнопка изменить HitBox */
						{
							if (ImGui::Button("Изменить", ImVec2(100, 20)))
							{
								AddLog("Изменение Hit-box для:");
								AddLog(personSelected.c_str());
								AddLog("\n");

								DrawingHitBox = true;
								persConPtr->persons.at(k)->hitbox_visability = false;

								ImGui::GetStyle().Alpha = 0.1f;
							}

							if (DrawingHitBox)
							{
								if (!SettedFirstPoint)
								{
									mouseHelpInfo = "Нажмите ЛКМ, чтобы поставить\nпервую точку.";

									if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
									{
										auto pos = wnd->mouse.GetPos();
										firstPoint = { (float)pos.first, (float)pos.second };

										SettedFirstPoint = true;
										std::ostringstream oss;
										oss << "Поставлена первая точка:\n" <<
											"[x: " << firstPoint.x <<
											"; y: " << firstPoint.y << "]\n";

										AddLog(oss.str().c_str());
									}
								}
								else if (!SettedSecondPoint)
								{
									mouseHelpInfo = "Нажмите ПКМ, чтобы поставить\nвторую точку.";

									int ms_posX = wnd->mouse.GetPosX();
									int ms_posY = wnd->mouse.GetPosY();

									HitBox hb(std::string("Drown hitbox"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
									wnd->Gfx().DrawHitBox(hb);

									if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
									{
										auto pos = wnd->mouse.GetPos();
										secondPoint = { (float)pos.first, (float)pos.second };

										HitBox hb_new(persConPtr->persons.at(k)->name + std::string(" hitbox"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
										persConPtr->persons.at(k)->SetHitBox(hb);
										persConPtr->persons.at(k)->hitbox_visability = true;

										std::ostringstream oss;
										oss << "Поставлена вторая точка:\n" <<
											"[x: " << secondPoint.x <<
											"; y: " << secondPoint.y << "]\n";

										AddLog(oss.str().c_str());

										AddLog("Сохранение Hit-box:\n");

										auto actual_hb = persConPtr->persons.at(k)->hitbox;

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-ltx", actual_hb.GetCoordinates().x,
											persConPtr->dataPath,
											&applog
											);

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-lty", actual_hb.GetCoordinates().y,
											persConPtr->dataPath,
											&applog
											);

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-rbx", actual_hb.GetCoordinates().z,
											persConPtr->dataPath,
											&applog
											);

										EngineFunctions::SetNewValue<float>(
											personSelected,
											"hb-rby", actual_hb.GetCoordinates().w,
											persConPtr->dataPath,
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

						ImGui::Separator(); // Разделитель
					}
					/*******************************************/

					ImGui::NewLine();
					ImGui::NewLine();

					/* Если нажата кнопка сохранить текущие настройки персонажа */
					{
						if (ImGui::Button("Сохранить", ImVec2(100, 20)))
						{
							AddLog("Сохранение настроек для: ");
							AddLog(personSelected.c_str());
							AddLog("\n");

							SavingSettings = true;
						}

						if (SavingSettings)
						{
							/* Сохранение позиции и скорости */
							
							EngineFunctions::SetNewValue<float>(
								personSelected,
								"pos-x", persConPtr->persons.at(k)->position.x,
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"pos-y", persConPtr->persons.at(k)->position.y,
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"speed", persConPtr->persons.at(k)->speed,
								persConPtr->dataPath,
								&applog
								);

							/**********************/

							/* Сохранение настроек эффекта */
							
							EngineFunctions::SetNewValue<bool>(
								personSelected,
								"eff-a", persConPtr->persons.at(k)->effect.Active,
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"eff-d", persConPtr->persons.at(k)->effect.Duration,
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"eff-t", persConPtr->persons.at(k)->effect.Time,
								persConPtr->dataPath,
								&applog
								);

							/*******************************/

							SavingSettings = false;
						}
					}
					/************************************************************/

					ImGui::SameLine();

					/* Если нажата кнопка удалить персонажа */
					{
						if (ImGui::Button("Удалить", ImVec2(100, 20)))
						{
							for (auto i = persConPtr->persons.begin(); i != persConPtr->persons.end(); i++)
							{
								if (i->get()->GetName() == personSelected)
								{
									persConPtr->persons.erase(i);
									break;
								}
							}
							
							EngineFunctions::DeleteJsonObject(personSelected, persConPtr->dataPath);
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
	if (ImGui::Begin("Персонажи", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text(mPersPtr->name.c_str());
	}

	ImGui::End();
}

void GUISystem::ShowMainPersonControl()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginChild(""))
		{
			/* Переменные управления сбросом интерфейса */

			bool posDirty = false;		// Контроль позиции
			bool effDirty = false;		// Котнроль эффекта
			bool speedDirty = false;	// Котнроль скорости

			const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

			/********************************************/

			/* Элементы управления позицией и скорости главного персонажа */

			if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Позиция:");
				dcheck(ImGui::SliderFloat("X", &mPersPtr->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
				dcheck(ImGui::SliderFloat("Y", &mPersPtr->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

				ImGui::Text("Скорость:");
				dcheck(ImGui::SliderFloat("", &mPersPtr->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);

				ImGui::Separator();	// Разделитель
			}
			
			/**************************************************************/

			/* Элементы управления эффектом главного персонажа */

			if (ImGui::CollapsingHeader("Эффекты"))
			{
				dcheck(ImGui::SliderFloat("Продолжитель.", &mPersPtr->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
				dcheck(ImGui::SliderFloat("Время", &mPersPtr->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

				ImGui::Checkbox("Активен", &mPersPtr->effect.Active);

				ImGui::Separator();	// Разделитель
			}

			/***************************************************/

			/* Элементы управления хитбоксом главного персонажа */
			
			if (ImGui::CollapsingHeader("Hit-box"))
			{
				ImGui::Checkbox("Показать", &mPersPtr->hitbox_visability);

				/* Если нажата кнопка изменить HitBox */
				{
					if (ImGui::Button("Изменить", ImVec2(100, 20)))
					{
						AddLog("Изменение Hit-box для: ");
						AddLog(mPersPtr->name.c_str());
						AddLog("\n");

						DrawingHitBox = true;
						mPersPtr->hitbox_visability = false;

						ImGui::GetStyle().Alpha = 0.1f;
					}

					if (DrawingHitBox)
					{
						if (!SettedFirstPoint)
						{
							mouseHelpInfo = "Нажмите ЛКМ, чтобы поставить\nпервую точку.";

							if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								firstPoint = { (float)pos.first, (float)pos.second };

								SettedFirstPoint = true;
								std::ostringstream oss;
								oss << "Поставлена первая точка:\n" <<
									"[x: " << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss.str().c_str());
							}
						}
						else if (!SettedSecondPoint)
						{
							mouseHelpInfo = "Нажмите ПКМ, чтобы поставить\nвторую точку.";

							int ms_posX = wnd->mouse.GetPosX();
							int ms_posY = wnd->mouse.GetPosY();

							HitBox hb(std::string("Drown hitbox"), firstPoint.x, firstPoint.y, (float)ms_posX, (float)ms_posY);
							wnd->Gfx().DrawHitBox(hb);

							if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								secondPoint = { (float)pos.first, (float)pos.second };

								HitBox hb_new(mPersPtr->name + std::string(" hitbox"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
								mPersPtr->SetHitBox(hb);
								mPersPtr->hitbox_visability = true;

								std::ostringstream oss;
								oss << "Поставлена вторая точка:\n" <<
									"[x: " << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss.str().c_str());

								AddLog("Сохранение Hit-box:\n");

								auto actual_hb = mPersPtr->hitbox;

								EngineFunctions::SetNewValue<float>(
									mPersPtr->name,
									"hb-ltx", actual_hb.GetCoordinates().x,
									mPersPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									mPersPtr->name,
									"hb-lty", actual_hb.GetCoordinates().y,
									mPersPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									mPersPtr->name,
									"hb-rbx", actual_hb.GetCoordinates().z,
									mPersPtr->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									mPersPtr->name,
									"hb-rby", actual_hb.GetCoordinates().w,
									mPersPtr->dataPath,
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

				ImGui::Separator(); // Разделитель
			}

			/**************************************/

			/****************************************************/

			if (ImGui::CollapsingHeader("Камера"))
			{
				ShowCameraControl();
			}

			ImGui::NewLine();
			ImGui::NewLine();

			/* Если нажата кнопка сохранить текущие настройки главного персонажа */
			{
				if (ImGui::Button("Сохранить", ImVec2(100, 20)))
				{
					AddLog("Сохранение настроек для: ");
					AddLog(mPersPtr->name.c_str());
					AddLog("\n");

					SavingSettings = true;
				}

				if (SavingSettings)
				{
					/* Сохранение позиции и скорости */

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"pos-x", mPersPtr->position.x,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"pos-y", mPersPtr->position.y,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"speed", mPersPtr->speed,
						mPersPtr->dataPath,
						&applog
						);

					/**********************/

					/* Сохранение настроек эффекта */

					EngineFunctions::SetNewValue<bool>(
						mPersPtr->name,
						"eff-a", mPersPtr->effect.Active,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"eff-d", mPersPtr->effect.Duration,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"eff-t", mPersPtr->effect.Time,
						mPersPtr->dataPath,
						&applog
						);

					/*******************************/

					/* Пересохранение hitbox */

					auto actual_hb = mPersPtr->hitbox;

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"hb-ltx", actual_hb.GetCoordinates().x,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"hb-lty", actual_hb.GetCoordinates().y,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"hb-rbx", actual_hb.GetCoordinates().z,
						mPersPtr->dataPath,
						&applog
						);

					EngineFunctions::SetNewValue<float>(
						mPersPtr->name,
						"hb-rby", actual_hb.GetCoordinates().w,
						mPersPtr->dataPath,
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

	if (ImGui::Begin("Объекты", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginTabBar("PhEngTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Линии"))
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
						if(ImGui::Button("Удалить"))
						{
							std::string deletedLineName = l->name;
							AddLog("Удаление ");
							AddLog(deletedLineName.c_str());
							AddLog("...\n");

							phEngPtr->DeleteLineAt(l);
							EngineFunctions::DeleteJsonObject(deletedLineName, phEngPtr->dataPath);

							AddLog("Линия ");
							AddLog(deletedLineName.c_str());
							AddLog(" удалена\n");

							ImGui::EndPopup();
							
							break;
						}

						ImGui::EndPopup();
					}
				}
				
				ImGui::NewLine();

				// Если нажата кнопка добавить линию
				if (ImGui::Button("Добавить"))
				{
					AddLog("Добавление линии...");
					AddLog("\n");

					AddingObject = true;

					ImGui::GetStyle().Alpha = 0.1f;
				}

				/* Если идёт добавление линии */
				{
					if (AddingObject)
					{
						if (!SettedFirstPoint)
						{
							mouseHelpInfo = "Идёт добавление линии.\nНажмите ЛКМ, чтобы поставить\nпервую точку.";

							if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								firstPoint = { (float)pos.first, (float)pos.second };

								SettedFirstPoint = true;
								std::ostringstream oss;
								oss << "Поставлена первая точка:\n" <<
									"[x: "  << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss.str().c_str());
							}
						}
						else if (!SettedSecondPoint)
						{
							mouseHelpInfo = "Нажмите ПКМ, чтобы поставить\nвторую точку.";

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
								oss << "Поставлена вторая точка:\n" <<
									"[x: "  << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss.str().c_str());

								AddLog("Сохранение линии:\n");

								using std::to_string;

								// Открытие файла с данными о физике сцены
								std::ifstream dataFile(phEngPtr->dataPath);
								if (!dataFile.is_open())
								{
									throw ("Не удаётся открыть файл с данными о физике сцены");
								}

								// Чтение файла
								json j;
								dataFile >> j;

								// Закрытие файла
								dataFile.close();

								// Новая линия
								std::ostringstream newLine;
								newLine << "\"" << line_name.str() << "\":[{";

								newLine << "\"start-x\": "	<< firstPoint.x  << ",";
								newLine << "\"start-y\" : " << firstPoint.y  << ",";
								newLine << "\"end-x\" : "	<< secondPoint.x << ",";
								newLine << "\"end-y\" : "	<< secondPoint.y << "}]";
								
								// Подготовка к вставке в файл
								std::string json_str = j.dump();
								size_t pos_of_par = json_str.find_last_of('}');
								size_t pos_of_par2 = json_str.find_last_of(']');

								json_str.at(pos_of_par) = ' ';
								json_str.at(pos_of_par2 + 1) = ',';

								// Запись в файл данных новой линии
								std::ofstream ostream(phEngPtr->dataPath);
								ostream << json_str + newLine.str() + '}';

								// Закрытие файла
								ostream.close();

								std::ostringstream oss_l;
								oss_l << "Добавлено [" << line_name.str() << "]\n";

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

			if (ImGui::BeginTabItem("Hit-Box'ы"))
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
						if (ImGui::Button("Удалить"))
						{
							std::string deletedHitBoxName = hb->name;
							AddLog("Удаление ");
							AddLog(deletedHitBoxName.c_str());
							AddLog("...\n");

							phEngPtr->DeleteHitBoxAt(hb);
							EngineFunctions::DeleteJsonObject(deletedHitBoxName, phEngPtr->dataPath);

							AddLog("Hit-Box ");
							AddLog(deletedHitBoxName.c_str());
							AddLog(" удалён\n");

							ImGui::EndPopup();

							break;
						}

						ImGui::EndPopup();
					}
				}

				ImGui::NewLine();

				// Если нажата кнопка добавить hitbox
				if (ImGui::Button("Добавить"))
				{
					AddLog("Добавление Hit-Box'а...");
					AddLog("\n");

					AddingObject = true;

					ImGui::GetStyle().Alpha = 0.1f;
				}

				/* Если идёт добавление hitbox */
				{
					if (AddingObject)
					{
						if (!SettedFirstPoint)
						{
							mouseHelpInfo = "Идёт добавление Hit-Box'а.\nНажмите ЛКМ, чтобы поставить\nпервую точку.";

							if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
							{
								auto pos = wnd->mouse.GetPos();
								firstPoint = { (float)pos.first, (float)pos.second };

								SettedFirstPoint = true;
								std::ostringstream oss;
								oss << "Поставлена первая точка:\n" <<
									"[x: "  << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss.str().c_str());
							}
						}
						else if (!SettedSecondPoint)
						{
							mouseHelpInfo = "Нажмите ПКМ, чтобы поставить\nвторую точку.";

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
								oss << "Поставлена вторая точка:\n" <<
									"[x: "  << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss.str().c_str());

								AddLog("Сохранение Hit-Box'а:\n");

								using std::to_string;

								// Открытие файла с данными о физике сцены
								std::ifstream dataFile(phEngPtr->dataPath);
								if (!dataFile.is_open())
								{
									throw ("Не удаётся открыть файл с данными о физике сцены");
								}

								// Чтение файла
								json j;
								dataFile >> j;

								// Закрытие файла
								dataFile.close();

								// Новый hitbox
								std::ostringstream newLine;
								newLine << "\"" << hb_name.str() << "\":[{";

								newLine << "\"lt-x\": " <<  firstPoint.x << ",";
								newLine << "\"lt-y\" : " << firstPoint.y << ",";
								newLine << "\"rb-x\" : " << secondPoint.x << ",";
								newLine << "\"rb-y\" : " << secondPoint.y << "}]";

								// Подготовка к вставке в файл
								std::string json_str = j.dump();
								size_t pos_of_par = json_str.find_last_of('}');
								size_t pos_of_par2 = json_str.find_last_of(']');

								json_str.at(pos_of_par) = ' ';
								json_str.at(pos_of_par2 + 1) = ',';

								// Запись в файл данных нового hitbox
								std::ofstream ostream(phEngPtr->dataPath);
								ostream << json_str + newLine.str() + '}';

								// Закрытие файла
								ostream.close();

								std::ostringstream oss_l;
								oss_l << "Добавлено [" << hb_name.str() << "]\n";

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

	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (objectSelected.find("line") != objectSelected.npos)
		{
			// Цикл по линиям
			for (size_t k = 0; k < phEngPtr->lines.size(); k++)
			{
				// Поиск выбранной линии
				if (phEngPtr->lines.at(k).name == objectSelected)
				{
					if (ImGui::BeginChild(""))
					{
						/* Переменные управления сбросом интерфейса */

						bool posDirty = false;		// Контроль позиции

						const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение					

						/********************************************/

						/* Элементы управления позициями точек линии */
						if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("Позиция начальной точки:");
							dcheck(ImGui::SliderFloat("Xs", &phEngPtr->lines.at(k).start.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &phEngPtr->lines.at(k).start.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

							ImGui::Text("Позиция конечной точки:");
							dcheck(ImGui::SliderFloat("Xe", &phEngPtr->lines.at(k).end.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &phEngPtr->lines.at(k).end.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

							if (ImGui::Button("Нормировать X", ImVec2(100, 20)))
							{
								AddLog("Нормирование по Xs для: ");
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

							if (ImGui::Button("Нормировать Y", ImVec2(100, 20)))
							{
								AddLog("Нормирование по Ys для: ");
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

							ImGui::Separator();	// Разделитель
						}
						/*********************************************/

						/* Элементы управления линией */
						if (ImGui::CollapsingHeader("Изменение", ImGuiTreeNodeFlags_DefaultOpen))
						{
							/* Если нажата кнопка перерисовать линию */
							{
								if (ImGui::Button("Перерисовать", ImVec2(100, 20)))
								{
									AddLog("Изменение линии: ");
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
										mouseHelpInfo = "Нажмите ЛКМ, чтобы поставить\nпервую точку.";

										if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
										{
											auto pos = wnd->mouse.GetPos();
											firstPoint = { (float)pos.first, (float)pos.second };

											SettedFirstPoint = true;
											std::ostringstream oss;
											oss << "Поставлена первая точка:\n" <<
												"[x: " << firstPoint.x <<
												"; y: " << firstPoint.y << "]\n";

											AddLog(oss.str().c_str());
										}
									}
									else if (!SettedSecondPoint)
									{
										mouseHelpInfo = "Нажмите ПКМ, чтобы поставить\nвторую точку.";

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
											oss << "Поставлена вторая точка:\n" <<
												"[x: " << secondPoint.x <<
												"; y: " << secondPoint.y << "]\n";

											AddLog(oss.str().c_str());

											AddLog("Сохранение линии:\n");

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

							ImGui::Separator();	// Разделитель
						}
						/******************************/

						ImGui::NewLine();
						ImGui::NewLine();

						/* Если нажата кнопка сохранить текущие настройки линии */
						{
							if (ImGui::Button("Сохранить", ImVec2(100, 20)))
							{
								AddLog("Сохранение настроек для: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								SavingSettings = true;
							}

							if (SavingSettings)
							{
								/* Сохранение координат */

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
			// Цикл по хитбоксам
			for (size_t k = 0; k < phEngPtr->hitboxes.size(); k++)
			{
				// Поиск выбранного hitbox
				if (phEngPtr->hitboxes.at(k).name == objectSelected)
				{
					if (ImGui::BeginChild(""))
					{
						/* Переменные управления сбросом интерфейса */

						bool posDirty = false;		// Контроль позиции
						bool boxDirty = false;		// Контроль позиции

						const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение					

						/********************************************/

						/* Элементы управления позициями точек линии */
						if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("Позиция левой верхней точки:");
							dcheck(ImGui::SliderFloat("Xs", &phEngPtr->hitboxes.at(k).coordinates.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &phEngPtr->hitboxes.at(k).coordinates.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

							ImGui::Text("Позиция правой нижней точки:");
							dcheck(ImGui::SliderFloat("Xe", &phEngPtr->hitboxes.at(k).coordinates.z, -1000.0f, 1000.0f, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &phEngPtr->hitboxes.at(k).coordinates.w, -1000.0f, 1000.0f, "%.2f"), posDirty);

							ImGui::NewLine();

							dcheck(ImGui::SliderFloat("Длина стороны квадрата", &sq_l, 0.0f, 100.0f), boxDirty);

							if (ImGui::Button("Сделать квадрат", ImVec2(100, 21)))
							{
								AddLog("Создание квадрата из: ");
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

							ImGui::Separator();	// Разделитель
						}
						/*********************************************/

						/* Элементы управления hitbox */
						if (ImGui::CollapsingHeader("Изменение", ImGuiTreeNodeFlags_DefaultOpen))
						{
							/* Если нажата кнопка перерисовать hitbox */
							{
								if (ImGui::Button("Перерисовать", ImVec2(100, 20)))
								{
									AddLog("Изменение Hit-Box'а: ");
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
										mouseHelpInfo = "Идёт изменение Hit-Box'а.\nНажмите ЛКМ, чтобы поставить\nпервую точку.";

										if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
										{
											auto pos = wnd->mouse.GetPos();
											firstPoint = { (float)pos.first, (float)pos.second };

											SettedFirstPoint = true;
											std::ostringstream oss;
											oss << "Поставлена первая точка:\n" <<
												"[x: "  << firstPoint.x <<
												"; y: " << firstPoint.y << "]\n";

											AddLog(oss.str().c_str());
										}
									}
									else if (!SettedSecondPoint)
									{
										mouseHelpInfo = "Нажмите ПКМ, чтобы поставить\nвторую точку.";

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
											oss << "Поставлена вторая точка:\n" <<
												"[x: "  << secondPoint.x <<
												"; y: " << secondPoint.y << "]\n";

											AddLog(oss.str().c_str());

											AddLog("Сохранение Hit-Box'а:\n");

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

							ImGui::Separator();	// Разделитель
						}
						/******************************/

						ImGui::NewLine();
						ImGui::NewLine();

						/* Если нажата кнопка сохранить текущие настройки hitbox */
						{
							if (ImGui::Button("Сохранить", ImVec2(100, 20)))
							{
								AddLog("Сохранение настроек для: ");
								AddLog(objectSelected.c_str());
								AddLog("\n");

								SavingSettings = true;
							}

							if (SavingSettings)
							{
								/* Сохранение координат */

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

void GUISystem::ShowLayersControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.2f),
		DispSize.y * 0.6f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("Слои", &ShowLayersSettings))
	{
		for (size_t i = 0; i < layersPtr->objects.size(); i++)
		{
			char label[128];
			sprintf_s(label, layersPtr->objects[i]->GetName().c_str(), objectSelected);

			std::string contextMenuId = "Context Menu for " + layersPtr->objects[i]->GetName();

			ImGui::Bullet();
			if (ImGui::Selectable(label, objectSelected == layersPtr->objects[i]->GetName().c_str()))
			{
				objectSelected = layersPtr->objects[i]->GetName();
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("На задний план"))
				{
					layersPtr->MoveDown(i);
				}

				if (ImGui::Button("На передний план"))
				{
					layersPtr->MoveUp(i);
				}

				ImGui::EndPopup();
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowCameraControl()
{
	if (ImGui::Button("Фиксировать мир"))
	{
		mPersPtr->cameraMode = MainPerson::CameraMode::SteadyWorld;
	}

	if (ImGui::Button("Фиксировать игрока"))
	{
		mPersPtr->cameraMode = MainPerson::CameraMode::SteadyPerson;
	}
}

/*******************************************/