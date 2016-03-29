#pragma once 

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	static bool RegisterClass( HINSTANCE hInstance );
	bool Create( HINSTANCE hInstance );
	void Show( int cmdShow );
	void OnCloseDialog(); 
	void OnChangeColor();

protected:
	void OnDestroy();
	void OnSize();
	void OnPaint();
	void OnShowDialog();
	
private:
	HWND handle;
	HWND dialogHandle;
	COLORREF currentEllipseColor;

	static LRESULT WINAPI windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};