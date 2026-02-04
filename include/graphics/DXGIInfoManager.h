#pragma once
#include "win/CustomWin.h"
#include <vector>
#include <string>
#include <wrl.h>
#include <dxgidebug.h>

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void SetMessageStart() noexcept;
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long msgStart = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDXGIInfoQueue;
};
