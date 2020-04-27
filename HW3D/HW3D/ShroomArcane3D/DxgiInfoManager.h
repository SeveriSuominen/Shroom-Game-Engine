#pragma once
#pragma once
#include "ShroomArcaneWin.h"
#include <vector>
#include "ShroomArcaneWin.h"
#include "ShroomArcaneGraphics.h"
#include <dxgidebug.h>
#include <memory>

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