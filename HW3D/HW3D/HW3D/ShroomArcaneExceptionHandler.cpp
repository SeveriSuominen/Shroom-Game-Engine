#include "ShroomArcane3D.h"

void ShroomArcaneExceptionHandler::Handle(HWND handle, LPCTSTR lpText, LPCTSTR lpCaption) noexcept
{
	//Only message box for now
	MessageBox
	(
		handle,
		lpText,
		lpCaption,

		MB_OK || MB_ICONEXCLAMATION
	);
}
