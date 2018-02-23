#include "AppStatGrabber.h"

//errno_t GetExeInfo(TCHAR* _path)
//{
//	//If the function fails, the return value is zero.
//	const auto retSize = GetFileVersionInfoSize(_path, nullptr);
//	if (!retSize)
//	{
//		return GetLastError();
//	}
//
//	auto pBlock = new DWORD[retSize / sizeof(DWORD)];
//
//	//If the function fails, the return value is zero.
//	if (!GetFileVersionInfo(_path, NULL, retSize, pBlock))
//	{
//		delete[] pBlock;
//		return GetLastError();
//	}
//
//	// 
//
//	struct LANGANDCODEPAGE {
//		WORD wLanguage;
//		WORD wCodePage;
//	} *pLangCodePage;
//
//	UINT cpSz;
//
//	if (!VerQueryValue(pBlock, _T("\\VarFileInfo\\Translation"), reinterpret_cast<LPVOID*>(&pLangCodePage), &cpSz))
//	{
//		delete[] pBlock;
//		return GetLastError();
//	}
//
//	TCHAR paramNameBuf[256]; // здесь формируем имя параметра
//	TCHAR *paramValue;       // здесь будет указатель на значение параметра, который нам вернет функция VerQueryValue
//	UINT paramSz;            // здесь будет длина значения параметра, который нам вернет функция VerQueryValue
//
//	constexpr TCHAR *paramNames[] = {
//		_T("FileDescription"),
//		_T("CompanyName"),
//		_T("FileVersion"),
//		_T("InternalName"),
//		_T("LegalCopyright"),
//		_T("LegalTradeMarks"),
//		_T("OriginalFilename"),
//		_T("ProductName"),
//		_T("ProductVersion"),
//		_T("Comments"),
//		_T("Author")
//	};
//
//	for (int cpIdx = 0; cpIdx < (int)(cpSz / sizeof(struct LANGANDCODEPAGE)); cpIdx++)
//	{
//		// перебираем имена параметров
//		for (int paramIdx = 0; paramIdx < sizeof(paramNames) / sizeof(char*); paramIdx++)
//		{
//			// формируем имя параметра ( \StringFileInfo\кодовая_страница\имя_параметра )
//			_stprintf(paramNameBuf, _T("\\StringFileInfo\\%04x%04x\\%s"),
//				pLangCodePage[cpIdx].wLanguage,  // ну, или можно сделать фильтр для
//				pLangCodePage[cpIdx].wCodePage,  // какой-то отдельной кодовой страницы
//				paramNames[paramIdx]);
//
//			//if (VerQueryValue(pBlock, paramNameBuf, (LPVOID*)&paramValue, &paramSz))
//			//	std::wcout << L"\t\t" << paramNames[paramIdx] << L":\t\t" << paramValue << std::endl;
//			//else
//			//	std::wcout << L"\t\t" << paramNames[paramIdx] << L"\t\tHет информации " << std::endl;
//		}
//	}
//
//	delete[] pBlock;
//	return 0;
//}
