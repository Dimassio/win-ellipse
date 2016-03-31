#include <Windows.h>
#include "resource.h"
#include "EllipseWindow.h"
#include <cassert>

CEllipseWindow::CEllipseWindow()
{
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
	handle = ::CreateWindow( L"Ellipse", L"Эллипс и Hello world",
							 WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
							 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
							 LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MENU1 ) ), hInstance, this );
	// Ellipse initializing
	RECT rect;
	::GetClientRect( handle, &rect );
	currWinHeight = ( rect.bottom - rect.top );
	currWinWidth = ( rect.right - rect.left );
	ellipse.SetColor( RGB( 100, 100, 100 ) );
	ellipse.SetCenter( ( rect.right - rect.left ) / 2, ( rect.bottom - rect.top ) / 2 );
	RECT newRect;
	newRect.left = ( rect.right - rect.left ) / 2 - 300;
	newRect.right = ( rect.right - rect.left ) / 2 + 300;
	newRect.top = ( rect.bottom - rect.top ) / 2 - 80;
	newRect.bottom = ( rect.bottom - rect.top ) / 2 + 80;
	ellipse.SetSize( newRect );

	// Text initializing
	text.string = L"Hello world!";
	text.length = 12;
	text.font = FW_NORMAL;
	text.rect.left = ellipse.GetLeft();
	text.rect.right = ellipse.GetRight();
	text.rect.bottom = ellipse.GetBottom();
	text.rect.top = ellipse.GetTop();

	// Base settings
	baseEllipse = ellipse;
	baseText = text;
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
	int newHeight = HIWORD( lParam );
	int newWidth = LOWORD( lParam );
	double kHeight = ( double ) newHeight / ( double ) currWinHeight;
	double kWidth = ( double ) newWidth / ( double ) currWinWidth;
	RECT newRect;
	newRect.left = ellipse.GetCenter().x - ( ellipse.GetRight() - ellipse.GetLeft() ) * kWidth / 2;
	newRect.right = ellipse.GetCenter().x + ( ellipse.GetRight() - ellipse.GetLeft() ) * kWidth / 2;
	newRect.top = ellipse.GetCenter().y - ( ellipse.GetBottom() - ellipse.GetTop() ) * kHeight / 2;
	newRect.bottom = ellipse.GetCenter().y + ( ellipse.GetBottom() - ellipse.GetTop() ) * kHeight / 2;
	ellipse.SetSize( newRect );
	currWinHeight = newHeight;
	currWinWidth = newWidth;
	::InvalidateRect( handle, 0, FALSE );
}

void CEllipseWindow::OnPaint()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( handle, &ps );  // Контекст устройства

	RECT rect;
	::GetClientRect( handle, &rect );
	// DC, в памяти, НЕ НА ЭКРАНЕ
	HDC newHdc = ::CreateCompatibleDC( hdc );
	// Создаем Битмап на всю область
	HBITMAP bitmap = ::CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
	// Выбираем битмапку 
	HGDIOBJ oldbitmap = ::SelectObject( newHdc, bitmap ); // Возвращается предыдущую настройку

	// ======================================
	// Перерисовываем фон
	HBITMAP back = ::LoadBitmap( ::GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_BITMAP1 ) );
	HBRUSH brush = ::CreatePatternBrush( back );
	::FillRect( newHdc, &rect, brush );
	::DeleteObject( brush );
	::DeleteObject( back );

	// Рисуем эллипс
	ellipse.Draw( newHdc );

	// Пишем текст
	HFONT newFont = ::CreateFont( 25, 20, 120, 0, text.font, 3, 4, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
								  DEFAULT_PITCH | FF_DONTCARE, L"Arial" );
	HFONT oldFont = ( HFONT ) SelectObject( newHdc, newFont );
	::DrawText( newHdc, text.string, text.length, &text.rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	::SelectObject( newHdc, oldFont ); // Переключились на прежнюю настройку шрифта
	::DeleteObject( newFont );
	::DeleteObject( oldFont );
	// ========================================
	// Переносим на контекст монитора контекст памяти, на котором мы рисовали
	::BitBlt( hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, newHdc, 0, 0, SRCCOPY );
	// Переключились на битмапку, нанесенную на экран
	::SelectObject( newHdc, oldbitmap ); 

	// Убираем за собой
	::DeleteObject( bitmap );
	::DeleteDC( newHdc );
	::DeleteObject( oldbitmap );

	::EndPaint( handle, &ps );
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
				case IDC_RESET:
					currentWindow->OnResetDialog();
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

void CEllipseWindow::OnResetDialog()
{
	ellipse = baseEllipse;
	text = baseText;
	::SetScrollPos( handle, SB_VERT, 250, TRUE );
	::SetScrollPos( handle, SB_HORZ, 410, TRUE );
	hScroll.currPos = 410;
	vScroll.currPos = 250;
	InvalidateRect( handle, 0, FALSE );
	::SetWindowText( GetDlgItem( dialogHandle, IDC_EDIT1 ), text.string );
	::CheckRadioButton( dialogHandle, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 );
}

void CEllipseWindow::OnOkDialog( HWND hwndDlg )
{
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO1 ) == BST_CHECKED ) {
		text.font = FW_NORMAL;
	}
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO2 ) == BST_CHECKED ) {
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
	::InvalidateRect( handle, 0, FALSE );
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
	::SetWindowText( GetDlgItem( dialogHandle, IDC_EDIT1 ), text.string );
	::ShowWindow( dialogHandle, SW_SHOWNORMAL );
}

void CEllipseWindow::OnCloseDialog()
{
	::DestroyWindow( dialogHandle );
}

void CEllipseWindow::OnVScroll( WPARAM wParam )
{
	switch( LOWORD( wParam ) ) {
		case SB_LINEUP:
			vScroll.currPos = max( 0, vScroll.currPos - 1 );
			break;
		case SB_LINEDOWN:
			vScroll.currPos = min( vScroll.maxRange, vScroll.currPos + 1 );
			break;
		case SB_PAGEUP:
			vScroll.currPos = max( 0, vScroll.currPos - 10 );
			break;
		case SB_PAGEDOWN:
			vScroll.currPos = min( vScroll.maxRange, vScroll.currPos + 10 );
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			vScroll.currPos = HIWORD( wParam );
			break;
		default:
			break;
	}
	// Изменили положение скролла
	// Пока не сделали SetScrollPos она не изменится
	int delta = vScroll.currPos - GetScrollPos( handle, SB_VERT );
	if( delta != 0 ) {
		ellipse.Move( delta, SB_VERT );
		text.rect.bottom -= delta;
		text.rect.top -= delta;
		SetScrollPos( handle, SB_VERT, vScroll.currPos, TRUE );
		::InvalidateRect( handle, 0, FALSE );
	}
}

void CEllipseWindow::OnHScroll( WPARAM wParam )
{
	switch( LOWORD( wParam ) ) {
		case SB_LINELEFT:
			hScroll.currPos = max( 0, hScroll.currPos - 1 );
			break;
		case SB_LINERIGHT:
			hScroll.currPos = min( hScroll.maxRange, hScroll.currPos + 1 );
			break;
		case SB_PAGELEFT:
			hScroll.currPos = max( 0, hScroll.currPos - 10 );
			break;
		case SB_PAGERIGHT:
			hScroll.currPos = min( hScroll.maxRange, hScroll.currPos + 10 );
			break;
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			hScroll.currPos = HIWORD( wParam );
			break;
		default:
			break;
	}
	int delta = hScroll.currPos - GetScrollPos( handle, SB_HORZ );
	if( delta != 0 ) {
		ellipse.Move( delta, SB_HORZ );
		text.rect.left -= delta;
		text.rect.right -= delta;
		SetScrollPos( handle, SB_HORZ, hScroll.currPos, TRUE );
		InvalidateRect( handle, 0, FALSE );
	}
}

void CEllipseWindow::OnCreate()
{
	vScroll.currPos = 250;
	vScroll.maxRange = 500;
	::SetScrollRange( handle, SB_VERT, 0, 500, FALSE );
	::SetScrollPos( handle, SB_VERT, 250, TRUE );
	hScroll.currPos = 410;
	hScroll.maxRange = 820;
	::SetScrollRange( handle, SB_HORZ, 0, 820, FALSE );
	::SetScrollPos( handle, SB_HORZ, 410, TRUE );
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
			break;
		}
		case WM_CREATE:
			currentWindow->OnCreate();
			break;
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
			} break;
		case WM_VSCROLL:
			currentWindow->OnVScroll( wParam );
			break;
		case WM_HSCROLL:
			currentWindow->OnHScroll( wParam );
			break;
	}

	return ::DefWindowProc( hWnd, message, wParam, lParam );
}

void CEllipseWindow::drawBitmap()
{
	PAINTSTRUCT ps;
	BITMAP bm;
	HDC hdc = ::BeginPaint( handle, &ps );
	HDC hdcMem = ::CreateCompatibleDC( hdc );

	HBITMAP g_hbmBall = ::LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_HUMAN ) );
	HBITMAP g_hbmMask = ::LoadBitmap( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDB_MASSk ) );
	::GetObject( g_hbmBall, sizeof( BITMAP ), &bm );

	SelectObject( hdcMem, g_hbmMask );
	BitBlt( hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND );

	SelectObject( hdcMem, g_hbmBall );
	BitBlt( hdc, 0, bm.bmHeight, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT );

	::DeleteDC( hdcMem );
	::EndPaint( handle, &ps );
}