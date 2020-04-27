#pragma once
//Error exception helper macros
#define SWND_EXC(hr)		 Window::HRException(__LINE__, __FILE__, hr)
#define SWND_EXC_LAST()		 Window::HRException(__LINE__, __FILE__, GetLastError())
#define SWND_NO_GFX_EXC()    Window::NoGFXException(__LINE__, __FILE__)