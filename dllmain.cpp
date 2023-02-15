#include <windows.h>
#include "debug.h"
#include "Interfaces.h"
#include "globals.h"
#include "hooks.h"
#include "c_gmod_player.h"
#include "sigscan.h"
#include "c_playerresource.h"

CInterfaces* Interface = nullptr;
CDebugConsole* g_pDebug = nullptr;
CGlobalVars* g_pGlobals = nullptr;

HMODULE hDll;

bool g_bShutDown = false;
BOOL WINAPI MainThread(HMODULE hThread)
{
	g_pDebug = new CDebugConsole();
	g_pDebug->CreateInstance();
	g_pDebug->Print("Initializing Cheat...\n");

	// initializing stuff
	Interface = new CInterfaces();
	Hooks = new CHooks();

	g_pDebug->Print("Getting localplayer...\n");
	uintptr_t callLocalPlayer = (uintptr_t)((uintptr_t)(*(void***)Interface->ClientModeShared)[21] + 
#ifndef _WIN64
		4
#else
		18
#endif
		);
	int call = *(int*)callLocalPlayer;
	GetLocalPlayer = (C_GMOD_Player*(*)())(callLocalPlayer + call + 4);

#ifndef _WIN64
	g_pGlobals = (CGlobalVars*)(**(DWORD***)(void*)(
		(uintptr_t)(*(void***)Interface->Client)[11] + 5
	));
#else
	{
		uintptr_t mov = (uintptr_t)((*(void***)Interface->Client)[11]) + 9;
		g_pGlobals = *(CGlobalVars**)(
			mov + *(int*)mov + 4
		);
	}
#endif



	g_pDebug->Print("Setting up hooks...\n");

	Interface->Dump("client.dll");
	Interface->Dump("engine.dll");
	Interface->Dump("materialsystem.dll");

	// disconnects from the server with your custom reason then crashes your game with a lua trace lol
	// might be useful at some point
	if (false)
	{
		DWORD netchan = **(DWORD**)(FindSignature("engine.dll", "\x55\x8b\xec\x51\xa1\x00\x00\x00\x00\x8b\x50\x00\x8d\x48\x00\x85\xd2\x74\x00\x80\x3a\x00\x74\x00\xe8\x00\x00\x00\x00\x50\xe8\x00\x00\x00\x00\x83\xc4\x00\xeb\x00\x8b\x40\x00\x85\xc0", "xxxxx????xx?xx?xxx?xx?x?x????xx????xx?x?xx?xx") + 0x31);

		typedef void(__thiscall* tDisconnect)(void* pThis, const char* reason);
		tDisconnect Disconnect = *(tDisconnect*)(*(DWORD*)netchan + 0x90);
		g_pDebug->Print("Disconnect: %p\n", Disconnect);

		Disconnect((void*)netchan, "Copper door knobs are self-disinfecting.");
	}

	Hooks->EndScene->Hook();
	Hooks->CreateMove->Hook();

	while (!g_bShutDown)
	{
		Sleep(25);
	}

	g_pDebug->Print("Unloading...\n");
	Hooks->Release();
	Sleep(100);
	delete Interface;
	Sleep(50);
	g_pDebug->Release();
	Sleep(100);
	FreeLibraryAndExitThread(hDll, 0);
	Sleep(100);
	FreeLibraryAndExitThread(hThread, 0);
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0));
		hDll = hModule;
	}
	return TRUE;
}