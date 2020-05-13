#include "ShroomWin32Tools.h"
#include <windows.h>
#include <shobjidl.h> 
#include <sstream>

std::filesystem::path ShroomWin32Tools::OpenFileDialog()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		const COMDLG_FILTERSPEC c_rgSaveTypes[] =
		{
			{L"Shroom save file (*.shroom)", L"*.shroom"}
			/*{L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
			{L"Text Document (*.txt)",       L"*.txt"},
			{L"All Documents (*.*)",         L"*.*"}*/
		};

		pFileOpen->SetFileTypes
		(
			1, c_rgSaveTypes
		);

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					LPWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//std::stringstream ss;
						//ss << pszFilePath;

						std::wstring ws(pszFilePath);
						std::string path = std::string(ws.begin(), ws.end());

						return std::filesystem::path(path);

						//MessageBox(NULL, path.c_str(), "File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return std::filesystem::path();
}

std::filesystem::path ShroomWin32Tools::SaveAsDialog()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *pSaveFile;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pSaveFile));

		const COMDLG_FILTERSPEC c_rgSaveTypes[] =
		{
			{L"Shroom save file (*.shroom)", L"*.shroom"}
			/*{L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
			{L"Text Document (*.txt)",       L"*.txt"},
			{L"All Documents (*.*)",         L"*.*"}*/
		};

		pSaveFile->SetFileTypes
		(
			1, c_rgSaveTypes
		);

		std::string  temp = std::string(".shroom");
		std::wstring stemp = std::wstring(temp.begin(), temp.end());

		pSaveFile->SetDefaultExtension
		(
			stemp.c_str()
		);

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pSaveFile->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pSaveFile->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					LPWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//std::stringstream ss;
						//ss << pszFilePath;

						std::wstring ws(pszFilePath);
						std::string path = std::string(ws.begin(), ws.end());

						return std::filesystem::path(path);

						//MessageBox(NULL, path.c_str(), "File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pSaveFile->Release();
		}
		CoUninitialize();
	}
	return std::filesystem::path();
}
