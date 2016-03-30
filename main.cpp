#include <Windows.h>
#include "EllipseWindow.h"
#include "Ellipse.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR commandLine, int nCmdShow )
{
	CEllipseWindow myWindow;
	CEllipseWindow::RegisterClass( hInstance );
	myWindow.Create( hInstance );
	myWindow.Show( nCmdShow );

	MSG msg;
	BOOL getMessageResult = 0;
	while( ( getMessageResult = ::GetMessage( &msg, NULL, 0, 0 ) ) != 0 ) {
		if( getMessageResult == -1 ) {
			return -1;
		}
		::TranslateMessage( &msg );
		::DispatchMessage( &msg );
	}

	return msg.wParam;
}
