#include "endscene.h"
#include "debug.h"
#include "Interfaces.h"
#include "globals.h"
#include "c_gmod_player.h"
#include "drawing.h"

CDrawingWrapper* draw;

WNDPROC oWndProc = nullptr;
void Initialize(IDirect3DDevice9* pDevice)
{

    D3DDEVICE_CREATION_PARAMETERS params;
    pDevice->GetCreationParameters(&params);

    oWndProc = (WNDPROC)SetWindowLongPtr(params.hFocusWindow, GWLP_WNDPROC, (LONG)WndProc);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.MouseDrawCursor = false;

    ImGui_ImplDX9_Init(pDevice);
    ImGui_ImplWin32_Init(params.hFocusWindow);
}

void Shutdown(IDirect3DDevice9* pDevice)
{
    D3DDEVICE_CREATION_PARAMETERS params;
    pDevice->GetCreationParameters(&params);

    SetWindowLongPtr(params.hFocusWindow, GWLP_WNDPROC, (LONG)oWndProc);

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
}

bool isOpen = true;
tEndScene oEndScene = nullptr;
HRESULT __stdcall hkEndScene(IDirect3DDevice9* pDevice)
{
    if (g_bShutDown)
        return oEndScene(pDevice);

    static bool init = false;
    if (!init)
    {
        Initialize(pDevice);
        init = true;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImDrawList* list = ImGui::GetBackgroundDrawList();
    if (!draw)
        draw = new CDrawingWrapper(list);
    if (!draw->DrawList)
        draw->DrawList = list;

    for (int i = 1; i <= g_pGlobals->maxClients; i++)
    {
        C_GMOD_Player* ply = (C_GMOD_Player*)Interface->ClientEntityList->GetClientEntity(i);
        if (!ply)
            continue;

        if (ply->GetClientNetworkable()->IsDormant())
            continue;

        draw->DrawTextScreenA(ply->GetName(), ply->GetABSOrigin(), ImColor(255, 255, 255), TEXT_ALIGN_CENTER, TEXT_ALIGN_CENTER);
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) isOpen = !isOpen;
    if (isOpen)
    {
        ImGui::Begin("Window", &isOpen);
        if (ImGui::Button("Shutdown"))
        {
            g_bShutDown = true;
        }

        CInterfaces::ModuleInterfaceReg* regs = Interface->InterfaceRegs;
        for (CInterfaces::ModuleInterfaceReg* reg = regs; reg != nullptr; reg = reg->pNext)
        {
            if (ImGui::CollapsingHeader(reg->szModule))
            {
                for (CInterfaces::InterfaceReg* pInterface = reg->pInterfaceReg; pInterface != nullptr; pInterface = pInterface->next)
                {
                    ImGui::Text("%s", pInterface->name);
                }
            }
        }

        ImGui::End();
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    if (g_bShutDown)
    {
        Shutdown(pDevice);
    }

	return oEndScene(pDevice);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (isOpen && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}