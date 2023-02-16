#include "tramphook.h"
#include "Debug.h"

uintptr_t TrampHook(PVOID source, PVOID hook, SIZE_T bytes)
{
	// 5 bytes for jmp
	if (bytes < 5) return 0;

	// allocate memory for original instructions + jump
	PVOID allocatedMem = VirtualAlloc(0, bytes + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	g_pDebug->Print("Allocated memory: %p\n", allocatedMem);

	{ // jmp from allocated memory back to source
		memcpy(allocatedMem, source, bytes);
		BYTE* jmp = (BYTE*)((uintptr_t)allocatedMem + bytes);
		*jmp = 0xE9;
		*(int*)(jmp + 1) = ((uintptr_t)source + 5) - (uintptr_t)jmp - 5;
	}

	{ // jmp from source to hook
		DWORD dwProtect;
		VirtualProtect(source, bytes, PAGE_EXECUTE_READWRITE, &dwProtect);
		memset(source, 0x90, bytes);
		BYTE* jmp = (BYTE*)((uintptr_t)source);
		*jmp = 0xE9;
		*(int*)(jmp + 1) = (uintptr_t)hook - (uintptr_t)jmp - 5;
		VirtualProtect(source, bytes, dwProtect, &dwProtect);
	}

	return (uintptr_t)allocatedMem;
}

void RestoreTrampHook(PVOID original, PVOID source, SIZE_T bytes)
{
	DWORD dwProtect;
	VirtualProtect(source, bytes, PAGE_EXECUTE_READWRITE, &dwProtect);
	memcpy(source, original, bytes);
	VirtualProtect(source, bytes, dwProtect, &dwProtect);
}