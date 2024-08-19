#include "const.h"
#include "main.h"
#include "drive.h"
#include "Audio.h"
#include "core.h"

Drives graphics;
AudioDrives Sounds;

void PeiPeiWindow::TimerProc(HWND hWnd,UINT unnamedParam2,UINT_PTR unnamedParam3,DWORD unnamedParam4) {
	graphics.SetNowTimePoint();
	graphics.Clear();

	if (WindowPointer[hWnd]->AnimationCount >= WindowPointer[hWnd]->Images.size())
		WindowPointer[hWnd]->AnimationCount = 0;
	graphics.DrawBitmap(WindowPointer[hWnd]->Images[WindowPointer[hWnd]->AnimationCount]);

	WindowPointer[hWnd]->deltatimePP += graphics.deltatime();
	if (WindowPointer[hWnd]->deltatimePP >= std::chrono::milliseconds(60).count()) {
		WindowPointer[hWnd]->AnimationCount++;
		WindowPointer[hWnd]->deltatimePP = 0;
	}

	graphics.EndDraw();

	graphics.getfps(graphics.deltatime());
	UpdateWindow(WindowPointer[hWnd]->Window_hWnd);
	graphics.SetLastTimePoint();
}

LRESULT PeiPeiWindow::SettingProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT CALLBACK PeiPeiWindow::WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	switch (message)
	{
	case WM_DESTROY:
		graphics.Quit();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		graphics.SetHWND(hWnd);
		if (!graphics.Init()) {
			MessageBox(hWnd, "初始化失败\n佩佩无法显示了QwQ", "警告", MB_OK);
			PostQuitMessage(2);
		}
		Sounds.SetStream("PeiPei.flac");
		Sounds.PlayAudio();
		SetTimer(hWnd, 1, 1, TimerProc);
		break;
	case WM_KEYDOWN:
		if (wParam == 83) {
			ShowWindow(WindowPointer[hWnd]->Setting_hWnd, SW_HIDE);
			ShowWindow(WindowPointer[hWnd]->Setting_hWnd, SW_SHOW);
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

PeiPeiWindow::PeiPeiWindow(HINSTANCE hinstance, HINSTANCE lphinstance, LPSTR args, int showcmd) {
	WNDCLASS Window = { 0 };
	Window.cbClsExtra = 0;
	Window.cbWndExtra = 0;
	Window.hInstance = hinstance;
	Window.hIcon = LoadIcon(hinstance,MAKEINTRESOURCE(IDI_ICON2));
	Window.hCursor = nullptr;
	Window.hbrBackground = (HBRUSH)COLOR_WINDOW;
	Window.lpszMenuName = nullptr;
	Window.lpszClassName = WindowClassName;
	Window.lpfnWndProc = WindowProc;
	RegisterClass(&Window);

	WNDCLASS Setting = { 0 };
	Setting.cbClsExtra = 0;
	Setting.cbWndExtra = 0;
	Setting.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON2));
	Setting.lpszClassName = SettingClassName;
	Setting.lpfnWndProc = SettingProc;
	RegisterClass(&Setting);

	Window_Hinstance = hinstance;
	Window_hWnd = CreateWindow(WindowClassName, WindowName, WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX, 10, 10, WindowSize[0], WindowSize[1], nullptr, nullptr, hinstance, nullptr);
	Setting_hWnd = CreateWindow(SettingClassName, SettingName, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, 10, 10, 400, 400, Window_hWnd, nullptr, hinstance, nullptr);

	for (int i = 1; i < 76; i++)
	{
		std::wstring path = L"PeiPei\\" + std::to_wstring(i) + L".png";
		graphics.LoadBitmapToFile(&TempBitmap, path);
		Images.push_back(TempBitmap);
	}

	ShowWindow(Setting_hWnd, SW_SHOW);
	ShowWindow(Window_hWnd, SW_SHOW);
	WindowPointer.insert({ Window_hWnd,this });
}

bool PeiPeiWindow::MessageLoop() {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return false;
}

void PeiPeiWindow::WindowQuit(int QuitCode) {
	graphics.Quit();
	PostQuitMessage(QuitCode);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE lphinstance, LPSTR args, int showcmd) {
	PeiPeiWindow PeiWindow = { hinstance,lphinstance,args,showcmd };
	PeiWindow.MessageLoop();
	return 0;
}