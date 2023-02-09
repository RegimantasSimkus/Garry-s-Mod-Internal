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
	Interface->Dump("materialsystem.dll");

	g_pDebug->Print("LocalPlayer: %p (%p)\n", GetLocalPlayer(), GetLocalPlayer);

	// will need to find a reference to this function as the address will change after a restart
	DWORD pGetActiveWeapon = FindSignature("client.dll", "\x8b\x91\x00\x00\x00\x00\x83\xfa\x00\x74\x00\xa1\x00\x00\x00\x00\x8b\xca\x81\xe1\x00\x00\x00\x00\xc1\xea\x00\x03\xc9\x39\x54\xc8\x00\x75\x00\x8b\x44\xc8\x00\xc3\x33\xc0\xc3\xcc\xcc\xcc\xcc\xcc\x55\x8b\xec\x56\x8b\x75", "xx????xx?x?x????xxxx????xx?xxxxx?x?xxx?xxxxxxxxxxxxxxx");
	typedef void* (__thiscall* tGetActiveWeapon)(void* pThis);
	tGetActiveWeapon GetActiveWeapon = (tGetActiveWeapon)pGetActiveWeapon;

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

		C_GMOD_Player* localplayer = GetLocalPlayer();
		if (localplayer)
		{
			g_pDebug->Print("LocalActiveWeapon = %p\n", GetActiveWeapon(localplayer));
		}

		C_GMOD_Player* bot = (C_GMOD_Player*)Interface->ClientEntityList->GetClientEntity(2);
		if (bot)
		{
			g_pDebug->Print("BotWeapon = %p\n", GetActiveWeapon(bot));
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