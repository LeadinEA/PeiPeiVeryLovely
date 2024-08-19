#pragma once

class PeiPeiWindow
{
public:
	PeiPeiWindow(HINSTANCE hinstance, HINSTANCE lphinstance, LPSTR args, int showcmd);

	bool MessageLoop();

	void WindowQuit(int QuitCode);
	
	HINSTANCE Window_Hinstance;
	HWND Window_hWnd;
private:
	const char* WindowClassName = "PeiPeiVeryLovely";
	const char* SettingClassName = "PeiPeiSetting";
	const char* WindowName = "佩佩宇宙第一可爱！！！";
	const char* SettingName = "想要进入佩佩的设置吗？";

	int WindowSize[2] = { 922,518 };

	int AnimationCount = 0;
	int TempCount = 0;

	float deltatimePP = 0.f;

	HWND Setting_hWnd;

	ID2D1Bitmap* TempBitmap;
	std::vector<ID2D1Bitmap*> Images;

	static inline std::map<HWND,PeiPeiWindow*> WindowPointer;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SettingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void TimerProc(HWND hWnd, UINT unnamedParam2, UINT_PTR unnamedParam3, DWORD unnamedParam4);
};