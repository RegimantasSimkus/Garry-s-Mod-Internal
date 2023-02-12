#include "c_weapon.h"
#include "Interfaces.h"

const char* C_Weapon::GetLuaBase()
{
    ILuaInterface* lua = Interface->LuaShared->GetInterface(LUA_STATE_CLIENT);

    const char* base = "";
    if (!lua)
        return base;

    CLuaObject* obj = GetLuaTable();

    obj->Push();
    lua->GetField(-1, "Base");

    if (lua->GetType(-1) == lua->String)
    {
        base = lua->GetString(-1);
    }

    lua->Pop(2);
    return base;
}