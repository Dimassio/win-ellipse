#pragma once 

#include "Ellipse.h"

struct TText {
	wchar_t* string;
	int length;
	RECT rect;
	UINT font;
};

class CEllipseWindow {
public:
	CEllipseWindow();
	~CEllipseWindow();

	static bool RegisterClass( HINSTANCE hInstance );
	bool Create( HINSTANCE hInstance );
	void Show( int cmdShow );
	
	// Dialog
	void OnShowDialog();
	void OnCloseDialog();
	void OnChangeColor();
	void OnOkDialog( HWND handle );

protected:
	// Window
	void OnDestroy();
	void OnSize( LPARAM lParam );
	void OnPaint();

private:
	HWND handle;
	HWND dialogHandle;
	int currWinWidth;
	int currWinHeight;
	CEllipse ellipse;
	TText text;
	

	static LRESULT WINAPI windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};