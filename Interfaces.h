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

	struct ModuleInterfaceReg
	{
		InterfaceReg* pInterfaceReg;
		const char* szModule;
		ModuleInterfaceReg* pNext;
	};

	IClientDLLSharedAppSystems* ClientDLLSharedAppSystems;
	IClientEntityList* ClientEntityList;
	IVDebugOverlay* DebugOverlay;
	CHLClient* Client;
	ClientModeShared* ClientModeShared;
	IVEngineClient* EngineClient;
	IEngineTool* EngineTool;
	ILuaShared* LuaShared;

	typedef ::ClientModeShared* (__stdcall* tGetClientModeNormal)();
	tGetClientModeNormal GetClientModeNormal;

	ModuleInterfaceReg* InterfaceRegs;

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
		uintptr_t pCallGetClientModeNormal = ((uintptr_t)((*(void***)Client)[50]) +
#ifndef _WIN64
			4
#else
			17
#endif
			);
		g_pDebug->Print("pCallGetClientModeNormal: %p\n", pCallGetClientModeNormal);
		// relative address + instruction address + 4 size of address = function address
		uintptr_t pGetClientModeNormal = pCallGetClientModeNormal + (*(int*)pCallGetClientModeNormal) + 4;
		g_pDebug->Print("pGetClientModeNormal: %p\n", pGetClientModeNormal);

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

		uintptr_t jmpoffset = (uintptr_t)((uintptr_t)pCreateInterface + 
#ifdef _WIN64
			1
#else
			5
#endif
			);

		// + 4 for size of address/pointer
		uintptr_t pCreateInterfaceInternal = jmpoffset + *(int*)jmpoffset + 4;


		// pointer to the first interface

#ifndef _WIN64
		InterfaceReg* pInterfaceRegs = **(InterfaceReg***)(pCreateInterfaceInternal + 6);
#else
		uintptr_t mov = pCreateInterfaceInternal + 18;
		InterfaceReg* pInterfaceRegs = *(InterfaceReg**)(mov + 4 + *(int*)mov);
#endif
		

#ifdef _WIN64
		g_pDebug->Print("pInterfaceRegs -> %p\n", pInterfaceRegs);
#endif

		bool found = false;

		ModuleInterfaceReg* modReg = InterfaceRegs;
		ModuleInterfaceReg* pLast = modReg;
		if (modReg)
		{
			for (modReg = InterfaceRegs; modReg != nullptr; modReg = modReg->pNext)
			{
				pLast = modReg;
				if (!strcmp(modReg->szModule, szMod))
				{
					found = true;
					break;
				}

				if (!modReg->pNext)
					break;
			}
		}

		if (!found)
		{
			ModuleInterfaceReg* reg = new ModuleInterfaceReg();
			reg->szModule = szMod;
			reg->pInterfaceReg = pInterfaceRegs;

			if (InterfaceRegs == nullptr)
				InterfaceRegs = reg;
			else if (pLast)
				pLast->pNext = reg;
		}

//#ifdef _WIN64
//		if (pInterfaceRegs)
//		{
//			for (InterfaceReg* pCur = pInterfaceRegs; pCur != nullptr; pCur = pCur->next)
//			{
//				g_pDebug->Print("%s (%p)\n", pCur->name, pCur->GetAddress());
//			}
//		}
//		else
//		{
//			g_pDebug->Print("No interfaces found.\n");
//		}
//#endif

	}
};

extern CInterfaces* Interface;