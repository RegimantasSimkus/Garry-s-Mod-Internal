#pragma once
#include "vector.h"
#include "iclientnetworkable.h"


class C_GMOD_Player
{
private:
	virtual void* func0();
	virtual void* func1();
	virtual void* func2();
public:
	virtual void* GetCollideable();
	virtual IClientNetworkable* GetClientNetworkable();
	virtual void* GetClientRenderable();
private:
	virtual void* GetThis0();
	virtual void* GetThis1();
public:
	virtual void* GetClientThinkable();
	virtual Vector& GetABSOrigin();
	virtual Vector& GetABSAngles();

	const char* GetName()
	{
		return (const char*)((DWORD)this + 0x30A4);
	}
};

static C_GMOD_Player*(*GetLocalPlayer)(void);