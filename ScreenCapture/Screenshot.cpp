#include "stdafx.h"
#include "Screenshot.h"

#include <stdio.h>
#include <time.h>
#include <atlimage.h>

Screenshot::Screenshot()
{
}


Screenshot::~Screenshot()
{
	DeleteDC(m_hMemDC);
	DeleteObject(m_hBitmapSnapshot);
}

void Screenshot::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

void Screenshot::StartScreenshot()
{
	HDC hdcScreen;

	// Create a normal DC and a memory DC for the entire screen. The
	// normal DC provides a "snapshot" of the screen contents. The
	// memory DC keeps a copy of this "snapshot" in the associated
	// bitmap.
	hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	if (m_hMemDC == NULL) {
		m_hMemDC = CreateCompatibleDC(hdcScreen);
	}

	// Create a compatible bitmap for hdcScreen.
	if (m_hBitmapSnapshot == NULL) {
		m_hBitmapSnapshot = CreateCompatibleBitmap(hdcScreen,
			GetDeviceCaps(hdcScreen, HORZRES),
			GetDeviceCaps(hdcScreen, VERTRES));
	}

	if (m_hBitmapSnapshot == NULL) {
		MessageBox(m_hWnd, TEXT("Can't create a compatible bitmap."), TEXT("Error"), MB_OK);
	}

	// Select the bitmaps into the compatible DC.
	if (!SelectObject(m_hMemDC, m_hBitmapSnapshot)) {
		MessageBox(m_hWnd, TEXT("SelectObject Failed"), TEXT("Error"), MB_OK);
	}

	// Hide the application window.
	ShowWindow(m_hWnd, SW_HIDE);
	Sleep(100);

	// Copy color data for the entire display into a
	// bitmap that is selected into a compatible DC.
	if (!BitBlt(m_hMemDC, 0, 0, m_ResX, m_ResY, hdcScreen, 0, 0, SRCCOPY)) {
		MessageBox(m_hWnd, TEXT("BitBlt Failed"), TEXT("Error"), MB_OK);
	}

	// Redraw the application window.
	ShowWindow(m_hWnd, SW_SHOW);

	// Copy Bitmap to Clipboard
	OpenClipboard(m_hWnd);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, m_hBitmapSnapshot);
	CloseClipboard();

	// Delete
	DeleteDC(hdcScreen);
}

void Screenshot::StartFullScreenSnapshot(HWND hWnd)
{
}

void Screenshot::StartWindowSnapshot(HWND hWnd)
{
	HDC hdcScreen;

	// Create a normal DC and a memory DC for the entire screen. The
	// normal DC provides a "snapshot" of the screen contents. The
	// memory DC keeps a copy of this "snapshot" in the associated
	// bitmap.
	hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	if (m_hMemDC == NULL) {
		m_hMemDC = CreateCompatibleDC(hdcScreen);
	}

	CRect rect;
	GetWindowRect(hWnd, rect);
	//GetClientRect(hWnd, rect);

	// Create a compatible bitmap for hdcScreen.
	if (m_hBitmapSnapshot == NULL) {
		m_hBitmapSnapshot = CreateCompatibleBitmap(hdcScreen, rect.Width(), rect.Height());
	}

	if (m_hBitmapSnapshot == NULL) {
		MessageBox(m_hWnd, TEXT("Can't create a compatible bitmap."), TEXT("Error"), MB_OK);
	}

	// Select the bitmaps into the compatible DC.
	if (!SelectObject(m_hMemDC, m_hBitmapSnapshot)) {
		MessageBox(m_hWnd, TEXT("SelectObject Failed"), TEXT("Error"), MB_OK);
	}

	// Copy color data for the entire display into a
	// bitmap that is selected into a compatible DC.
	if (!BitBlt(m_hMemDC, 0, 0, rect.Width(), rect.Height(), hdcScreen, rect.left, rect.top, SRCCOPY)) {
		MessageBox(m_hWnd, TEXT("BitBlt Failed"), TEXT("Error"), MB_OK);
	}

	// Copy Bitmap to Clipboard
	OpenClipboard(m_hWnd);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, m_hBitmapSnapshot);
	CloseClipboard();

	// Delete
	DeleteDC(hdcScreen);
}

void Screenshot::GetScreenResolution()
{
	// Get screen resolution.
	m_ResX = GetSystemMetrics(SM_CXSCREEN);
	m_ResY = GetSystemMetrics(SM_CYSCREEN);
}

int Screenshot::GetScreenResolutionX()
{
	if (m_ResX == -1)
		GetScreenResolution();
	return m_ResX;
}

int Screenshot::GetScreenResolutionY()
{
	if (m_ResY == -1)
		GetScreenResolution();
	return m_ResY;
}

void Screenshot::SaveScreenshot(WCHAR *imageType)
{
	// Prepare image filename
	WCHAR imageDirname[256] = TEXT("screenshot");
	WCHAR imageFilename[256];
	WCHAR dateString[256];
	GetDateString(dateString);
	wsprintf(imageFilename, TEXT("%s\\%s%s"), imageDirname, dateString, imageType);

	// Save image
	CreateDirectory(imageDirname, NULL);
	CImage image;
	image.Attach(m_hBitmapSnapshot);
	image.Save(imageFilename);
}

void Screenshot::GetDateString(WCHAR *buf)
{
	struct tm t;
	time_t now;

	time(&now);
	localtime_s(&t, &now);

	wsprintf(buf, TEXT("%04d-%02d-%02d_%02d-%02d-%02d"),
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}