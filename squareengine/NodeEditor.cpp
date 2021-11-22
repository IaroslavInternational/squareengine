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

    if (ImGui::Begin("�������� �����", NULL,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar))
    {
        // ����
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("��������"))
            {
                if (ImGui::MenuItem("�������"))
                {
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("����"))
                {
                    if (ImGui::MenuItem("��������"))
                    {
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
    // ����� �����
    for (auto& node : nodes)
    {
        ImGui::PushItemWidth(150.0f); 
        imnodes::SetNodeEditorSpacePos(node.id, ImVec2(0.0f, node.position.y));
        imnodes::BeginNode(node.id);

        /* ��������� */
       
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.cmd.c_str());
        imnodes::EndNodeTitleBar();

        /*************/

        /* ������� */
     
        imnodes::BeginStaticAttribute(node.id << 8);
        ImGui::Text("�������� �������");
        ImGui::SliderInt("val", &node.value, -10000, 10000);
        imnodes::EndStaticAttribute();
    
        /***********/

        imnodes::EndNode();
        ImGui::PopItemWidth();
    }
}

void NodeEditor::AddNode(size_t id, std::string cmd, int value)
{
    const float node_margin = ScriptNode::NextSize(nodes.size());

    nodes.emplace_back(id, cmd, value, DirectX::XMFLOAT2{ 0.0f, node_margin });
}

void NodeEditor::ShowLeftPanel(ImVec2 size)
{
    ImGui::BeginChild("�������� ����� | ����� ������", size);

    // ������ ������
    if (ImGui::BeginCombo("�������", cmdSelected.c_str()))
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
    
    if (ImGui::Button("���������"))
    {
       
    }

    /*if (isPopup)
    {
        ImGui::OpenPopup("�������������");
        if (ImGui::BeginPopup("�������������"),
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)
        {
            ImGui::Text("������������ ������?");
            ImGui::Text(EngineFunctions::AttachStrings<std::string>("������� ������: ", FindCamNodeById(camIdToPopup)->name).c_str());

            if (ImGui::Button("��������"))
            {
                mcon.GetPtr2ModelByName(FindModNodeById(modIdToPopup)->name)->get()->DisconnectCamera();

                ConncetCam2Model(camIdToPopup, modIdToPopup);
                isPopup = false;

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("������"))
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

    ImGui::BeginChild("�������� ����� | ������ ������", size);

    imnodes::BeginNodeEditor();

    Init();

    RenderNodes();

    imnodes::EndNodeEditor();

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

    oss << NODE_EDITOR_LOG << "������ " << FindCamNodeById(cam_id)->name <<
        "��������� � ������ " << FindModNodeById(mod_id)->name << " � ���������: \n" <<
        "[������ �� x] " << std::to_string(FindCamNodeById(cam_id)->offset.x) << "\n" <<
        "[������ �� y] " << std::to_string(FindCamNodeById(cam_id)->offset.y) << "\n" <<
        "[������ �� z] " << std::to_string(FindCamNodeById(cam_id)->offset.z) << "\n";
        
    applog->AddLog(oss.str().c_str());*/
}