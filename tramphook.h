#pragma once
#include <Windows.h>
#include "debug.h"

uintptr_t TrampHook(PVOID targetfunction, PVOID hook, SIZE_T bytes);
void RestoreTrampHook(PVOID original, PVOID source, SIZE_T bytes);

class CTrampHook
{
public:
	PVOID pTargetFunction;
	SIZE_T nBytes;
	PVOID* pOriginalBytes;
	PVOID pHook;
	bool bEnabled = false;

	CTrampHook(PVOID pTargetFunction, PVOID pHook, SIZE_T nBytes, PVOID* pOutOriginal)
	{
		this->pTargetFunction = pTargetFunction;
		this->nBytes = nBytes;
		this->pOriginalBytes = pOutOriginal;
		this->pHook = pHook;
	}

	void Enable()
	{
		if (bEnabled) return;

		*pOriginalBytes = (PVOID)TrampHook(this->pTargetFunction, this->pHook, this->nBytes);
		bEnabled = true;
	}

	void Disable()
	{
		if (!bEnabled) return;

		RestoreTrampHook(*pOriginalBytes, this->pTargetFunction, this->nBytes);
		bEnabled = false;
	}

	bool Toggle()
	{
		if (bEnabled)
			Disable();
		else
			Enable();
	}

	void Restore()
	{
		Disable();
	}
};