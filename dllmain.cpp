#include <windows.h>
#include "debug.h"
#include "Interfaces.h"

CInterfaces* Interface = nullptr;
CDebugConsole* g_pDebug = nullptr;
BOOL WINAPI MainThread(LPVOID)
{
	g_pDebug = new CDebugConsole();
	g_pDebug->CreateInstance();

	g_pDebug->Print("Initializing Cheat...\n");
	Interface = new CInterfaces();
	
	// taking this from CHLClient::HudUpdate as it's pretty much the first instruction, easy to get
	void* g_pGlobals = **(DWORD***)(void*)(
		(uintptr_t)(*(void***)Interface->Client)[11] + 5
		);


	typedef void* (*tGetClientModeNormal)();

	// call instruction for GetClientModeNormal
	// found in CHLClient::CanRecordDemo aka index 50 of Interface->Client
	DWORD pCallGetClientModeNormal = ((DWORD)((uintptr_t)((*(void***)Interface->Client)[50]) + 4));

	// relative address + instruction address + 4 size of address = function address
	DWORD pGetClientModeNormal = (*(DWORD*)pCallGetClientModeNormal) + pCallGetClientModeNormal + 4;

	tGetClientModeNormal fnGetClientModeNormal = (tGetClientModeNormal)pGetClientModeNormal;
	Interface->ClientModeShared = (ClientModeShared*)fnGetClientModeNormal();

	g_pDebug->Print("g_pGlobals -> %p\n", g_pGlobals);
	g_pDebug->Print("GetClientModeNormal -> %p\n", Interface->ClientModeShared);
	
	Sleep(5000);
	g_pDebug->Release();
	Sleep(50);

	delete g_pDebug;
	delete Interface;
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