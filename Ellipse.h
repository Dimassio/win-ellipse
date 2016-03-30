#pragma once
#include <Windows.h>

const int MinEllipseSizeX = 30;
const int MinEllipseSizeY = 30;

class CEllipse {
public:
	CEllipse();

	CEllipse( const POINT& center, const POINT& size, const COLORREF color );

	void Draw( HDC dc ) const;

	void SetColor( const COLORREF newColor );

	void SetCenter( const int x, const int y );

	void SetSize( const int x, const int y );

	int GetLeft() const;
	
	int GetRight() const;
	
	int GetTop() const;
	
	int GetBottom() const;

private:
	POINT center;
	POINT size; // Расстояние от центра
	COLORREF color;
};