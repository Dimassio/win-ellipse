#pragma once
#include <Windows.h>

class CEllipse {
public:
	CEllipse();

	CEllipse( const POINT& center, const RECT& size, const COLORREF color );

	void Draw( HDC dc ) const;

	void SetColor( const COLORREF newColor );

	void SetCenter( const int x, const int y );

	void SetSize( const RECT& rect );

	int GetLeft() const;
	
	int GetRight() const;
	
	int GetTop() const;
	
	int GetBottom() const;

	POINT GetCenter() const;

private:
	POINT center;
	RECT rect;
	COLORREF color;
};