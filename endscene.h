#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

typedef HRESULT(__stdcall* tEndScene)(LPDIRECT3DDEVICE9* pDevice);
extern tEndScene oEndScene;

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9* pDevice);