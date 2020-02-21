#pragma once

#include "Vec2.h"

class Rect
{
public:
	Rect() = default;
	Rect( int left_in,int right_in,int top_in,int bottom_in );
	Rect( const Vec2& topLeft,const Vec2& bottomRight );
	Rect( const Vec2& topLeft,int width,int height );
	bool IsOverlappingWith( const Rect& other ) const;
	bool IsContainedBy( const Rect& other ) const;
	static Rect FromCenter( const Vec2& center,int halfWidth,int halfHeight );
	Rect GetExpanded( int offset ) const;
	Vec2 GetCenter() const;
public:
	int left;
	int right;
	int top;
	int bottom;
};