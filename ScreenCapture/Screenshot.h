#pragma once
class Screenshot
{
private:
	HWND m_hWnd;
	HBITMAP m_hBitmapSnapshot = NULL;
	int m_ResX = -1;
	int m_ResY = -1;

public:
	HDC m_hMemDC = NULL;

private:
	void GetScreenResolution();
	void GetDateString(WCHAR *buf);

public:
	Screenshot();
	~Screenshot();

	void Init(HWND hWnd);
	void StartScreenshot();
	void StartFullScreenSnapshot(HWND hWnd);
	void StartWindowSnapshot(HWND hWnd);
	int GetScreenResolutionX();
	int GetScreenResolutionY();
	void SaveScreenshot(WCHAR *imageType);
};
