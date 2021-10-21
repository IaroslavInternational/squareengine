#include "GUISystem.h"

#include "imgui/implot.h"
#include "EngineFunctions.hpp"
#include "HitBox.h"
#include "Line.h"

GUISystem::GUISystem(std::shared_ptr<Window>				 wnd,
					 MainPerson*							 hero,
					 PersonContainer*						 persCon,
					 InteractableObject2DContainer*			 Iobj,
					 ObjectsQueue*							 objQueue,
					 std::shared_ptr<Physics::PhysicsEngine> phEngPtr,
					 std::shared_ptr<Camera>				 camera)
	:
	wnd(wnd),
	hero(hero),
	persCon(persCon),
	IobjCon(Iobj),
	objQueue(objQueue),
	phEngPtr(phEngPtr),
	camera(camera)
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

	animationNames.push_back("Ходьба влево");
	animationNames.push_back("Ходьба вправо");
	animationNames.push_back("Ходьба вверх");
	animationNames.push_back("Ходьба вниз");
	animationNames.push_back("Покой влево");
	animationNames.push_back("Покой вправо");
	animationNames.push_back("Покой вверх");
	animationNames.push_back("Покой вниз");
}

/* Главные методы для отрисовки интерфейса */

void GUISystem::Show(float dt)
{
	ShowMenu();
	ShowLeftSide();
	ShowRightSide(dt);
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

				if (ImGui::MenuItem("Камера"))
				{
					ShowCameraSettings ? ShowCameraSettings = false : ShowCameraSettings = true;
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

				if (ImGui::MenuItem("Интерактивные объекты"))
				{
					if (ShowIobjEnum && ShowIobjSettings)
					{
						DisableSides();

						ShowIobjEnum = false;
						ShowIobjSettings = false;
					}
					else
					{
						DisableSides();

						ShowIobjEnum = true;
						ShowIobjSettings = true;
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

			if (ImGui::MenuItem("Слои"))
			{
				ShowLayersSettings ? ShowLayersSettings = false : ShowLayersSettings = true;
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
	else if (ShowIobjEnum)
	{
		ShowIobjList();
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

void GUISystem::ShowRightSide(float dt)
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
		ShowMainPersonControl(dt);

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowPhysicsEngineObjSettings)
	{
		ShowPhysicsEngineObjControl();

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);

	}
	else if (ShowIobjSettings)
	{
		ShowIobjControl();

		ImGui::SetNextWindowPos({ roundf(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowTriggersSettings)
	{

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

	if (ShowCameraSettings)
	{
		ShowCameraControl();
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
	ShowIobjEnum = false;
	ShowIobjSettings = false;
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
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.52f, 0.85f, 0.059, 1.0f));
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
		for (auto p = persCon->persons.begin(); p != persCon->persons.end(); p++)
		{
			char label[128];
			sprintf_s(label, p->get()->name.c_str(), personSelected);

			std::string contextMenuId = "Context Menu for " + p->get()->name;

			ImGui::Bullet();
			if (ImGui::Selectable(label, personSelected == p->get()->name.c_str()))
			{
				personSelected = p->get()->name;
				LoadedPreview = false;
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("Удалить"))
				{
					std::string deletedPersonName = p->get()->name;
					AddLog("Удаление ");
					AddLog(deletedPersonName.c_str());
					AddLog("...\n");

					objQueue->DeleteObjectAt(p->get()->name);
					persCon->DeletePersonAt(p);
					EngineFunctions::DeleteJsonObject(deletedPersonName, persCon->dataPath);

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
		for (size_t k = 0; k < persCon->persons.size(); k++)
		{
			// Поиск выбранного персонажа
			if (persCon->persons.at(k)->name == personSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					if (ImGui::BeginTabBar("mpSet", ImGuiTabBarFlags_None))
					{
						if (ImGui::BeginTabItem("Объект"))
						{
							SpawnDefaultObject2DControl(persCon->persons.at(k).get(), persCon->dataPath);							

							// КОСТЫЛЬ \\ !
							DirectX::XMFLOAT2 delta;
							delta.x = persCon->persons.at(k)->position.x - persCon->persons.at(k)->hitbox.coordinates.x + persCon->persons.at(k)->dx;
							delta.y = persCon->persons.at(k)->position.y - persCon->persons.at(k)->hitbox.coordinates.y + persCon->persons.at(k)->dy;

							persCon->persons.at(k)->hitbox.Translate(delta);

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Дополнительно"))
						{
							/* Переменные управления сбросом интерфейса */

							bool effDirty = false;		// Котнроль эффекта
							bool speedDirty = false;	// Котнроль скорости

							const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

							/********************************************/

							/* Элементы управления позицией и скорости персонажа */

							if (ImGui::CollapsingHeader("Физика", ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Text("Скорость:");
								dcheck(ImGui::SliderFloat(" ", &persCon->persons.at(k)->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);

								ImGui::Separator(); // Разделитель
							}

							/******************************************/

							/* Элементы управления эффектом персонажа */

							if (ImGui::CollapsingHeader("Эффекты"))
							{
								dcheck(ImGui::SliderFloat("Продолжитель.", &persCon->persons.at(k)->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
								dcheck(ImGui::SliderFloat("Время", &persCon->persons.at(k)->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

								ImGui::Checkbox("Активен", &persCon->persons.at(k)->effect.Active);

								ImGui::Separator();	// Разделитель
							}

							/******************************************/

							/* Элементы управления хитбоксом персонажа */

							if (ImGui::CollapsingHeader("Hit-box"))
							{
								ImGui::Checkbox("Показать", &persCon->persons.at(k)->hitbox.visability);

								/* Если нажата кнопка изменить HitBox */
								{
									if (ImGui::Button("Изменить", ImVec2(100, 20)))
									{
										AddLog("Изменение Hit-box для:");
										AddLog(personSelected.c_str());
										AddLog("\n");

										DrawingHitBox = true;
										persCon->persons.at(k)->hitbox.visability = false;

										ImGui::GetStyle().Alpha = 0.1f;
									}

									if (DrawingHitBox)
									{
										auto new_hb = CreateNewHitBox();

										if (new_hb.name != "empty")
										{
											persCon->persons.at(k)->SetHitBox(new_hb);
											persCon->persons.at(k)->hitbox.visability = true;

											AddLog("Сохранение Hit-box:\n");
											EngineFunctions::SaveHitBoxData(personSelected, persCon->persons.at(k)->hitbox, persCon->dataPath, &applog);
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
									/* Сохранение скорости */

									EngineFunctions::SetNewValue<float>(
										personSelected,
										"speed", persCon->persons.at(k)->speed,
										persCon->dataPath,
										&applog
										);

									/***********************/

									/* Сохранение настроек эффекта */

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

									/* Пересохранение hitbox */
									
									EngineFunctions::SaveHitBoxData(personSelected, persCon->persons.at(k)->hitbox, persCon->dataPath, &applog);

									/*************************/

									SavingSettings = false;
								}
							}
							/************************************************************/

							ImGui::SameLine();

							/* Если нажата кнопка удалить персонажа */
							{
								if (ImGui::Button("Удалить", ImVec2(100, 20)))
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

							ImGui::EndTabItem();
						}

						ImGui::EndTabBar();
					}
					
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
		ImGui::Bullet();
		if (ImGui::Selectable(hero->name.c_str(), heroSelected == hero->name))
		{
			heroSelected = hero->name;
			LoadedPreview = false;
		}
	}

	ImGui::End();
}

void GUISystem::ShowMainPersonControl(float dt)
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (ImGui::BeginChild(""))
		{
			if (ImGui::BeginTabBar("mpSet", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Объект"))
				{
					SpawnDefaultObject2DControl(hero, hero->dataPath);					

					ImGui::EndTabItem();
				}
				
				if (ImGui::BeginTabItem("Дополнительно"))
				{
					/* Переменные управления сбросом интерфейса */

					bool effDirty = false;		// Котнроль эффекта
					bool speedDirty = false;	// Котнроль скорости
					bool a_hdDirty = false;		// Котнроль скорости анимации
					bool a_sDirty = false;		// Котнроль анимации
					bool a_sizeDirty = false;		// Котнроль анимации

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

					/********************************************/

					/* Элементы управления позицией и скорости главного персонажа */

					if (ImGui::CollapsingHeader("Физика", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("Скорость:");
						dcheck(ImGui::SliderFloat(" ", &hero->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);

						ImGui::Separator();	// Разделитель
					}

					/**************************************************************/

					/* Элементы управления эффектом главного персонажа */

					if (ImGui::CollapsingHeader("Эффекты"))
					{
						dcheck(ImGui::SliderFloat("Продолжитель.", &hero->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
						dcheck(ImGui::SliderFloat("Время", &hero->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

						ImGui::Checkbox("Активен", &hero->effect.Active);

						ImGui::Separator();	// Разделитель
					}

					/***************************************************/

					/* Элементы управления хитбоксом главного персонажа */

					if (ImGui::CollapsingHeader("Hit-box"))
					{
						ImGui::Checkbox("Показать", &hero->hitbox.visability);

						/* Если нажата кнопка изменить HitBox */
						{
							if (ImGui::Button("Изменить", ImVec2(100, 20)))
							{
								AddLog("Изменение Hit-box для: ");
								AddLog(hero->name.c_str());
								AddLog("\n");

								DrawingHitBox = true;
								hero->hitbox.visability = false;

								ImGui::GetStyle().Alpha = 0.1f;
							}

							if (DrawingHitBox)
							{
								auto new_hb = CreateNewHitBox();

								if (new_hb.name != "empty")
								{
									hero->SetHitBox(new_hb);
									hero->hitbox.visability = true;

									AddLog("Сохранение Hit-box:\n");
									EngineFunctions::SaveHitBoxData(heroSelected, hero->hitbox, hero->dataPath, &applog);
								}
							}
						}
						/**************************************/

						ImGui::Separator(); // Разделитель
					}

					/****************************************************/

					/* Элементы управления анимацией главного персонажа */
					
					if (ImGui::CollapsingHeader("Анимации"))
					{
						std::ostringstream str;
						str << "Кол-во анимаций: " << hero->animations.size();						
						ImGui::Text(str.str().c_str());
						
						if (ImGui::BeginCombo("Анимация", animSelected.c_str()))
						{
							for (size_t i = 0; i < hero->animations.size(); i++)
							{
								if (ImGui::Selectable(hero->animations[i].name.c_str(), animSelected == hero->animations[i].name))
								{
									animSelected = hero->animations[i].name;
									animSelectedId = i;
								}
							}

							ImGui::EndCombo();
						}

						if (animSelected != "")
						{

							ImGui::SliderInt("Кадр", &hero->animations[animSelectedId].iCurFrame, 0, hero->animations[animSelectedId].frames.size());							
							dcheck(ImGui::SliderFloat("Превью", &scaleFrame, 1.0f, 5.0f, "%.2f"), a_sDirty);

							ImGui::NewLine();

							curFrame = hero->animations[animSelectedId].iCurFrame;

							curAnimW = (float)hero->animations[animSelectedId].width;
							curAnimH = (float)hero->animations[animSelectedId].height;

							previewSize = ImVec2(
								curAnimW * scaleFrame,
								curAnimH * scaleFrame
							);

							ltNormPixel = ImVec2(
								curAnimW + curAnimW * curFrame,
								curAnimH * animSelectedId
								);
						
							rtNormPixel = ImVec2(
								2.0f * curAnimW + curAnimW * curFrame,
									   curAnimH + curAnimH * animSelectedId
							);

							ltNormPixel.x /= sprite_width;
							ltNormPixel.y /= sprite_height;	
							rtNormPixel.x /= sprite_width;
							rtNormPixel.y /= sprite_height;

							ImGui::Text("Превью:");

							ImGui::Image((void*)loadedSprite.Get(),
								previewSize,
								ltNormPixel,
								rtNormPixel);
						}

						ImGui::NewLine();

						dcheck(ImGui::SliderFloat("Задержка", &hero->animations[animSelectedId].holdTime, 0.01f, 1.0f), a_hdDirty);
						
						if(ImGui::Button("Создать анимацию"))
						{
							CreatingAnimation = true;
						}
						
						if (CreatingAnimation)
						{
							auto animations = ShowAnimationCreatingDialog(dt);

							if (!animations.empty())
							{
								CreatingAnimation = false;
							}
						}

						ImGui::Separator();
					}

					/****************************************************/

					/* Элементы управления камерой главного персонажа */
					
					if (ImGui::CollapsingHeader("Камера"))
					{
						SpawnCameraToHeroControl();

						ImGui::Separator();
					}

					/**************************************************/

					ImGui::NewLine();
					ImGui::NewLine();

					/* Если нажата кнопка сохранить текущие настройки главного персонажа */
					{
						if (ImGui::Button("Сохранить", ImVec2(100, 20)))
						{
							AddLog("Сохранение настроек для: ");
							AddLog(hero->name.c_str());
							AddLog("\n");

							SavingSettings = true;
						}

						if (SavingSettings)
						{
							/* Сохранение скорости */

							EngineFunctions::SetNewValue<float>(
								hero->name,
								"speed", hero->speed,
								hero->dataPath,
								&applog
								);

							/**********************/

							/* Сохранение настроек эффекта */

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

							/* Пересохранение hitbox */

							auto actual_hb = hero->hitbox;

							EngineFunctions::SaveHitBoxData(heroSelected, actual_hb, hero->dataPath, &applog);

							/*************************/

							/* Сохранение настроек камеры */
							
							EngineFunctions::SetNewValue<size_t>(
								hero->name,
								"camera-mode", static_cast<size_t>(hero->cameraMode),
								hero->dataPath,
								&applog
								);
							
							/******************************/

							SavingSettings = false;
						}
					}
					/*********************************************************************/

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

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
	if (ImGui::Begin("Настройки слоёв", &ShowLayersSettings, ImGuiWindowFlags_NoResize))
	{
		for (size_t i = 0; i < objQueue->queue.size(); i++)
		{
			std::ostringstream objName;
			objName << "Слой " << objQueue->queue[i]->GetLayer() << ": " << objQueue->queue[i]->GetName();

			char label[128];
			sprintf_s(label, objName.str().c_str(), objectSelected);

			std::string contextMenuId = "Context Menu for " + objName.str();

			ImGui::Bullet();
			if (ImGui::Selectable(label, objectSelected == objName.str().c_str()))
			{
				objectSelected = objName.str();
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("На задний план"))
				{
					objQueue->MoveDown(i);

					for (size_t k = i; k < objQueue->queue.size(); k++)
					{
						if (objQueue->queue[k]->name.find("obj") != objQueue->queue[k]->name.npos)
						{
							for (auto& io : IobjCon->objects)
							{
								if (io->name == objQueue->queue[k]->name)
								{
									if (hero->layer < io->layer)
									{
										io->drawGhostable = true;
									}
								}
							}
						}
					}
				}

				if (ImGui::Button("На передний план"))
				{
					objQueue->MoveUp(i);

					for (size_t k = i; k < objQueue->queue.size(); k++)
					{
						if (objQueue->queue[k]->name.find("obj") != objQueue->queue[k]->name.npos)
						{
							for (auto& io : IobjCon->objects)
							{
								if (io->name == objQueue->queue[k]->name)
								{
									if (hero->layer < io->layer)
									{
										io->drawGhostable = true;
									}
								}
							}
						}
					}
				}

				ImGui::EndPopup();
			}
		}

		ImGui::Separator();

		if (ImGui::Button("Сохранить"))
		{
			SavingLayersSettings = true;
		}

		if (SavingLayersSettings)
		{
			for (size_t i = 0; i < objQueue->queue.size(); i++)
			{
				std::string pathToFile = "";

				if (objQueue->queue[i]->name.find("obj") != objQueue->queue[i]->name.npos)
				{
					pathToFile = IobjCon->dataPath;
				}
				else if (objQueue->queue[i]->name.find("mainperson") != objQueue->queue[i]->name.npos)
				{
					pathToFile = hero->dataPath;
				}
				else
				{
					pathToFile = persCon->dataPath;
				}

				EngineFunctions::SetNewValue<size_t>(
					objQueue->queue[i]->name,
					"layer", objQueue->queue[i]->layer,
					pathToFile,
					&applog);
			}

			SavingLayersSettings = false;
		}
	}

	ImGui::End();
}

void GUISystem::ShowIobjList()
{
	if (ImGui::Begin("Объекты", NULL,
		ImGuiWindowFlags_NoMove   | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto o = IobjCon->objects.begin(); o != IobjCon->objects.end(); o++)
		{
			char label[128];
			sprintf_s(label, o->get()->name.c_str(), IobjSelected);

			std::string contextMenuId = "Context Menu for " + o->get()->name;

			ImGui::Bullet();
			if (ImGui::Selectable(label, IobjSelected == o->get()->name.c_str()))
			{
				IobjSelected = o->get()->name;
				LoadedPreview = false;
				isCaclulatedDeltas = false;
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("Удалить"))
				{
					std::string deletedObjName = o->get()->name;
					AddLog("Удаление ");
					AddLog(deletedObjName.c_str());
					AddLog("...\n");

					objQueue->DeleteObjectAt(o->get()->name);
					IobjCon->DeleteObjectAt(o);

					EngineFunctions::DeleteJsonObject(deletedObjName, IobjCon->dataPath);

					AddLog("Объект ");
					AddLog(deletedObjName.c_str());
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
			AddingIobj = true;
		}

		if (AddingIobj)
		{
			auto d = ShowAddingIobjDialog();

			if (d.value().name != "")
			{
				Surface2D im(d.value().pathToSprite);

				DirectX::XMFLOAT4 hb_coord;
				hb_coord.x = d.value().position.x;
				hb_coord.y = d.value().position.y;
				hb_coord.z = d.value().position.x + im.GetWidth();
				hb_coord.w = d.value().position.y + im.GetHeight();
			
				IobjCon->objects.push_back(std::make_unique<InteractableObject2D>(d.value().name, d.value().position, d.value().layer, d.value().pathToSprite, HitBox(d.value().name + std::string(" hitbox"), hb_coord)));
				objQueue->queue.push_back(IobjCon->objects.back().get());

				using std::to_string;

				// Открытие файла с данными о физике сцены
				std::ifstream dataFile(IobjCon->dataPath);
				if (!dataFile.is_open())
				{
					throw ("Не удаётся открыть файл с данными о физике сцены");
				}

				// Чтение файла
				json j;
				dataFile >> j;

				// Закрытие файла
				dataFile.close();

				// Новый объект
				std::ostringstream newLine;
				newLine << "\"" << d.value().name << "\":[{";

				newLine << "\"pos-x\": "    << d.value().position.x << ",";
				newLine << "\"pos-y\" : "   << d.value().position.y << ",";
				newLine << "\"hb-ltx\" : "  << hb_coord.x << ",";
				newLine << "\"hb-lty\" : "  << hb_coord.y << ",";
				newLine << "\"hb-rbx\" : "  << hb_coord.z << ",";
				newLine << "\"hb-rby\" : "  << hb_coord.w << ",";
				newLine << "\"g-deep\" : "  << 2.0f  << ",";
				newLine << "\"g-able\" : "  << false << ",";
				newLine << "\"layer\" : "   << d.value().layer << ",";
				newLine << "\"path\" : \""  << d.value().pathToSprite << "\"}]";

				// Подготовка к вставке в файл
				std::string json_str = j.dump();
				size_t pos_of_par = json_str.find_last_of('}');
				size_t pos_of_par2 = json_str.find_last_of(']');

				json_str.at(pos_of_par) = ' ';
				json_str.at(pos_of_par2 + 1) = ',';

				// Запись в файл данных новой линии
				std::ofstream ostream(IobjCon->dataPath);
				ostream << json_str + newLine.str() + '}';

				// Закрытие файла
				ostream.close();

				std::ostringstream oss_l;
				oss_l << "Добавлено [" << d.value().name << "]\n";

				AddLog(oss_l.str().c_str());

				AddingIobj = false;
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowIobjControl()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		// Цикл по интерактивным объектам
		for (int k = 0; k < IobjCon->objects.size(); k++)
		{
			// Поиск выбранного интерактивного объекта
			if (IobjCon->objects.at(k)->name == IobjSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					if (ImGui::BeginTabBar("ioSet", ImGuiTabBarFlags_None))
					{
						if (ImGui::BeginTabItem("Объект"))
						{
							SpawnDefaultObject2DControl(IobjCon->objects.at(k).get(), IobjCon->dataPath);
							
							// КОСТЫЛЬ \\ !		
							if (!isCaclulatedDeltas)
							{					
								hb_delta.x = IobjCon->objects.at(k)->hitbox.coordinates.x - IobjCon->objects.at(k)->position.x;
								hb_delta.y = IobjCon->objects.at(k)->hitbox.coordinates.y - IobjCon->objects.at(k)->position.y;

								isCaclulatedDeltas = true;
							}

							DirectX::XMFLOAT2 delta;
							delta.x = IobjCon->objects.at(k)->position.x - IobjCon->objects.at(k)->hitbox.coordinates.x + hb_delta.x;
							delta.y = IobjCon->objects.at(k)->position.y - IobjCon->objects.at(k)->hitbox.coordinates.y + hb_delta.y;

							IobjCon->objects.at(k)->hitbox.Translate(delta);
							
							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Дополнительно"))
						{
							/* Переменные управления сбросом интерфейса */

							bool deepDirty = false; // Контроль позиции

							const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

							/********************************************/

							/* Элементы управления параметрами коллизий объекта */
							
							if (ImGui::CollapsingHeader("Параметры коллизий", ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Text("Степень прозрачности:");
								dcheck(ImGui::SliderFloat("Глубина", &IobjCon->objects.at(k)->deep, 1.0f, 100.0f, "%.3f"), deepDirty);

								ImGui::Checkbox("Прозрачность", &IobjCon->objects.at(k)->drawGhostable);

								ImGui::Separator();
							}

							/****************************************************/
							
							/* Элементы управления хитбоксом интерактивного объекта */

							if (ImGui::CollapsingHeader("Hit-box"))
							{
								ImGui::Checkbox("Показать", &IobjCon->objects.at(k)->hitbox.visability);

								/* Если нажата кнопка изменить HitBox */
								{
									if (ImGui::Button("Изменить", ImVec2(100, 20)))
									{
										AddLog("Изменение Hit-box для:");
										AddLog(IobjSelected.c_str());
										AddLog("\n");

										DrawingHitBox = true;
										IobjCon->objects.at(k)->hitbox.visability = false;

										ImGui::GetStyle().Alpha = 0.1f;
									}

									if (DrawingHitBox)
									{
										auto new_hb = CreateNewHitBox();

										if (new_hb.name != "empty")
										{
											IobjCon->objects.at(k)->SetHitBox(new_hb);
											IobjCon->objects.at(k)->hitbox.visability = true;

											AddLog("Сохранение Hit-box:\n");
											EngineFunctions::SaveHitBoxData(IobjSelected, IobjCon->objects.at(k)->hitbox, IobjCon->dataPath, &applog);
										
											isCaclulatedDeltas = false;
										}
									}
								}
								/**************************************/

								ImGui::Separator(); // Разделитель
							}
							/*******************************************/

							/* Если нажата кнопка сохранить текущие настройки объекта */							
							{
								if (ImGui::Button("Сохранить", ImVec2(100, 20)))
								{
									AddLog("Сохранение настроек для: ");
									AddLog(IobjSelected.c_str());
									AddLog("\n");

									SavingSettings = true;
								}

								if (SavingSettings)
								{
									EngineFunctions::SetNewValue<float>(
										IobjSelected,
										"g-deep", IobjCon->objects.at(k)->deep,
										IobjCon->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<bool>(
										IobjSelected,
										"g-able", IobjCon->objects.at(k)->drawGhostable,
										IobjCon->dataPath,
										&applog
										);

									EngineFunctions::SaveHitBoxData(IobjSelected, IobjCon->objects.at(k)->hitbox, IobjCon->dataPath, &applog);

									SavingSettings = false;
								}
							}
							/**********************************************************/
							
							ImGui::EndTabItem();
						}

						ImGui::EndTabBar();
					}

					ImGui::EndChild();
				}

				break;
			}
		}
	}

	ImGui::End();
}

void GUISystem::SpawnDefaultObject2DControl(Object2D* obj, std::string dataPath)
{
	/* Переменные управления сбросом интерфейса */

	bool posDirty = false; // Контроль позиции
	bool scaleDirty = false; // Контроль позиции

	const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

	/********************************************/

	/* Элементы управления позицией и скорости главного персонажа */

	if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Позиция:");
		dcheck(ImGui::SliderFloat("X", &obj->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &obj->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

		ImGui::NewLine();

		std::ostringstream t;
		t << "Слой: " << obj->layer;
		ImGui::Text(t.str().c_str());

		ImGui::Separator();	// Разделитель
	}

	/**************************************************************/

	/* Элементы управления позицией и скорости главного персонажа */

	if (ImGui::CollapsingHeader("Изображение"))
	{
		if (!LoadedPreview)
		{
			bool ret = wnd->Gfx().LoadTextureFromFile(obj->image.GetFileName().c_str(), loadedSprite.GetAddressOf(), &sprite_width, &sprite_height);
			IM_ASSERT(ret);

			LoadedPreview = true;
		}

		dcheck(ImGui::SliderFloat("Размер", &scaleObj, 0.1f, 20.0f, "%.4f"), scaleDirty);
		
		ImGui::Image((void*)loadedSprite.Get(), ImVec2(sprite_width * scaleObj, sprite_height * scaleObj));

		ImGui::Separator();	// Разделитель

		if (ImGui::Button("Загрузить", ImVec2(100, 20)))
		{
			AddLog("Загрузка спрайта для: ");
			AddLog(obj->name.c_str());
			AddLog("\n");

			LoadingSprite = true;
			LoadedPreview = false;
		}
	}

	if (LoadingSprite)
	{
		std::string imagePath = ShowLoadingSpriteDilaog();

		if (imagePath != "")
		{
			Surface2D im(imagePath);
			obj->SetSurface(im);
			
			if (obj->name.find("obj") != obj->name.npos)
			{
				for (auto& io : IobjCon->objects)
				{
					if (io->name == obj->name)
					{
						DirectX::XMFLOAT4 hb_coord;
						hb_coord.x = obj->position.x;
						hb_coord.y = obj->position.y;
						hb_coord.z = obj->position.x + im.GetWidth();
						hb_coord.w = obj->position.y + im.GetHeight();

						io->hitbox = HitBox(io->name + std::string("hitbox"), hb_coord);

						AddLog("Обновлён Hit-Box ");
						AddLog(" для:");
						AddLog(obj->name.c_str());
						AddLog("\n");

						break;
					}
				}
			}

			AddLog("Загружен спрайт ");
			AddLog(imagePath.c_str());
			AddLog(" для:");
			AddLog(obj->name.c_str());
			AddLog("\n");

			LoadingSprite = false;
		}
	}

	/**************************************************************/

	ImGui::NewLine();
	ImGui::NewLine();

	/* Если нажата кнопка сохранить текущие настройки для объекта */
	{
		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			AddLog("Сохранение настроек для: ");
			AddLog(obj->name.c_str());
			AddLog("\n");

			SavingSettings = true;
		}

		if (SavingSettings)
		{
			/* Сохранение позиции */

			EngineFunctions::SetNewValue<float>(
				obj->name,
				"pos-x", obj->position.x,
				dataPath,
				&applog
				);

			EngineFunctions::SetNewValue<float>(
				obj->name,
				"pos-y", obj->position.y,
				dataPath,
				&applog
				);

			/**********************/

			/* Сохранение изображения */
			
			EngineFunctions::SetNewValue<std::string>(
				obj->name,
				"path", obj->image.GetFileName(),
				dataPath,
				&applog
				);
			
			/**************************/

			SavingSettings = false;
		}
	}
	/*********************************************************************/
}

std::string GUISystem::ShowLoadingSpriteDilaog()
{
	std::string chosenSprite = "";

	SetPanelSizeAndPosition(0, 0.60f, 0.70f, 0.2f, 0.15f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 1.0f));
	if (ImGui::Begin("Загрузить изображение", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		namespace fs = std::filesystem;

		fs::path dir = "Images/";
		size_t col_counter = 1;

		if (ImGui::BeginTable("table1", 5))
		{
			for (fs::directory_iterator it(dir), end; it != end; ++it)
			{
				if (it->path().extension() == ".bmp")
				{
					if (col_counter == 1)
					{
						ImGui::TableNextRow();
					}

					if (col_counter == 6)
					{
						ImGui::TableNextRow();
						col_counter = 1;
					}

					ImGui::TableNextColumn();

					bool ret = wnd->Gfx().LoadTextureFromFile("Icons/bmp_icon.bmp", loadedSprite.GetAddressOf(), &sprite_width, &sprite_height);
					IM_ASSERT(ret);

					ImGui::Image((void*)loadedSprite.Get(), ImVec2((float)sprite_width, (float)sprite_height));

					if (ImGui::IsItemClicked())
					{
						chosenSprite = it->path().generic_string();
					}

					ImGui::Text(it->path().filename().string().c_str());
				}

				col_counter++;
			}
		}

		ImGui::EndTable();

		ImGui::NewLine();

		if (ImGui::Button("Отмена"))
		{
			ChosingIobj = false;
			LoadingSprite = false;
			AddingIobj = false;
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();

	return chosenSprite;
}

std::optional<IobjData> GUISystem::ShowAddingIobjDialog()
{
	IobjData data;

	SetPanelSizeAndPosition(0, 0.3f, 0.2f, 0.35f, 0.4f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 1.0f));
	if (ImGui::Begin("Добавление объекта", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Путь к спрайту: ");
		
		if (IobjPath == "")
		{
			ImGui::Text("...");
		}
		else
		{
			ImGui::Text(IobjPath.c_str());
		}
		
		ImGui::SameLine();
		
		if (ImGui::Button("Выбрать"))
		{
			ChosingIobj = true;
		}

		if (ChosingIobj)
		{
			IobjPath = ShowLoadingSpriteDilaog();

			if (IobjPath != "")
			{
				std::ostringstream n;
				n << "obj " << IobjCon->objects.size();
				
				data.name = n.str();
				data.pathToSprite = IobjPath;
				data.position = DirectX::XMFLOAT2(0.0f, 0.0f);
				data.layer = objQueue->queue.size();
				
				ChosingIobj = false;
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Отмена"))
		{
			ChosingIobj = false;
			LoadingSprite = false;
			AddingIobj = false;
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();

	return data;
}

HitBox GUISystem::CreateNewHitBox()
{
	HitBox hb_new("empty", 0.0f, 0.0f, 0.0f, 0.0f);

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

			hb_new = HitBox(hero->name + std::string(" hitbox"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);

			std::ostringstream oss;
			oss << "Поставлена вторая точка:\n" <<
				"[x: " << secondPoint.x <<
				"; y: " << secondPoint.y << "]\n";

			AddLog(oss.str().c_str());

			mouseHelpInfo = "";
			SettedSecondPoint = true;
		}
	}
	else if (SettedFirstPoint && SettedSecondPoint)
	{
		SettedFirstPoint = false;
		SettedSecondPoint = false;
		DrawingHitBox = false;

		ImGui::GetStyle().Alpha = 1.0f;
	}

	return hb_new;
}

void GUISystem::ShowCameraControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.25f),
			  DispSize.y * 0.5f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("Камера", &ShowCameraSettings, ImGuiWindowFlags_NoResize))
	{
		/* Переменные управления сбросом интерфейса */

		bool posDirty = false;	 // Контроль позиции
		bool speedDirty = false; // Контроль скорости

		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

		/********************************************/

		/* Элементы управления позицией камеры */
		
		if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Текущая позиция:");
			std::ostringstream cPos;
			cPos << "X: " << camera->position.x << " Y: " << camera->position.y;
			ImGui::Text(cPos.str().c_str());

			ImGui::Text("Исходная позиция:");
			dcheck(ImGui::SliderFloat("X", &camera->initPosition.x, -1000.0f, 1000.0f), posDirty);
			dcheck(ImGui::SliderFloat("Y", &camera->initPosition.y, -1000.0f, 1000.0f), posDirty);

			if (ImGui::Button("Вернуть на исходную позицию"))
			{
				AddLog("Возвращение камеры на исходную позицию...\n");
				camera->SetPosition(camera->initPosition);

				std::ostringstream str;
				str << "Камера установлена по кординатам: " << "X: " << camera->position.x << " Y: " << camera->position.y << "\n";
				AddLog(str.str().c_str());
			}					   

			ImGui::Separator();
		}
		
		/***************************************/

		/* Элементы управления no-clip камеры */
		
		if (ImGui::CollapsingHeader("Перемещение"))
		{
			ImGui::Checkbox("No-clip", &camera->noclip);
			dcheck(ImGui::SliderFloat("Скорость No-clip", &camera->noclipSpeed, 1.0f, 1000.0f), speedDirty);

			ImGui::Separator();
		}

		/**************************************/

		/* Элементы управления взаимодействия камеры с игроком */
		
		if (ImGui::CollapsingHeader("Взаимодействие с игроком"))
		{
			SpawnCameraToHeroControl();

			ImGui::Separator();
		}

		/*******************************************************/

		if (ImGui::Button("Сохранить"))
		{
			SavingSettings = true;
		}

		if (SavingSettings)
		{
			EngineFunctions::SetNewValue<float>(
				"camera",
				"pos-x", camera->initPosition.x,
				camera->dataPath,
				&applog
				);	
			
			EngineFunctions::SetNewValue<float>(
				"camera",
				"pos-y", camera->initPosition.y,
				camera->dataPath,
				&applog
				);

			EngineFunctions::SetNewValue<float>(
				"camera",
				"nc-speed", camera->noclipSpeed,
				camera->dataPath,
				&applog
				);

			EngineFunctions::SetNewValue<bool>(
				"camera",
				"nc-able", camera->noclip,
				camera->dataPath,
				&applog
				);

			EngineFunctions::SetNewValue<size_t>(
				hero->name,
				"camera-mode", static_cast<size_t>(hero->cameraMode),
				hero->dataPath,
				&applog
				);

			SavingSettings = false;
		}
	}

	ImGui::End();
}

std::vector<Animation> GUISystem::ShowAnimationCreatingDialog(float dt)
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.6f),
		DispSize.y * 0.7f
	);

	SetPanelSizeAndPosition(0, 0.60f, 0.70f, 0.2f, 0.1f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 1.0f));
	if (ImGui::Begin("Создание/Изменение анимации", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize))
	{
		if (ImGui::CollapsingHeader("Загрузка", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Путь к спрайту: ");

			if (animPath == "")
			{
				ImGui::Text("...");
			}
			else
			{
				ImGui::Text(animPath.c_str());
			}

			ImGui::SameLine();

			if (ImGui::Button("Выбрать"))
			{
				ChoosingAnimation = true;
				CreatingAnimtionLoaded = false;
				animPath = "";
			}

			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Спрайт", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ChoosingAnimation)
			{
				if (animPath != "")
				{
					//
					ImGui::BeginChild("Превью спрайта", { PanelSize.x / 2 - 2, PanelSize.y  });
					
					if (!CreatingAnimtionLoaded)
					{
						bool ret = wnd->Gfx().LoadTextureFromFile(animPath.c_str(), loadedSprite.GetAddressOf(), &sprite_width, &sprite_height);
						IM_ASSERT(ret);

						CreatingAnimtionLoaded = true;
					}

					bool sizesDirty = false;
					bool scaleDirty = false; // Контроль размера
					bool a_sDirty = false; // Контроль размера
					bool a_fDirty = false; // Контроль размера

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

					dcheck(ImGui::SliderFloat("Размер", &scaleObj, 0.1f, 20.0f, "%.4f"), scaleDirty);
					ImGui::Image((void*)loadedSprite.Get(), ImVec2(sprite_width * scaleObj, sprite_height * scaleObj));
					
					ImGui::EndChild();

					ImGui::SameLine();
					
					//
					ImGui::BeginChild("Настройки анимации", { PanelSize.x / 2 - 2, PanelSize.y });
					
					if (ImGui::BeginCombo("Тип анимации", newAnimNameSelected.c_str()))
					{
						for (size_t i = 0; i < animationNames.size(); i++)
						{
							if (ImGui::Selectable(animationNames[i].c_str()))
							{
								newAnimNameSelected = animationNames[i];
								animSelectedId = i;
							}
						}

						ImGui::EndCombo();
					}

					curAnimW = newFrameWidth;
					curAnimH = newFrameHeight;

					previewSize = ImVec2(
						curAnimW * scaleFrame,
						curAnimH * scaleFrame
					);

					ltNormPixel = ImVec2(
						curAnimW + curAnimW * curFrame,
						curAnimH * animSelectedId
					);

					rtNormPixel = ImVec2(
						2.0f * curAnimW + curAnimW * curFrame,
						curAnimH + curAnimH * animSelectedId
					);

					ltNormPixel.x /= sprite_width;
					ltNormPixel.y /= sprite_height;
					rtNormPixel.x /= sprite_width;
					rtNormPixel.y /= sprite_height;


					ImGui::SetCursorPosX((PanelSize.x / 2 - 2 - curAnimW * scaleFrame) / 2);
					ImGui::Image((void*)loadedSprite.Get(),
						previewSize,
						ltNormPixel,
						rtNormPixel);
					ImGui::SetCursorPosX((PanelSize.x / 2 - 2 - ImGui::CalcTextSize("Превью кадра").x) / 2);
					ImGui::Text("Превью кадра");

					dcheck(ImGui::SliderFloat("Ширина", &newFrameWidth, 0.1f, 500.0f, "%.4f"), sizesDirty);
					dcheck(ImGui::SliderFloat("Высота", &newFrameHeight, 0.1f, 500.0f, "%.4f"), sizesDirty);
					dcheck(ImGui::SliderFloat("Размер превью", &scaleFrame, 1.0f, 5.0f, "%.2f"), a_sDirty);
					dcheck(ImGui::SliderInt("Кол-во кадров", &maxFrames, 0, 16), a_fDirty);
					dcheck(ImGui::SliderInt("Кадр", &curFrame, 0, maxFrames), a_fDirty);

					if (ImGui::Button("Создать анимацию"))
					{
						animationsPreview.emplace_back(Animation((int)newFrameWidth, (int)newFrameHeight * animSelectedId, (int)newFrameWidth, (int)newFrameHeight, maxFrames, Surface2D(animPath), 0.16f, animationNames[animSelectedId]));
					}

					if (!animationsPreview.empty())
					{
						ImGui::Separator();

						std::ostringstream amAnim;
						amAnim << "Создано " << animationsPreview.size() << " анимаций:";
						ImGui::Text(amAnim.str().c_str());

						for (auto& a : animationsPreview)
						{
							ImGui::Text(a.name.c_str());

							if (a.name == "Ходьба влево")
							{
								animPlayingId = 0;
							}
							else if (a.name == "Ходьба вправо")
							{
								animPlayingId = 1;
							}
							else if (a.name == "Ходьба вверх")
							{
								animPlayingId = 2;
							}
							else if (a.name == "Ходьба вниз")
							{
								animPlayingId = 3;
							}

							previewSize = ImVec2(
								a.width * scaleFrame,
								a.height * scaleFrame
							);

							ltNormPixel = ImVec2(
								a.width + a.width * a.iCurFrame,
								a.height * animPlayingId
							);

							rtNormPixel = ImVec2(
								2.0f * a.width + a.width * a.iCurFrame,
								a.height + a.height * animPlayingId
							);

							ltNormPixel.x /= sprite_width;
							ltNormPixel.y /= sprite_height;
							rtNormPixel.x /= sprite_width;
							rtNormPixel.y /= sprite_height;

							ImGui::Image((void*)loadedSprite.Get(),
								previewSize,
								ltNormPixel,
								rtNormPixel);

							a.Update(dt);

							ImGui::Separator();
						}

						if (ImGui::Button("Очистить"))
						{
							animationsPreview.clear();
						}
					}

					ImGui::EndChild();
					
					//ChoosingAnimation = false;
				}
				else
				{
					animPath = ShowLoadingSpriteDilaog();
				}
			}

			ImGui::Separator();
		}

		ImGui::NewLine();

		if (ImGui::Button("Отмена"))
		{
			LoadingSprite = false;
			CreatingAnimation = false;
		}

		ImGui::End();
	}
	ImGui::PopStyleColor();

	return std::vector<Animation>();
}

void GUISystem::SpawnCameraToHeroControl()
{
	if (ImGui::Button("Фиксировать мир"))
	{
		hero->cameraMode = MainPerson::CameraMode::SteadyWorld;
	}

	if (ImGui::Button("Фиксировать игрока"))
	{
		hero->cameraMode = MainPerson::CameraMode::SteadyPerson;
	}
}

/*******************************************/