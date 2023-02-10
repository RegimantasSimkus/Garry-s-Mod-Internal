#pragma once
#include "c_baseentity.h"

class C_Weapon : public C_BaseEntity
{
public:
	int Clip1()
	{
		return *(int*)((uintptr_t)this + 0x1818);
	}
};