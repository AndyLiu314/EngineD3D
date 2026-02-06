#pragma once
#include "win/CustomWin.h"
#include "core/EngineException.h"
#include "graphics/DXGIInfoManager.h"
#include <d3d11.h>
#include <vector>
#include <wrl.h>

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

	class InfoException : public EngineException
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;

		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;

	private:
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
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;
	void DrawTestTriangle(float angle, float x, float y);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
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
#define EGFX_THROW_INFO_ONLY(call) infoManager.SetMessageStart(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException(__LINE__, __FILE__, v);}}
#else
#define EGFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr)
#define EGFX_THROW_FAILED_INFO(hrcall) EGFX_THROW_NOINFO(hrcall)
#define EGFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#define EGFX_THROW_INFO_ONLY(call) (call)
#endif
