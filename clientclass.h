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
	// This info comes from the receive data table.
public:
	RecvProp();

	void					InitArray(int nElements, int elementStride);

	int						GetNumElements() const;
	void					SetNumElements(int nElements);

	int						GetElementStride() const;
	void					SetElementStride(int stride);

	int						GetFlags() const;

	const char* GetName() const;
	void* GetType() const;

	RecvTable* GetDataTable() const;
	void					SetDataTable(RecvTable* pTable);

	void* GetProxyFn() const;
	void					SetProxyFn(void* fn);

	void* GetDataTableProxyFn() const;
	void					SetDataTableProxyFn(void* fn);

	int						GetOffset() const;
	void					SetOffset(int o);

	// Arrays only.
	RecvProp* GetArrayProp() const;
	void					SetArrayProp(RecvProp* pProp);

	// Arrays only.
	void					SetArrayLengthProxy(void* proxy);
	void* GetArrayLengthProxy() const;

	bool					IsInsideArray() const;
	void					SetInsideArray();

	// Some property types bind more data to the prop in here.
	const void* GetExtraData() const;
	void				SetExtraData(const void* pData);

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* GetParentArrayPropName();
	void				SetParentArrayPropName(const char* pArrayPropName);

public:

	const char* m_pVarName;
	SendPropType			m_RecvType;
	int						m_Flags;
	int						m_StringBufferSize;


private:

	bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

	// Extra data that certain special property types bind to the property here.
	const void* m_pExtraData;

	// If this is an array (DPT_Array).
	RecvProp* m_pArrayProp;
	void*	m_ArrayLengthProxy;

	void* m_ProxyFn;
	void* m_DataTableProxyFn;	// For RDT_DataTable.

	RecvTable* m_pDataTable;		// For RDT_DataTable.
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	// If it's one of the numbered "000", "001", etc properties in an array, then
	// these can be used to get its array property name for debugging.
	const char* m_pParentArrayPropName;
};


class RecvTable
{
public:

	typedef RecvProp	PropType;

	RecvTable();
	RecvTable(RecvProp* pProps, int nProps, const char* pNetTableName);
	~RecvTable();

	void		Construct(RecvProp* pProps, int nProps, const char* pNetTableName);

	int			GetNumProps();
	RecvProp* GetProp(int i);

	const char* GetName();

	// Used by the engine while initializing array props.
	void		SetInitialized(bool bInitialized);
	bool		IsInitialized() const;

	// Used by the engine.
	void		SetInMainList(bool bInList);
	bool		IsInMainList() const;


public:

	// Properties described in a table.
	RecvProp* m_pProps;
	int				m_nProps;

	// The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
	// will have their own decoders that include props for all their children).
	void* m_pDecoder;

	const char* m_pNetTableName;	// The name matched between client and server.


private:

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