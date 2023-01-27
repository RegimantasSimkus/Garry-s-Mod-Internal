#pragma once
#include <Windows.h>
#include "debug.h"
#include "ibaseclientdll.h"
#include "icliententitylist.h"
#include "ivdebugoverlay.h"
#include "chlclient.h"
#include "clientmodeshared.h"
#include "iluashared.h"

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

	ILuaShared* LuaShared;

	CInterfaces()
	{
		// could find a good amount of interfaces in CHLClient::Init
		ClientDLLSharedAppSystems = CreateInterface<IClientDLLSharedAppSystems>("client.dll", "VClientDllSharedAppSystems001");
		ClientEntityList = CreateInterface<IClientEntityList>("client.dll", "VClientEntityList003");
		DebugOverlay = CreateInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay003");
		Client = CreateInterface<CHLClient>("client.dll", "VClient017");
		LuaShared = CreateInterface<ILuaShared>("lua_shared.dll", "LUASHARED003");

		// call instruction for GetClientModeNormal
		// found in CHLClient::CanRecordDemo aka index 50 of Interface->Client
		DWORD pCallGetClientModeNormal = ((DWORD)((uintptr_t)((*(void***)Client)[50]) + 4));

		// relative address + instruction address + 4 size of address = function address
		DWORD pGetClientModeNormal = (*(DWORD*)pCallGetClientModeNormal) + pCallGetClientModeNormal + 4;

		GetClientModeNormal = (tGetClientModeNormal)pGetClientModeNormal;
		ClientModeShared = GetClientModeNormal();
	}

	typedef void* (*tCreateInterface)(const char*, int*);
	template <typename T>
	T* CreateInterface(const char* szMod, const char* szInterface)
	{
		tCreateInterface pCreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandleA(szMod), "CreateInterface");

		int failed = 0;
		T* ret = (T*)pCreateInterface(szInterface, &failed);
		if (failed)
			g_pDebug->Print("Failed to create interface (%s)\n", szInterface);
		else
			g_pDebug->Print("Found interface %s\t->\t%p\n", szInterface, ret);
		return ret;
	}

	void Dump(const char* szMod)
	{
		HMODULE hMod = GetModuleHandleA(szMod);
		if (!hMod)
			return;

		uintptr_t pCreateInterface = (uintptr_t)GetProcAddress(hMod, "CreateInterface");
		if (!pCreateInterface)
			return;

		DWORD jmpoffset = *(DWORD*)((uintptr_t)pCreateInterface + 5);

		// + 5 for the jmp instruction address
		// + 4 for size of address/pointer
		DWORD pCreateInterfaceInternal = pCreateInterface + 5 + 4 + jmpoffset;

		g_pDebug->Print("interface internal: %p\n", pCreateInterfaceInternal);
		void* pInterfaceRegs = **(DWORD***)(pCreateInterfaceInternal + 6);
		g_pDebug->Print("regs: %p\n", pInterfaceRegs);

		g_pDebug->Print("== %s ==\n", szMod);
		if (pInterfaceRegs)
		{
			void* pCur = pInterfaceRegs;
			while (pCur)
			{
				const char* name = *(const char**)((DWORD)pCur + 4);
				g_pDebug->Print("%s\n", name);
				pCur = *(DWORD**)((DWORD)pCur + 8);
			}
		}

	}
};

extern CInterfaces* Interface;