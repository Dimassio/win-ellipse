#pragma once 

#include "Ellipse.h"

struct TText {
	wchar_t* string;
	int length;
	RECT rect;
	UINT font;
};

struct TScroll {
	int currPos;
	int maxRange;
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
	void OnResetDialog();

protected:
	// Window
	void OnDestroy();
	void OnSize( LPARAM lParam );
	void OnPaint();
	void OnVScroll( WPARAM wParam );
	void OnHScroll( WPARAM wParam );
	void OnCreate();

private:
	HWND handle;
	HWND dialogHandle;
	int currWinWidth;
	int currWinHeight;

	CEllipse ellipse;
	CEllipse baseEllipse;

	TText text;
	TText baseText;

	TScroll vScroll;
	TScroll hScroll;
	
	void drawBitmap();
	static LRESULT WINAPI windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};