#pragma once

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
	DPT_String,
	DPT_Array,	// An array of the base types (can't be of datatables).
	DPT_DataTable,
#ifdef SUPPORTS_INT64
	DPT_Int64,
#endif

	DPT_NUMSendPropTypes

} SendPropType;

class RecvTable;
class RecvProp
{
public:
	const char* m_pVarName;
	SendPropType			m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;
	bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void*	m_ArrayLengthProxy;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;	// For RDT_DataTable.
	RecvTable* m_pDataTable;		// For RDT_DataTable.
	int						m_Offset;
	int						m_ElementStride;
	int						m_nElements;
	const char* m_pParentArrayPropName;
};


class RecvTable
{
public:

	typedef RecvProp	PropType;
public:
	RecvProp* m_pProps;
	int				m_nProps;
	void* m_pDecoder;
	const char* m_pNetTableName;	// The name matched between client and server.
	bool			m_bInitialized;
	bool			m_bInMainList;
};


class ClientClass
{
public:
	const char* GetName()
	{
		return m_pNetworkName;
	}

	void*		m_pCreateFn;
	void* m_pCreateEventFn;	// Only called for event objects.
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int						m_ClassID;	// Managed by the engine.
};