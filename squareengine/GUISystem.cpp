#include "GUISystem.h"

#include "imgui/implot.h"
#include "EngineFunctions.hpp"
#include "HitBox.h"
#include "Line.h"

GUISystem::GUISystem(std::shared_ptr<Window>				 wnd,
					 PersonContainer*						 persons,
					 MainPerson*							 mPersPtr,
					 std::shared_ptr<Physics::PhysicsEngine> phEngPtr)
	:
	wnd(wnd),
	phEngPtr(phEngPtr),
	persConPtr(persons),
	mPersPtr(mPersPtr)
{
	SetGUIColors();

	for (size_t i = 0; i < N_POINTS; i++)
	{
		counters[i] = float(i);
	}

	auto GPU_Data = AdapterReader::GetAdapterData();

	for (auto& d : GPU_Data)
	{
		gpu_desc.emplace(std::wstring(d.desc.Description), round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
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

	if (ShowFPSGraph)
	{
		ShowFPS();
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

				if (ImGui::MenuItem("Физика"))
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

	SetPanelWidthAndPosition(0, 0.2f, 0.75f);

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

	SetPanelWidthAndPosition(corner, 0.2f, 1.0f);

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
		ImGui::SetNextWindowSize({ io.DisplaySize.x * 0.15f, io.DisplaySize.y * 0.15f }, ImGuiCond_FirstUseEver);

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

	SetPanelWidthAndPosition(2, 0.2f, 0.25f);

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

	SetPanelWidthAndPosition(3, 0.6f, 0.25f, -0.2f);

	/* Содержимое */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* Конец нижней стороны */
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
		ImGui::Text("FPS:");
		ImGui::Text("%.3f мс/кадр (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
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
		float average = 60.0f;
		float sum = 0.0f;

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
				sum += arr[i];
			}

			average = sum / N_POINTS;
		}



		ImPlot::SetNextPlotLimits(0, N_POINTS - 1, average - 1.0f, average + 1.0f);
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

			ImGui::Bullet();
			if (ImGui::Selectable(label, personSelected == p->get()->GetName().c_str()))
			{
				personSelected = p->get()->GetName();
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

					ImGui::Text("Позиция:");
					dcheck(ImGui::SliderFloat("X", &persConPtr->persons.at(k)->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
					dcheck(ImGui::SliderFloat("Y", &persConPtr->persons.at(k)->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

					ImGui::Text("Скорость:");
					dcheck(ImGui::SliderFloat("",  &persConPtr->persons.at(k)->speed,		0.0f,    1000.0f, "%.2f"), speedDirty);

					/******************************************/

					ImGui::Separator();	// Разделитель

					/* Элементы управления эффектом персонажа */

					ImGui::Text("Эффект:");
					dcheck(ImGui::SliderFloat("Продолжитель.", &persConPtr->persons.at(k)->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
					dcheck(ImGui::SliderFloat("Время",		   &persConPtr->persons.at(k)->effect.Time,	    0.0f, 100.0f, "%.3f"), effDirty);

					ImGui::Checkbox("Активен", &persConPtr->persons.at(k)->effect.Active);

					/******************************************/

					ImGui::Separator();	// Разделитель

					/* Элементы управления хитбоксом персонажа */
					
					ImGui::Text("Hit-box:");
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
										"[x: "  << secondPoint.x <<
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

			ImGui::Text("Позиция:");
			dcheck(ImGui::SliderFloat("X", &mPersPtr->position.x, -1000.0f, 1000.0f, "%.2f"), posDirty);
			dcheck(ImGui::SliderFloat("Y", &mPersPtr->position.y, -1000.0f, 1000.0f, "%.2f"), posDirty);

			ImGui::Text("Скорость:");
			dcheck(ImGui::SliderFloat("",  &mPersPtr->speed,	   0.0f, 1000.0f, "%.2f"), speedDirty);

			/**************************************************************/

			ImGui::Separator();	// Разделитель

			/* Элементы управления эффектом главного персонажа */

			ImGui::Text("Эффект:");
			dcheck(ImGui::SliderFloat("Продолжитель.", &mPersPtr->effect.Duration, 0.0f, 100.0f, "%.3f"), effDirty);
			dcheck(ImGui::SliderFloat("Время",		   &mPersPtr->effect.Time,	   0.0f, 100.0f, "%.3f"), effDirty);

			ImGui::Checkbox("Активен",				   &mPersPtr->effect.Active);

			/***************************************************/

			ImGui::Separator();	// Разделитель

			/* Элементы управления хитбоксом главного персонажа */
			
			ImGui::Text("Hit-box:");
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

							HitBox hb_new(mPersPtr->name + std::string(" hitbox"), firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
							mPersPtr->SetHitBox(hb);
							mPersPtr->hitbox_visability = true;

							std::ostringstream oss;
							oss << "Поставлена вторая точка:\n" <<
								"[x: "  << secondPoint.x <<
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
			/**************************************/

			/****************************************************/

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

					ImGui::Bullet();
					if (ImGui::Selectable(label, objectSelected == l->GetName().c_str()))
					{
						objectSelected = l->GetName();
					}
				}
				
				ImGui::NewLine();

				// Если нажата кнопка добавить линию
				if (ImGui::Button("Добавить"))
				{
					AddLog("Добавление линии...");
					AddLog("\n");

					AddingObject = true;
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

								// Открытие файла с данными о моделях
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

								// Новая модель
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

								// Запись в файл данных новой модели
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

					ImGui::Bullet();
					if (ImGui::Selectable(label, objectSelected == hb->GetName().c_str()))
					{
						objectSelected = hb->GetName();
					}
				}

				ImGui::NewLine();

				// Если нажата кнопка добавить hitbox
				if (ImGui::Button("Добавить"))
				{

				}

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
		// Цикл по линиям
		for (size_t k = 0; k < phEngPtr->lines.size(); k++)
		{
			// Поиск выбранной линии
			if (phEngPtr->lines.at(k).name == objectSelected)
			{
				if (ImGui::BeginChild(""))
				{
					/* Переменные управления сбросом интерфейса */

					bool posDirty = false;		// Контроль позиции начала
					bool effDirty = false;		// Котнроль эффекта
					bool speedDirty = false;	// Котнроль скорости

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
						}

						ImGui::SameLine();

						if (ImGui::Button("Нормировать Y", ImVec2(100, 20)))
						{
							AddLog("Нормирование по Ys для: ");
							AddLog(objectSelected.c_str());
							AddLog("\n");

							phEngPtr->lines.at(k).end.y = phEngPtr->lines.at(k).start.y;
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

										Line line_new(objectSelected, firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
										phEngPtr->UpdateLineAt(k, line_new);

										std::ostringstream oss;
										oss << "Поставлена вторая точка:\n" <<
											"[x: "  << secondPoint.x <<
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

	ImGui::End();
}

/*******************************************/