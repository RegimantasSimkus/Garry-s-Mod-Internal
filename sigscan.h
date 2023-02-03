#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

static uintptr_t FindSignature(const char* mod, const char* pattern, const char* mask)
{
	MODULEINFO modInfo;
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(mod), &modInfo, sizeof(MODULEINFO)))
		return 0;

	DWORD base = (DWORD)modInfo.lpBaseOfDll;
	DWORD size = modInfo.SizeOfImage;

	size_t maskLen = strlen(mask);
	for (DWORD i = base; i < base + size - maskLen; i++)
	{
		bool found = true;
		for (size_t j = 0; j < maskLen; j++) 
		{
			if (mask[j] == '?') continue;

			char c = *(char*)(i + j);
			if (c != pattern[j])
			{
				found = false;
				break;
			}
		}
		if (found)
			return i;
	}
	return 0;
}