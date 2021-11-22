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
    if (!IsShow)
    {
        return;
    }

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
                    IsShow = false;

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

std::optional<std::string> NodeEditor::GetScriptPath()
{
    if (createdScriptPath != "")
    {
        return createdScriptPath;
    }
    else
    {
        return std::nullopt;
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
        imnodes::SetNodeGridSpacePos(node.id, ImVec2(0.0f, node.position.y));
        imnodes::BeginNode(node.id);

        /* ��������� */
       
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node.cmd.c_str());
        imnodes::EndNodeTitleBar();

        /*************/

        /* ������� */
     
        imnodes::BeginStaticAttribute(node.id << 8);
        ImGui::Text("�������� �������");
        ImGui::SliderInt("value", &node.value, -10000, 10000);
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
                AddNode(GenerateNodeId(), cmd_list[i], 0);
            }
        }
        ImGui::EndCombo();
    }
    
    if (ImGui::Button("�������"))
    {
        std::ostringstream oss;

        for (auto& n : nodes)
        {
            oss << n.cmd << "(" << n.value << ")" << ";\n";
        }

        std::ofstream ofs;
        ofs.open("Scripts\\test.sqes");
        ofs << oss.str();
        ofs.close();

        createdScriptPath = "Scripts\\test.sqes";
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

size_t NodeEditor::GenerateNodeId()
{
    for (size_t i = 0; i < nodes.size(); i++)
    {
        auto num = EngineFunctions::GenerateRandomNumber<size_t>(1, 100);

        if (num != nodes[i].id)
        {
            return num;
        }
        else
        {
            i = 0;
        }
    }
}