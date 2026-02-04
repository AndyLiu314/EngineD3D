#pragma once
#include "win/CustomWin.h"
#include "core/EngineException.h"
#include "graphics/DXGIInfoManager.h"
#include <d3d11.h>
#include <vector>

class Graphics 
{
public:
	class Exception : public EngineException
	{
	public:
		Exception(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;

		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;

	private:
		std::string reason;
	};

public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
	#ifdef _DEBUG
	DXGIInfoManager infoManager;
	#endif
};

#define EGFX_EXCEPT_NOINFO(hr) Graphics::Exception(__LINE__, __FILE__, hr)
#define EGFX_THROW_FAILED_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw EGFX_EXCEPT_NOINFO(hr)

#ifdef _DEBUG
#define EGFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define EGFX_THROW_FAILED_INFO(hrcall) infoManager.SetMessageStart(); if( FAILED( hr = (hrcall) ) ) throw EGFX_EXCEPT(hr)
#define EGFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#else
#define EGFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr)
#define EGFX_THROW_FAILED_INFO(hrcall) EGFX_THROW_NOINFO(hrcall)
#define EGFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#endif
