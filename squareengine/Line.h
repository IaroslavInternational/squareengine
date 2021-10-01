#pragma once

#include <DirectXMath.h>
#include <string>

namespace Physics
{
	class Line
	{
	public:
		friend class GUISystem;
	public:
		Line(std::string name, DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end);
		Line(std::string name, float start_x, float start_y, float end_x, float end_y);
	public:
		bool IsIntersect(Line line);
		std::string		  GetName();
		DirectX::XMFLOAT2 GetStartPoint();
		DirectX::XMFLOAT2 GetEndPoint();
	private:
		std::string		  name;
		DirectX::XMFLOAT2 start;
		DirectX::XMFLOAT2 end;
	};
}