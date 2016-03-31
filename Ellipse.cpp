#include "Ellipse.h"
#include <cassert>

CEllipse::CEllipse()
{
}

CEllipse::CEllipse( const POINT& _center, const RECT& _size, const COLORREF _color ):
	center(_center), rect(_size), color(_color)
{
}

void CEllipse::Draw( HDC dc ) const
{
	HBRUSH brush = ::CreateSolidBrush( color );  // ÷вет эллипса
	::SelectObject( dc, brush );
	Ellipse( dc, rect.left, rect.top, rect.right, rect.bottom );
	::DeleteObject( brush );
}


void CEllipse::SetColor( const COLORREF newColor )
{
	color = newColor;
}

void CEllipse::SetSize( const RECT& _rect )
{
	rect = _rect;
}

void CEllipse::SetCenter( const int x, const int y )
{
	center.x = x;
	center.y = y;
}

int CEllipse::GetLeft() const
{
	return rect.left;
}

int CEllipse::GetRight() const
{
	return rect.right;
}

int CEllipse::GetTop() const
{
	return rect.top;
}

int CEllipse::GetBottom() const
{
	return rect.bottom;
}

POINT CEllipse::GetCenter() const
{
	return center;
}

void CEllipse::Move( int delta, int direction )
{
	switch( direction ) {
		case SB_VERT:
			center.y -= delta;
			rect.top -= delta;
			rect.bottom -= delta;
			break;
		case SB_HORZ:
			center.x -= delta;
			rect.left -= delta;
			rect.right -= delta;
			break;
		default:
			assert( false );
	}
}