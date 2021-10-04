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
		void operator=(Line line);
	public:
		bool			  IsIntersect(Line line);
		bool			  IsVisible();
		void			  SetVisibility(bool state);
		void			  SetStartPoint(DirectX::XMFLOAT2 p);
		void			  SetEndPoint(DirectX::XMFLOAT2 p);
		std::string		  GetName();
		DirectX::XMFLOAT2 GetStartPoint();
		DirectX::XMFLOAT2 GetEndPoint();
	private:
		std::string		  name;
		DirectX::XMFLOAT2 start;
		DirectX::XMFLOAT2 end;
		bool			  visability = true;
	};
}