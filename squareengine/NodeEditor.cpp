#include "NodeEditor.h"

#include "EngineFunctions.hpp"
#include "imgui\imgui.h"

NodeEditor::NodeEditor()
{
    context = imnodes::EditorContextCreate();
}

NodeEditor::~NodeEditor()
{
    imnodes::EditorContextFree(context);
}

void NodeEditor::Show()
{
    ImGuiIO& io = ImGui::GetIO();

    const float width = 0.85f;
    const float height = 0.85f;

    float PanelW = round(io.DisplaySize.x * width);
    float PanelH = io.DisplaySize.y * height;

    ImVec2 PanelPos = ImVec2(
        io.DisplaySize.x * (1.0f - width) / 2.0f,
        io.DisplaySize.y * (1.0f - height) / 2.0f
    );

    ImGui::SetNextWindowPos(PanelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(PanelW, PanelH));

    if (ImGui::Begin("Редактор узлов", NULL,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar))
    {
        // Меню
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Редактор"))
            {
                if (ImGui::MenuItem("Закрыть"))
                {
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Поле"))
                {
                    if (ImGui::MenuItem("Очистить"))
                    {
                        links.clear();
                        nodes.clear();

                        ImGui::EndMenu();
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenuBar();

        ShowLeftPanel(ImVec2(PanelW * 0.2f, PanelH));

        ImGui::SameLine();

        ShowRightPanel(ImVec2(PanelW * 0.8f, PanelH));
    }

    ImGui::End();
}

void NodeEditor::Init()
{
    if (!IsInit)
    {
        imnodes::Style& style = imnodes::GetStyle();
        style.colors[imnodes::ColorStyle_TitleBar] = IM_COL32(232, 27, 86, 255);
        style.colors[imnodes::ColorStyle_TitleBarSelected] = IM_COL32(241, 108, 146, 255);

        imnodes::GetIO().link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyShift;
        imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

        IsInit = true;
    }
}

void NodeEditor::BeginFrame()
{
	imnodes::CreateContext();
}

void NodeEditor::EndFrame()
{
	imnodes::DestroyContext();
}

void NodeEditor::RenderNodes()
{
    // Блоки камер
    for (auto& node : nodes)
    {
        ImGui::PushItemWidth(150.0f);
        imnodes::BeginNode(node.id);
       
        /* Заголовок */
       
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.cmd.c_str());
        imnodes::EndNodeTitleBar();

        /*************/

        /* Вход */
     
        if (node.id == 0)
        {
            imnodes::BeginStaticAttribute(node.id << 8);
        }
        else 
        {
            imnodes::BeginInputAttribute(node.id << 8);
        }

        ImGui::Text("Значение команды");
        ImGui::SliderInt("val", &node.value, -10000, 10000);
       
        if (node.id == 0)
        {
            imnodes::EndStaticAttribute();
        }
        else
        {
            imnodes::EndInputAttribute();
        }
    
        /********/

        /* Выход */
        
        imnodes::BeginOutputAttribute(node.id << 16);
        const float text_width = ImGui::CalcTextSize("Выход").x;
        ImGui::Indent(150.f + ImGui::CalcTextSize("val").x - text_width);
        ImGui::NewLine();
        ImGui::TextUnformatted("Выход");
        imnodes::EndOutputAttribute();
        
        /*********/

        imnodes::EndNode();
        ImGui::PopItemWidth();
    }

    for (auto& link : links)
    {
        imnodes::Link(link.id, link.start_attr, link.end_attr);
    }
}

void NodeEditor::AddNode(size_t id, std::string cmd, int value)
{
    const float node_margin = ScriptNode::NextSize(nodes.size());

    imnodes::SetNodeEditorSpacePos(id, ImVec2(0.0f, node_margin));
    nodes.emplace_back(id, cmd, value, DirectX::XMFLOAT2{ 0.0f, node_margin });
}

void NodeEditor::ShowLeftPanel(ImVec2 size)
{
    ImGui::BeginChild("Редактор узлов | Левая панель", size);

    // Список команд
    if (ImGui::BeginCombo("Команды", cmdSelected.c_str()))
    {
        for (size_t i = 0; i < cmd_list.size(); i++)
        {
            if (ImGui::Selectable(cmd_list[i].c_str(), cmdSelected == cmd_list[i]))
            {
                AddNode(nodes.size() * i + 1, cmd_list[i], 0);
            }
        }
        ImGui::EndCombo();
    }
    
    if (ImGui::Button("Сохранить"))
    {
        if (links.size() != 0)
        {
            for (auto& link : links)
            {
                int link_start = link.start_attr;   // out id
                int link_end   = link.end_attr;     // in id

                int camId = link.start_attr >> 16;  // id start
                int modId = link.end_attr >> 8;     // id end
            }
        }
    }

    /*if (isPopup)
    {
        ImGui::OpenPopup("Подтверждение");
        if (ImGui::BeginPopup("Подтверждение"),
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)
        {
            ImGui::Text("Перезаписать камеру?");
            ImGui::Text(EngineFunctions::AttachStrings<std::string>("Текущая камера: ", FindCamNodeById(camIdToPopup)->name).c_str());

            if (ImGui::Button("Добавить"))
            {
                mcon.GetPtr2ModelByName(FindModNodeById(modIdToPopup)->name)->get()->DisconnectCamera();

                ConncetCam2Model(camIdToPopup, modIdToPopup);
                isPopup = false;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Отмена"))
            {
                isPopup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }*/

    ImGui::EndChild();
}

void NodeEditor::ShowRightPanel(ImVec2 size)
{
    imnodes::EditorContextSet(context);

    ImGui::BeginChild("Редактор узлов | Правая панель", size);

    imnodes::BeginNodeEditor();

    Init();

    RenderNodes();

    imnodes::EndNodeEditor();

    {
        Link link;
        if (imnodes::IsLinkCreated(&link.start_attr, &link.end_attr))
        {
            link.id = ++current_id;
            links.push_back(link);
        }
    }

    {
        int link_id;        
        if (imnodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                links.begin(), links.end(), [link_id](const Link& link) -> bool {
                return link.id == link_id;
            });

            assert(iter != links.end());
            links.erase(iter);
        }
    }

    ImGui::EndChild();
}

void NodeEditor::ConncetCam2Model(int cam_id, int mod_id)
{
 /*   mcon.GetPtr2ModelByName(FindModNodeById(mod_id)->name)->get()->ConnectCamera(
        camcon.GetPtr2CameraByName(FindCamNodeById(cam_id)->name),
        DirectX::XMFLOAT3(
            FindCamNodeById(cam_id)->offset.x,
            FindCamNodeById(cam_id)->offset.y,
            FindCamNodeById(cam_id)->offset.z
        ));

    std::ostringstream oss;

    oss << NODE_EDITOR_LOG << "Камера " << FindCamNodeById(cam_id)->name <<
        "привязана к модели " << FindModNodeById(mod_id)->name << " с отступами: \n" <<
        "[отступ по x] " << std::to_string(FindCamNodeById(cam_id)->offset.x) << "\n" <<
        "[отступ по y] " << std::to_string(FindCamNodeById(cam_id)->offset.y) << "\n" <<
        "[отступ по z] " << std::to_string(FindCamNodeById(cam_id)->offset.z) << "\n";
        
    applog->AddLog(oss.str().c_str());*/
}