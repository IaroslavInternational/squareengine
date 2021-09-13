#pragma once

#include "Colors.h"
#include <string>
#include "RectI.h"

class Surface2D
{
public:
	Surface2D( const std::string& filename );
	Surface2D( int width,int height );
	Surface2D( const Surface2D& );
	~Surface2D();
	Surface2D& operator=( const Surface2D& );
	void PutPixel( int x,int y,Color c );
	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;
private:
	Color* pPixels = nullptr;
	int width;
	int height;
};