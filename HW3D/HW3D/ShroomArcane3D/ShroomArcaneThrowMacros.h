#pragma once

//Graphics exception checking/throwing macros (some with dxgi infos)
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

#ifdef NDEBUG
#define INIT_DXINFOMNG(gfx) HRESULT hr
#else
#define INIT_DXINFOMNG(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif