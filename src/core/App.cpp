#include "core/App.h"
#include <sstream>

App::App()
	: wnd(800, 300, L"Liao Boom Square")
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
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}
