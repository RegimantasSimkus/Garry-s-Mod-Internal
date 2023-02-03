#include "endscene.h"
#include "debug.h"

tEndScene oEndScene = nullptr;
HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9* pDevice)
{
	g_pDebug->Print("%s", "x");
	return oEndScene(pDevice);
}