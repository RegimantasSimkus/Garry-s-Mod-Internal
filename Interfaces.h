#pragma once
#include <Windows.h>
#include "debug.h"
#include "ibaseclientdll.h"
#include "icliententitylist.h"
#include "ivdebugoverlay.h"
#include "chlclient.h"
#include "clientmodeshared.h"
class CInterfaces
{
public:
	IClientDLLSharedAppSystems* ClientDLLSharedAppSystems;
	IClientEntityList* ClientEntityList;
	IVDebugOverlay* DebugOverlay;
	CHLClient* Client;
	ClientModeShared* ClientModeShared;

	typedef ::ClientModeShared* (*tGetClientModeNormal)();
	tGetClientModeNormal GetClientModeNormal;

	CInterfaces()
	{
		// could find a good amount of interfaces in CHLClient::Init
		ClientDLLSharedAppSystems = CreateInterface<IClientDLLSharedAppSystems>("client.dll", "VClientDllSharedAppSystems001");
		ClientEntityList = CreateInterface<IClientEntityList>("client.dll", "VClientEntityList003");
		DebugOverlay = CreateInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay003");
		Client = CreateInterface<CHLClient>("client.dll", "VClient017");


		// call instruction for GetClientModeNormal
		// found in CHLClient::CanRecordDemo aka index 50 of Interface->Client
		DWORD pCallGetClientModeNormal = ((DWORD)((uintptr_t)((*(void***)Client)[50]) + 4));

		// relative address + instruction address + 4 size of address = function address
		DWORD pGetClientModeNormal = (*(DWORD*)pCallGetClientModeNormal) + pCallGetClientModeNormal + 4;

		GetClientModeNormal = (tGetClientModeNormal)pGetClientModeNormal;
		ClientModeShared = GetClientModeNormal();
	}

	template <typename T>
	T* CreateInterface(const char* szMod, const char* szInterface)
	{
		typedef void* (*tCreateInterface)(const char*, int*);
		tCreateInterface pCreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(szMod), "CreateInterface");

		int failed = 0;
		T* ret = (T*)pCreateInterface(szInterface, &failed);
		if (failed)
			g_pDebug->Print("Failed to create interface (%s)\n", szInterface);
		else
			g_pDebug->Print("Found interface %s\t->\t%p\n", szInterface, ret);
		return ret;
	}
};

extern CInterfaces* Interface;