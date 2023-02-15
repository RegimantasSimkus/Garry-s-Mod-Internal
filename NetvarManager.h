#pragma once
#include "clientclass.h"

class CNetvarManager
{
public:
	unsigned int FindOffset(const char* netvar, RecvTable* table);
	unsigned int GetNetvar(const char* table, const char* netvar);
	void DumpClass(const char* classname);
	void DumpTable(const char* classname, RecvTable* table, int nested=0);
};