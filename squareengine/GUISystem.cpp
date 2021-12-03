#include "GUISystem.h"

#include "imgui/implot.h"
#include "EngineFunctions.hpp"
#include "HitBox.h"
#include "Line.h"

#include <future>

#define BIG_POPUP_PANEL_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
#define SIDE_PANEL_FLAGS      BIG_POPUP_PANEL_FLAGS   | ImGuiWindowFlags_NoBringToFrontOnFocus

GUISystem::GUISystem(Scene* scene)
	:
	curSceneName(scene->name),
	wnd(scene->wnd),
	hero(&scene->world.hero),
	persons(&scene->world.persons),
	Iobjects(&scene->world.Iobjects),
	objQueue(&scene->world.objQueue),
	triggers(&scene->world.triggers),
	phEngPtr(scene->phEngine),
	nEditor(),
	camera(scene->camera),
	animSpritePreview(1, 1),
	viewportWidth(wnd->Gfx().GetWidth()),
	viewportHeight(wnd->Gfx().GetHeight())
{
	for (size_t i = 0; i < cameraModeNames.size(); i++)
	{
		if (cameraModeNames[i].second == hero->cameraMode)
		{
			modeSelected == cameraModeNames[i].first;
			break;
		}
	}

	AddLog("Установка цветовой гаммы GUI...\n");
	SetGUIColors();
	AddLog("Установка выполнена\n");

	for (size_t i = 0; i < N_POINTS; i++)
	{
		counters[i] = float(i);
	}
	
	AddLog("Получение данных о графическом адаптере...\n");
	auto GPU_Data = AdapterReader::GetAdapterData();

	for (auto& d : GPU_Data)
	{
		if (d.desc.DedicatedVideoMemory != 0)
		{
			gpu_desc.emplace(std::wstring(d.desc.Description), round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
		}
	}
	AddLog("Данные получены\n");

	AddLog("Установка вомзожных вариантов анимаций...\n");
	animationNames.push_back("Ходьба влево");
	animationNames.push_back("Ходьба вправо");
	animationNames.push_back("Ходьба вверх");
	animationNames.push_back("Ходьба вниз");
	animationNames.push_back("Покой влево");
	animationNames.push_back("Покой вправо");
	animationNames.push_back("Покой вверх");
	animationNames.push_back("Покой вниз");
	AddLog("Установка выполнена\n");
}

/* Главные методы для отрисовки интерфейса */

void GUISystem::Show(float dt)
{
	ShowMenu();

	if (IsShow)
	{
		ShowLeftSide(dt);
		ShowRightSide(dt);
		ShowLeftBottomSide();
		ShowBottomPanel();
		ShowOptionalPanel();
	}
}

void GUISystem::Hide()
{
	AddLog("Скрывание GUI...\n");
	
	IsShow = false;

	AddLog("GUI скрыт\n");
}

void GUISystem::BeginFrame()
{
	nEditor.BeginFrame();
}

void GUISystem::EndFrame()
{
	nEditor.EndFrame();
}

void GUISystem::AddLog(const std::ostringstream& oss)
{
	AddLog(oss.str().c_str());
}

void GUISystem::AddLog(std::string str)
{
	AddLog(str.c_str());
}

void GUISystem::AddLog(const char* text)
{
	applog.AddLog(text);
}

void GUISystem::LoadScene(Scene* scene)
{	
	AddLog("Загрузка сцены...\n");
	IsUpdatingScene = false;

	curSceneName = scene->name;
	wnd			 = scene->wnd;
	hero		 = &scene->world.hero;
	persons		 = &scene->world.persons;
	Iobjects	 = &scene->world.Iobjects;
	objQueue	 = &scene->world.objQueue;
	phEngPtr	 = scene->phEngine;
	camera		 = scene->camera;

	AddLog("Сцена ");
	AddLog(scene->name);
	AddLog(" загружена\n");
}

std::pair<bool, std::string> GUISystem::UpdatingScene()
{
	return std::pair<bool, std::string>(IsUpdatingScene, curSceneName);
}

bool GUISystem::AddingScene()
{
	return IsAddingScene;
}

void GUISystem::SetAddingSceneState(bool state)
{
	IsAddingScene = state;
}

bool GUISystem::IsTriggersAble()
{
	return IsTriggersAvailable;
}

/*******************************************/

/* Методы настройки и отрисовки панелей */

void GUISystem::SetGUIColors()
{
	/* Стиль интерфейса */

	ImGui::GetStyle().FrameRounding    = 4.0f;								// Закругление компонентов
	ImGui::GetStyle().WindowBorderSize = 0.0f;								// Размер границы
	ImGui::GetStyle().WindowRounding   = 10.0f;								// Закругление окон
	ImGui::GetStyle().GrabRounding     = 10.0f;								// Закругление ползунка
	ImGui::GetStyle().GrabMinSize      = 20.0f;								// Мин. размер ползунка
	
	// Цвета
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_MenuBarBg]        = ImVec4(0.15f, 0.36f, 0.39f, 1.00f);	// Главное меню
	colors[ImGuiCol_WindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.84f);	// Фон окна
	colors[ImGuiCol_TitleBg]          = ImVec4(0.24f, 0.00f, 0.20f, 0.73f);	// Меню окна
	colors[ImGuiCol_TitleBgActive]    = ImVec4(0.15f, 0.00f, 0.07f, 0.73f); // Наведение на меню окна
	colors[ImGuiCol_FrameBg]	      = ImVec4(0.00f, 0.50f, 0.38f, 0.54f);	// Компонента
	colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.00f, 0.18f, 0.15f, 0.40f);	// Наведение на компоненту
	colors[ImGuiCol_FrameBgActive]    = ImVec4(0.06f, 0.48f, 0.45f, 0.67f);	// Активные компоненты
	colors[ImGuiCol_CheckMark]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);	// Галочка
	colors[ImGuiCol_SliderGrab]       = ImVec4(0.37f, 0.70f, 0.00f, 1.00f);	// Ползунок слайдера
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.82f, 0.19f, 1.00f);	// Активный ползунок слайдера
	colors[ImGuiCol_Button]			  = ImVec4(0.56f, 0.05f, 0.05f, 0.59f);	// Кнопка
	colors[ImGuiCol_ButtonHovered]    = ImVec4(0.26f, 0.01f, 0.17f, 1.00f);	// Наведение на кнопку
	colors[ImGuiCol_ButtonActive]     = ImVec4(0.03f, 0.55f, 0.48f, 1.00f);	// Активная кнопка
	colors[ImGuiCol_Separator]		  = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);	// Разделитель
	colors[ImGuiCol_Tab]			  = ImVec4(0.43f, 0.00f, 0.00f, 0.86f);	// Раздел
	colors[ImGuiCol_TabHovered]		  = ImVec4(0.15f, 0.00f, 0.00f, 0.80f);	// Наведение на раздел
	colors[ImGuiCol_TabActive]		  = ImVec4(0.11f, 0.63f, 0.14f, 0.53f);	// Активный раздел
	colors[ImGuiCol_ScrollbarGrab]    = ImVec4(0.19f, 0.67f, 0.65f, 1.00f);	// Ползунок
	colors[ImGuiCol_TableHeaderBg]    = ImVec4(0.31f, 0.04f, 0.04f, 0.81f);	// Блок заголовка таблицы
	colors[ImGuiCol_Header]			  = ImVec4(0.50f, 0.09f, 0.70f, 0.31f);	// Заголовок
	colors[ImGuiCol_HeaderActive]	  = ImVec4(0.00f, 0.57f, 0.49f, 1.00f);	// Активный заголовок
	colors[ImGuiCol_HeaderHovered]	  = ImVec4(0.40f, 0.22f, 0.59f, 0.80f);	// Наведение на заголовк

	/********************/
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
			if (ImGui::MenuItem("Сохранить всё"))
			{
				auto thread = std::async(&GUISystem::SaveAll, this);
				thread.get();
			}

			if (ImGui::MenuItem("Выход"))
			{
				AddLog("Завершение работы...");
				exit(0);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Окна"))
		{
			if (ImGui::MenuItem("Сцены"))
			{
				ShowScenesSettings ? ShowScenesSettings = false : ShowScenesSettings = true;
			}

			if (ImGui::BeginMenu("Система"))
			{
				if (ImGui::MenuItem("Игра"))
				{
					ShowScriptsSettings ? ShowScriptsSettings = false : ShowScriptsSettings = true;
				}

				if (ImGui::BeginMenu("Графика"))
				{
					if (ImGui::MenuItem("Viewport"))
					{
						ShowViewportSettings ? ShowViewportSettings = false : ShowViewportSettings = true;
					}

					if (ImGui::MenuItem("Настройки"))
					{
						ShowGraphicsSettings ? ShowGraphicsSettings = false : ShowGraphicsSettings = true;
					}

					ImGui::EndMenu();
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

				if (ImGui::MenuItem("Проект"))
				{
					ShowProjectSettings ? ShowProjectSettings = false : ShowProjectSettings = true;
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

				if (ImGui::BeginMenu("Триггеры"))
				{
					if (ImGui::MenuItem("Объекты"))
					{
						if (ShowTriggersEnum && ShowTriggersSettings)
						{
							DisableSides();

							ShowTriggersEnum = false;
							ShowTriggersSettings = false;
						}
						else
						{
							DisableSides();

							ShowTriggersEnum = true;
							ShowTriggersSettings = true;
						}
					}

					if (ImGui::MenuItem("Контроль"))
					{
						ShowTriggerInfoSettings ? ShowTriggerInfoSettings = false : ShowTriggerInfoSettings = true;
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Камера"))
			{
				ShowCameraSettings ? ShowCameraSettings = false : ShowCameraSettings = true;
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
				ShowFPSChart ? ShowFPSChart = false : ShowFPSChart = true;
			}

			if (ImGui::MenuItem("Лог"))
			{
				ShowLogs ? ShowLogs = false : ShowLogs = true;
			}

			std::string title = "";

			if (IsShow)
			{
				title = "Скрыть всё";
			}
			else
			{
				title = "Показать всё";
			}

			if (ImGui::MenuItem(title.c_str()))
			{
				IsShow ? IsShow = false : IsShow = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void GUISystem::ShowLeftSide(float dt)
{
	/* Левая сторона */

	SetPanelSizeAndPosition(0, 0.2f, 0.75f);

	/* Содержимое */

	if (ShowPersonEnum)
	{
		ShowPersonList(dt);
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
	else if (ShowTriggersEnum)
	{
		ShowTriggerList();
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

	if (ShowMainPersonSettings)
	{
		ShowMainPersonControl(dt);
	}
	else if (ShowPersonEnum)
	{
		ShowPersonControl(dt);
	}
	else if (ShowPhysicsEngineObjSettings)
	{
		ShowPhysicsEngineObjControl();

	}
	else if (ShowIobjSettings)
	{
		ShowIobjControl();
	}
	else if (ShowTriggersSettings)
	{
		ShowTriggerControl();
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
		SetPanelSizeAndPosition(0, 0.25f, 0.65f, 0.375f, 0.1625f);	
		ShowPhysicsEngineSettings();
	}
	else if (ShowPhysicsEngineObjInfo)
	{
		ShowPhysicsEngineObjHelp();
	}

	if (ShowGraphicsSettings)
	{
		SetPanelSizeAndPosition(0, 0.25f, 0.65f, 0.375f, 0.1625f);
		ShowGraphicsEngineSettings();
	}

	if (ShowViewportSettings)
	{
		SetPanelSizeAndPosition(0, 0.25f, 0.65f, 0.375f, 0.1625f);
		ShowViewportControl();
	}

	if (ShowProjectSettings)
	{
		SetPanelSizeAndPosition(0, 0.25f, 0.65f, 0.375f, 0.1625f);
		ShowProjectControl();
	}

	if (ShowFPSChart)
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

	if (ShowScenesSettings)
	{
		ShowScenesControl();
	}

	if (ShowScriptsSettings)
	{
		ShowScriptsControl();
	}

	if (ShowTriggerInfoSettings)
	{
		ShowTriggerInfoControl();
	}

	if (mouseHelpInfo == "")
	{
		std::ostringstream pos;
		pos << "x: "   << ImGui::GetMousePos().x + camera->position.x
			<< "\ny: " << ImGui::GetMousePos().y + camera->position.y;

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
	ShowTriggersEnum = false;
	ShowTriggersSettings = false;
	ShowPhysicsSettings = false;
	ShowFPSChart = false;
	ShowIobjEnum = false;
	ShowIobjSettings = false;
}

/****************************************/

/* Методы отрисовки конкретных интерфейсов */

void GUISystem::ShowMainPersonList()
{
	if (draggingObjId == -1)
	{
		if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			if ((hero->hitbox - DirectX::XMFLOAT2(hero->dx, hero->dy)).IsOverlap({ mPosX, mPosY }))
			{
				draggingObjId = 0;

				AddLog("Перемещение мышкой объекта ");
				AddLog(hero->name);
				AddLog("\n");
			}
		}
	}
	else
	{
		if (!wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			hero->hitbox.visability = true;
			draggingObjId = -1;

			AddLog("Перемещение завершенно\n");
		}
		else
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			float dx = hero->dx;
			float dy = hero->dy;

			hero->hitbox.visability = false;

			hero->SetPosition({ mPosX, mPosY });
			hero->hitbox.coordinates.z = hero->position.x + dx + fabs(hero->hitbox.coordinates.z - hero->hitbox.coordinates.x);
			hero->hitbox.coordinates.w = hero->position.y + dy + fabs(hero->hitbox.coordinates.w - hero->hitbox.coordinates.y);
			hero->hitbox.coordinates.x = hero->position.x + dx;
			hero->hitbox.coordinates.y = hero->position.y + dy;
		}
	}

	if (ImGui::Begin("Персонажи", NULL, SIDE_PANEL_FLAGS))
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

void GUISystem::ShowPersonList(float dt)
{
	if (draggingObjId == -1)
	{
		if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			for (size_t i = 0; i < persons->elements.size(); i++)
			{
				if ((persons->elements[i]->hitbox - DirectX::XMFLOAT2(persons->elements[i]->dx, persons->elements[i]->dy)).IsOverlap({ mPosX, mPosY }))
				{
					draggingObjId = i;

					AddLog("Перемещение мышкой объекта ");
					AddLog(persons->elements[i]->name);
					AddLog("\n");
					
					break;
				}
			}
		}
	}
	else
	{
		if (!wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			persons->elements[draggingObjId]->hitbox.visability = true;
			draggingObjId = -1;

			AddLog("Перемещение завершенно\n");
		}
		else
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			float dx = persons->elements[draggingObjId]->dx;
			float dy = persons->elements[draggingObjId]->dy;

			persons->elements[draggingObjId]->hitbox.visability = false;

			persons->elements[draggingObjId]->SetPosition({ mPosX, mPosY });
			persons->elements[draggingObjId]->hitbox.coordinates.z = persons->elements[draggingObjId]->position.x + dx + fabs(persons->elements[draggingObjId]->hitbox.coordinates.z - persons->elements[draggingObjId]->hitbox.coordinates.x);
			persons->elements[draggingObjId]->hitbox.coordinates.w = persons->elements[draggingObjId]->position.y + dy + fabs(persons->elements[draggingObjId]->hitbox.coordinates.w - persons->elements[draggingObjId]->hitbox.coordinates.y);
			persons->elements[draggingObjId]->hitbox.coordinates.x = persons->elements[draggingObjId]->position.x + dx;
			persons->elements[draggingObjId]->hitbox.coordinates.y = persons->elements[draggingObjId]->position.y + dy;
		}
	}

	if (ImGui::Begin("Персонажи", NULL, SIDE_PANEL_FLAGS))
	{
		for (auto p = persons->elements.begin(); p != persons->elements.end(); p++)
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
					AddLog("Удаление персонажа...\n");

					std::string deletedPersonName = p->get()->name;

					objQueue->DeleteObjectAt(p->get()->name);
					persons->DeleteAt(p);
					EngineFunctions::DeleteJsonObject(deletedPersonName, persons->dataPath);

					AddLog("Персонаж ");
					AddLog(deletedPersonName);
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
			IsAddingPerson = true;
		}

		if (IsAddingPerson)
		{	
			if (!GotPersonData)
			{
				optPdata = ShowAddingPersonDialog(dt);
			}

			if (optPdata.value().name != "")
			{
				GotPersonData = true;

				optPdata.value().animPersonData = ShowAnimationCreatingDialog(dt);

				if (!optPdata.value().animPersonData.empty())
				{
					AddLog("Создана анимация для ");
					AddLog("персонажа\n");

					std::vector<Animation> newAnim;

					for (auto& a : optPdata.value().animPersonData)
					{
						newAnim.emplace_back(Animation(a.pStart, a.pEnd, a.width, a.height, a.frames, hero->image, a.ft, a.name));
					}

					auto data = optPdata.value();

					DirectX::XMFLOAT4 hb_coord;
					hb_coord.x = optPdata.value().position.x;
					hb_coord.y = optPdata.value().position.y;
					hb_coord.z = optPdata.value().position.x + data.animPersonData[0].width;
					hb_coord.w = optPdata.value().position.y + data.animPersonData[0].height;

					persons->elements.push_back(std::make_unique<Person>(data.name, data.position, 100.0f, 10.0f, data.layer, data.pathToSprite, Color(255, 0, 255), HitBox(data.name + std::string(" hitbox"), hb_coord), data.animPersonData[0], ""));
					objQueue->queue.push_back(persons->elements.back().get());

					using std::to_string;

					// Открытие файла с данными о физике сцены
					std::ifstream dataFile(persons->dataPath);
					if (!dataFile.is_open())
					{
						throw ("Не удаётся открыть файл с данными о персонажах");
					}

					// Чтение файла
					json j;
					dataFile >> j;

					// Закрытие файла
					dataFile.close();

					// Новый объект
					std::ostringstream newLine;
					newLine << "\"" << data.name << "\":[{";

					newLine << "\"pos-x\": "   << data.position.x << ",";
					newLine << "\"pos-y\" : "  << data.position.y << ",";
					newLine << "\"hb-ltx\" : " << hb_coord.x << ",";
					newLine << "\"hb-lty\" : " << hb_coord.y << ",";
					newLine << "\"hb-rbx\" : " << hb_coord.z << ",";
					newLine << "\"hb-rby\" : " << hb_coord.w << ",";
					newLine << "\"layer\" : "  << data.layer << ",";
					newLine << "\"speed\" : "  << 100.0f     << ",";
					newLine << "\"eff-a\" : "  << false      << ",";
					newLine << "\"eff-d\" : "  << 0.045f     << ",";
					newLine << "\"eff-t\" : "  << 0.0f       << ",";
					newLine << "\"name\" : \"" << data.name  << "\",";
					newLine << "\"a-fa\" : "   << data.animPersonData[0].frames  << ",";
					newLine << "\"a-fw\" : "   << data.animPersonData[0].width   << ",";
					newLine << "\"a-fh\" : "   << data.animPersonData[0].height  << ",";
					newLine << "\"a-ft\" : "   << data.animPersonData[0].ft  << ",";
					newLine << "\"a-ps\" : "   << data.animPersonData[0].pStart  << ",";
					newLine << "\"a-pe\" : "   << data.animPersonData[0].pEnd    << ",";
					newLine << "\"chr-r\" : "  << 255							 << ",";
					newLine << "\"chr-g\" : "  << 0								 << ",";
					newLine << "\"chr-b\" : "  << 255							 << ",";

					newLine << "\"script\" : " << ""							 << ",";
					newLine << "\"path\" : \"" << data.pathToSprite << "\"}]";

					// Подготовка к вставке в файл
					std::string json_str = j.dump();
					size_t pos_of_par = json_str.find_last_of('}');
					size_t pos_of_par2 = json_str.find_last_of(']');

					json_str.at(pos_of_par) = ' ';

					if (j.size() != 0)
					{
						json_str.at(pos_of_par2 + 1) = ',';
					}
					else
					{
						json_str.at(pos_of_par2 + 1) = '{';
					}

					// Запись в файл данных новой линии
					std::ofstream ostream(persons->dataPath);
					ostream << json_str + newLine.str() + '}';

					// Закрытие файла
					ostream.close();

					AddLog("Персонаж ");
					AddLog(data.name);
					AddLog(" добавлен\n");

					IsAddingPerson = false;
				}
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowIobjList()
{
	if (draggingObjId == -1)
	{
		if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			for (size_t i = 0; i < Iobjects->elements.size(); i++)
			{
				if (Iobjects->elements[i]->hitbox.IsOverlap({ mPosX, mPosY }))
				{
					draggingObjId = i;
					
					AddLog("Перемещение мышкой объекта ");
					AddLog(Iobjects->elements[i]->name);
					AddLog("\n");

					break;
				}
			}
		}
	}
	else
	{
		if (!wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			Iobjects->elements[draggingObjId]->hitbox.visability = true;
			draggingObjId = -1;		

			AddLog("Перемещение завершенно\n");
		}
		else
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			float dx = Iobjects->elements[draggingObjId]->hitbox.coordinates.x - Iobjects->elements[draggingObjId]->position.x;
			float dy = Iobjects->elements[draggingObjId]->hitbox.coordinates.y - Iobjects->elements[draggingObjId]->position.y;

			Iobjects->elements[draggingObjId]->hitbox.visability = false;

			Iobjects->elements[draggingObjId]->SetPosition({ mPosX, mPosY });
			Iobjects->elements[draggingObjId]->hitbox.coordinates.z = Iobjects->elements[draggingObjId]->position.x + dx + fabs(Iobjects->elements[draggingObjId]->hitbox.coordinates.z - Iobjects->elements[draggingObjId]->hitbox.coordinates.x);
			Iobjects->elements[draggingObjId]->hitbox.coordinates.w = Iobjects->elements[draggingObjId]->position.y + dy + fabs(Iobjects->elements[draggingObjId]->hitbox.coordinates.w - Iobjects->elements[draggingObjId]->hitbox.coordinates.y);
			Iobjects->elements[draggingObjId]->hitbox.coordinates.x = Iobjects->elements[draggingObjId]->position.x + dx;
			Iobjects->elements[draggingObjId]->hitbox.coordinates.y = Iobjects->elements[draggingObjId]->position.y + dy;
		}
	}

	if (ImGui::Begin("Объекты", NULL, SIDE_PANEL_FLAGS))
	{
		for (auto o = Iobjects->elements.begin(); o != Iobjects->elements.end(); o++)
		{
			char label[128];
			sprintf_s(label, o->get()->name.c_str(), IobjSelected);

			std::string contextMenuId = "Context Menu for " + o->get()->name;

			ImGui::Bullet();
			if (ImGui::Selectable(label, IobjSelected == o->get()->name.c_str()))
			{
				IobjSelected = o->get()->name;
				LoadedPreview = false;
				IsCaclulatedDeltas = false;
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("Удалить"))
				{
					AddLog("Удаление интерактивного объекта...\n");

					std::string deletedObjName = o->get()->name;
					size_t deletedObjId;
					size_t deletedObjLayer;

					for (size_t i = 0; i < Iobjects->elements.size(); i++)
					{
						if (Iobjects->elements[i]->name == deletedObjName)
						{
							deletedObjId = i;
							deletedObjLayer = Iobjects->elements[i]->layer;
							break;
						}
					}
					
					objQueue->DeleteObjectAt(o->get()->name);
					Iobjects->DeleteAt(o);

					EngineFunctions::DeleteJsonObject(deletedObjName, Iobjects->dataPath);

					for (size_t i = deletedObjId; i < Iobjects->elements.size(); i++)
					{
						std::ostringstream src;
						src << "obj " << i + 1;

						std::ostringstream dst;
						dst << "obj " << i;

						EngineFunctions::ChangeObjectName(src.str(), dst.str(), Iobjects->dataPath);
						
						Iobjects->elements[i]->name = EngineFunctions::StrReplace(Iobjects->elements[i]->name, src.str(), dst.str());	
					}

					SaveIobjData();
					SaveLayersData();

					AddLog("Интерактивный объект ");
					AddLog(deletedObjName);
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
			AddLog("Добавление...\n");
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

				Iobjects->elements.push_back(std::make_unique<InteractableObject2D>(d.value().name, d.value().position, d.value().layer, d.value().pathToSprite, Color(0, 0, 0), false, HitBox(d.value().name + std::string(" hitbox"), hb_coord)));
				objQueue->queue.push_back(Iobjects->elements.back().get());

				IsCaclulatedDeltas = false;

				using std::to_string;

				// Открытие файла с данными о физике сцены
				std::ifstream dataFile(Iobjects->dataPath);
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

				newLine << "\"pos-x\": "   << d.value().position.x	 << ",";
				newLine << "\"pos-y\" : "  << d.value().position.y	 << ",";
				newLine << "\"hb-ltx\" : " << hb_coord.x			 << ",";
				newLine << "\"hb-lty\" : " << hb_coord.y			 << ",";
				newLine << "\"hb-rbx\" : " << hb_coord.z			 << ",";
				newLine << "\"hb-rby\" : " << hb_coord.w			 << ",";
				newLine << "\"g-deep\" : " << 2.0f					 << ",";
				newLine << "\"g-able\" : " << false					 << ",";
				newLine << "\"layer\" : "  << d.value().layer		 << ",";
				newLine << "\"chr-r\" : "  << 0						 << ",";
				newLine << "\"chr-g\" : "  << 0						 << ",";
				newLine << "\"chr-b\" : "  << 0						 << ",";
				newLine << "\"chr-a\" : "  << false					 << ",";
				newLine << "\"path\" : \"" << d.value().pathToSprite << "\"}]";

				// Подготовка к вставке в файл
				std::string json_str = j.dump();
				size_t pos_of_par = json_str.find_last_of('}');
				size_t pos_of_par2 = json_str.find_last_of(']');

				json_str.at(pos_of_par) = ' ';
				
				if (j.size() != 0)
				{
					json_str.at(pos_of_par2 + 1) = ',';
				}
				else
				{
					json_str.at(pos_of_par2 + 1) = '{';
				}

				// Запись в файл данных новой линии
				std::ofstream ostream(Iobjects->dataPath);
				ostream << json_str + newLine.str() + '}';

				// Закрытие файла
				ostream.close();

				AddLog("Интерактивный объект ");
				AddLog(d.value().name);
				AddLog(" добавлен\n");

				AddingIobj = false;
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowTriggerList()
{
	using namespace Physics;

	if (ImGui::Begin("Триггеры", NULL, SIDE_PANEL_FLAGS))
	{
		for (auto t = triggers->elements.begin(); t != triggers->elements.end(); t++)
		{
			char label[128];
			sprintf_s(label, t->name.c_str(), triggerSelected);

			std::string contextMenuId = "Context Menu for " + t->name;

			ImGui::Bullet();
			if (ImGui::Selectable(label, triggerSelected == t->name.c_str()))
			{
				triggerSelected = t->name;
			}
			if (ImGui::BeginPopupContextItem(contextMenuId.c_str()))
			{
				if (ImGui::Button("Удалить"))
				{
					AddLog("Удаление триггера...\n");

					std::string deletedTriggerName = t->name;

					triggers->DeleteAt(t);
					EngineFunctions::DeleteJsonObject(deletedTriggerName, triggers->dataPath);

					AddLog("Триггер ");
					AddLog(deletedTriggerName);
					AddLog(" удалён\n");

					ImGui::EndPopup();

					break;
				}

				ImGui::EndPopup();
			}
		}

		ImGui::NewLine();

		// Если нажата кнопка добавить триггер
		if (ImGui::Button("Добавить"))
		{
			AddLog("Добавление триггера...\n");

			AddingTrigger = true;

			ImGui::GetStyle().Alpha = 0.1f;
		}

		/* Если идёт добавление триггера */
		{
			if (AddingTrigger)
			{
				if (!SettedFirstPoint)
				{
					mouseHelpInfo = "Идёт добавление триггера.\nНажмите ЛКМ, чтобы поставить\nпервую точку.";

					if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
					{
						auto pos = wnd->mouse.GetPos();
						firstPoint = { (float)pos.first, (float)pos.second };

						SettedFirstPoint = true;
						std::ostringstream oss;
						oss << "Поставлена первая точка:\n" <<
							"[x: "  << firstPoint.x <<
							"; y: " << firstPoint.y << "]\n";

						AddLog(oss);
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

						std::ostringstream trigger_name;
						trigger_name << "trig " << triggers->GetSize();

						std::ostringstream default_goal_name;
						int next_scene_goal = std::stoi(EngineFunctions::StrReplace(curSceneName, "Scene ", "")) + 1;

						if (next_scene_goal == EngineFunctions::GetScenesNames().size() + 1)
						{
							next_scene_goal--;
						}

						default_goal_name << "Scene " << next_scene_goal;

						triggers->Add(std::move(Trigger(trigger_name.str(), firstPoint, secondPoint, TriggerType::SceneTrigger, default_goal_name.str())));

						std::ostringstream oss;
						oss << "Поставлена вторая точка:\n" <<
							"[x: " << secondPoint.x <<
							"; y: " << secondPoint.y << "]\n";

						AddLog(oss);

						using std::to_string;

						// Открытие файла с данными о физике сцены
						std::ifstream dataFile(triggers->dataPath);
						if (!dataFile.is_open())
						{
							throw ("Не удаётся открыть файл с данными о триггерах");
						}

						// Чтение файла
						json j;
						dataFile >> j;

						// Закрытие файла
						dataFile.close();

						// Новая линия
						std::ostringstream newTrigger;
						newTrigger << "\"" << trigger_name.str() << "\":[{";

						newTrigger << "\"start-x\": " << firstPoint.x << ",";
						newTrigger << "\"start-y\" : " << firstPoint.y << ",";
						newTrigger << "\"end-x\" : " << secondPoint.x << ",";
						newTrigger << "\"end-y\" : " << secondPoint.y << ",";
						newTrigger << "\"type\" : " << static_cast<size_t>(TriggerType::SceneTrigger) << ",";
						newTrigger << "\"goal\" : \"" << default_goal_name.str() << "\"}]";

						// Подготовка к вставке в файл
						std::string json_str = j.dump();
						size_t pos_of_par = json_str.find_last_of('}');
						size_t pos_of_par2 = json_str.find_last_of(']');

						json_str.at(pos_of_par) = ' ';

						if (j.size() != 0)
						{
							json_str.at(pos_of_par2 + 1) = ',';
						}
						else
						{
							json_str.at(pos_of_par2 + 1) = '{';
						}

						// Запись в файл данных новой линии
						std::ofstream ostream(triggers->dataPath);
						ostream << json_str + newTrigger.str() + '}';

						// Закрытие файла
						ostream.close();

						AddLog("Триггер ");
						AddLog(trigger_name);
						AddLog(" добавлена\n");

						mouseHelpInfo = "";
						SettedSecondPoint = true;
					}
				}
				else if (SettedFirstPoint && SettedSecondPoint)
				{
					AddingTrigger = false;
					SettedFirstPoint = false;
					SettedSecondPoint = false;

					ImGui::GetStyle().Alpha = 1.0f;
				}
			}
		}
		/******************************/
	}

	ImGui::End();
}

void GUISystem::ShowPhysicsEngineObjList()
{
	if (draggingObjId == -1)
	{
		if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			for (size_t i = 0; i < phEngPtr->hitboxes.size(); i++)
			{
				if (phEngPtr->hitboxes[i].IsOverlap({ mPosX, mPosY }))
				{
					draggingObjId = i;

					AddLog("Перемещение мышкой объекта ");
					AddLog(phEngPtr->hitboxes[i].name);
					AddLog("\n");
				}
			}
		}
	}
	else
	{
		if (!wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
		{
			draggingObjId = -1;

			AddLog("Перемещение завершенно\n");
		}
		else
		{
			auto mPosX = (float)wnd->mouse.GetPosX();
			auto mPosY = (float)wnd->mouse.GetPosY();

			phEngPtr->hitboxes[draggingObjId].coordinates.z = mPosX + fabs(phEngPtr->hitboxes[draggingObjId].coordinates.z - phEngPtr->hitboxes[draggingObjId].coordinates.x);
			phEngPtr->hitboxes[draggingObjId].coordinates.w = mPosY + fabs(phEngPtr->hitboxes[draggingObjId].coordinates.w - phEngPtr->hitboxes[draggingObjId].coordinates.y);
			phEngPtr->hitboxes[draggingObjId].coordinates.x = mPosX;
			phEngPtr->hitboxes[draggingObjId].coordinates.y = mPosY;
			
		}
	}

	using namespace Physics;

	if (ImGui::Begin("Объекты", NULL, SIDE_PANEL_FLAGS))
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
						if (ImGui::Button("Удалить"))
						{
							AddLog("Удаление линии...\n");

							std::string deletedLineName = l->name;				
							size_t deletedLineId;

							for (size_t i = 0; i < phEngPtr->lines.size(); i++)
							{
								if (phEngPtr->lines[i].name == deletedLineName)
								{
									deletedLineId = i;
									break;
								}
							}

							phEngPtr->DeleteLineAt(l);
							EngineFunctions::DeleteJsonObject(deletedLineName, phEngPtr->dataPath);

							for (size_t i = deletedLineId; i < phEngPtr->lines.size(); i++)
							{
								std::ostringstream src;
								src << "line " << i + 1;

								std::ostringstream dst;
								dst << "line " << i;

								EngineFunctions::ChangeObjectName(src.str(), dst.str(), phEngPtr->dataPath);

								phEngPtr->lines[i].name = EngineFunctions::StrReplace(phEngPtr->lines[i].name, src.str(), dst.str());
							}

							SavePhysicsEngineData();

							AddLog("Линия ");
							AddLog(deletedLineName);
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
					AddLog("Добавление линии...\n");

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
									 "[x: " << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss);
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
									"[x: " << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss);

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

								newLine << "\"start-x\": " << firstPoint.x << ",";
								newLine << "\"start-y\" : " << firstPoint.y << ",";
								newLine << "\"end-x\" : " << secondPoint.x << ",";
								newLine << "\"end-y\" : " << secondPoint.y << "}]";

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

								AddLog("Линия ");
								AddLog(line_name);
								AddLog(" добавлена\n");

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
							AddLog("Удаление Hit-box...\n");

							std::string deletedHitBoxName = hb->name;
							size_t deletedHitBoxId;

							for (size_t i = 0; i < phEngPtr->hitboxes.size(); i++)
							{
								if (phEngPtr->hitboxes[i].name == deletedHitBoxName)
								{
									deletedHitBoxId = i;
									break;
								}
							}

							phEngPtr->DeleteHitBoxAt(hb);
							EngineFunctions::DeleteJsonObject(deletedHitBoxName, phEngPtr->dataPath);

							for (size_t i = deletedHitBoxId; i < phEngPtr->hitboxes.size(); i++)
							{
								std::ostringstream src;
								src << "hb " << i + 1;

								std::ostringstream dst;
								dst << "hb " << i;

								EngineFunctions::ChangeObjectName(src.str(), dst.str(), phEngPtr->dataPath);

								phEngPtr->hitboxes[i].name = EngineFunctions::StrReplace(phEngPtr->hitboxes[i].name, src.str(), dst.str());
							}

							SavePhysicsEngineData();

							AddLog("Hit-box ");
							AddLog(deletedHitBoxName);
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
					AddLog("Добавление HitBox...\n");

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
									 "[x: " << firstPoint.x <<
									"; y: " << firstPoint.y << "]\n";

								AddLog(oss);
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
									 "[x: " << secondPoint.x <<
									"; y: " << secondPoint.y << "]\n";

								AddLog(oss);

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

								newLine << "\"lt-x\": " << firstPoint.x << ",";
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

								AddLog("Hit-box ");
								AddLog(hb_name);
								AddLog(" добавлен\n");

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

void GUISystem::ShowMainPersonControl(float dt)
{
	if (ImGui::Begin("Опции", NULL, SIDE_PANEL_FLAGS))
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

				if (ImGui::BeginTabItem("Бой"))
				{
					SpawnDefaultAliveObject2DControl(hero, hero->dataPath);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Дополнительно"))
				{
					/* Переменные управления сбросом интерфейса */

					bool effDirty = false;		// Котнроль эффекта
					bool speedDirty = false;	// Котнроль скорости
					bool gravityDirty = false;	// Контроль гравитации
					bool jHeightDirty = false;	// Контроль высоты прыжка
					bool a_hdDirty = false;		// Котнроль скорости анимации
					bool a_sDirty = false;		// Котнроль анимации
					bool a_sizeDirty = false;	// Котнроль анимации

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

					/********************************************/

					/* Элементы управления позицией и скорости главного персонажа */

					if (ImGui::CollapsingHeader("Физика", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("Скорость:");
						dcheck(ImGui::SliderFloat(" ", &hero->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);

						ImGui::Text("Гравитация:");
						dcheck(ImGui::SliderFloat("у.е.", &hero->gravity, 0.0f, 1000.0f, "%.2f"), gravityDirty);

						ImGui::Text("Высота прыжка:");
						dcheck(ImGui::SliderInt("степень", &hero->jump_height, 0, 20), jHeightDirty);
						
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
								AddLog("Изменение Hit-box...\n");

								DrawingHitBox = true;
								hero->hitbox.visability = false;

								ImGui::GetStyle().Alpha = 0.1f;
							}

							if (DrawingHitBox)
							{
								auto new_hb = CreateNewHitBox();

								if (new_hb.name != "empty")
								{
									AddLog("Создан Hit-box ");
									AddLog(new_hb.name);
									AddLog("\n");

									hero->SetHitBox(new_hb);
									hero->hitbox.visability = true;

									//new_hb.Translate(camera->position);
									EngineFunctions::SaveHitBoxData(heroSelected, new_hb, hero->dataPath, &applog);
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
							ImGui::SliderInt("Кадр", &hero->animations[animSelectedId].iCurFrame, 0, (int)hero->animations[animSelectedId].frames.size());
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
						hero->holdTime = hero->animations[animSelectedId].holdTime;

						for (auto& a : hero->animations)
						{
							a.holdTime = hero->holdTime;
						}

						if (ImGui::Button("Создать анимацию"))
						{
							AddLog("Создание анимации...\n");

							CreatingAnimation = true;
						}

						if (CreatingAnimation)
						{
							auto animations = ShowAnimationCreatingDialog(dt);

							if (!animations.empty())
							{
								AddLog("Создана анимация для ");
								AddLog("главного персонажа\n");

								hero->image = Surface2D(animPath);
								std::vector<Animation> newAnim;

								for (auto& a : animations)
								{
									newAnim.emplace_back(Animation(a.pStart, a.pEnd, a.width, a.height, a.frames, hero->image, a.ft, a.name));
								}

								hero->SetAnimation(newAnim);

								EngineFunctions::SaveAnimationData(
									hero->name,
									AnimationData(animations[0].pStart, animations[0].pEnd, animations[0].width, animations[0].height, animations[0].frames, animations[0].ft, animations[0].name),
									hero->dataPath,
									&applog);

								EngineFunctions::SetNewValue<std::string>(
									hero->name,
									"path", hero->image.GetFileName(),
									hero->dataPath,
									&applog
									);

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
							AddLog("Сохранение настроек для главного персонажа...\n");

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

							EngineFunctions::SaveHitBoxData(hero->name, actual_hb, hero->dataPath, &applog);

							/*************************/

							/* Сохранение настроек камеры */

							EngineFunctions::SetNewValue<size_t>(
								hero->name,
								"camera-mode", static_cast<size_t>(hero->cameraMode),
								hero->dataPath,
								&applog
								);

							/******************************/

							/* Сохранение настроек анимаций */

							EngineFunctions::SetNewValue<float>(
								hero->name,
								"a-ft", hero->holdTime,
								hero->dataPath,
								&applog
								);

							/********************************/

							/* Сохранение настроек физики для персонажа */

							EngineFunctions::SetNewValue<float>(
								hero->name,
								"gravity", hero->gravity,
								hero->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<int>(
								hero->name,
								"j-h", hero->jump_height,
								hero->dataPath,
								&applog
								);

							/********************************************/

							AddLog("Настройки сохранены\n");

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

void GUISystem::ShowPersonControl(float dt)
{
	if (ImGui::Begin("Опции", NULL, SIDE_PANEL_FLAGS))
	{
		// Цикл по персонажам
		for (size_t k = 0; k < persons->elements.size(); k++)
		{
			// Поиск выбранного персонажа
			if (persons->elements.at(k)->name == personSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					if (ImGui::BeginTabBar("mpSet", ImGuiTabBarFlags_None))
					{
						if (ImGui::BeginTabItem("Объект"))
						{
							SpawnDefaultObject2DControl(persons->elements.at(k).get(), persons->dataPath);							

							// КОСТЫЛЬ \\ !
							DirectX::XMFLOAT2 delta;
							delta.x = persons->elements.at(k)->position.x - persons->elements.at(k)->hitbox.coordinates.x + persons->elements.at(k)->dx;
							delta.y = persons->elements.at(k)->position.y - persons->elements.at(k)->hitbox.coordinates.y + persons->elements.at(k)->dy;

							persons->elements.at(k)->hitbox.Translate(delta);

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Бой"))
						{
							SpawnDefaultAliveObject2DControl(persons->elements.at(k).get(), persons->dataPath);

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Скрипт"))
						{
							if (ImGui::CollapsingHeader("Информация", ImGuiTreeNodeFlags_DefaultOpen))
							{
								std::ostringstream oss;

								if (persons->elements.at(k)->scriptPath != "")
								{
									oss << "Скрипт: " << persons->elements.at(k)->scriptPath;
								}
								else
								{
									oss << "Скрипт не установлен";
								}

								ImGui::Text(oss.str().c_str());

								if (ImGui::CollapsingHeader("Код"))
								{
									std::ostringstream code;

									for (size_t i = 0; i < persons->elements.at(k)->script.commands.size(); i++)
									{
										code << i + 1 << ".\t" << persons->elements.at(k)->script.commands[i].first << "(" << persons->elements.at(k)->script.commands[i].second << ");\n";
									}

									ImGui::TextWrapped(code.str().c_str());

									ImGui::Separator();
								}

								ImGui::Separator();
							}

							if (ImGui::CollapsingHeader("Создание"))
							{
								if (ImGui::Button("Создать"))
								{
									persons->IsScriptsRunning = false;
									CreatingScript = true;
								}

								if (CreatingScript)
								{
									nEditor.Show();

									auto newScriptPath = nEditor.GetScriptPath();

									if (newScriptPath.has_value())
									{
										persons->elements.at(k)->SetScript(newScriptPath.value());
										
										EngineFunctions::SetNewValue<std::string>(
											persons->elements.at(k)->name,
											"script", newScriptPath.value(),
											persons->dataPath,
											&applog
											);

										CreatingScript = false;
									}
								}

								ImGui::Separator();
							}

							ImGui::EndTabItem();
						}

						if (ImGui::BeginTabItem("Дополнительно"))
						{
							/* Переменные управления сбросом интерфейса */

							bool effDirty = false;		// Котнроль эффекта
							bool speedDirty = false;	// Котнроль скорости
							bool a_hdDirty = false;		// Котнроль скорости анимации
							bool a_sDirty = false;		// Котнроль анимации
							bool a_sizeDirty = false;	// Котнроль анимации

							const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

							/********************************************/

							/* Элементы управления позицией и скорости персонажа */

							if (ImGui::CollapsingHeader("Физика", ImGuiTreeNodeFlags_DefaultOpen))
							{
								ImGui::Text("Скорость:");
								dcheck(ImGui::SliderFloat(" ", &persons->elements.at(k)->speed, 0.0f, 1000.0f, "%.2f"), speedDirty);

								ImGui::Separator(); // Разделитель
							}

							/*****************************************************/

							/* Элементы управления эффектом персонажа */

							if (ImGui::CollapsingHeader("Эффекты"))
							{
								dcheck(ImGui::SliderFloat("Продолжитель.", &persons->elements.at(k)->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
								dcheck(ImGui::SliderFloat("Время", &persons->elements.at(k)->effect.Time, 0.0f, 100.0f, "%.3f"), effDirty);

								ImGui::Checkbox("Активен", &persons->elements.at(k)->effect.Active);

								ImGui::Separator();	// Разделитель
							}

							/******************************************/

							/* Элементы управления хитбоксом персонажа */

							if (ImGui::CollapsingHeader("Hit-box"))
							{
								ImGui::Checkbox("Показать", &persons->elements.at(k)->hitbox.visability);

								/* Если нажата кнопка изменить HitBox */
								{
									if (ImGui::Button("Изменить", ImVec2(100, 20)))
									{
										AddLog("Изменение Hit-box...\n");

										DrawingHitBox = true;
										persons->elements.at(k)->hitbox.visability = false;

										ImGui::GetStyle().Alpha = 0.1f;
									}

									if (DrawingHitBox)
									{
										auto new_hb = CreateNewHitBox();

										if (new_hb.name != "empty")
										{
											AddLog("Создан Hit-box ");
											AddLog(new_hb.name);
											AddLog("\n");

											persons->elements.at(k)->SetHitBox(new_hb);
											persons->elements.at(k)->hitbox.visability = true;

											new_hb.Translate(camera->position);
											EngineFunctions::SaveHitBoxData(personSelected, new_hb, persons->dataPath, &applog);
										}
									}
								}
								/**************************************/

								ImGui::Separator(); // Разделитель
							}
							/*******************************************/

							/* Элементы управления анимацией главного персонажа */

							if (ImGui::CollapsingHeader("Анимации"))
							{								
								std::ostringstream str;
								str << "Кол-во анимаций: " << persons->elements.at(k)->animations.size();
								ImGui::Text(str.str().c_str());
								
								if (ImGui::BeginCombo("Анимация", animSelected.c_str()))
								{
									for (size_t i = 0; i < persons->elements.at(k)->animations.size(); i++)
									{
										if (ImGui::Selectable(persons->elements.at(k)->animations[i].name.c_str(), animSelected == persons->elements.at(k)->animations[i].name))
										{
											animSelected = persons->elements.at(k)->animations[i].name;
											animSelectedId = i;
										}
									}

									ImGui::EndCombo();
								}

								if (animSelected != "")
								{

									ImGui::SliderInt("Кадр", &persons->elements.at(k)->animations[animSelectedId].iCurFrame, 0, (int)persons->elements.at(k)->animations[animSelectedId].frames.size());
									dcheck(ImGui::SliderFloat("Превью", &scaleFrame, 1.0f, 5.0f, "%.2f"), a_sDirty);

									ImGui::NewLine();

									curFrame = persons->elements.at(k)->animations[animSelectedId].iCurFrame;

									curAnimW = (float)persons->elements.at(k)->animations[animSelectedId].width;
									curAnimH = (float)persons->elements.at(k)->animations[animSelectedId].height;

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

								dcheck(ImGui::SliderFloat("Задержка", &persons->elements.at(k)->animations[animSelectedId].holdTime, 0.01f, 1.0f), a_hdDirty);
								persons->elements.at(k)->holdTime = persons->elements.at(k)->animations[animSelectedId].holdTime;

								for (auto& a : persons->elements.at(k)->animations)
								{
									a.holdTime = persons->elements.at(k)->holdTime;
								}

								if (ImGui::Button("Создать анимацию"))
								{
									AddLog("Создание анимации...\n");
									CreatingAnimation = true;
								}

								if (CreatingAnimation)
								{
									auto animations = ShowAnimationCreatingDialog(dt);

									if (!animations.empty())
									{
										AddLog("Создана анимация для ");
										AddLog(persons->elements.at(k)->name);
										AddLog("\n");

										persons->elements.at(k)->image = Surface2D(animPath);
										std::vector<Animation> newAnim;

										for (auto& a : animations)
										{
											newAnim.emplace_back(Animation(a.pStart, a.pEnd, a.width, a.height, a.frames, persons->elements.at(k)->image, a.ft, a.name));
										}

										persons->elements.at(k)->SetAnimation(newAnim);

										EngineFunctions::SaveAnimationData(
											persons->elements.at(k)->name,
											AnimationData(animations[0].pStart, animations[0].pEnd, animations[0].width, animations[0].height, animations[0].frames, animations[0].ft, animations[0].name),
											persons->dataPath,
											&applog);

										CreatingAnimation = false;
									}
								}

								ImGui::Separator();
							}

							/****************************************************/

							ImGui::NewLine();
							ImGui::NewLine();

							/* Если нажата кнопка сохранить текущие настройки персонажа */
							{
								if (ImGui::Button("Сохранить", ImVec2(100, 20)))
								{
									AddLog("Сохранение настроек для ");
									AddLog(persons->elements.at(k)->name);
									AddLog("\n");

									SavingSettings = true;
								}

								if (SavingSettings)
								{
									/* Сохранение скорости */

									EngineFunctions::SetNewValue<float>(
										personSelected,
										"speed", persons->elements.at(k)->speed,
										persons->dataPath,
										&applog
										);

									/***********************/

									/* Сохранение настроек эффекта */

									EngineFunctions::SetNewValue<bool>(
										personSelected,
										"eff-a", persons->elements.at(k)->effect.Active,
										persons->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<float>(
										personSelected,
										"eff-d", persons->elements.at(k)->effect.Duration,
										persons->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<float>(
										personSelected,
										"eff-t", persons->elements.at(k)->effect.Time,
										persons->dataPath,
										&applog
										);

									/*******************************/

									/* Пересохранение hitbox */
									
									auto hitbox = persons->elements[k]->hitbox;
									EngineFunctions::SaveHitBoxData(personSelected, hitbox, persons->dataPath, &applog);

									/*************************/

									AddLog("Настройки сохранены\n");

									SavingSettings = false;
								}
							}
							/************************************************************/

							ImGui::SameLine();

							/* Если нажата кнопка удалить персонажа */
							{
								if (ImGui::Button("Удалить", ImVec2(100, 20)))
								{
									AddLog("Удаление ");
									AddLog(persons->elements.at(k)->name);
									AddLog("\n");

									for (auto i = persons->elements.begin(); i != persons->elements.end(); i++)
									{
										if (i->get()->name == personSelected)
										{
											persons->elements.erase(i);
											break;
										}
									}

									EngineFunctions::DeleteJsonObject(personSelected, persons->dataPath);

									AddLog("Персонаж удалён\n");
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

void GUISystem::ShowIobjControl()
{
	if (ImGui::Begin("Опции", NULL, SIDE_PANEL_FLAGS))
	{
		// Цикл по интерактивным объектам
		for (int k = 0; k < Iobjects->elements.size(); k++)
		{
			// Поиск выбранного интерактивного объекта
			if (Iobjects->elements.at(k)->name == IobjSelected)
			{
				if (ImGui::BeginChild(""))
				{
					if (ImGui::BeginTabBar("ioSet", ImGuiTabBarFlags_None))
					{
						if (ImGui::BeginTabItem("Объект"))
						{
							SpawnDefaultObject2DControl(Iobjects->elements.at(k).get(), Iobjects->dataPath);

							// КОСТЫЛЬ \\ !		
							if (!IsCaclulatedDeltas)
							{
								hb_delta.x = Iobjects->elements.at(k)->hitbox.coordinates.x - Iobjects->elements.at(k)->position.x;
								hb_delta.y = Iobjects->elements.at(k)->hitbox.coordinates.y - Iobjects->elements.at(k)->position.y;

								IsCaclulatedDeltas = true;
							}

							DirectX::XMFLOAT2 delta;
							delta.x = Iobjects->elements.at(k)->position.x - Iobjects->elements.at(k)->hitbox.coordinates.x + hb_delta.x;
							delta.y = Iobjects->elements.at(k)->position.y - Iobjects->elements.at(k)->hitbox.coordinates.y + hb_delta.y;

							Iobjects->elements.at(k)->hitbox.Translate(delta);

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
								dcheck(ImGui::SliderFloat("Глубина", &Iobjects->elements.at(k)->deep, 1.0f, 100.0f, "%.3f"), deepDirty);

								ImGui::Checkbox("Прозрачность", &Iobjects->elements.at(k)->drawGhostable);
								ImGui::Checkbox("Хромакей", &Iobjects->elements.at(k)->chromaKeyAble);

								ImGui::Separator();
							}

							/****************************************************/

							/* Элементы управления хитбоксом интерактивного объекта */

							if (ImGui::CollapsingHeader("Hit-box"))
							{
								ImGui::Checkbox("Показать", &Iobjects->elements.at(k)->hitbox.visability);

								/* Если нажата кнопка изменить HitBox */
								{
									if (ImGui::Button("Изменить", ImVec2(100, 20)))
									{
										AddLog("Изменение Hit-box...\n");

										DrawingHitBox = true;
										Iobjects->elements.at(k)->hitbox.visability = false;

										ImGui::GetStyle().Alpha = 0.1f;
									}

									if (DrawingHitBox)
									{
										auto new_hb = CreateNewHitBox();

										if (new_hb.name != "empty")
										{
											AddLog("Создан Hit-box ");
											AddLog(new_hb.name);
											AddLog("\n");

											Iobjects->elements.at(k)->SetHitBox(new_hb);
											Iobjects->elements.at(k)->hitbox.visability = true;

											new_hb.Translate(camera->position);
											EngineFunctions::SaveHitBoxData(IobjSelected, new_hb, Iobjects->dataPath, &applog);

											IsCaclulatedDeltas = false;
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
									AddLog("Сохранение настроек для ");
									AddLog(Iobjects->elements.at(k)->name);
									AddLog("\n");

									SavingSettings = true;
								}

								if (SavingSettings)
								{
									EngineFunctions::SetNewValue<float>(
										IobjSelected,
										"g-deep", Iobjects->elements.at(k)->deep,
										Iobjects->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<bool>(
										IobjSelected,
										"g-able", Iobjects->elements.at(k)->drawGhostable,
										Iobjects->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<bool>(
										IobjSelected,
										"chr-a", Iobjects->elements.at(k)->chromaKeyAble,
										Iobjects->dataPath,
										&applog
										);

									auto hitbox = Iobjects->elements.at(k)->hitbox;
									EngineFunctions::SaveHitBoxData(IobjSelected, hitbox, Iobjects->dataPath, &applog);

									AddLog("Настройки сохранены\n");

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

void GUISystem::ShowTriggerControl()
{
	if (ImGui::Begin("Опции", NULL, SIDE_PANEL_FLAGS))
	{
		// Цикл по триггерам
		for (int k = 0; k < triggers->elements.size(); k++)
		{
			// Поиск выбранного триггера
			if (triggers->elements.at(k).name == triggerSelected)
			{
				if (ImGui::BeginChild(""))
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
							dcheck(ImGui::SliderFloat("Xs", &triggers->elements.at(k).line.start.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &triggers->elements.at(k).line.start.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

							ImGui::Text("Позиция конечной точки:");
							dcheck(ImGui::SliderFloat("Xe", &triggers->elements.at(k).line.end.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &triggers->elements.at(k).line.end.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

							if (ImGui::Button("Нормировать X", ImVec2(100, 20)))
							{
								AddLog("Нормирование по оси X...\n");

								triggers->elements.at(k).line.end.x = triggers->elements.at(k).line.start.x;

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"start-x", triggers->elements.at(k).line.start.x,
									triggers->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"end-x", triggers->elements.at(k).line.end.x,
									triggers->dataPath,
									&applog
									);

								AddLog("Нормирование выполнено\n");
							}

							ImGui::SameLine();

							if (ImGui::Button("Нормировать Y", ImVec2(100, 20)))
							{
								AddLog("Нормирование по оси Y...\n");

								triggers->elements.at(k).line.end.y = triggers->elements.at(k).line.start.y;

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"start-y", triggers->elements.at(k).line.start.y,
									triggers->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"end-y", triggers->elements.at(k).line.end.y,
									triggers->dataPath,
									&applog
									);

								AddLog("Нормирование выполнено\n");
							}

							ImGui::Separator();	// Разделитель
						}
						/*********************************************/

						/* Элементы управления линией */
						if (ImGui::CollapsingHeader("Изменение"))
						{
							/* Если нажата кнопка перерисовать линию */
							{
								if (ImGui::Button("Перерисовать", ImVec2(100, 20)))
								{
									AddLog("Изменение линии...\n");

									DrawingLine = true;
									triggers->elements.at(k).line.visability = false;

									ImGui::GetStyle().Alpha = 0.1f;
								}

								if (DrawingLine)
								{
									using namespace Physics;

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

											AddLog(oss);
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

											Line line_new(triggerSelected + std::string(" line"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
											line_new.Translate(camera->position);
											triggers->UpdateLineAt(k, line_new);

											std::ostringstream oss;
											oss << "Поставлена вторая точка:\n" <<
												"[x: "  << secondPoint.x <<
												"; y: " << secondPoint.y << "]\n";

											AddLog(oss);

											EngineFunctions::SetNewValue<float>(
												triggerSelected,
												"start-x", triggers->elements.at(k).line.start.x,
												triggers->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												triggerSelected,
												"end-x", triggers->elements.at(k).line.end.x,
												triggers->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												triggerSelected,
												"start-y", triggers->elements.at(k).line.start.y,
												triggers->dataPath,
												&applog
												);

											EngineFunctions::SetNewValue<float>(
												triggerSelected,
												"end-y", triggers->elements.at(k).line.end.y,
												triggers->dataPath,
												&applog
												);

											mouseHelpInfo = "";

											AddLog("Линия ");
											AddLog(line_new.name);
											AddLog(" изменена\n");

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

						if (ImGui::CollapsingHeader("Параметры триггера"))
						{
							if (ImGui::BeginCombo("Тип триггера", trigType.c_str()))
							{
								std::string curType = "";
								
								for (size_t i = 0; i < static_cast<size_t>(TriggerType::Count); i++)
								{
									if (static_cast<TriggerType>(i) == TriggerType::SceneTrigger)
									{
										curType = "Сценовой триггер";
									}

									if (static_cast<TriggerType>(i) == TriggerType::SoundTrigger)
									{
										curType = "Звуковой триггер";
									}

									if (static_cast<TriggerType>(i) == TriggerType::CutSceneTrigger)
									{
										curType = "Кат-сценовой триггер";
									}

									if (ImGui::Selectable(curType.c_str()))
									{
										trigType   = curType;
										trigTypeId = i;
									}
								}

								ImGui::EndCombo();
							}

							if(trigType != "" && static_cast<TriggerType>(trigTypeId) == TriggerType::SceneTrigger)
							{
								if (ImGui::BeginCombo("Переход на сцену", trigScenesPreview.c_str()))
								{
									for (size_t i = 1; i <= EngineFunctions::GetScenesNames().size(); i++)
									{
										std::ostringstream oss;
										oss << "Scene " << i;

										if (ImGui::Selectable(oss.str().c_str()))
										{
											trigScenesPreview = oss.str().c_str();
										}
									}
									
									ImGui::EndCombo();
								}

								if (ImGui::Button("Сохранить цель", ImVec2(100, 20)))
								{
									EngineFunctions::SetNewValue<std::string>(
										triggers->elements.at(k).name,
										"goal",
										trigScenesPreview,
										triggers->dataPath,
										&applog
										);
								}
							}

							ImGui::Separator();	// Разделитель
						}

						ImGui::NewLine();
						ImGui::NewLine();

						/* Если нажата кнопка сохранить текущие настройки линии */
						{
							if (ImGui::Button("Сохранить", ImVec2(100, 20)))
							{
								AddLog("Сохранение настроек для ");
								AddLog(phEngPtr->lines.at(k).name);
								AddLog("\n");

								SavingSettings = true;
							}

							if (SavingSettings)
							{
								/* Сохранение координат */

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"start-x", triggers->elements.at(k).line.start.x,
									triggers->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"end-x", triggers->elements.at(k).line.end.x,
									triggers->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"start-y", triggers->elements.at(k).line.start.y,
									triggers->dataPath,
									&applog
									);

								EngineFunctions::SetNewValue<float>(
									triggerSelected,
									"end-y", triggers->elements.at(k).line.end.y,
									triggers->dataPath,
									&applog
									);

								/************************/

								AddLog("Настройки сохранены\n");

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

void GUISystem::ShowPhysicsEngineObjControl()
{
	using namespace Physics;

	if (ImGui::Begin("Опции", NULL, SIDE_PANEL_FLAGS))
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
							dcheck(ImGui::SliderFloat("Xs", &phEngPtr->lines.at(k).start.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &phEngPtr->lines.at(k).start.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

							ImGui::Text("Позиция конечной точки:");
							dcheck(ImGui::SliderFloat("Xe", &phEngPtr->lines.at(k).end.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &phEngPtr->lines.at(k).end.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

							if (ImGui::Button("Нормировать X", ImVec2(100, 20)))
							{
								AddLog("Нормирование по оси X...\n");

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

								AddLog("Нормирование выполнено\n");
							}

							ImGui::SameLine();

							if (ImGui::Button("Нормировать Y", ImVec2(100, 20)))
							{
								AddLog("Нормирование по оси Y...\n");

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

								AddLog("Нормирование выполнено\n");
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
									AddLog("Изменение линии...\n");

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

											AddLog(oss);
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
											
											AddLog(oss);
																						
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

											AddLog("Линия ");
											AddLog(line_new.name);
											AddLog(" изменена\n");

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
								AddLog("Сохранение настроек для ");
								AddLog(phEngPtr->lines.at(k).name);
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

								AddLog("Настройки сохранены\n");

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

						/* Элементы управления позициями точек hitbox */
						if (ImGui::CollapsingHeader("Положение", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGui::Text("Позиция левой верхней точки:");
							dcheck(ImGui::SliderFloat("Xs", &phEngPtr->hitboxes.at(k).coordinates.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ys", &phEngPtr->hitboxes.at(k).coordinates.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

							ImGui::Text("Позиция правой нижней точки:");
							dcheck(ImGui::SliderFloat("Xe", &phEngPtr->hitboxes.at(k).coordinates.z, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
							dcheck(ImGui::SliderFloat("Ye", &phEngPtr->hitboxes.at(k).coordinates.w, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

							ImGui::NewLine();

							dcheck(ImGui::SliderFloat("Длина стороны квадрата", &sq_l, 0.0f, 100.0f), boxDirty);

							if (ImGui::Button("Сделать квадрат", ImVec2(100, 21)))
							{
								AddLog("Создание квадрата...\n");

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

								AddLog("Квадрат создан\n");
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
									AddLog("Изменение Hit-box...\n");

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

											AddLog(oss);
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
											hb_new.Translate(camera->position);
											phEngPtr->UpdateHitBoxAt(k, hb_new);

											std::ostringstream oss;
											oss << "Поставлена вторая точка:\n" <<
												"[x: "  << secondPoint.x <<
												"; y: " << secondPoint.y << "]\n";

											AddLog(oss);

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

											AddLog("Hit-box ");
											AddLog(hb_new.name);
											AddLog(" изменён\n");

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
								AddLog("Сохранение настроек для ");
								AddLog(phEngPtr->hitboxes.at(k).name);
								AddLog(" ...\n");

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

								AddLog("Настройки сохранены\n");

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
							for (auto& io : Iobjects->elements)
							{
								if (io->name == objQueue->queue[k]->name)
								{
									AddLog(objQueue->queue[k]->name);
									AddLog(" сдвинут на задний план\n");

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
							for (auto& io : Iobjects->elements)
							{
								if (io->name == objQueue->queue[k]->name)
								{
									AddLog(objQueue->queue[k]->name);
									AddLog(" сдвинут на передний план\n");

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
			AddLog("Сохранение настроек слоёв...\n");
			
			SavingSettings = true;
		}

		if (SavingSettings)
		{
			for (size_t i = 0; i < objQueue->queue.size(); i++)
			{
				std::string pathToFile = "";

				if (objQueue->queue[i]->name.find("obj") != objQueue->queue[i]->name.npos)
				{
					pathToFile = Iobjects->dataPath;
				}
				else if (objQueue->queue[i]->name.find("mainperson") != objQueue->queue[i]->name.npos)
				{
					pathToFile = hero->dataPath;
				}
				else
				{
					pathToFile = persons->dataPath;
				}

				EngineFunctions::SetNewValue<size_t>(
					objQueue->queue[i]->name,
					"layer", objQueue->queue[i]->layer,
					pathToFile,
					&applog);
			}

			AddLog("Настройки сохранены\n");

			SavingSettings = false;
		}
	}

	ImGui::End();
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
			dcheck(ImGui::SliderFloat("X", &camera->initPosition.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT), posDirty);
			dcheck(ImGui::SliderFloat("Y", &camera->initPosition.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT), posDirty);

			if (ImGui::Button("Вернуть на исходную позицию"))
			{
				AddLog("Возращение на исходную позицию...\n");
				camera->SetPosition(camera->initPosition);

				std::ostringstream str;
				str << "Камера установлена по кординатам: " << "X: " << camera->position.x << " Y: " << camera->position.y << "\n";

				AddLog(str);
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
			AddLog("Сохранение настроек камеры...\n");

			SavingSettings = true;
		}

		if (SavingSettings)
		{
			EngineFunctions::SetNewValue<float>(
				"camera",
				"pos-x", camera->position.x,
				camera->dataPath,
				&applog
				);

			EngineFunctions::SetNewValue<float>(
				"camera",
				"pos-y", camera->position.y,
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

			AddLog("Настройки сохранены\n");

			SavingSettings = false;
		}
	}

	ImGui::End();
}

void GUISystem::ShowScenesControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.2f),
		DispSize.y * 0.6f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("Сцены", &ShowScenesSettings, ImGuiWindowFlags_NoResize))
	{
		auto scenesNames = EngineFunctions::GetScenesNames();

		for (size_t i = 0; i < scenesNames.size(); i++)
		{
			char label[128];
			sprintf_s(label, scenesNames[i].c_str(), objectSelected);

			std::string contextMenuId = "Context Menu for " + scenesNames[i];

			ImGui::Bullet();
			if (ImGui::Selectable(label, curSceneName == scenesNames[i]))
			{
				if (curSceneName != scenesNames[i])
				{
					curSceneName = scenesNames[i];
					IsUpdatingScene = true;
				}
			}
		}

		if (ImGui::Button("Добавить"))
		{
			EngineFunctions::CreateScene();
			IsAddingScene = true;
		}

		ImGui::Separator();

		if (ImGui::Button("Сохранить"))
		{
			SavingScenesSettings = true;
		}

		if (SavingScenesSettings)
		{
			SavingScenesSettings = false;
		}
	}

	ImGui::End();
}

void GUISystem::SpawnCameraToHeroControl()
{
	if (ImGui::BeginCombo("Режим камеры", modeSelected.c_str()))
	{
		for (size_t i = 0; i < cameraModeNames.size(); i++)
		{
			if (ImGui::Selectable(cameraModeNames.at(i).first.c_str(), modeSelected == cameraModeNames.at(i).first))
			{				
				modeSelected = cameraModeNames.at(i).first;
				hero->cameraMode = cameraModeNames.at(i).second;

				AddLog("Режим камеры \"");
				AddLog(modeSelected);
				AddLog("\" установлен\n");
			}
		}

		ImGui::EndCombo();
	}
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
		dcheck(ImGui::SliderFloat("X", &obj->position.x, -wnd->Gfx().POS_X_LIMIT, wnd->Gfx().POS_X_LIMIT, "%.2f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &obj->position.y, -wnd->Gfx().POS_Y_LIMIT, wnd->Gfx().POS_Y_LIMIT, "%.2f"), posDirty);

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
			AddLog("Загрузка нового спрайта для ");
			AddLog(obj->name);
			AddLog("...\n");

			LoadingSprite = true;
		}
	}

	if (ImGui::CollapsingHeader("Хромакей"))
	{
		ImGui::Separator();	// Разделитель

		ImGui::ColorEdit3("Цвет", chromaColor);

		if (ImGui::Button("Применить", ImVec2(100, 20)))
		{
			AddLog("Сохранение хромакея для ");
			AddLog(obj->name);
			AddLog("...\n");

			obj->chromaKey = Color(char(chromaColor[0] * 255), char(chromaColor[1] * 255), char(chromaColor[2] * 255));

			EngineFunctions::SaveColorData(obj->name, obj->chromaKey, dataPath);

			AddLog("Цвет хромакея сохранён\n");
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
				for (auto& io : Iobjects->elements)
				{
					if (io->name == obj->name)
					{
						DirectX::XMFLOAT4 hb_coord;
						hb_coord.x = obj->position.x;
						hb_coord.y = obj->position.y;
						hb_coord.z = obj->position.x + im.GetWidth();
						hb_coord.w = obj->position.y + im.GetHeight();

						io->hitbox = HitBox(io->name + std::string(" hitbox"), hb_coord);
						
						AddLog("Hit-box обновлён\n");

						IsCaclulatedDeltas = false;

						break;
					}
				}
			}

			AddLog("Новый спрайт загружен\n");

			LoadingSprite = false;
			LoadedPreview = false;
		}
	}

	/**************************************************************/

	ImGui::NewLine();
	ImGui::NewLine();

	/* Если нажата кнопка сохранить текущие настройки для объекта */
	{
		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			AddLog("Сохранение настроек для ");
			AddLog(obj->name);
			AddLog("...\n");

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
			
			AddLog("Настройки сохранены\n");

			SavingSettings = false;
		}
	}
	/*********************************************************************/
}

void GUISystem::SpawnDefaultAliveObject2DControl(AliveObject2D* obj, std::string dataPath)
{
	/* Переменные управления сбросом интерфейса */

	bool hDirty = false; // Контроль здоровья
	bool dDirty = false; // Контроль урона

	const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

	/********************************************/

	/* Элементы управления уровнями атак и здоровья */

	if (ImGui::CollapsingHeader("Общее", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Здоровье:");
		dcheck(ImGui::SliderFloat("HP", &obj->health, 0.0f, 1000.0f, "%.2f"), hDirty);

		ImGui::Text("Урон:");
		dcheck(ImGui::SliderFloat("ATK", &obj->damage, 0.0f, 1000.0f, "%.2f"), dDirty);

		ImGui::Separator();	// Разделитель
	}

	/**************************************************************/

	ImGui::NewLine();
	ImGui::NewLine();

	/* Если нажата кнопка сохранить текущие настройки для объекта */
	{
		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			AddLog("Сохранение настроек для ");
			AddLog(obj->name);
			AddLog("...\n");

			SavingSettings = true;
		}

		if (SavingSettings)
		{
			/* Сохранение позиции */

			EngineFunctions::SetNewValue<float>(
				obj->name,
				"hlt", obj->health,
				dataPath,
				&applog
				);

			EngineFunctions::SetNewValue<float>(
				obj->name,
				"dmg", obj->damage,
				dataPath,
				&applog
				);

			/**********************/

			AddLog("Настройки сохранены\n");

			SavingSettings = false;
		}
	}
	/*********************************************************************/
}

void GUISystem::ShowScriptsControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.25f),
		DispSize.y * 0.5f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("Игра", &ShowScriptsSettings, ImGuiWindowFlags_NoResize))
	{
		/* Переменные управления сбросом интерфейса */

		//bool posDirty = false;	 // Контроль позиции
		//bool speedDirty = false; // Контроль скорости
		//
		//const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

		/********************************************/

		/* Элементы управления игровым циклом игры */

		if (ImGui::CollapsingHeader("Запуск", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Запустить"))
			{
				AddLog("Запуск игрового цикла...\n");
				
				persons->IsScriptsRunning = true;

				AddLog("Цикл запущен\n");
			}

			ImGui::SameLine();

			if (ImGui::Button("Остановить"))
			{
				AddLog("Остановка игрового цикла...\n");

				persons->IsScriptsRunning = false;

				AddLog("Цикл остановлен\n");
			}

			ImGui::Separator();
		}

		/*******************************************/

		if (ImGui::Button("Сохранить"))
		{
			AddLog("Сохранение настроек камеры...\n");

			SavingSettings = true;
		}

		if (SavingSettings)
		{

			AddLog("Настройки сохранены\n");

			SavingSettings = false;
		}
	}

	ImGui::End();
}

void GUISystem::ShowTriggerInfoControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.2f),
		DispSize.y * 0.1f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("Контроль триггеров", &ShowTriggerInfoSettings, ImGuiWindowFlags_NoResize))
	{
		ImGui::Checkbox("Игнорировать триггеры", &IsTriggersAvailable);
	}

	ImGui::End();
}

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
		ImGuiWindowFlags_NoMove			  | ImGuiWindowFlags_NoResize		 | 
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | 
		ImGuiWindowFlags_NoCollapse		  | ImGuiWindowFlags_NoNav			 | ImGuiWindowFlags_NoBringToFrontOnFocus))
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
	if (ImGui::Begin("График FPS", &ShowFPSChart))
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

			pYMin = average - 20.0;
			pYMax = average + 20.0;
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
			ImGuiWindowFlags_NoDecoration    | ImGuiWindowFlags_AlwaysAutoResize   |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoBackground  | ImGuiWindowFlags_NoInputs))
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
			ImGuiWindowFlags_NoDecoration    | ImGuiWindowFlags_AlwaysAutoResize   |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
			| ImGuiWindowFlags_NoBackground  | ImGuiWindowFlags_NoInputs))
		{
			ImGui::Text(hb.name.c_str());
			ImGui::End();
		}
	}
}

void GUISystem::ShowPhysicsEngineSettings()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 0.95f));
	if (ImGui::Begin("Настройки физического движка", &ShowPhysicsSettings, BIG_POPUP_PANEL_FLAGS))
	{
		using namespace Physics;

		if (ImGui::CollapsingHeader("Общая информация", ImGuiTreeNodeFlags_DefaultOpen))
		{

			ImGui::TextWrapped(
				"Физический движок позволяет настраивать коллизию объектов и их физические параметры. "
				"Основными примитивами для создания объектов являтся линии и прямоугольники (Hit-Box).\n");
			ImGui::TextWrapped("Занимаемый объём памяти данными о линиях %d байт (%.1f килобайт).\n",		   sizeof(phEngPtr->lines)    + sizeof(Line)   * phEngPtr->lines.size(),    float(sizeof(phEngPtr->lines)    + sizeof(Line)   * phEngPtr->lines.size())    / (1024.0f));
			ImGui::TextWrapped("Занимаемый объём памяти данными о прямоугольниках %d байт (%.1f килобайт).\n", sizeof(phEngPtr->hitboxes) + sizeof(HitBox) * phEngPtr->hitboxes.size(), float(sizeof(phEngPtr->hitboxes) + sizeof(HitBox) * phEngPtr->hitboxes.size()) / (1024.0f));

			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Цвета"))
		{
			ImGui::ColorEdit3("Цвет линий", phEngPtr->lineColor);
			ImGui::ColorEdit3("Цвет прямоугольников", phEngPtr->hbColor);

			if (ImGui::Button("Сохранить"))
			{
				AddLog("Сохранение цветовых настроек физического движка...\n");

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

				AddLog("Настройки сохранены\n");
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
				AddLog("Сохранение настроек физического движка...\n");

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

void GUISystem::ShowGraphicsEngineSettings()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 0.95f));
	if (ImGui::Begin("Настройки графики", &ShowGraphicsSettings, BIG_POPUP_PANEL_FLAGS))
	{
		if (ImGui::CollapsingHeader("Настройки мира", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Границы мира");
			ImGui::SliderFloat("X", &wnd->Gfx().POS_X_LIMIT, 1000.0f, 500000.0f);
			ImGui::SliderFloat("Y", &wnd->Gfx().POS_Y_LIMIT, 1000.0f, 500000.0f);

			if (ImGui::Button("Сохранить"))
			{
				AddLog("Сохранение настроек мира графического движка...\n");

				EngineFunctions::SetNewValue<float>(
					"settings",
					"lim-x",
					wnd->Gfx().POS_X_LIMIT,
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"lim-y",
					wnd->Gfx().POS_Y_LIMIT,
					wnd->Gfx().dataPath,
					&applog
					);

				AddLog("Настройки сохранены\n");
			}

			ImGui::Separator();
		}

		if(ImGui::CollapsingHeader("Цветовые настройки"))
		{
			ImGui::Checkbox("Показать задний фон", &wnd->Gfx().IsBackgroundDrawn);
			ImGui::ColorEdit3("Цвет заднего фона", wnd->Gfx().backgroundColor);
			
			ImGui::NewLine();
			
			ImGui::Checkbox("Показать сетку", &wnd->Gfx().IsGridDrawn);
			ImGui::ColorEdit3("Цвет сетки", wnd->Gfx().gridColor);
			ImGui::SliderFloat("Размер сетки", &wnd->Gfx().gridScale, 1.0f, 1000.0f);

			if (ImGui::Button("Сохранить"))
			{
				AddLog("Сохранение цветовых настроек графического движка...\n");

				EngineFunctions::SetNewValue<bool>(
					"settings",
					"b-s",
					wnd->Gfx().IsBackgroundDrawn,
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"b-r",
					wnd->Gfx().backgroundColor[0],
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"b-g",
					wnd->Gfx().backgroundColor[1],
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"b-b",
					wnd->Gfx().backgroundColor[2],
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<bool>(
					"settings",
					"g-s",
					wnd->Gfx().IsGridDrawn,
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"g-sc",
					wnd->Gfx().gridScale,
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"g-r",
					wnd->Gfx().gridColor[0],
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"g-g",
					wnd->Gfx().gridColor[1],
					wnd->Gfx().dataPath,
					&applog
					);

				EngineFunctions::SetNewValue<float>(
					"settings",
					"g-b",
					wnd->Gfx().gridColor[2],
					wnd->Gfx().dataPath,
					&applog
					);

				AddLog("Настройки сохранены\n");
			}

			ImGui::Separator();
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

void GUISystem::ShowProjectControl()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 0.95f));
	if (ImGui::Begin("Настройки проекта", &ShowProjectSettings, BIG_POPUP_PANEL_FLAGS))
	{
		if (ImGui::CollapsingHeader("Общая информация", ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::ostringstream oss;
			oss << "Текущий проект: "  << EngineFunctions::GetProjectName() << "\n"
				<< "Количество сцен: " << EngineFunctions::GetScenesNames().size();
			ImGui::TextWrapped(oss.str().c_str());

			ImGui::Separator();
		}
	}

	ImGui::End();
	ImGui::PopStyleColor();
}

void GUISystem::ShowViewportControl()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 0.95f));
	if (ImGui::Begin("Настройки Viewport", &ShowViewportSettings, BIG_POPUP_PANEL_FLAGS))
	{
		if (ImGui::CollapsingHeader("Общая информация", ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::ostringstream oss;
			oss << "Viewport - это отображаемая прямоуглольная область на экране. Задаётся шириной и высотой. Нужен " <<
				   "для корректного отображения содержимого на разных мониторах.\n";

			oss << "Примеры разрешений:\n" <<
					" - 1366x768\n" <<
					" - 1440x900\n" <<
					" - 1536x864\n" <<
					" - 1600x900\n" <<
					" - 1920x1080";

			ImGui::TextWrapped(oss.str().c_str());

			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Настройки"))
		{
			bool dirty = false;

			const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

			dcheck(ImGui::SliderInt("Ширина", &viewportWidth , 100, 4000), dirty);
			dcheck(ImGui::SliderInt("Высота", &viewportHeight, 100, 2500), dirty);		

			if (ImGui::Button("Применить"))
			{
				AddLog("Изменение Viewport...\n");

				D3D11_VIEWPORT vp;
				vp.Width =  (float)viewportWidth;
				vp.Height = (float)viewportHeight;
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				vp.TopLeftX = float(1366 - viewportWidth) / 2.0f; // Нужно брать актуальные параметры экрана !!!
				vp.TopLeftY = float(768 - viewportHeight) / 2.0f; // Нужно брать актуальные параметры экрана !!!	

				wnd->Gfx().SetViewPort(vp);

				AddLog("Viewport обновлён\n");
			}

			ImGui::Separator();
		}
	}
	ImGui::PopStyleColor();

	ImGui::End();
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
				 "[x: " << firstPoint.x + camera->position.x <<
				"; y: " << firstPoint.y + camera->position.y << "]\n";

			AddLog(oss);
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
				 "[x: " << secondPoint.x + camera->position.x <<
				"; y: " << secondPoint.y + camera->position.y << "]\n";

			AddLog(oss);

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

std::string GUISystem::ShowLoadingSpriteDilaog()
{
	std::string chosenSprite = "";

	SetPanelSizeAndPosition(0, 0.60f, 0.70f, 0.2f, 0.15f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 1.0f));
	if (ImGui::Begin("Загрузить изображение", NULL, BIG_POPUP_PANEL_FLAGS))
	{
		namespace fs = std::filesystem;

		fs::path dir = "Assets/Images/";
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
			IsAddingPerson = false;
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
	if (ImGui::Begin("Добавление объекта", NULL, BIG_POPUP_PANEL_FLAGS))
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
				n << "obj " << Iobjects->elements.size();
				
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

std::optional<PersonData> GUISystem::ShowAddingPersonDialog(float dt)
{
	PersonData data;

	SetPanelSizeAndPosition(0, 0.3f, 0.2f, 0.35f, 0.4f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 1.0f));
	if (ImGui::Begin("Добавление персонажа", NULL, BIG_POPUP_PANEL_FLAGS))
	{
		ImGui::Text("Путь к спрайту: ");

		if (PersonPath == "")
		{
			ImGui::Text("...");
		}
		else
		{
			ImGui::Text(PersonPath.c_str());
		}

		ImGui::SameLine();

		if (ImGui::Button("Выбрать"))
		{
			ChoosingPerson = true;
		}

		if (ChoosingPerson)
		{
			PersonPath = ShowLoadingSpriteDilaog();

			if (PersonPath != "")
			{
				std::ostringstream n;
				n << "person " << persons->elements.size() + 1;

				data.name = n.str();
				data.pathToSprite = PersonPath;
				data.position = DirectX::XMFLOAT2(0.0f, 0.0f);
				data.layer = objQueue->queue.size();

				ChoosingPerson = false;
				CreatingAnimation = true;
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Отмена"))
		{
			ChoosingPerson = false;
			LoadingSprite = false;
			IsAddingPerson = false;
			CreatingAnimation = false;
		}

		ImGui::End();
		ImGui::PopStyleColor();

		return data;
	}
}

std::vector<AnimationData> GUISystem::ShowAnimationCreatingDialog(float dt)
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.6f),
		DispSize.y * 0.7f
	);

	SetPanelSizeAndPosition(0, 0.60f, 0.70f, 0.2f, 0.1f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.039f, 0.0f, 0.015f, 1.0f));
	if (ImGui::Begin("Создание/Изменение анимации", NULL, BIG_POPUP_PANEL_FLAGS))
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
					ImGui::BeginChild("Превью спрайта", { PanelSize.x / 2 - 2, PanelSize.y });

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
					dcheck(ImGui::SliderFloat("Задержка", &framesHoldTime, 0.0f, 1.0f, "%.2f"), a_fDirty);

					if (ImGui::Button("Создать анимацию"))
					{
						animationsPreview.emplace_back(Animation((int)newFrameWidth, (int)newFrameHeight * (int)animSelectedId, (int)newFrameWidth, (int)newFrameHeight, maxFrames, animSpritePreview, framesHoldTime, animationNames[animSelectedId]));
						animationsData.emplace_back(AnimationData((int)newFrameWidth, (int)newFrameHeight * (int)animSelectedId, (int)newFrameWidth, (int)newFrameHeight, maxFrames, framesHoldTime, animationNames[animSelectedId]));
					}

					if (ImGui::Button("Завершить"))
					{
						size_t n;

						for (n = 0; n < animationNames.size(); n++)
						{
							if (animationNames[n] == "Покой влево")
							{
								break;
							}
						}

						for (size_t i = n; i < animationNames.size(); i++)
						{
							animationsPreview.emplace_back(Animation(0, (int)newFrameHeight * int(i - n), (int)newFrameWidth, (int)newFrameHeight, 1, animSpritePreview, framesHoldTime, animationNames[i]));
							animationsData.emplace_back(AnimationData(0, (int)newFrameHeight * int(i - n), (int)newFrameWidth, (int)newFrameHeight, 1, framesHoldTime, animationNames[i]));
						}

						ChoosingAnimation = false;
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

							for (size_t i = 0; i < animationNames.size(); i++)
							{
								if (a.name == animationNames[i])
								{
									animPlayingId = i;
								}
							}

							previewSize = ImVec2(
								(float)a.width * scaleFrame,
								(float)a.height * scaleFrame
							);

							ltNormPixel = ImVec2(
								(float)a.width + float(a.width * a.iCurFrame),
								(float)a.height * (float)animPlayingId
							);

							rtNormPixel = ImVec2(
								2.0f * (float)a.width + float(a.width * a.iCurFrame),
								(float)a.height + float(a.height * animPlayingId)
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
							animationsData.clear();
						}
					}

					ImGui::EndChild();
				}
				else
				{
					animPath = ShowLoadingSpriteDilaog();

					if (animPath != "")
					{
						animSpritePreview = Surface2D(animPath);
					}
				}
			}

			ImGui::Separator();
		}

		ImGui::NewLine();

		if (ImGui::Button("Отмена"))
		{
			LoadingSprite = false;
			CreatingAnimation = false;
			IsAddingPerson = false;
		}

		ImGui::End();
	}
	ImGui::PopStyleColor();

	if (ChoosingAnimation)
	{
		return std::vector<AnimationData>();
	}
	else
	{
		return animationsData;
	}
}

/*******************************************/

/* Методы сохранения данных */

void GUISystem::SaveAll()
{
	AddLog("Сохранение всех настроек и изменений...\n");

	SaveMainPersonData();
	SavePersonsData();
	SaveIobjData();
	SaveTriggersData();
	SaveLayersData();
	SaveCameraData();
	SaveScenesData();
	SavePhysicsEngineData();
	SaveGraphicsEngineData();

	AddLog("Все настройки сохранены успешно\n");
}

void GUISystem::SaveMainPersonData()
{
	AddLog("Сохранение настроек для главного персонажа...\n");

	/* Сохранение позиции */

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

	/**********************/

	/* Сохранение изображения */

	EngineFunctions::SetNewValue<std::string>(
		hero->name,
		"path", hero->image.GetFileName(),
		hero->dataPath,
		&applog
		);

	/**************************/

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

	EngineFunctions::SaveColorData(hero->name, hero->chromaKey, hero->dataPath);

	/*******************************/

	/* Пересохранение hitbox */

	auto actual_hb = hero->hitbox;

	EngineFunctions::SaveHitBoxData(hero->name, actual_hb, hero->dataPath, &applog);

	/*************************/

	/* Сохранение настроек камеры */

	EngineFunctions::SetNewValue<size_t>(
		hero->name,
		"camera-mode", static_cast<size_t>(hero->cameraMode),
		hero->dataPath,
		&applog
		);

	/******************************/

	/* Сохранение настроек боя */

	EngineFunctions::SetNewValue<float>(
		hero->name,
		"hlt", hero->health,
		hero->dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		hero->name,
		"dmg", hero->damage,
		hero->dataPath,
		&applog
		);

	/***************************/

	/* Сохранение настроек анимаций */

	EngineFunctions::SetNewValue<float>(
		hero->name,
		"a-ft", hero->holdTime,
		hero->dataPath,
		&applog
		);

	/********************************/

	/* Сохранение настроек физики для персонажа */

	EngineFunctions::SetNewValue<float>(
		hero->name,
		"gravity", hero->gravity,
		hero->dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<int>(
		hero->name,
		"j-h", hero->jump_height,
		hero->dataPath,
		&applog
		);

	/********************************************/

	AddLog("Настройки сохранены\n");
}

void GUISystem::SavePersonsData()
{
	AddLog("Сохранение настроек для персонажей...\n");

	for (size_t i = 0; i < persons->elements.size(); i++)
	{
		/* Сохранение позиции */

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"pos-x", persons->elements[i]->position.x,
			persons->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"pos-y", persons->elements[i]->position.y,
			persons->dataPath,
			&applog
			);

		/**********************/

		/* Сохранение скорости */

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"speed", persons->elements[i]->speed,
			persons->dataPath,
			&applog
			);

		/***********************/

		/* Сохранение настроек эффекта */

		EngineFunctions::SetNewValue<bool>(
			persons->elements[i]->name,
			"eff-a", persons->elements[i]->effect.Active,
			persons->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"eff-d", persons->elements[i]->effect.Duration,
			persons->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"eff-t", persons->elements[i]->effect.Time,
			persons->dataPath,
			&applog
			);

		EngineFunctions::SaveColorData(persons->elements[i]->name, persons->elements[i]->chromaKey, persons->dataPath);

		/*******************************/

		/* Пересохранение hitbox */

		auto hitbox = persons->elements[i]->hitbox;
		EngineFunctions::SaveHitBoxData(persons->elements[i]->name, hitbox, persons->dataPath, &applog);

		/*************************/

		/* Сохранение настроек боя */

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"hlt", persons->elements[i]->health,
			persons->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			persons->elements[i]->name,
			"dmg", persons->elements[i]->damage,
			persons->dataPath,
			&applog
			);

		/***************************/

		/* Сохранение скрипта */

		EngineFunctions::SetNewValue<std::string>(
			persons->elements[i]->name,
			"script", persons->elements[i]->scriptPath,
			persons->dataPath,
			&applog
			);

		/**********************/
	}

	AddLog("Настройки сохранены\n");
}

void GUISystem::SaveIobjData()
{
	AddLog("Сохранение настроек для интерактивных объектов...\n");

	for (size_t i = 0; i < Iobjects->elements.size(); i++)
	{
		/* Сохранение позиции */

		EngineFunctions::SetNewValue<float>(
			Iobjects->elements[i]->name,
			"pos-x", Iobjects->elements[i]->position.x,
			Iobjects->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			Iobjects->elements[i]->name,
			"pos-y", Iobjects->elements[i]->position.y,
			Iobjects->dataPath,
			&applog
			);

		/**********************/

		EngineFunctions::SetNewValue<float>(
			Iobjects->elements[i]->name,
			"g-deep", Iobjects->elements[i]->deep,
			Iobjects->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<bool>(
			Iobjects->elements[i]->name,
			"g-able", Iobjects->elements[i]->drawGhostable,
			Iobjects->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<bool>(
			Iobjects->elements[i]->name,
			"chr-a", Iobjects->elements[i]->chromaKeyAble,
			Iobjects->dataPath,
			&applog
			);

		EngineFunctions::SaveColorData(Iobjects->elements[i]->name, Iobjects->elements[i]->chromaKey, Iobjects->dataPath);

		auto hitbox = Iobjects->elements[i]->hitbox;
		EngineFunctions::SaveHitBoxData(Iobjects->elements[i]->name, hitbox, Iobjects->dataPath, &applog);
	}

	AddLog("Настройки сохранены\n");
}

void GUISystem::SaveTriggersData()
{
	AddLog("Сохранение настроек для триггеров...\n");

	for (size_t i = 0; i < triggers->elements.size(); i++)
	{
		/* Сохранение координат */

		EngineFunctions::SetNewValue<float>(
			triggers->elements[i].name,
			"start-x", triggers->elements.at(i).line.start.x,
			triggers->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			triggers->elements[i].name,
			"end-x", triggers->elements.at(i).line.end.x,
			triggers->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			triggers->elements[i].name,
			"start-y", triggers->elements.at(i).line.start.y,
			triggers->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			triggers->elements[i].name,
			"end-y", triggers->elements.at(i).line.end.y,
			triggers->dataPath,
			&applog
			);

		/************************/
	}
	
	AddLog("Настройки сохранены\n");
}

void GUISystem::SaveLayersData()
{
	AddLog("Сохранение настроек для слоёв...\n");

	for (size_t i = 0; i < objQueue->queue.size(); i++)
	{
		std::string pathToFile = "";

		if (objQueue->queue[i]->name.find("obj") != objQueue->queue[i]->name.npos)
		{
			pathToFile = Iobjects->dataPath;
		}
		else if (objQueue->queue[i]->name.find("mainperson") != objQueue->queue[i]->name.npos)
		{
			pathToFile = hero->dataPath;
		}
		else
		{
			pathToFile = persons->dataPath;
		}

		EngineFunctions::SetNewValue<size_t>(
			objQueue->queue[i]->name,
			"layer", objQueue->queue[i]->layer,
			pathToFile,
			&applog);
	}

	AddLog("Настройки сохранены\n");
}

void GUISystem::SaveCameraData()
{
	AddLog("Сохранение настроек для камеры...\n");

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

	AddLog("Настройки сохранены\n");
}

void GUISystem::SaveScenesData()
{
}

void GUISystem::SavePhysicsEngineData()
{
	AddLog("Сохранение настроек физического движка...\n");

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

	for (size_t i = 0; i < phEngPtr->lines.size(); i++)
	{
		/* Сохранение координат */

		EngineFunctions::SetNewValue<float>(
			phEngPtr->lines[i].name,
			"start-x", phEngPtr->lines[i].start.x,
			phEngPtr->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			phEngPtr->lines[i].name,
			"start-y", phEngPtr->lines[i].start.y,
			phEngPtr->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			phEngPtr->lines[i].name,
			"end-x", phEngPtr->lines[i].end.x,
			phEngPtr->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			phEngPtr->lines[i].name,
			"end-y", phEngPtr->lines[i].end.y,
			phEngPtr->dataPath,
			&applog
			);

		/************************/
	}

	for (size_t i = 0; i < phEngPtr->hitboxes.size(); i++)
	{
		/* Сохранение координат */

		EngineFunctions::SetNewValue<float>(
			phEngPtr->hitboxes[i].name,
			"lt-x", phEngPtr->hitboxes[i].coordinates.x,
			phEngPtr->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			phEngPtr->hitboxes[i].name,
			"lt-y", phEngPtr->hitboxes[i].coordinates.y,
			phEngPtr->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			phEngPtr->hitboxes[i].name,
			"rb-x", phEngPtr->hitboxes[i].coordinates.z,
			phEngPtr->dataPath,
			&applog
			);

		EngineFunctions::SetNewValue<float>(
			phEngPtr->hitboxes[i].name,
			"rb-y", phEngPtr->hitboxes[i].coordinates.w,
			phEngPtr->dataPath,
			&applog
			);

		/************************/
	}

	AddLog("Настройки сохранены\n");
}

void GUISystem::SaveGraphicsEngineData()
{
	AddLog("Сохранение настроек физического движка...\n");

	EngineFunctions::SetNewValue<float>(
		"settings",
		"lim-x",
		wnd->Gfx().POS_X_LIMIT,
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"lim-y",
		wnd->Gfx().POS_Y_LIMIT,
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<bool>(
		"settings",
		"b-s",
		wnd->Gfx().IsBackgroundDrawn,
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"b-r",
		wnd->Gfx().backgroundColor[0],
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"b-g",
		wnd->Gfx().backgroundColor[1],
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"b-b",
		wnd->Gfx().backgroundColor[2],
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<bool>(
		"settings",
		"g-s",
		wnd->Gfx().IsGridDrawn,
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"g-sc",
		wnd->Gfx().gridScale,
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"g-r",
		wnd->Gfx().gridColor[0],
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"g-g",
		wnd->Gfx().gridColor[1],
		wnd->Gfx().dataPath,
		&applog
		);

	EngineFunctions::SetNewValue<float>(
		"settings",
		"g-b",
		wnd->Gfx().gridColor[2],
		wnd->Gfx().dataPath,
		&applog
		);

	AddLog("Настройки сохранены\n");
}

/****************************/