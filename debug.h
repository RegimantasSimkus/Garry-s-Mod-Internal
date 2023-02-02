#pragma once
#include <Windows.h>
#include <iostream>	

class CDebugConsole
{
protected:
	size_t m_iPrints = 0;
public:
	FILE* pFile;

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
		AttachConsole(GetCurrentProcessId());
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	}

	void Release()
	{
		if (pFile)
			fclose(pFile);
		FreeConsole();
	}
#endif
};

extern CDebugConsole* g_pDebug;