#pragma once

#include "Shroom.h"

class ShroomExceptionHandler
{
public: 
		static void Handle(HWND handle, LPCTSTR lpText, LPCTSTR lpCaption) noexcept;
	;
};