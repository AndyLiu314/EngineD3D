#include "win/Window.h"
#include "utility/Timer.h"

class App
{
public:
	App();
	App(int width, int height, const wchar_t* name);

	int Go();

private:
	void DoFrame();
	
	Window wnd;
	Timer timer;
};
