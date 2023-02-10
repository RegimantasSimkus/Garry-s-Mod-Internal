#pragma once
#include "vector.h"
#include "iclientnetworkable.h"
#include "c_playerresource.h"

class C_GMOD_Player;
static C_GMOD_Player* (*GetLocalPlayer)(void);


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

	int EntIndex()
	{
		return this->GetClientNetworkable()->EntIndex();
	}

	const char* GetName()
	{
		return GameResource()->GetPlayerName(EntIndex());
	}

	int GetFrags()
	{
		return GameResource()->GetFrags(EntIndex());
	}

	int GetDeaths()
	{
		return GameResource()->GetDeaths(EntIndex());
	}

	int GetHealth()
	{
		return GameResource()->GetHealth(EntIndex());
	}

	int GetPing()
	{
		return GameResource()->GetPing(EntIndex());
	}

	int GetTeam()
	{
		return GameResource()->GetTeam(EntIndex());
	}

	const char* GetTeamName()
	{
		return GameResource()->GetTeamName(GetTeam());
	}

	bool IsLocalPlayer()
	{
		return (void*)this == (void*)GetLocalPlayer();
	}

	bool IsAlive()
	{
		return GameResource()->IsAlive(EntIndex());
	}

	bool IsFakePlayer()
	{
		return GameResource()->IsFakePlayer(EntIndex());
	}

	int GetScore()
	{
		return GameResource()->GetPlayerScore(EntIndex());
	}

	void* GetActiveWeapon()
	{
		using GetActiveWeapon = void* (__thiscall*)(void*);
		return ((GetActiveWeapon)( (*(void***)this)[279]) )(this);
	}
};