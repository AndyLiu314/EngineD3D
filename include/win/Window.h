#pragma once
#include "win/CustomWin.h"
#include "core/EngineException.h"
#include "input/Keyboard.h"

class Window
{
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

public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;

	Window& operator=(const Window&) = delete;

public:
	Keyboard kbd;

private:
	int width;
	int height;
	HWND hWnd;

	/**
	* Initial windows procedure that runs during window creation.
	* Captures the 'this' windows object pointer passed through CreateWindow().
	* Stores the pointer in the window's user data and switches to the Thunk procedure.
	*/
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	/**
	* Regular windows procedure after setup is finished.
	* Grabs window pointer from window's user data and calls message handler method.
	*/
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

#define EWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define EWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())
