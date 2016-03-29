#include <Windows.h>
#include "resource.h"
#include "EllipseWindow.h"

CEllipseWindow::CEllipseWindow()
{
	currentEllipseColor = RGB( 255, 255, 255 );
}

CEllipseWindow::~CEllipseWindow()
{
}

bool CEllipseWindow::RegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX tag;
	tag.cbSize = sizeof( WNDCLASSEX );
	tag.style = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc = windowProc;
	tag.cbClsExtra = 0;
	tag.cbWndExtra = 0;
	tag.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	tag.hCursor = LoadCursor( NULL, IDC_ARROW );
	tag.hbrBackground = CreatePatternBrush( LoadBitmap( hInstance, MAKEINTRESOURCE( IDB_BITMAP1 ) ) );
	tag.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );
	tag.lpszClassName = L"Ellipse";
	tag.hInstance = hInstance;
	tag.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

	return ::RegisterClassEx( &tag );
}

bool CEllipseWindow::Create( HINSTANCE hInstance )
{
	handle = ::CreateWindow( L"Ellipse", L"Ёллипс и Hello world", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
							 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
							 LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU1 ) ), hInstance, this );
	return handle != 0;
}

void CEllipseWindow::Show( int nCmdShow )
{
	::ShowWindow( handle, nCmdShow );
	::UpdateWindow( handle );
}

void CEllipseWindow::OnDestroy()
{
	::PostQuitMessage( 0 );
}

void CEllipseWindow::OnSize()
{
}

void CEllipseWindow::OnPaint()
{
	// paint ellipse and @Hello world@
	/*
	ƒл€ ресайза битмапки
	HBITMAP hBitmap = LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_BITMAP1 ) );
	BITMAP bp;
	GetObject( hBitmap, sizeof( bp ), &bp );

	HDC hdcInst, hdcBitmap;
	PAINTSTRUCT ps;
	hdcInst = BeginPaint( handle , &ps );
	hdcBitmap = CreateCompatibleDC( hdcInst );
	SelectObject( hdcBitmap, hBitmap );

	RECT r;
	GetClientRect( handle, &r );

	StretchBlt( hdcInst, 0, 0, r.right - r.left, r.bottom - r.top, hdcBitmap, 0, 0, bp.bmWidth, bp.bmHeight, MERGECOPY );

	DeleteDC( hdcBitmap );
	EndPaint( handle, &ps );*/
}

void CEllipseWindow::OnChangeColor()
{
	CHOOSECOLOR colorStruct;
	COLORREF acrCustClr[16];
	DWORD rgbCurrent = 0;
	ZeroMemory( &colorStruct, sizeof( colorStruct ) );
	colorStruct.lStructSize = sizeof( colorStruct );
	colorStruct.hwndOwner = handle;
	colorStruct.lpCustColors = ( LPDWORD ) acrCustClr;
	colorStruct.rgbResult = rgbCurrent;
	colorStruct.Flags = CC_FULLOPEN | CC_RGBINIT;
	if( !::ChooseColor( &colorStruct ) ) {
		::MessageBox( handle, L"Wrong choose color", NULL, MB_OK );
		return;
	} else {
		currentEllipseColor = colorStruct.rgbResult;
	}
}

BOOL WINAPI DialogProc( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	CEllipseWindow* currentWindow = reinterpret_cast< CEllipseWindow* >( ::GetWindowLong( ::GetParent( hwndDlg ), GWL_USERDATA ) );
	switch( message ) {
		case WM_CLOSE:
			currentWindow->OnCloseDialog();
			return TRUE;
		case WM_COMMAND:
			switch( LOWORD( wParam ) ) {
				case IDC_CHANGE_COLOR:
					currentWindow->OnChangeColor();
					return TRUE;
				case IDOK:
					// ok button
					currentWindow->OnCloseDialog();
					return TRUE;
				case IDCANCEL:
					// cancel button
					currentWindow->OnCloseDialog();
					return TRUE;
			}
	}
	return FALSE;
}

void CEllipseWindow::OnShowDialog()
{
	dialogHandle = ::CreateDialog( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDD_DIALOG1 ), handle, DialogProc );
	::ShowWindow( dialogHandle, SW_SHOWNORMAL );
}

void CEllipseWindow::OnCloseDialog()
{
	::DestroyWindow( dialogHandle );
}

LRESULT WINAPI CEllipseWindow::windowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	CEllipseWindow* currentWindow = reinterpret_cast< CEllipseWindow* >( ::GetWindowLong( hWnd, GWL_USERDATA ) );
	switch( message ) {
		case WM_NCCREATE:
		{
			CREATESTRUCT* str = ( CREATESTRUCT* ) lParam;
			::SetWindowLong( hWnd, GWL_USERDATA, LONG( str->lpCreateParams ) );
			( ( CEllipseWindow* )::GetWindowLong( hWnd, GWL_USERDATA ) )->handle = hWnd;
		} break;
		case WM_SIZE:
			currentWindow->OnSize();
			break;
		case WM_DESTROY:
			currentWindow->OnDestroy();
			break;
		case WM_PAINT:
			currentWindow->OnPaint();
			break;
		case WM_COMMAND:
			switch( LOWORD( wParam ) ) {
				case ID_MENU:
					currentWindow->OnShowDialog();
					break;
				default:
					break;
			}
	}

	return ::DefWindowProc( hWnd, message, wParam, lParam );
}
