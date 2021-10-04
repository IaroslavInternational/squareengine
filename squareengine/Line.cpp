#include "Line.h"

#include "EngineFunctions.hpp"

Physics::Line::Line(std::string name, DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end)
	:
    name(name),
	start(start),
	end(end)
{}

Physics::Line::Line(std::string name, float start_x, float start_y, float end_x, float end_y)
	:
	Line(name, DirectX::XMFLOAT2({ start_x, start_y }), DirectX::XMFLOAT2({ end_x, end_y }))
{}

void Physics::Line::operator=(Line line)
{
    name = line.name;
    start = line.start;
    end = line.end;
    visability = line.visability;
}

bool Physics::Line::IsIntersect(Line line)
{
    float x1 = start.x,      y1 = start.y,      x2 = end.x,      y2 = end.y;
    float x3 = line.start.x, y3 = line.start.y, x4 = line.end.x, y4 = line.end.y;

    float eps = 5.0f;

    if (y1 == y2 && y3 == y4  &&  x3 <= x1 && x2 <= x4 ||
      ((x1 <= x3 && x2 >= x3) || (x1 <= x4 && x2 >= x4)))
    {
        if (fabs(y1 - y3) <= eps)
        {
            return true;
        }
    }

    if (x1 == x2 && x3 == x4  &&  y3 <= y1 && y2 <= y4 ||
      ((y1 <= y3 && y2 >= y3) || (y1 <= y4 && y2 >= y4)))
    {
        if (fabs(x1 - x3) <= eps)
        {
            return true;
        }
    }

    float Ua, Ub, numerator_a, numerator_b, denominator;

    denominator = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);

    if (denominator == 0) 
    {
        if ((x1 * y2 - x2 * y1) * (x4 - x3) - (x3 * y4 - x4 * y3) * (x2 - x1) == 0.0f &&
            (x1 * y2 - x2 * y1) * (y4 - y3) - (x3 * y4 - x4 * y3) * (y2 - y1) == 0.0f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else 
    {
        numerator_a = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
        numerator_b = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
        Ua = numerator_a / denominator;
        Ub = numerator_b / denominator;
        
        return (Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1);
    }
}

bool Physics::Line::IsVisible()
{
    return visability;
}

void Physics::Line::SetVisibility(bool state)
{
    visability = state;
}

void Physics::Line::SetStartPoint(DirectX::XMFLOAT2 p)
{
    start = p;
}

void Physics::Line::SetEndPoint(DirectX::XMFLOAT2 p)
{
    end = p;
}

std::string Physics::Line::GetName()
{
    return name;
}

DirectX::XMFLOAT2 Physics::Line::GetStartPoint()
{
    return start;
}

DirectX::XMFLOAT2 Physics::Line::GetEndPoint()
{
    return end;
}
