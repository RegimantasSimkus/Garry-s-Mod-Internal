#include "endscene.h"
#include "debug.h"
#include "Interfaces.h"
#include "globals.h"
#include "c_gmod_player.h"
#include "drawing.h"
#include <string>

CDrawingWrapper* draw;

WNDPROC oWndProc = nullptr;
void Initialize(IDirect3DDevice9* pDevice)
{

    D3DDEVICE_CREATION_PARAMETERS params;
    pDevice->GetCreationParameters(&params);

    g_pDebug->Print("HWND: %p\n", params.hFocusWindow);
    oWndProc = (WNDPROC)SetWindowLongPtr(params.hFocusWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

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

    SetWindowLongPtr(params.hFocusWindow, GWLP_WNDPROC, (LONG_PTR)oWndProc);

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
}

#define SCRIPT_LENGTH 1028

void DrawGLuaExecutor()
{
    static char script[SCRIPT_LENGTH];

    ImGui::Begin("gLua Executor");
    
    ImGui::InputTextMultiline("##gLua_Exec_Code", script, SCRIPT_LENGTH);

    if (ImGui::Button("Execute"))
    {
        ILuaInterface* lua = Interface->LuaShared->GetInterface(LUA_STATE_CLIENT);
        if (lua)
        {
            for (int i = 0; i < g_pGlobals->maxClients; i++)
            {
                C_GMOD_Player* ply = (C_GMOD_Player*)Interface->ClientEntityList->GetClientEntity(i);
                if (!ply) continue;
                CLuaObject* tbl = ply->GetLuaTable();
                
                tbl->Push();
                lua->GetField(-1, "PrivateVariables");
                if (lua->GetType(-1) != lua->Table)
                {
                    lua->Pop(2);
                    continue;
                }

                lua->PushNil();
                while (lua->Next(-2))
                {
                    if (lua->GetType(-2) == lua->String)
                    {
                        const char* key = lua->GetString(-2);

                        if (lua->GetType(-1) == lua->Number)
                        {
                            int num = lua->GetNumber(-1);
                            g_pDebug->Print("%s - [%s] = %d\n", ply->GetName(), key, num);
                        }
                    }


                    lua->Pop();
                }
                lua->Pop(4);
            }
        }
    }

    ImGui::End();
}

bool isOpen = false;
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
    if (draw->DrawList != list)
        draw->DrawList = list;

    for (int i = 1; i <= g_pGlobals->maxClients; i++)
    {
        C_GMOD_Player* ply = (C_GMOD_Player*)Interface->ClientEntityList->GetClientEntity(i);
        if (!ply)
            continue;

        if (ply->GetClientNetworkable()->IsDormant())
            continue;

        float x, y;
        std::string name = ply->GetName();
        name.append(" " + std::to_string(ply->GetHealth()));

        draw->DrawTextScreenA(name.c_str(), ply->GetABSOrigin(), ImColor(255, 255, 255), TEXT_ALIGN_CENTER, TEXT_ALIGN_CENTER, &x, &y);

        C_Weapon* weapon = ply->GetActiveWeapon();
        if (weapon)
        {
            draw->DrawTextA(weapon->GetClassname(), ImVec2(x, y + 2), ImColor(255, 255, 255), TEXT_ALIGN_CENTER, TEXT_ALIGN_TOP);
        }
    }

    if (GetAsyncKeyState(VK_INSERT) & 1) isOpen = !isOpen;
    if (isOpen)
    {
        static bool glua_executor = false;
        if (glua_executor) DrawGLuaExecutor();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("gLua Executor", nullptr, &glua_executor);

                ImGui::EndMenu();
            }


            ImGui::EndMainMenuBar();
        }

         ImGui::Begin("Window", &isOpen);
        if (ImGui::Button("Close") || GetAsyncKeyState(VK_END) & 1)
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
    if (isOpen)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
        return true;
    }

    return CallWindowProcA(oWndProc, hWnd, msg, wParam, lParam);
}