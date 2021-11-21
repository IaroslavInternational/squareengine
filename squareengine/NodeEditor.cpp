#include "NodeEditor.h"
#include "EngineFunctions.hpp"

#include "imgui\imgui.h"

NodeEditor::NodeEditor(AppLog* log)
    :
    applog(log)
{
    context = imnodes::EditorContextCreate();
}

NodeEditor::~NodeEditor()
{
    imnodes::EditorContextFree(context);
}

void NodeEditor::BeginFrame()
{
	imnodes::CreateContext();
}

void NodeEditor::EndFrame()
{
	imnodes::DestroyContext();
}

void NodeEditor::Show(bool *IsShown)
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

    if (ImGui::Begin("�������� �����", IsShown,
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
                    *IsShown = false;
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("����"))
                {
                    if (ImGui::MenuItem("��������"))
                    {
                        links.clear();

                        cNodes.clear();
                        mNodes.clear();

                        current_id = 0;
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

void NodeEditor::RenderNodes()
{
    // ����� �����
    for (auto& node : cNodes)
    {
        ImGui::PushItemWidth(150.0f);
        imnodes::BeginNode(node.id);
     
        // ���������
        imnodes::BeginNodeTitleBar();
       // ImGui::TextUnformatted(EngineFunctions::AttachStrings<std::string>("������", node.name).c_str());
        imnodes::EndNodeTitleBar();

        // ����
        // ---

        /* ������� */
        
        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::Text("�������:");
        ImGui::Text("x:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.x).c_str());
        ImGui::Text("y:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.y).c_str());
        ImGui::Text("z:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.z).c_str());
        imnodes::EndStaticAttribute();

        ImGui::NewLine();

        imnodes::BeginStaticAttribute(node.id << 8);
        ImGui::Text("������ ������:");
        ImGui::SliderFloat("������-x", &node.offset.x, -150.0f, 150.0f, "%.3f");
        ImGui::SliderFloat("������-y", &node.offset.y, -150.0f, 150.0f, "%.3f");
        ImGui::SliderFloat("������-z", &node.offset.z, -150.0f, 150.0f, "%.3f");
        imnodes::EndStaticAttribute();
        
        ImGui::NewLine();

        ImGui::Checkbox("������� ���������", &node.isOffset_CurrentPos);

        if (ImGui::Button("������� ���������"))
        {
            node.offset.x = 0.0f;
            node.offset.y = 0.0f;
            node.offset.z = 0.0f;

            node.isOffset_CurrentPos = false;
        }

        if (node.isOffset_CurrentPos)
        {
            node.offset.x = node.pos.x;
            node.offset.y = node.pos.y;
            node.offset.z = node.pos.z;
        }

        /***********/
        
        // �����
        imnodes::BeginOutputAttribute(node.id << 24);
        const float text_width = ImGui::CalcTextSize("�����").x;
        ImGui::Indent(150.f + ImGui::CalcTextSize("value").x - text_width);
        ImGui::NewLine();
        ImGui::TextUnformatted("�����");
        imnodes::EndOutputAttribute();

        imnodes::EndNode();
        ImGui::PopItemWidth();
    }

    // ����� �������
    for (auto& node : mNodes)
    {
        ImGui::PushItemWidth(150.0f);
        imnodes::BeginNode(node.id);

        // ���������
        imnodes::BeginNodeTitleBar();
       // ImGui::TextUnformatted(EngineFunctions::AttachStrings<std::string>("������", node.name).c_str());
        imnodes::EndNodeTitleBar();

        // �������
        imnodes::BeginStaticAttribute(node.id << 16);
        ImGui::Text("�������:");
        ImGui::Text("x:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.x).c_str());
        ImGui::Text("y:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.y).c_str());
        ImGui::Text("z:"); ImGui::SameLine();
        ImGui::Text(std::to_string(node.pos.z).c_str());
        imnodes::EndStaticAttribute();

        // ����
        imnodes::BeginInputAttribute(node.id << 8);
        ImGui::NewLine();
        ImGui::TextUnformatted("������");
        imnodes::EndInputAttribute();

        // �����
        // --

        imnodes::EndNode();
        ImGui::PopItemWidth();
    }

    for (auto& link : links)
    {
        imnodes::Link(link.id, link.start_attr, link.end_attr);
    }
}

void NodeEditor::AddCameraNode(int id, std::string name)
{
    current_delta_cam += 10.0f;

    imnodes::SetNodeEditorSpacePos(id, ImVec2(0.0f, current_delta_cam));
   // cNodes.emplace_back(CameraNode(id, name, camcon.GetPtr2CameraByName(name)->GetPosition()));
}

void NodeEditor::AddModelNode(int id, std::string name)
{
    current_delta_model += 10.0f;

    imnodes::SetNodeEditorSpacePos(id, ImVec2(100.0f, current_delta_model));
 //   mNodes.emplace_back(ModelNode(id, name, mcon.GetPtr2ModelByName(name)->get()->GetPosition()));
}

void NodeEditor::ShowLeftPanel(ImVec2 size)
{
    ImGui::BeginChild("�������� ����� | ����� ������", size);

    // ������ �����
   /* if (ImGui::BeginCombo("������", camcon->GetName().c_str()))
    {
        for (size_t i = 0; i < camcon.CamerasAmount(); i++)
        {
            const bool isSelected = i == activeCam;
            if (ImGui::Selectable(camcon.GetCameraNameByIndex(i).c_str(), isSelected))
            {
                activeCam = i;
                AddCameraNode((int)i, camcon.GetCameraNameByIndex(i));
            }
        }
        ImGui::EndCombo();
    }
    */
    // ������ �������
  /*  if (ImGui::BeginCombo("������", mcon.GetModelNameByIndex(0).c_str()))
    {
        for (size_t i = 0; i < mcon.ModelsAmount(); i++)
        {
            const bool isSelected = i == activeModel;
            if (ImGui::Selectable(mcon.GetModelNameByIndex(i).c_str(), isSelected))
            {
                activeModel = i;
                AddModelNode((int)i + 100, mcon.GetModelNameByIndex(i));
            }
        }
        ImGui::EndCombo();
    }*/

    if (ImGui::Button("���������"))
    {
        if (links.size() != 0)
        {
            for (auto& link : links)
            {
                int link_start = link.start_attr;   // cam out id
                int link_end = link.end_attr;       // mod in id

                int camId = link.start_attr >> 24;  // cam id
                int modId = link.end_attr >> 8;     // mod id

               /* if (!mcon.GetPtr2ModelByName(FindModNodeById(modId)->name)->get()->IsCameraConne�ted())
                {
                    ConncetCam2Model(camId, modId);

                    break;
                }
                else
                {
                    camIdToPopup = camId;
                    modIdToPopup = modId;

                    isPopup = true;
                    
                    break;
                }*/
            }
        }
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

NodeEditor::CameraNode* NodeEditor::FindCamNodeById(int id)
{
    for (auto& node : cNodes)
    {
        if (node.id == id)
        {
            return &node;
        }
    }
}

NodeEditor::ModelNode* NodeEditor::FindModNodeById(int id)
{
    for (auto& node : mNodes)
    {
        if (node.id == id)
        {
            return &node;
        }
    }
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