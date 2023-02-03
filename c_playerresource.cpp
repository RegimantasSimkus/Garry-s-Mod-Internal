#include "c_playerresource.h"
#include "sigscan.h"

C_PlayerResource** g_PlayerResource = nullptr;

DWORD gameResourceOffset;
C_PlayerResource* PlayerResource()
{
	// not sure what this function actually does. its a __thiscall for something
	// am using it to retrieve the pointer to g_PlayerResource
	// 99% sure that (g_PlayerResource + 5000) is g_GameResource
	static DWORD playerResourceFunc = FindSignature("client.dll", "\x56\x8B\x35\x00\x00\x00\x00\x85\xF6\x74\x21", "xxx????xxxx");
	if (!g_PlayerResource)
	{
		g_PlayerResource = *(C_PlayerResource***)(playerResourceFunc + 3);
		gameResourceOffset = *(DWORD*)(playerResourceFunc + 19);
	}

	return *g_PlayerResource;
}

IGameResources* GameResource()
{
	C_PlayerResource* pr = PlayerResource();
	if (!pr)
		return nullptr;
	return (IGameResources*)(pr + gameResourceOffset);
}