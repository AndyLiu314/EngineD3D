#include "core/App.h"
#include <sstream>

App::App()
	: wnd(800, 600, L"Liao Boom Square")
{
}

App::App(int width, int height, const wchar_t* name)
	: wnd(width, height, name)
{
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c,c,1.0f);
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(),
		wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-(wnd.mouse.GetPosY() / 300.0f - 1.0f)
	);
	wnd.Gfx().EndFrame();
}
