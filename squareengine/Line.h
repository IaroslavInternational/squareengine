#pragma once

#include <DirectXMath.h>

namespace Physics
{
	class Line
	{
	public:
		Line(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end);
		Line(float start_x, float start_y, float end_x, float end_y);
	public:
		bool IsIntersect(Line line);
		DirectX::XMFLOAT2 GetStartPoint();
		DirectX::XMFLOAT2 GetEndPoint();
	private:
		DirectX::XMFLOAT2 start;
		DirectX::XMFLOAT2 end;
	};
}