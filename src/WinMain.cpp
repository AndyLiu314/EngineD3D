#include "win/Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	try
	{
		Window wnd(800, 300, L"Donkey Fart Box");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gResult == -1)
		{
			return -1;
		}

		return static_cast<int>(msg.wParam);
	}

	catch (const EngineException& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}

	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	catch (...)
	{
		MessageBox(nullptr, L"No Details Available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}
