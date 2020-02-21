#include "Rect.h"

Rect::Rect( int left_in,int right_in,int top_in,int bottom_in )
	:
	left( left_in ),
	right( right_in ),
	top( top_in ),
	bottom( bottom_in )
{
}

Rect::Rect( const Vec2& topLeft,const Vec2 & bottomRight )
	:
	Rect( topLeft.x,bottomRight.x,topLeft.y,bottomRight.y )
{
}

Rect::Rect( const Vec2& topLeft,int width,int height )
	:
	Rect( topLeft,topLeft + Vec2( width,height ) )
{
}

bool Rect::IsOverlappingWith( const Rect& other ) const
{
	return right > other.left && left < other.right
		&& bottom > other.top && top < other.bottom;
}

bool Rect::IsContainedBy( const Rect & other ) const
{
	return left >= other.left && right <= other.right &&
		top >= other.top && bottom <= other.bottom;
}

Rect Rect::FromCenter( const Vec2 & center,int halfWidth,int halfHeight )
{
	const Vec2 half( halfWidth,halfHeight );
	return Rect( center - half,center + half );
}

Rect Rect::GetExpanded( int offset ) const
{
	return Rect( left - offset,right + offset,top - offset,bottom + offset );
}

Vec2 Rect::GetCenter() const
{
	return Vec2( (left + right) / 2,(top + bottom) / 2 );
}
