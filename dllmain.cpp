#include <windows.h>
#include "debug.h"
#include "Interfaces.h"
#include "globals.h"
#include "hooks.h"

CInterfaces* Interface = nullptr;
CDebugConsole* g_pDebug = nullptr;
CGlobalVars* g_pGlobals = nullptr;
BOOL WINAPI MainThread(LPVOID)
{
	g_pDebug = new CDebugConsole();
	g_pDebug->CreateInstance();
	g_pDebug->Print("Initializing Cheat...\n");

	// initializing stuff
	Interface = new CInterfaces();
	Hooks = new CHooks();

	// taking this from CHLClient::HudUpdate as it's pretty much the first instruction, easy to get
	g_pGlobals = (CGlobalVars*)(**(DWORD***)(void*)(
		(uintptr_t)(*(void***)Interface->Client)[11] + 5
		));

	g_pDebug->Print("Setting up hooks...\n");

	Interface->Dump("client.dll");
	Interface->Dump("engine.dll");

	Hooks->CreateMove->Hook();

	g_pDebug->Print("So far so good\n");

	Sleep(10000);
	g_pDebug->Print("Unloading...\n");
	Hooks->Release();
	Sleep(100);
	delete Interface;
	g_pDebug->Release();
	return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0));
	}
	return TRUE;
}