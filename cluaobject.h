#pragma once

class CLuaObject
{
public:	
	// pushes the object onto the stack
	int Push()
	{
		return ((int(__thiscall*)(void*))((*(void***)this)[33]))(this);
	}
};