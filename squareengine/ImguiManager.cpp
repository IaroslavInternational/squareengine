#include "ImguiManager.h"
#include "imgui/imgui.h"
#include "imgui/implot.h"

#include <filesystem>

ImguiManager::ImguiManager()
{
	namespace fs = std::filesystem;
	if( !fs::exists( "imgui.ini" ) && fs::exists( "imgui_default.ini" ) )
	{
		fs::copy_file( "imgui_default.ini","imgui.ini" );
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
		
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}
