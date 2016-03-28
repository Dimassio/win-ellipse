#pragma once 

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	static bool RegisterClass( HINSTANCE hInstance );
	bool Create( HINSTANCE hInstance );
	void Show( int cmdShow );

protected:
	void OnDestroy();
	void OnSize();
	void OnPaint();
	void OnChangeColor();

private:
	HWND handle;
	COLORREF currentEllipseColor;

	static LRESULT WINAPI windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};