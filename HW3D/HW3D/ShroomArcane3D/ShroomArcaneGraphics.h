#pragma once
#include "ShroomArcaneWin.h"
#include "Components.h"

//Only declarations so we also need a links to library see CCP file
#include <d3d11.h>


class ShroomArcaneGraphics
{
//--------------------------------------------------------------------------
//Base
//--------------------------------------------------------------------------
public:
	ShroomArcaneGraphics(HWND whdl);
	ShroomArcaneGraphics(const ShroomArcaneGraphics&) = delete;
	ShroomArcaneGraphics& operator=(const ShroomArcaneGraphics&) = delete;
   ~ShroomArcaneGraphics();

//--------------------------------------------------------------------------
//API
//--------------------------------------------------------------------------
public:
	void EndFrame();
	void ClearBuffer(Color c);

//--------------------------------------------------------------------------
//DX11
//--------------------------------------------------------------------------
private:
	ID3D11Device*           pDevice     = nullptr;
	IDXGISwapChain*		    pSwapChain  = nullptr;
	ID3D11DeviceContext*    pContext	= nullptr;
	ID3D11RenderTargetView* pTarget		= nullptr;

//--------------------------------------------------------------------------
// Exceptions
//--------------------------------------------------------------------------
public:
	class  Exception   : public ShroomArcaneException
	{
		using ShroomArcaneException::ShroomArcaneException;
	};
	class  HRException : public Exception
	{
	public:
		HRException(int line,  const char* file, HRESULT hr) noexcept;
		const char* what()     const noexcept override;
		const char* GetType()  const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		
	private:
		HRESULT hr;
	};

	class DeviceRemovedException : public HRException 
	{
		using HRException::HRException;
	public: 
		const char* GetType() const noexcept override;
	};
};
//--------------------------------------------------------------------------