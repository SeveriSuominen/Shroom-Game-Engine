#pragma once

#include "ShroomArcane3D.h"

class ShroomArcaneExceptionHandler
{
public: 
		static void Handle(HWND handle, LPCTSTR lpText, LPCTSTR lpCaption) noexcept;
	;
};