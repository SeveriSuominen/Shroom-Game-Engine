#pragma once
#include "ShroomArcaneWin.h"

#include "Components.h"

#include <wrl.h> //Smart COM Pointers
#include "DxgiInfoManager.h"
#include <d3d11.h> //Only declarations so we also need a links to library see CCP file

#include <vector>

class ShroomArcaneGraphics
{
	friend class Bindable;
//--------------------------------------------------------------------------
//Base
//--------------------------------------------------------------------------
public:
	ShroomArcaneGraphics(HWND whdl);
	ShroomArcaneGraphics(const ShroomArcaneGraphics&) = delete;
	ShroomArcaneGraphics& operator=(const ShroomArcaneGraphics&) = delete;
   ~ShroomArcaneGraphics() = default; //SMART POINTERS USED, NO NEED FOR DEC DESTRCR

//--------------------------------------------------------------------------
//API
//--------------------------------------------------------------------------
public:
	void EndFrame();
	void ClearBuffer(Color c);
	void DrawIndexed(UINT count) noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
//--------------------------------------------------------------------------
//DX11
//--------------------------------------------------------------------------
private:
	Microsoft::WRL::ComPtr<ID3D11Device>           pDevice     = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> 	       pSwapChain  = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    pContext	   = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget	   = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV        = nullptr;
private:

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	DirectX::XMMATRIX projection;

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
		using Exception::Exception;
	public:
		HRException(int line,  const char* file, HRESULT hr) noexcept;
		HRException(int line,  const char* file, HRESULT hr, std::vector<std::string> trace) noexcept;
		const char* what()     const noexcept override;
		const char* GetType()  const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
		
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> trace) noexcept;
		const char* what()     const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};

	class DeviceRemovedException : public HRException 
	{
		using HRException::HRException;
	public: 
		const char* GetType() const noexcept override;
	private:
		std::string info;
	};
};
//--------------------------------------------------------------------------