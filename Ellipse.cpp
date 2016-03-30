#include "Ellipse.h"

CEllipse::CEllipse()
{
}

CEllipse::CEllipse( const POINT& _center, const POINT& _size, const COLORREF _color ):
	center(_center), size(_size), color(_color)
{
}

void CEllipse::Draw( HDC dc ) const
{
	HBRUSH brush = ::CreateSolidBrush( color );  // ÷вет эллипса
	::SelectObject( dc, brush );
	Ellipse( dc, center.x - size.x, center.y - size.y, center.x + size.x, center.y + size.y );
	::DeleteObject( brush );
}


void CEllipse::SetColor( const COLORREF newColor )
{
	color = newColor;
}

void CEllipse::SetSize( const int x, const int y )
{
	if( x <= MinEllipseSizeX ) {
		size.x = MinEllipseSizeX;
	} else {
		size.x = x;
	}
	if( y <= MinEllipseSizeY ) {
		size.y = MinEllipseSizeY;
	} else {
		size.y = y;
	}
	
}

void CEllipse::SetCenter( const int x, const int y )
{
	center.x = x;
	center.y = y;
}

int CEllipse::GetLeft() const
{
	return center.x - size.x;
}

int CEllipse::GetRight() const
{
	return center.x + size.x;
}

int CEllipse::GetTop() const
{
	return center.y - size.y;
}

int CEllipse::GetBottom() const
{
	return center.y + size.y;
}