#pragma once
#include "ShroomArcaneWin.h"
#include "Components.h"

//Only declarations so we also need a links to library see CCP file
#include <d3d11.h>

class ShroomArcaneGraphics
{
public:
	ShroomArcaneGraphics(HWND whdl);
	ShroomArcaneGraphics(const ShroomArcaneGraphics&) = delete;
	ShroomArcaneGraphics& operator=(const ShroomArcaneGraphics&) = delete;
   ~ShroomArcaneGraphics();
    
public:
	void EndFrame();
	void ClearBuffer(Color c);
private:
	ID3D11Device*           pDevice     = nullptr;
	IDXGISwapChain*		    pSwapChain  = nullptr;
	ID3D11DeviceContext*    pContext	= nullptr;
	ID3D11RenderTargetView* pTarget		= nullptr;
};