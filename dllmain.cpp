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

	DWORD callLocalPlayer = (DWORD)((DWORD)(*(void***)Interface->ClientModeShared)[21] + 4);
	DWORD jmp = *(DWORD*)callLocalPlayer;
	GetLocalPlayer = (C_GMOD_Player*(*)())(callLocalPlayer + jmp + 4);

	g_pGlobals = (CGlobalVars*)(**(DWORD***)(void*)(
		(uintptr_t)(*(void***)Interface->Client)[11] + 5
		));

	g_pDebug->Print("Setting up hooks...\n");

	Interface->Dump("client.dll");
	Interface->Dump("engine.dll");

	// + 0x116 = mov ..., esi
	// + 0x116 + 2 = esi
	DWORD pd33device9 = **(DWORD**)(FindSignature("shaderapidx9.dll", "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x53\x56\x57", "xxxxx????xxx") + 0x116 + 2);
	g_pDebug->Print("pdevice -> %p\n", pd33device9);

	DWORD netchan = **(DWORD**)(FindSignature("engine.dll", "\x55\x8b\xec\x51\xa1\x00\x00\x00\x00\x8b\x50\x00\x8d\x48\x00\x85\xd2\x74\x00\x80\x3a\x00\x74\x00\xe8\x00\x00\x00\x00\x50\xe8\x00\x00\x00\x00\x83\xc4\x00\xeb\x00\x8b\x40\x00\x85\xc0", "xxxxx????xx?xx?xxx?xx?x?x????xx????xx?x?xx?xx") + 0x31);
	g_pDebug->Print("baseclient: %p\n", netchan);

	// disconnects from the server with your custom reason then crashes your game with a lua trace lol
	// might be useful at some point
	if (false)
	{
		typedef void(__thiscall* tDisconnect)(void* pThis, const char* reason);
		tDisconnect Disconnect = *(tDisconnect*)(*(DWORD*)netchan + 0x90);
		g_pDebug->Print("Disconnect: %p\n", Disconnect);

		Disconnect((void*)netchan, "Copper door knobs are self-disinfecting.");
	}

	Hooks->EndScene->Hook();

	// Hooks->CreateMove->Hook();

	g_pDebug->Print("So far so good\n");

	while (!g_bShutDown)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			g_bShutDown = true;
			Sleep(300);
			break;
		}
		Sleep(10);
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