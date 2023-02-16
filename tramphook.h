#pragma once
#include <Windows.h>
#include "debug.h"

class Dummy
{
public:
	int total = 0;


};

static void __stdcall DummyFunction()
{
	g_pDebug->Print("Im a dummy function!\n");
}

void __stdcall DummyHook();

uintptr_t TrampHook(PVOID targetfunction, PVOID hook, SIZE_T bytes);