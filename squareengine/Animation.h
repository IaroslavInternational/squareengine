#pragma once

#include "Surface2D.h"
#include "Graphics.h"
#include <vector>
#include <DirectXMath.h>

class Animation
{
public:
	Animation( int x,int y,int width,int height,int count,const Surface2D& sprite,float holdTime,Color chroma = Colors::Magenta );
	void Draw(DirectX::XMFLOAT2 coordinates,Graphics& gfx ) const;
	void Draw(DirectX::XMFLOAT2 coordinates,Graphics& gfx,const RectI& clip ) const;
	// this version of draw replaces all opaque pixels with specified color
	void DrawColor( DirectX::XMFLOAT2 coordinates,Graphics& gfx,Color c ) const;
	void Update( float dt );
private:
	void Advance();
private:
	Color chroma;
	const Surface2D& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};