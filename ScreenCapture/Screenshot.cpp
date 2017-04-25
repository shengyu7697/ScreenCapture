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
	DeleteDC(hdcSnapshot);
	DeleteObject(hbmSnapshot);
}

void Screenshot::StartScreenshot(HWND hWnd)
{
	HDC hdcScreen;

	// Create a normal DC and a memory DC for the entire screen. The
	// normal DC provides a "snapshot" of the screen contents. The
	// memory DC keeps a copy of this "snapshot" in the associated
	// bitmap.
	hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	if (hdcSnapshot == NULL) {
		hdcSnapshot = CreateCompatibleDC(hdcScreen);
	}

	// Create a compatible bitmap for hdcScreen.
	if (hbmSnapshot == NULL) {
		hbmSnapshot = CreateCompatibleBitmap(hdcScreen,
			GetDeviceCaps(hdcScreen, HORZRES),
			GetDeviceCaps(hdcScreen, VERTRES));
	}

	if (hbmSnapshot == NULL) {
		MessageBox(hWnd, TEXT("Can't create a compatible bitmap."), TEXT("Error"), MB_OK);
	}

	// Select the bitmaps into the compatible DC.
	if (!SelectObject(hdcSnapshot, hbmSnapshot)) {
		MessageBox(hWnd, TEXT("SelectObject Failed"), TEXT("Error"), MB_OK);
	}

	// Hide the application window.
	ShowWindow(hWnd, SW_HIDE);
	Sleep(100);

	// Copy color data for the entire display into a
	// bitmap that is selected into a compatible DC.
	if (!BitBlt(hdcSnapshot, 0, 0, mResX, mResY, hdcScreen, 0, 0, SRCCOPY)) {
		MessageBox(hWnd, TEXT("BitBlt Failed"), TEXT("Error"), MB_OK);
	}

	// Redraw the application window.
	ShowWindow(hWnd, SW_SHOW);

	// Copy Bitmap to Clipboard
	OpenClipboard(hWnd);
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, hbmSnapshot);
	CloseClipboard();

	// Delete
	DeleteDC(hdcScreen);
}

void Screenshot::GetScreenResolution()
{
	// Get screen resolution.
	mResX = GetSystemMetrics(SM_CXSCREEN);
	mResY = GetSystemMetrics(SM_CYSCREEN);
}

int Screenshot::GetScreenResolutionX()
{
	if (mResX == -1)
		GetScreenResolution();
	return mResX;
}

int Screenshot::GetScreenResolutionY()
{
	if (mResY == -1)
		GetScreenResolution();
	return mResY;
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
	image.Attach(hbmSnapshot);
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