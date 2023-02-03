#pragma once
#include <Windows.h>

class VMTHook
{
public:
	// pointer to vTable[index]
	PVOID* pFunc;

	// the original function
	PVOID* pOriginal;

	// our hook
	PVOID pHook;

	// whether we've hooked the function
	BOOL bHooked = FALSE;

	~VMTHook()
	{
		if (bHooked) Restore();
	}

	VMTHook(PVOID* pAddy, PVOID pHook, PVOID* pOriginal)
	{
		this->pFunc = pAddy;
		this->pHook = pHook;
		this->pOriginal = pOriginal;
	}

	void Hook()
	{
		if (bHooked) return;
		bHooked = TRUE;
		DWORD dwOldProtect;
		VirtualProtect(pFunc, sizeof(PVOID), PAGE_EXECUTE_READWRITE, &dwOldProtect);
		*pOriginal = *pFunc;
		*pFunc = pHook;
		VirtualProtect(pFunc, sizeof(PVOID), dwOldProtect, &dwOldProtect);
	}

	void Restore()
	{
		if (!bHooked) return;
		bHooked = FALSE;
		DWORD dwOldProtect;
		VirtualProtect(pFunc, sizeof(PVOID), PAGE_EXECUTE_READWRITE, &dwOldProtect);
		*pFunc = *pOriginal;
		VirtualProtect(pFunc, sizeof(PVOID), dwOldProtect, &dwOldProtect);
	}
};

class VMTHookManager
{
public:
	PVOID pInst;
	PVOID* vTable;

	VMTHook* Hook(int index, PVOID pHook, PVOID* pOriginal)
	{
		DWORD dwOldProtect;
		VirtualProtect(&vTable[index], sizeof(PVOID), PAGE_EXECUTE_READWRITE, &dwOldProtect);
		VMTHook* hook = new VMTHook(&vTable[index], pHook, pOriginal);
		VirtualProtect(&vTable[index], sizeof(PVOID), dwOldProtect, &dwOldProtect);
		return hook;
	}

	template <typename tFunc>
	tFunc Hook(int index, PVOID pNewFunc)
	{
		DWORD dwOldProtect;
		VirtualProtect(&vTable[index], sizeof(PVOID), PAGE_EXECUTE_READWRITE, &dwOldProtect);
		PVOID pOriginal = vTable[index];
		vTable[index] = pNewFunc;
		VirtualProtect(&vTable[index], sizeof(PVOID), dwOldProtect, &dwOldProtect);
		return (tFunc)pOriginal;
	}

	//template <typename tFunc>
	//void Hook(int index, PVOID pNewFunc, PVOID* pOriginal)
	//{
	//	*pOriginal = Hook<tFunc>(index, pNewFunc);
	//}

	void RestoreHook(int index, PVOID pOriginal)
	{
		DWORD dwOldProtect;
		VirtualProtect(&vTable[index], sizeof(PVOID), PAGE_EXECUTE_READWRITE, &dwOldProtect);
		vTable[index] = pOriginal;
		VirtualProtect(&vTable[index], sizeof(PVOID), dwOldProtect, &dwOldProtect);
	}

	VMTHookManager(PVOID pInst)
	{
		vTable = *(PVOID**)pInst;
		this->pInst = pInst;
	}
};