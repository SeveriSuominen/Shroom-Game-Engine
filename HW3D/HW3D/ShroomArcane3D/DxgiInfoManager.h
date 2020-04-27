#pragma once
#pragma once
#include "ShroomArcaneWin.h"
#include <vector>

#include <dxgidebug.h>
#include <memory>
#include <wrl.h> //Smart COM Pointers
#include <d3d11.h> //Only declarations so we also need a links to library see CCP file

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue = nullptr;
};