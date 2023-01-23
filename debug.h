#pragma once
#include <Windows.h>
#include <iostream>	

template <int rootLen, int strLen>
char* ConcatCStr(const char(&root)[rootLen], const char(&str)[strLen])
{
	char* buff = new char[rootLen + strLen - 1];
	memcpy(buff, root, rootLen - 1);
	memcpy(buff + rootLen, str, strLen);
	return buff;
}

class CDebugConsole
{
protected:
	size_t m_iPrints = 0;
	FILE* pFile;
public:
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
	}
};

extern CDebugConsole* g_pDebug;