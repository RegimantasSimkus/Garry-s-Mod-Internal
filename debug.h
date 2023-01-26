#pragma once
#include <Windows.h>
#include <iostream>	

class CDebugConsole
{
protected:
	size_t m_iPrints = 0;
	FILE* pFile;
public:

#ifndef _DEBUG

	void Print(const char* fmt, ...) {};
	void CreateInstance() {};
	void Release() {};

#else

	void Print(const char* fmt, ...)
	{
		va_list vl;
		va_start(vl, fmt);

		vprintf(fmt, vl);

		va_end(vl);
	}

	void CreateInstance()
	{
		AllocConsole();
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	}

	void Release()
	{
		if (pFile)
			fclose(pFile);
		pFile = nullptr;
		FreeConsole();
		delete this;
	}
#endif
};

extern CDebugConsole* g_pDebug;