// ScreenCapture.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include <Shlobj.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define BMP  1
#define JPEG 2
#define PNG  3

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HMENU hMenu;
HWND hStatic1;
int imageType = BMP;
wchar_t *folder_path = NULL;
HDC hdcSnapshot;
HBITMAP hbmSnapshot;
int iXRes;
int iYRes;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int SelectFolder(LPWSTR *ppszName);
void ScreenSnapshot(HWND hWnd);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SCREENCAPTURE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENCAPTURE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCREENCAPTURE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SCREENCAPTURE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 230, 150, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_SELECTION:
			ScreenSnapshot(hWnd);

			SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(hWnd, HWND_TOP, 0, 0, iXRes, iYRes, SWP_FRAMECHANGED);
			SetMenu(hWnd, NULL);
			break;
		case IDM_WINDOW:
			break;
		case IDM_FULLSCREEN:
			break;
		case IDM_IMAGETYPE_BMP:
			CheckMenuRadioItem(hMenu, IDM_IMAGETYPE_BMP, IDM_IMAGETYPE_PNG, IDM_IMAGETYPE_BMP, MF_BYCOMMAND);
			imageType = BMP;
			break;
		case IDM_IMAGETYPE_JPEG:
			CheckMenuRadioItem(hMenu, IDM_IMAGETYPE_BMP, IDM_IMAGETYPE_PNG, IDM_IMAGETYPE_JPEG, MF_BYCOMMAND);
			imageType = JPEG;
			break;
		case IDM_IMAGETYPE_PNG:
			CheckMenuRadioItem(hMenu, IDM_IMAGETYPE_BMP, IDM_IMAGETYPE_PNG, IDM_IMAGETYPE_PNG, MF_BYCOMMAND);
			imageType = PNG;
			break;
		case IDM_IMAGEFOLDER:
			SelectFolder(&folder_path);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		SetMenu(hWnd, hMenu);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		StretchBlt(hdc, 0, 0, iXRes, iYRes, hdcSnapshot, 0, 0, iXRes, iYRes, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		{
		// Initialize something.
		hMenu = GetMenu(hWnd);
		hStatic1 = CreateWindowEx(NULL, TEXT("STATIC"), TEXT(""),
			WS_CHILD, 10, 10, 200, 48,
			hWnd, NULL, LPCREATESTRUCT(lParam)->hInstance, NULL);

		// Set font.
		int PointSize = 12;
		hdc = GetDC(hWnd);
		int nHeight = -MulDiv(PointSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(hWnd, hdc);
		HFONT hFont = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Arial"));
		SendMessage(hStatic1, WM_SETFONT, (WPARAM)hFont, FALSE);

		// Get resolution.
		iXRes = GetSystemMetrics(SM_CXSCREEN);
		iYRes = GetSystemMetrics(SM_CYSCREEN);

		// Set default image type
		SendMessage(hWnd, WM_COMMAND, IDM_IMAGETYPE_BMP, 0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int SelectFolder(LPWSTR *ppszName)
{
	IFileOpenDialog *pfd;
	HRESULT hr;

	// CoCreate the dialog object.
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	if (FAILED(hr))
		return 0;

	DWORD dwOptions;
	hr = pfd->GetOptions(&dwOptions);
	if (FAILED(hr))
	{
		pfd->Release();
		return 0;
	}

	hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
	if (FAILED(hr))
	{
		pfd->Release();
		return 0;
	}

	// Show the Open dialog.
	hr = pfd->Show(NULL);
	if (FAILED(hr))
	{
		pfd->Release();
		return 0;
	}

	// Obtain the result of the user interaction.
	IShellItem *psi = NULL;
	hr = pfd->GetResult(&psi);
	if (FAILED(hr))
	{
		pfd->Release();
		return 0;
	}

	hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, ppszName);
	if (FAILED(hr))
	{
		psi->Release();
		pfd->Release();
		return 0;
	}

	// Release
	psi->Release();
	pfd->Release();
	return 1;
}

void ScreenSnapshot(HWND hWnd)
{
	HDC hdcScreen;

	// Create a normal DC and a memory DC for the entire screen. The
	// normal DC provides a "snapshot" of the screen contents. The
	// memory DC keeps a copy of this "snapshot" in the associated
	// bitmap.
	hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	hdcSnapshot = CreateCompatibleDC(hdcScreen);

	// Create a compatible bitmap for hdcScreen.
	hbmSnapshot = CreateCompatibleBitmap(hdcScreen,
		GetDeviceCaps(hdcScreen, HORZRES),
		GetDeviceCaps(hdcScreen, VERTRES));

	if (hbmSnapshot == 0) {
		MessageBox(hWnd,
			TEXT("Can't create a compatible bitmap."),
			TEXT("Error"), MB_OK);
	}

	// Select the bitmaps into the compatible DC.
	if (!SelectObject(hdcSnapshot, hbmSnapshot)) {
		MessageBox(hWnd,
			TEXT("SelectObject Failed"),
			TEXT("Error"), MB_OK);
	}

	// Hide the application window.
	ShowWindow(hWnd, SW_HIDE);
	Sleep(100);

	// Copy color data for the entire display into a
	// bitmap that is selected into a compatible DC.
	if (!BitBlt(hdcSnapshot, 0, 0, iXRes, iYRes,
		hdcScreen, 0, 0, SRCCOPY)) {
			MessageBox(hWnd,
			TEXT("BitBlt Failed"),
			TEXT("Error"), MB_OK);
	}

	// Redraw the application window.
	ShowWindow(hWnd, SW_SHOW);

	// Release
	DeleteDC(hdcScreen);
}