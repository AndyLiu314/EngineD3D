#pragma once
#include "win/CustomWin.h"
#include <vector>
#include <string>

class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager();
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void SetMessageStart() noexcept;
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long msgStart = 0u;
	struct IDXGIInfoQueue* pDXGIInfoQueue = nullptr;
};
