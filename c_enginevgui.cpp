#include "c_enginevgui.h"
#include "sigscan.h"

typedef CEngineVGui* (*tEngineVGui)();
tEngineVGui pEngineVGui = nullptr;
CEngineVGui* EngineVGui()
{
	static DWORD startupLua = FindSignature("client.dll", "\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x53\x68\x00\x00\x00\x00\x8B\xD9", "xxxxx????xx????xx");
	if (pEngineVGui)
		return pEngineVGui();

	DWORD call = startupLua + 27;
	pEngineVGui = (tEngineVGui)(call + *(int*)(call)+4);
	return pEngineVGui();
}

void CEngineVGui::PrintConsole(float something, const wchar_t* message)
{
	typedef void(__thiscall* tFunc)(void*, float, const wchar_t*);
	tFunc func = (tFunc)(*(DWORD*)this + 108);
	return func(this, something, message);
}