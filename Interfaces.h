#pragma once
#include <Windows.h>
#include "debug.h"
#include "ibaseclientdll.h"
#include "icliententitylist.h"
#include "ivdebugoverlay.h"
#include "chlclient.h"
#include "clientmodeshared.h"
#include "iluashared.h"
#include "ienginetool.h"
#include "ivengineclient.h"

class CInterfaces
{
public:
	class InterfaceReg
	{
	protected:
		typedef void* (__thiscall* tGetAddy)(void*);
		tGetAddy GetAddy;

	public:
		// the name of the interface
		const char* name;

		// a pointer to the next interface
		InterfaceReg* next;

		// gets a pointer to the interface
		void* GetAddress()
		{
			return GetAddy(this);
		}
	};

	IClientDLLSharedAppSystems* ClientDLLSharedAppSystems;
	IClientEntityList* ClientEntityList;
	IVDebugOverlay* DebugOverlay;
	CHLClient* Client;
	ClientModeShared* ClientModeShared;
	IVEngineClient* EngineClient;

	typedef ::ClientModeShared* (*tGetClientModeNormal)();
	tGetClientModeNormal GetClientModeNormal;

	IEngineTool* EngineTool;

	ILuaShared* LuaShared;

	CInterfaces()
	{
		// could find a good amount of interfaces in CHLClient::Init
		ClientDLLSharedAppSystems = CreateInterface<IClientDLLSharedAppSystems>("client.dll", "VClientDllSharedAppSystems001");
		ClientEntityList = CreateInterface<IClientEntityList>("client.dll", "VClientEntityList003");
		DebugOverlay = CreateInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay003");
		Client = CreateInterface<CHLClient>("client.dll", "VClient017");
		LuaShared = CreateInterface<ILuaShared>("lua_shared.dll", "LUASHARED003");
		EngineTool = CreateInterface<IEngineTool>("engine.dll", "VENGINETOOL003");
		EngineClient = CreateInterface<IVEngineClient>("engine.dll", "VEngineClient015");

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

		g_pDebug->Print("== %s ==\n", szMod);

		DWORD jmpoffset = (DWORD)((uintptr_t)pCreateInterface + 5);

		// + 4 for size of address/pointer
		uintptr_t pCreateInterfaceInternal = jmpoffset + *(DWORD*)jmpoffset + 4;

		// pointer to the first interface
		InterfaceReg* pInterfaceRegs = **(InterfaceReg***)(pCreateInterfaceInternal + 6);

		if (pInterfaceRegs)
		{
			for (InterfaceReg* pCur = pInterfaceRegs; pCur != nullptr; pCur = pCur->next)
			{
				g_pDebug->Print("%s (%p)\n", pCur->name, pCur->GetAddress());
			}
		}
		else
		{
			g_pDebug->Print("No interfaces found.\n");
		}

	}
};

extern CInterfaces* Interface;