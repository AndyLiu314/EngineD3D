#include "win/CustomWin.h"
#include "win/WindowsMessageMap.h"
#include <string>
#include <iostream>
#include <sstream>

// Helper function to convert std::string to std::wstring
inline std::wstring StringToWString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}

LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	static WindowsMessageMap messageMap;
	OutputDebugStringW(StringToWString(messageMap(msg, lParam, wParam)).c_str());

	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == 'F') {
			SetWindowText(hWnd, L"The 'F' key was pressed!");
		}
		break;

	case WM_KEYUP:
		if (wParam == 'F') {
			SetWindowText(hWnd, L"released F");
		}
		break;

	case WM_CHAR:
		{
			static std::wstring title;
			title.push_back(static_cast<wchar_t>(wParam));
			SetWindowText(hWnd, title.c_str());
		
		}
		break;

	case WM_LBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			std::wostringstream oss;
			oss << L"Mouse Clicked at (" << pt.x << L"," << pt.y << L")";
			SetWindowText(hWnd, oss.str().c_str());
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	const auto pClassName = L"EngineD3D";

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(
		0, pClassName,
		L"Fundamental Direct3D 11",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	BOOL resultCode;
	while ((resultCode = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (resultCode == -1) {
		return -1;
	}
	else {
		return msg.wParam;
	}
}
