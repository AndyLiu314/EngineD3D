#include "graphics/DXGIInfoManager.h"
#include "win/Window.h"
#include "graphics/Graphics.h"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")

DXGIInfoManager::DXGIInfoManager()
{
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw EWND_LAST_EXCEPT();
	}

	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
	);

	if (DxgiGetDebugInterface == nullptr)
	{
		throw EWND_LAST_EXCEPT();
	}

	HRESULT hr;
	EGFX_THROW_FAILED_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&pDXGIInfoQueue)));
}

DXGIInfoManager::~DXGIInfoManager()
{
	if (pDXGIInfoQueue != nullptr)
	{
		pDXGIInfoQueue->Release();
	}
}

void DXGIInfoManager::SetMessageStart() noexcept
{
	msgStart = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DXGIInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

	for (auto i = msgStart; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength = 0;
		EGFX_THROW_FAILED_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		EGFX_THROW_FAILED_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}
