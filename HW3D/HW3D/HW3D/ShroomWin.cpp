#include "App.h"

#include <string>
#include <sstream>

int CALLBACK  WinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow
) 
{
	try 
	{
		return App{}.Go();
	}
#ifdef __SHROOM_ARCANE_3D__
	catch (const ShroomArcaneException& e)
	{
		ShroomExceptionHandler::Handle(nullptr, e.what(), e.GetType());
	}
#endif 
	catch (const ShroomException& e)
	{
		ShroomExceptionHandler::Handle(nullptr, e.what(), e.GetType());
	}
	catch (const std::exception&  e)
	{
		ShroomExceptionHandler::Handle(nullptr, e.what(), "Standard Exception");
	}
	catch (...)
	{
		ShroomExceptionHandler::Handle(nullptr, "No details", "Unknown Exception");
	}
	return -1;
}
