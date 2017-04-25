#pragma once
class Screenshot
{
private:
	HBITMAP hBitmapSnapshot = NULL;
	int mResX = -1;
	int mResY = -1;

public:
	HDC hMemDC = NULL;

private:
	void GetScreenResolution();
	void GetDateString(WCHAR *buf);

public:
	Screenshot();
	~Screenshot();

	void StartScreenshot(HWND hWnd);
	int GetScreenResolutionX();
	int GetScreenResolutionY();
	void SaveScreenshot(WCHAR *imageType);
};
