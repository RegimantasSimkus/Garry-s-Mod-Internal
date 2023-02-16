#pragma once
#include <Windows.h>
#include "debug.h"

uintptr_t TrampHook(PVOID targetfunction, PVOID hook, SIZE_T bytes);
void RestoreTrampHook(PVOID original, PVOID source, SIZE_T bytes);