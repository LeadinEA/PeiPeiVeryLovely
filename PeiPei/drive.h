#pragma once

struct Color
{
	float r;
	float g;
	float b;
};

class Drives
{
public:
	Drives();

	void SetHWND(HWND Window_hWnd);
	bool Init();
	bool Quit();
	void Clear();
	Color To_Color(Color rgb);
	float To_Color(float ColorHex);

	int getfps(float deltaTime);
	float deltatime();

	void DrawBitmap(ID2D1Bitmap* Bitmap);
	void EndDraw();

	void SetNowTimePoint();
	void SetLastTimePoint();

	bool LoadBitmapToFile(ID2D1Bitmap** Bitmap, std::wstring FilePath);
private:
	ID2D1Factory* factory = nullptr;
	ID2D1HwndRenderTarget* graphics = nullptr;

	IWICImagingFactory* ImageFactory = nullptr;

	HRESULT Error;

	int frameCount = 0;
	int fps = 0;

	D2D1_POINT_2F corner = D2D1::Point2F(0, 0);

	static inline std::chrono::steady_clock::time_point timepoint;
	static inline std::chrono::steady_clock::time_point last;
	static inline std::chrono::steady_clock::time_point start;

	HWND hWnd = nullptr;
	RECT Window_Rect = {0};
};