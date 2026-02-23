#pragma once

#define EGFX_EXCEPT_NOINFO(hr) Graphics::Exception(__LINE__, __FILE__, hr)
#define EGFX_THROW_FAILED_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw EGFX_EXCEPT_NOINFO(hr)

#ifdef _DEBUG
	#define EGFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr, infoManager.GetMessages())
	#define EGFX_THROW_FAILED_INFO(hrcall) infoManager.SetMessageStart(); if( FAILED( hr = (hrcall) ) ) throw EGFX_EXCEPT(hr)
	#define EGFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, infoManager.GetMessages())
	#define EGFX_THROW_INFO_ONLY(call) infoManager.SetMessageStart(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException(__LINE__, __FILE__, v);}}
#else
	#define EGFX_EXCEPT(hr) Graphics::Exception(__LINE__, __FILE__, hr)
	#define EGFX_THROW_FAILED_INFO(hrcall) EGFX_THROW_FAILED_NOINFO(hrcall)
	#define EGFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
	#define EGFX_THROW_INFO_ONLY(call) (call)
#endif

#ifdef _DEBUG
	#define INFOMAN(gfx) HRESULT hr; DXGIInfoManager& infoManager = GetInfoManager((gfx))
#else
	#define INFOMAN(gfx) HRESULT hr
#endif
