#include "endscene.h"
#include "debug.h"

WNDPROC oWndProc = nullptr;
void Initialize(IDirect3DDevice9* pDevice)
{

    D3DDEVICE_CREATION_PARAMETERS params;
    pDevice->GetCreationParameters(&params);

    oWndProc = (WNDPROC)SetWindowLongPtr(params.hFocusWindow, GWLP_WNDPROC, (LONG)WndProc);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

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

    if (!isOpen)
        return oEndScene(pDevice);

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Window");
    if (ImGui::Button("Shutdown"))
    {
        g_bShutDown = true;
    }

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    if (g_bShutDown)
    {
        Shutdown(pDevice);
    }

	g_pDebug->Print("%s", "x");
	return oEndScene(pDevice);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (isOpen && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}