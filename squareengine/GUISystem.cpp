#include "GUISystem.h"

#include "EngineFunctions.hpp"
#include "HitBox.h"

GUISystem::GUISystem(std::shared_ptr<Window> wnd, PersonContainer* persons)
	:
	wnd(wnd),
	persConPtr(persons)
{
	SetGUIColors();

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

	if (mouseHelpInfo == "")
	{
		std::ostringstream pos;
		pos << "x: " << ImGui::GetMousePos().x
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
			if (ImGui::BeginMenu("Объекты"))
			{
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
		ShowFPSAndGPU();
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
	ImGui::SetNextWindowPos({ ImGui::GetMousePos().x, ImGui::GetMousePos().y + 30 }, ImGuiCond_Always);
	if (ImGui::Begin("mouse info helper", (bool*)0,
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
		| ImGuiWindowFlags_NoBackground))
	{
		ImGui::Text(info.c_str());
		ImGui::End();
	}
}

void GUISystem::DisableSides()
{
	ShowPersonEnum = false;
	ShowPersonSettings = false;
	ShowTriggersList = false;
	ShowTriggersSettings = false;
}

/****************************************/

/* Методы отрисовки конкретных интерфейсов */

void GUISystem::ShowFPSAndGPU()
{
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

void GUISystem::ShowMouseCoordinates()
{
}

void GUISystem::ShowLog()
{
	ImGui::Begin("Лог", NULL, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("Лог", NULL);
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
			if (persConPtr->persons.at(k)->GetName() == personSelected)
			{				
				if (ImGui::BeginChild(""))
				{
					/* Переменные управления сбросом интерфейса */

					bool posDirty   = false;	// Контроль позиции
					bool effDirty   = false;	// Котнроль эффекта
					bool speedDirty = false;	// Котнроль эффекта

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; }; // Выражение

					/********************************************/

					/* Элементы управления позицией и скорости персонажа */

					ImGui::Text("Позиция:");
					dcheck(ImGui::SliderFloat("X", &persConPtr->persons.at(k)->GetPositionPtr()->x, -1000.0f, 1000.0f, "%.2f"), posDirty);
					dcheck(ImGui::SliderFloat("Y", &persConPtr->persons.at(k)->GetPositionPtr()->y, -1000.0f, 1000.0f, "%.2f"), posDirty);

					ImGui::Text("Скорость:");
					dcheck(ImGui::SliderFloat("",   persConPtr->persons.at(k)->GetSpeedPtr(),		 0.0f,    1000.0f, "%.2f"), speedDirty);

					/******************************************/

					ImGui::Separator();	// Разделитель

					/* Элементы управления эффектом персонажа */

					ImGui::Text("Эффект:");
					dcheck(ImGui::SliderFloat("Продолжитель.", persConPtr->persons.at(k)->GetEffectDuration(), 0.0f, 100.0f, "%.3f"), effDirty);
					dcheck(ImGui::SliderFloat("Время",		   persConPtr->persons.at(k)->GetEffectTime(),	   0.0f, 100.0f, "%.3f"), effDirty);

					ImGui::Checkbox("Активен", persConPtr->persons.at(k)->GetEffectActive());

					/******************************************/

					ImGui::Separator();	// Разделитель

					/* Элементы управления хитбоксом персонажа */

					ImGui::Text("Hit-box:");
					ImGui::Checkbox("Показать", persConPtr->persons.at(k)->GetHitBoxVisability());

					/* Если нажата кнопка изменить HitBox */
					{
						if (ImGui::Button("Изменить", ImVec2(100, 20)))
						{
							AddLog("Изменение Hit-box:\n");

							DrawingHitBox = true;
							*persConPtr->persons.at(k)->GetHitBoxVisability() = false;
						}

						if (DrawingHitBox)
						{
							if (!SettedFirstPoint)
							{
								mouseHelpInfo = "Нажмите ЛКМ, чтобы поставить\nпервую точку.";

								if (wnd->mouse.LeftIsPressed() && wnd->mouse.IsInWindow())
								{
									auto pos = wnd->mouse.GetPos();
									firstPoint = { pos.first, pos.second };
									
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

								HitBox hb(firstPoint.x, firstPoint.y, ms_posX, ms_posY);
								wnd->Gfx().DrawHitBox(hb);

								if (wnd->mouse.RightIsPressed() && wnd->mouse.IsInWindow())
								{
									auto pos = wnd->mouse.GetPos();
									secondPoint = { pos.first, pos.second };

									HitBox hb_new(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
									persConPtr->persons.at(k)->SetHitBox(hb);
									*persConPtr->persons.at(k)->GetHitBoxVisability() = true;									
									
									std::ostringstream oss;
									oss << "Поставлена вторая точка:\n" <<
										"[x: " << secondPoint.x <<
										"; y: " << secondPoint.y << "]\n";

									AddLog(oss.str().c_str());

									AddLog("Сохранение Hit-box:\n");

									auto actual_hb = persConPtr->persons.at(k)->GetHitBox();

									EngineFunctions::SetNewValue<int>(
										personSelected,
										"hb-ltx", actual_hb.GetCoordinates().x,
										persConPtr->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<int>(
										personSelected,
										"hb-lty", actual_hb.GetCoordinates().y,
										persConPtr->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<int>(
										personSelected,
										"hb-rbx", actual_hb.GetCoordinates().z,
										persConPtr->dataPath,
										&applog
										);

									EngineFunctions::SetNewValue<int>(
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
							}
						}
					}
					/**************************************/

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
								"pos-x", persConPtr->persons.at(k)->GetPosition().x,
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"pos-y", persConPtr->persons.at(k)->GetPosition().y,
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"speed", *persConPtr->persons.at(k)->GetSpeedPtr(),
								persConPtr->dataPath,
								&applog
								);

							/**********************/

							/* Сохранение настроек эффекта */
							
							EngineFunctions::SetNewValue<bool>(
								personSelected,
								"eff-a", *persConPtr->persons.at(k)->GetEffectActive(),
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"eff-d", *persConPtr->persons.at(k)->GetEffectDuration(),
								persConPtr->dataPath,
								&applog
								);

							EngineFunctions::SetNewValue<float>(
								personSelected,
								"eff-t", *persConPtr->persons.at(k)->GetEffectTime(),
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

/*******************************************/