#include "Animation.h"

Animation::Animation( int x,int y,int width,int height,int count,
					  Surface2D& sprite, float holdTime, std::string name, Color chroma )
	:
	sprite( sprite ),
	holdTime( holdTime ),
	chroma( chroma ),
	name(name),
	width(width),
    height(height)
{
	for( int i = 0; i < count; i++ )
	{
		frames.emplace_back( x + i * width,x + (i + 1) * width,y,y + height );
	}
}

void Animation::Draw(DirectX::XMFLOAT2 coordinates,Graphics& gfx ) const
{
	gfx.DrawSprite(coordinates.x, coordinates.y,frames[iCurFrame],sprite,chroma );
}

void Animation::Draw(DirectX::XMFLOAT2 coordinates,Graphics& gfx,const RectI& clip ) const
{
	gfx.DrawSprite(coordinates.x, coordinates.y,frames[iCurFrame],clip,sprite,chroma );
}

void Animation::DrawColor(DirectX::XMFLOAT2 coordinates,Graphics& gfx,Color c ) const
{
	gfx.DrawSpriteSubstitute(coordinates.x, coordinates.y,c,frames[iCurFrame],sprite,chroma );
}

void Animation::Update( float dt )
{
	curFrameTime += dt;
	while( curFrameTime >= holdTime )
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Animation::Advance()
{
	if( ++iCurFrame >= frames.size() )
	{
		iCurFrame = 0;
	}
}
