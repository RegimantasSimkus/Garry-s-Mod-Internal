#include "tramphook.h"
#include "Debug.h"

void __stdcall DummyHook()
{
	g_pDebug->Print("Im a hook!\n");
}

/*
uintptr_t TrampHook(PVOID targetfunction, PVOID hook, SIZE_T bytes)
{
	// allocate memory for original instructions + jump
	PVOID allocatedMem = VirtualAlloc(0, bytes + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	{
		// copy the original instructions into our allocated area
		// memcpy_s(allocatedMem, bytes, targetfunction, bytes);

		// jmp to our hook
		BYTE* jmp = (BYTE*)((uintptr_t)allocatedMem + bytes);
		*jmp = 0xE9;
		*(int*)(jmp + 1) = (uintptr_t)hook - (uintptr_t)jmp - 5;
	}

	{
		// jump to our allocated memory from the original function
		DWORD dwProtect;
		VirtualProtect(targetfunction, bytes, PAGE_EXECUTE_READWRITE, &dwProtect);

		// nop all of the bytes so we can write over them & remove original instructions
		memset(targetfunction, 0x90, bytes);

		// insert jmp to our allocated memory
		BYTE* jmp = (BYTE*)targetfunction;
		*jmp = 0xE9;
		*(uintptr_t*)(jmp + 1) = (uintptr_t)allocatedMem - (uintptr_t)targetfunction - 5;
		VirtualProtect(targetfunction, bytes, dwProtect, &dwProtect);
	}

	g_pDebug->Print("Allocated memory: %p\n", allocatedMem);


	return 0;
}
*/

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