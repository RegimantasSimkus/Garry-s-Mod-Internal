#pragma once
#include "vector.h"
#include "sigscan.h"
#include "iclientnetworkable.h"
#include "cluaobject.h"

class C_Collideable
{
public:
	// returns the owner of the collisions (pEntity)
	virtual void* GetEntity();

	virtual Vector& GetMins();
	virtual Vector& GetMaxs();

private:
	virtual Vector& GetMins_Again(); // ?
	virtual Vector& GetMaxs_Again();
public:

	void* pThis;
	void* pEntity;

	// the min bound
	Vector mins;

	// the max bound
	Vector maxs;


};

class C_BaseEntity
{
private:
	virtual void* func0();
	virtual void* func1();
	virtual void* func2();
public:
	virtual C_Collideable* GetCollideable();
	virtual IClientNetworkable* GetClientNetworkable();
	virtual void* GetClientRenderable();

	const char* GetClassname()
	{
#ifdef _WIN64
		static uintptr_t call = FindSignature("client.dll", "\x40\x57\x48\x83\xEC\x50\x80\xB9\x00\x00\x00\x00\x00\x4C\x8B\xCA", "xxxxxxxx?????xxx") + 0xB9 + 1;
		static uintptr_t func = call + *(int*)call + 4;
#else
		static DWORD func = FindSignature("client.dll", "\x53\x8B\xD9\x8B\x83\x00\x00\x00\x00\x85\xC0\x74\x11", "xxxxx????xxxx");
#endif
		return ((const char* (
#ifdef _WIN64
			__fastcall
#else
			__thiscall
#endif
			*)(void*))(func))(this);
	}

	int PushEntity()
	{
		return ((int(__thiscall*)(void*))((*(void***)(this))[172]))(this);
	}

	CLuaObject* GetLuaTable()
	{
		return ((CLuaObject*(__thiscall*)(void*))((*(void***)(this))[188]))(this);
	}
};
