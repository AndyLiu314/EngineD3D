#include "graphics/Graphics.h"
#include "utility/StringConversion.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


/* --------- GRAPHICS EXCEPTION --------- */
Graphics::Exception::Exception(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	: EngineException(line, file), hr(hr)
{
	for (const auto& msg : infoMsgs)
	{
		info += msg;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl;

	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::Exception::GetType() const noexcept
{
	return "Engine Graphics Exception";
}

std::string Graphics::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf),
		0,
		nullptr
	);

	if (nMsgLen == 0)
	{
		return "Unidentified Error Code";
	}

	std::string errorString = ToNarrow(pMsgBuf);
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Graphics::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

std::string Graphics::Exception::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Engine Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	: EngineException(line, file)
{
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Engine Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
/* --------- GRAPHICS EXCEPTION --------- */


/* --------- GRAPHICS --------- */
Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT flags = 0u;
	#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	HRESULT hr;

	EGFX_THROW_FAILED_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	));

	wrl::ComPtr<ID3D11Resource> pBackBuffer;

	EGFX_THROW_FAILED_INFO(pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Resource),
		&pBackBuffer
	));

	EGFX_THROW_FAILED_INFO(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	));
}

void Graphics::EndFrame()
{
	HRESULT hr;
	#ifdef _DEBUG
	infoManager.SetMessageStart();
	#endif

	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw EGFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw EGFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float colour[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), colour);
}

void Graphics::DrawTestTriangle(float angle, float x, float y)
{
	dx::XMVECTOR v = dx::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
	auto result = dx::XMVector3Transform(v, dx::XMMatrixScaling(1.5f, 0.0f, 0.0f));
	auto xx = dx::XMVectorGetX(result);

	HRESULT hr;

	struct Vertex
	{
		struct
		{
			float x;
			float y;
		} pos;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} colour;
	};

	const Vertex vertices[] =
	{
		{0.0f, 0.5f, 255, 0, 0, 0},
		{0.5f, -0.5f, 0, 255, 0, 0},
		{-0.5f, -0.5f, 0, 0, 255, 0},
		{-0.3f, 0.3f, 0, 255, 0, 0},
		{0.3f, 0.3f, 0, 0, 255, 0},
		{0.0f, -1.0f, 255, 0, 0, 0},
	};

	/* Vertex Buffer */
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	EGFX_THROW_FAILED_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	/* Bind Vertex Buffer */
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	/* Index Buffer */
	const unsigned short indices[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
	};
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	EGFX_THROW_FAILED_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	/* Bind Index Buffer */
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	/* Constant Buffer */
	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(angle) *
				dx::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *
				dx::XMMatrixTranslation(x, y, 0.0f)
			)

		}
	};
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	EGFX_THROW_FAILED_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	/* Bind Constant Buffer */
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	/* Pixel Shader */
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	EGFX_THROW_FAILED_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	EGFX_THROW_FAILED_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	/* Vertex Shader */
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	EGFX_THROW_FAILED_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	EGFX_THROW_FAILED_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	/* Input Layout Of Vertices */
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
		{"Colour", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0u},

	};
	EGFX_THROW_FAILED_INFO(pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));
	pContext->IASetInputLayout(pInputLayout.Get());

	/* Bind Render Target */
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	/* Primitive Topology Setting To Triangle List */
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/* Configure Viewport */
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	EGFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}
/* --------- GRAPHICS --------- */
