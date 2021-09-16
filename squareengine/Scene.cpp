#include "Scene.h"

#include "EngineUtil.h"

#if IS_ENGINE_MODE
#include "AdapterData.h"
#include "imgui/imgui.h"
#endif // IS_ENGINE_MODE

#include <sstream>

Scene::Scene(std::string name,	std::shared_ptr<Window> _wnd, 
			 std::string data)
	:
	wnd(_wnd),
	name(name),
	sdr(data),
	pc(sdr.GetPersonContainerPath())
	
{	
#if IS_ENGINE_MODE
	SetGuiColors();
#endif // IS_ENGINE_MODE
}

Scene::~Scene()
{}

void Scene::ProcessInput(float dt)
{
	while (const auto e = wnd->kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd->CursorEnabled())
			{
				wnd->DisableCursor();
				wnd->mouse.EnableRaw();
			}
			else
			{
				wnd->EnableCursor();
				wnd->mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		case VK_SPACE:
			//person.ActivateEffect();
		default:
			break;
		}
	}

	DirectX::XMFLOAT2 dir = { 0.0f,0.0f };
	if (!wnd->CursorEnabled())
	{
		if (wnd->kbd.KeyIsPressed('W'))
		{
			dir.y -= 1.0f;
		}
		if (wnd->kbd.KeyIsPressed('A'))
		{
			dir.x -= 1.0f;
		}
		if (wnd->kbd.KeyIsPressed('S'))
		{
			dir.y += 1.0f;
		}
		if (wnd->kbd.KeyIsPressed('D'))
		{
			dir.x += 1.0f;
		}
	}


	//pc.Process(&wnd->kbd, wnd->CursorEnabled(), dt);
}

void Scene::Render(float dt)
{
	/* Начало кадра */

	wnd->Gfx().BeginFrame();

	ShowGui();
	pc.Draw(wnd->Gfx());

	wnd->Gfx().EndFrame();

	/***************/
}

std::pair<std::string, bool> Scene::IsOnTheSceneTrigger()
{
	return std::pair{ "NULL", false };
}

#if IS_ENGINE_MODE
/***************** Интерфейс *****************/

void Scene::SetPanelWidthAndPos(int corner, float width, float height, float x_offset, float y_offset)
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

void Scene::ShowMenu()
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
			if (ImGui::MenuItem("Редактор узлов"))
			{
				ShowNodeEditorWnd ? ShowNodeEditorWnd = false : ShowNodeEditorWnd = true;
			}

			if (ImGui::BeginMenu("Объекты"))
			{
				if (ImGui::MenuItem("Точечные источники света"))
				{
					if (ShowPLightsList && ShowPLightsSettings)
					{
						DisableSides();

						ShowPLightsList = false;
						ShowPLightsSettings = false;
					}
					else
					{
						DisableSides();

						ShowPLightsList = true;
						ShowPLightsSettings = true;
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

				if (ImGui::BeginMenu("Модели"))
				{
					if (ImGui::MenuItem("Окна"))
					{
						if (ShowModelsList && ShowModelsSettings)
						{
							DisableSides();

							ShowModelsList = false;
							ShowModelsSettings = false;
						}
						else
						{
							DisableSides();

							ShowModelsList = true;
							ShowModelsSettings = true;
						}
					}

					if (ImGui::MenuItem("Добавить"))
					{
					}

					//objects.models.OpenDialog();

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Камеры"))
				{
					if (ShowCamsList)
					{
						DisableSides();

						ShowCamsList = false;
					}
					else
					{
						DisableSides();

						ShowCamsList = true;
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("FPS & GPU"))
			{
				ShowHardwareInfo ? ShowHardwareInfo = false : ShowHardwareInfo = true;
			}

			if (ImGui::MenuItem("Лог"))
			{
				ShowLogs ? ShowLogs = false : ShowLogs = true;
			}

			if (ImGui::MenuItem("Отключить панели"))
			{
				DisableAll();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Scene::ShowLeftSide()
{
	/* Левая сторона */

	SetPanelWidthAndPos(0, 0.2f, 0.75f);

	/* Содержимое */
	
	if (ShowModelsList)
	{
		//objects.models.ShowLeftPanel();
	}
	else if (ShowTriggersList)
	{
		//objects.triggers.ShowLeftPanel();
	}
	else if (ShowPLightsList)
	{
		//objects.pointLights.ShowLeftPanel();
	}
	else if (ShowCamsList)
	{
		//objects.cameras.ShowLeftPanel();
	}

	/**************/

	ImGui::GetStyle().DisplayWindowPadding = { 0.0f, 0.0f };
	ImGui::GetStyle().DisplaySafeAreaPadding = { 0.0f, 0.0f };
	
	/* Конец левой стороны */
}

void Scene::ShowRightSide()
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

	SetPanelWidthAndPos(corner, 0.2f, 1.0f);

	/* Содержимое */

	if (ShowModelsSettings)
	{
		//objects.models.ShowRightPanel();

		ImGui::SetNextWindowPos({ round(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.2f, io.DisplaySize.y * 0.2f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowTriggersSettings)
	{
		//objects.triggers.ShowRightPanel();

		ImGui::SetNextWindowPos({ round(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f }, ImGuiCond_FirstUseEver);
		ShowTrigCheck();
	}
	else if (ShowPLightsSettings)
	{
		//objects.pointLights.ShowRightPanel();

		ImGui::SetNextWindowPos({ round(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f }, ImGuiCond_FirstUseEver);
	}
	else if (ShowCamsList)
	{
		//objects.cameras.ShowRightPanel();

		ImGui::SetNextWindowPos({ round(io.DisplaySize.x - RightPanelW), MenuHeight }, 0, RightPanelPivot);
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f }, ImGuiCond_FirstUseEver);
	}
	
	/**************/

	/* Конец правой стороны */
}

void Scene::ShowLeftBottomSide()
{
	/* Левая нижняя сторона */
	
	SetPanelWidthAndPos(2, 0.2f, 0.25f);

	/* Содержимое */

	if (ShowHardwareInfo)
	{
		ShowFPSAndGPU();
	}

	/**************/

	/* Конец левой нижней стороны */
}

void Scene::ShowBottomPanel()
{
	/* Нижняя стороны */

	SetPanelWidthAndPos(3, 0.6f, 0.25f, -0.2f);

	/* Содержимое */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* Конец нижней стороны */
}

void Scene::ShowFPSAndGPU()
{
	auto GPU_Data = AdapterReader::GetAdapterData();

	if (ImGui::Begin("Представление", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text("FPS:");
		ImGui::Text("%.3f мс/кадр (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Separator();

		ImGui::Text("Графическое оборудование:");
		for (auto& d : GPU_Data)
		{
			char name_gpu[256];
			sprintf_s(name_gpu, "%ws", d.desc.Description);

			if (ImGui::TreeNode(name_gpu))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(133.0f/255.0f, 219.0f/255.0f, 15.0f/255, 1.0f));
				ImGui::Text("Память: ~%.1f ГБ", round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
				ImGui::PopStyleColor();
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void Scene::ShowLog()
{
	pc.DrawLog();
}

void Scene::ShowNodeEditor()
{
	if (ShowNodeEditorWnd)
	{
		//objects.nEditor.Show(&ShowNodeEditorWnd);
	}
}

void Scene::DisableSides()
{
	ShowModelsList = false;
	ShowModelsSettings = false;
	ShowTriggersList = false;
	ShowTriggersSettings = false;	
	ShowPLightsList = false;
	ShowPLightsSettings = false;
	ShowCamsList = false;
}

void Scene::DisableAll()
{
	DisableSides();
	
	ShowHardwareInfo = false;
	ShowLogs = false;
}

void Scene::ShowGui()
{
	ShowMenu();
	ShowLeftSide();
	ShowRightSide();
	ShowLeftBottomSide();
	ShowBottomPanel();

	ShowNodeEditor();
}

void Scene::SetGuiColors()
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
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.48f, 0.45f, 0.67f);	// Активные компонента
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);		// Галочка
	colors[ImGuiCol_SliderGrab] = ImVec4(0.37f, 0.70f, 0.00f, 1.00f);		// Ползунок слайдера
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.82f, 0.19f, 1.00f);	// Актвиный ползунок слайдера
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

void Scene::ShowTrigCheck()
{
	if (ImGui::Begin("Попадание", NULL, 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		if (onTrigger)
		{
			ImGui::Text("Отслежено cтолкновение");
			ImGui::Text("Цель триггера:");
			ImGui::Text(triggerGoal);
		}
		else
		{
			onTrigger = false;
			ImGui::Text("Не на триггере");
		}
	}

	ImGui::End();
}

void Scene::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}

/*********************************************/
#endif

std::string Scene::GetName() const
{
	return name;
}