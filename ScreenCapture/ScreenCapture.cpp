// ScreenCapture.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "Screenshot.h"
#include <Shlobj.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define BMP  1
#define JPEG 2
#define PNG  3
#define HOTKEY_SNAPSHOT 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HMENU hMenu;
int imageType = BMP;
wchar_t *folder_path = NULL;
Screenshot screenshot;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int SelectFolder(LPWSTR *ppszName);
void GetWindowSize(HWND hWnd, int &width, int &height);

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
	   100, 100, 400, 300, NULL, NULL, hInstance, NULL);

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
			break;
		case IDM_WINDOW:
			break;
		case IDM_FULLSCREEN:
			screenshot.StartScreenshot(hWnd);

			if (imageType == PNG)
				screenshot.SaveScreenshot(TEXT(".png"));
			else if (imageType == JPEG)
				screenshot.SaveScreenshot(TEXT(".jpg"));
			else if (imageType == BMP)
				screenshot.SaveScreenshot(TEXT(".bmp"));
			else
				screenshot.SaveScreenshot(TEXT(".png"));
			break;
		case IDM_IMAGETYPE_PNG:
			CheckMenuRadioItem(hMenu, IDM_IMAGETYPE_PNG, IDM_IMAGETYPE_BMP, IDM_IMAGETYPE_PNG, MF_BYCOMMAND);
			imageType = PNG;
			break;
		case IDM_IMAGETYPE_JPEG:
			CheckMenuRadioItem(hMenu, IDM_IMAGETYPE_PNG, IDM_IMAGETYPE_BMP, IDM_IMAGETYPE_JPEG, MF_BYCOMMAND);
			imageType = JPEG;
			break;
		case IDM_IMAGETYPE_BMP:
			CheckMenuRadioItem(hMenu, IDM_IMAGETYPE_PNG, IDM_IMAGETYPE_BMP, IDM_IMAGETYPE_BMP, MF_BYCOMMAND);
			imageType = BMP;
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
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) // press ESC to exit
			DestroyWindow(hWnd);
		break;
	case WM_HOTKEY:
		switch (wParam)
		{
		case HOTKEY_SNAPSHOT:
			SendMessage(hWnd, WM_COMMAND, IDM_FULLSCREEN, 0);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		// Get window size.
		int width;
		int height;
		GetWindowSize(hWnd, width, height);

		// Draw snapshoted image.
		SetStretchBltMode(hdc, HALFTONE); // prevent distortion (STRETCH_DELETESCANS or HALFTONE)
		StretchBlt(hdc, 0, 0, width - 16, height - 58,
			screenshot.hdcSnapshot, 0, 0, screenshot.GetScreenResolutionX(), screenshot.GetScreenResolutionY(), SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		{
		// Initialize something.
		hMenu = GetMenu(hWnd);

		// Register hot key.
		RegisterHotKey(hWnd, HOTKEY_SNAPSHOT, MOD_NOREPEAT, VK_SNAPSHOT);
		RegisterHotKey(hWnd, HOTKEY_SNAPSHOT, MOD_CONTROL, VK_F1);

		// Set default image type
		SendMessage(hWnd, WM_COMMAND, IDM_IMAGETYPE_PNG, 0);

		// Disable non-finish feature
		EnableMenuItem(hMenu, IDM_SELECTION, MF_DISABLED);
		EnableMenuItem(hMenu, IDM_WINDOW, MF_DISABLED);
		EnableMenuItem(hMenu, IDM_IMAGEFOLDER, MF_DISABLED);
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

void GetWindowSize(HWND hWnd, int &width, int &height)
{
	RECT rect;
	if (GetWindowRect(hWnd, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
}