#include <Windows.h>
#include "resource.h"
#include "EllipseWindow.h"
#include <cassert>

CEllipseWindow::CEllipseWindow()
{
	text.string = L"Hello world!";
	text.length = 13;
	text.font = FW_NORMAL;
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
	handle = ::CreateWindow( L"Ellipse", L"Эллипс и Hello world", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
							 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
							 LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU1 ) ), hInstance, this );
	RECT rect;
	::GetClientRect( handle, &rect );
	currWinHeight = ( rect.bottom - rect.top );
	currWinWidth = ( rect.right - rect.left );
	ellipse.SetColor( RGB( 100, 100, 100 ) );
	ellipse.SetCenter( ( rect.right - rect.left ) / 2, ( rect.bottom - rect.top ) / 2 );
	ellipse.SetSize( 300, 100 );

	text.rect.left = ellipse.GetLeft();
	text.rect.right = ellipse.GetRight();
	text.rect.bottom = ellipse.GetBottom();
	text.rect.top = ellipse.GetTop();
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

void CEllipseWindow::OnSize( LPARAM lParam )
{
	RECT currentRect;
	::GetClientRect( handle, &currentRect );
	int newHeight = HIWORD( lParam );
	int newWidth = LOWORD( lParam );
	double kHeight = (double) newHeight / ( double ) currWinHeight;
	double kWidth = (double) newWidth / ( double ) currWinWidth;
	int currWidth = ellipse.GetRight() - ellipse.GetLeft();
	int currHeight = ellipse.GetBottom() - ellipse.GetTop();
	ellipse.SetSize( (currWidth * kWidth) / 2, (currHeight * kHeight) / 2 ); 
	currWinHeight = currentRect.bottom - currentRect.top;
	currWinWidth = currentRect.right - currentRect.left;
}

void CEllipseWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint( handle, &ps );  // Контекст устройства 
	ellipse.Draw( hdc );
	HFONT newFont = ::CreateFont( 25, 20, 120, 0, text.font, 3, 4, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						  DEFAULT_PITCH | FF_DONTCARE, L"Arial" );

	HFONT oldFont = ( HFONT ) SelectObject( hdc, newFont );
	::DrawText( hdc, text.string, text.length, &text.rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER | WS_EX_TRANSPARENT );
	::SelectObject( hdc, oldFont );
	::EndPaint( handle, &ps );
	DeleteObject( oldFont );
	DeleteObject( newFont );
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
		ellipse.SetColor( colorStruct.rgbResult );
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
					currentWindow->OnOkDialog( hwndDlg );
					currentWindow->OnCloseDialog();
					return TRUE;
				case IDCANCEL:
					currentWindow->OnCloseDialog();
					return TRUE;
			}
	}
	return FALSE;
}

void CEllipseWindow::OnOkDialog( HWND hwndDlg )
{
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO1 ) == BST_CHECKED ) {
		text.font = FW_NORMAL;
	}
	if( ::IsDlgButtonChecked( hwndDlg , IDC_RADIO2) == BST_CHECKED ) {
		text.font = FW_BOLD;
	}
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO3 ) == BST_CHECKED ) {
		text.font = FW_THIN;
	}
	HWND dlgHandle = ::GetDlgItem( hwndDlg, IDC_EDIT1 );
	DWORD length = ::SendMessage( dlgHandle, WM_GETTEXTLENGTH, 0, 0 ); // сообщение получить длинну текста
	wchar_t* buffer = new wchar_t[length];
	::SendMessage( dlgHandle, WM_GETTEXT, ( WPARAM ) length + 1, ( LPARAM ) buffer );
	if( buffer != L"" ) {
		text.string = buffer;
		text.length = length;
	}
	::InvalidateRect( handle, 0, TRUE );
}

void CEllipseWindow::OnShowDialog()
{
	dialogHandle = ::CreateDialog( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDD_DIALOG1 ), handle, DialogProc );
	switch( text.font ) {
		case FW_THIN:
			::CheckRadioButton( dialogHandle, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3 );
			break;
		case FW_BOLD:
			::CheckRadioButton( dialogHandle, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2 );
			break;
		case FW_NORMAL:
			::CheckRadioButton( dialogHandle, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 );
			break;
		default:
			assert( false );
	}
	::SendMessage( dialogHandle, WM_SETTEXT, ( WPARAM ) text.length + 1, ( LPARAM ) text.string );
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
			currentWindow->OnSize( lParam );
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
