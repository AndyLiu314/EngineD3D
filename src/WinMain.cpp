#include "core/App.h"
#include "utility/StringConversion.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	try
	{
		return App{800, 600, L"Liao Boom Square"}.Go();
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
