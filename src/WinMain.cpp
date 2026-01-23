#include "win/Window.h"
#include "utility/StringConversion.h"
#include <sstream>

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
			
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					oss << "Mouse Position: (" << e.GetPosX() << ", " << e.GetPosY() << ")";
					wnd.SetTitle(oss.str());
				}
			}
		}

		if (gResult == -1)
		{
			return -1;
		}

		return static_cast<int>(msg.wParam);
	}

	catch (const EngineException& e)
	{
		MessageBox(nullptr, ToWide(e.what()).c_str(), ToWide(e.GetType()).c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	catch (const std::exception& e)
	{
		MessageBox(nullptr, ToWide(e.what()).c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	catch (...)
	{
		MessageBox(nullptr, L"No Details Available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}
