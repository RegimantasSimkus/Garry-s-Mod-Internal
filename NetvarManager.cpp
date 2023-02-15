#include "NetvarManager.h"
#include "Interfaces.h"



unsigned int CNetvarManager::FindOffset(const char* netvar, RecvTable* table)
{
	for (size_t i = 0; i < table->m_nProps; i++)
	{
		RecvProp prop = table->m_pProps[i];
		RecvTable* datatable = prop.m_pDataTable;

		g_pDebug->Print("prop name: %s (datatable: %s)\n", prop.m_pVarName, (datatable != nullptr && datatable->m_pNetTableName != nullptr) ? datatable->m_pNetTableName : "none");
		
		if (!strcmp(prop.m_pVarName, netvar))
		{
			return prop.m_Offset;
		}

		if (prop.m_pDataTable)
		{
			unsigned int offset = FindOffset(netvar, prop.m_pDataTable);
			if (offset)
			{
				return prop.m_Offset + offset;
			}
		}
	}
	return 0;
}

unsigned int CNetvarManager::GetNetvar(const char* table, const char* netvar)
{
	ClientClass* classes = Interface->Client->GetAllClasses();
	for (ClientClass* pClass = classes; pClass != nullptr; pClass = pClass->m_pNext)
	{
		g_pDebug->Print("Class network name: %s\n", pClass->m_pNetworkName);

		if (!strcmp(table, pClass->m_pNetworkName))
		{
			RecvTable* pTable = pClass->m_pRecvTable;
			return FindOffset(netvar, pTable);
		}
	}

	return 0;
}


void CNetvarManager::DumpClass(const char* table)
{
	ClientClass* classes = Interface->Client->GetAllClasses();
	for (ClientClass* pClass = classes; pClass != nullptr; pClass = pClass->m_pNext)
	{
		if (table == nullptr || !strcmp(table, pClass->m_pNetworkName))
		{
			DumpTable(table != nullptr ? table : pClass->m_pNetworkName, pClass->m_pRecvTable);
			if (table)
				break;
		}
	}
}

void CNetvarManager::DumpTable(const char* classname, RecvTable* table, int nested)
{
	for (size_t i = 0; i < table->m_nProps; i++)
	{
		RecvProp prop = table->m_pProps[i];
		g_pDebug->Print("%s%s\t%s\n", std::string(nested, '\t').c_str(), classname, prop.m_pVarName);
		if (prop.m_pDataTable)
			DumpTable(prop.m_pDataTable->m_pNetTableName, prop.m_pDataTable, nested + 1);
	}
}