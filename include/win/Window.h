#pragma once
#include "win/CustomWin.h"
#include "core/EngineException.h"

class Window
{
public:
	Window(int width, int height, const wchar_t* name) noexcept;
	~Window();
	Window(const Window&) = delete;

	Window& operator=(const Window&) = delete;

private:
	int width;
	int height;
	HWND hWnd;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	class Exception : public EngineException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;

		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT hr;
	};

private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const wchar_t* wndClassName = L"EngineD3D Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
};

#define EWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
