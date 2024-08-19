#include "const.h"
#include "drive.h"
#include "core.h"

Drives::Drives() {

}

bool Drives::Init() {
	if (hWnd == nullptr) {
		return false;
	}
	using namespace D2D1;
	Error = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	GetWindowRect(hWnd, &Window_Rect);
	Error = factory->CreateHwndRenderTarget(RenderTargetProperties(), HwndRenderTargetProperties(hWnd, SizeU(Window_Rect.right - Window_Rect.left, Window_Rect.bottom - Window_Rect.top)), &graphics);
	CoInitialize(NULL);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,(LPVOID*)&ImageFactory);
	return true;
}

void Drives::SetHWND(HWND Window_hWnd) {
	hWnd = Window_hWnd;
}

//int getTime()
//{
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
//}

void Drives::SetNowTimePoint() {
	timepoint = std::chrono::steady_clock::now();
}

void Drives::SetLastTimePoint() {
	last = timepoint;
	SetNowTimePoint();
}

float Drives::deltatime() {
	using namespace std::chrono;
	//timepoint = std::chrono::steady_clock::now();
	//last = timepoint;
	//timepoint = std::chrono::steady_clock::now();
	//auto deltatime = (timepoint - last).count() / 1000.f / 1000.f;
	auto deltaTime = (float)duration_cast<milliseconds>(timepoint - last).count();
	return deltaTime;
}

int Drives::getfps(float deltaTime)
{
	//static int fps = 0;
	//static int timeLeft = 1000; // 取固定时间间隔为1秒
	//static int frameCount = 0;

	//++frameCount;
	//timeLeft -= deltaTime;
	//if (timeLeft < 0)
	//{
	//	fps = frameCount;
	//	frameCount = 0;
	//	timeLeft = 1000;
	//}
	//return fps;

	frameCount++; // 帧数加一

	if (std::chrono::duration_cast<std::chrono::seconds>(timepoint - start).count() >= 1) {

		//double elapsedTimeSeconds = deltaTime / 1000.f; // 将毫秒转换为秒
		//double refreshRate = static_cast<double>(frameCount) / elapsedTimeSeconds; // 计算刷新率（帧率）

		fps = frameCount;

		std::string out = std::to_string(deltaTime);
		std::string fps_out = std::to_string(fps);
		Base::PrintDebug(out + "\n");
		Base::PrintDebug(fps_out + "\n");

		// 重置帧数和时间
		start = timepoint;
		frameCount = 0;
	}

	//if (frameCount)
	//	MessageBox(NULL, "错误", "", MB_OK);

	return fps;
}

void Drives::DrawBitmap(ID2D1Bitmap* Bitmap) {
	D2D1_SIZE_F Size = graphics->GetSize();
	graphics->DrawBitmap(Bitmap, D2D1::RectF(corner.x,corner.y,corner.x + Size.width,corner.y + Size.height));
}

Color Drives::To_Color(Color rgb)
{
	rgb.r = rgb.r / 255;
	rgb.g = rgb.g / 255;
	rgb.b = rgb.b / 255;
	return rgb;
}

float Drives::To_Color(float ColorHex) {
	ColorHex = ColorHex / 255;
	return ColorHex;
}

void Drives::Clear() {
	graphics->BeginDraw();
	graphics->Clear(D2D1::ColorF(0,0,0));
}

void Drives::EndDraw() {
	graphics->EndDraw();
}

bool Drives::LoadBitmapToFile(ID2D1Bitmap** Bitmap,std::wstring FilePath) {
	IWICBitmapDecoder* Decoder = nullptr;
	IWICBitmapFrameDecode* Source = nullptr;
	IWICFormatConverter* Converter = nullptr;

	ImageFactory->CreateDecoderFromFilename(
		FilePath.c_str(),
		NULL,GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&Decoder
	);

	Decoder->GetFrame(0, &Source);

	ImageFactory->CreateFormatConverter(&Converter);

	Converter->Initialize(
		Source,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	graphics->CreateBitmapFromWicBitmap(Converter, NULL, Bitmap);

	if (Decoder != nullptr)
		Decoder->Release();
	if (Source != nullptr)
		Source->Release();
	if (Converter != nullptr)
		Converter->Release();

	return true;
}

bool Drives::Quit() {
	graphics->Release();
	factory->Release();
	ImageFactory->Release();
	CoUninitialize();
	return true;
}