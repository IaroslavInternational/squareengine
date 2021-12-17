#pragma once

#include "Surface2D.h"
#include "Graphics.h"
#include <vector>
#include <DirectXMath.h>

class Animation
{
public:
	friend class GUISystem;
public:
	Animation( int x,int y,int width,int height,int count,Surface2D& sprite,float holdTime, std::string name, Color chroma = Colors::Magenta);
	void Draw(DirectX::XMFLOAT2 coordinates,Graphics& gfx ) const;
	void Draw(DirectX::XMFLOAT2 coordinates,Graphics& gfx,const RectI& clip ) const;
	void DrawNonChroma(DirectX::XMFLOAT2 coordinates, Graphics& gfx) const;
	void DrawNonChroma(DirectX::XMFLOAT2 coordinates, Graphics& gfx, const RectI& clip) const;

	// this version of draw replaces all opaque pixels with specified color
	void DrawColor( DirectX::XMFLOAT2 coordinates,Graphics& gfx,Color c ) const;
	void Update( float dt );
private:
	void Advance();
private:
	Color chroma;
	Surface2D& sprite;
	std::vector<RectI> frames;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
	std::string name;
	int width;
	int height;
};