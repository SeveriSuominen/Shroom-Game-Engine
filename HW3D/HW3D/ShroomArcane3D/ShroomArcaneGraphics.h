#pragma once
#include "ShroomArcaneWin.h"
#include "Components.h"
#include <wrl.h> //Smart COM Pointers

#include <d3d11.h> //Only declarations so we also need a links to library see CCP file
#include "DxgiInfoManager.h"
#include <vector>

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) ShroomArcaneGraphics::HRException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw ShroomArcaneGraphics::HRException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) ShroomArcaneGraphics::HRException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) ShroomArcaneGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw ShroomArcaneGraphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) ShroomArcaneGraphics::HRException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) ShroomArcaneGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

class ShroomArcaneGraphics
{
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
	void DrawTestTriangle(float test);

//--------------------------------------------------------------------------
//DX11
//--------------------------------------------------------------------------
private:
	Microsoft::WRL::ComPtr<ID3D11Device>           pDevice     = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> 	       pSwapChain  = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    pContext	   = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget	   = nullptr;

//--------------------------------------------------------------------------
// Exceptions
//--------------------------------------------------------------------------
#ifndef NDEBUG;
private:
	DxgiInfoManager infoManager;
#endif

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