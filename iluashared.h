#pragma once

enum
{
	SPECIAL_GLOB,		// Global table
	SPECIAL_ENV,		// Environment table
	SPECIAL_REG,		// Registry table
};

enum lua_state_t : unsigned char
{
	LUA_STATE_CLIENT,
	LUA_STATE_SERVER,
	LUA_STATE_MENU
};

class ILuaInterface;

struct lua_State
{
	unsigned char garbage[70];

	ILuaInterface* luabase;
};

class ILuaInterface
{
public:
	struct UserData
	{
		void* data;
		unsigned char type; // Change me to a uint32 one day
	};

	typedef int (*CFunc)(lua_State* L);

	enum
	{
#ifdef GMOD_ALLOW_DEPRECATED
		// Deprecated: Use `None` instead of `Invalid`
		Invalid = -1,
#endif

		// Default Lua Types
		None = -1,
		Nil,
		Bool,
		LightUserData,
		Number,
		String,
		Table,
		Function,
		UserData,
		Thread,

		// GMod Types
		Entity,
		Vec,
		Angle,
		PhysObj,
		Save,
		Restore,
		DamageInfo,
		EffectData,
		MoveData,
		RecipientFilter,
		UserCmd,
		ScriptedVehicle,
		Material,
		Panel,
		Particle,
		ParticleEmitter,
		Texture,
		UserMsg,
		ConVar,
		IMesh,
		Matrix,
		Sound,
		PixelVisHandle,
		DLight,
		Video,
		File,
		Locomotion,
		Path,
		NavArea,
		SoundHandle,
		NavLadder,
		ParticleSystem,
		ProjectedTexture,
		PhysCollide,
		SurfaceInfo,

		Type_Count
	};

	virtual int         Top(void) = 0;
	virtual void        Push(int iStackPos) = 0;
	virtual void        Pop(int iAmt = 1) = 0;
	virtual void        GetTable(int iStackPos) = 0;
	virtual void        GetField(int iStackPos, const char* strName) = 0;
	virtual void        SetField(int iStackPos, const char* strName) = 0;
	virtual void        CreateTable() = 0;
	virtual void        SetTable(int iStackPos) = 0;
	virtual void        SetMetaTable(int iStackPos) = 0;
	virtual bool        GetMetaTable(int i) = 0;
	virtual void        Call(int iArgs, int iResults) = 0;
	virtual int         PCall(int iArgs, int iResults, int iErrorFunc) = 0;
	virtual int         Equal(int iA, int iB) = 0;
	virtual int         RawEqual(int iA, int iB) = 0;
	virtual void        Insert(int iStackPos) = 0;
	virtual void        Remove(int iStackPos) = 0;
	virtual int         Next(int iStackPos) = 0;
	virtual void* NewUserdata(unsigned int iSize) = 0;
	virtual void        ThrowError(const char* strError) = 0;
	virtual void        CheckType(int iStackPos, int iType) = 0;
	virtual void        ArgError(int iArgNum, const char* strMessage) = 0;
	virtual void        RawGet(int iStackPos) = 0;
	virtual void        RawSet(int iStackPos) = 0;
	virtual const char* GetString(int iStackPos = -1, unsigned int* iOutLen = NULL) = 0;
	virtual double      GetNumber(int iStackPos = -1) = 0;
	virtual bool        GetBool(int iStackPos = -1) = 0;
	virtual CFunc       GetCFunction(int iStackPos = -1) = 0;
	virtual void* GetUserdata(int iStackPos = -1) = 0;
	virtual void        PushNil() = 0;
	virtual void        PushString(const char* val, unsigned int iLen = 0) = 0;
	virtual void        PushNumber(double val) = 0;
	virtual void        PushBool(bool val) = 0;
	virtual void        PushCFunction(CFunc val) = 0;
	virtual void        PushCClosure(CFunc val, int iVars) = 0;
	virtual void        PushUserdata(void*) = 0;
	virtual int         ReferenceCreate() = 0;
	virtual void        ReferenceFree(int i) = 0;
	virtual void        ReferencePush(int i) = 0;
	virtual void        PushSpecial(int iType) = 0;
	virtual bool        IsType(int iStackPos, int iType) = 0;
	virtual int         GetType(int iStackPos) = 0;
	virtual const char* GetTypeName(int iType) = 0;
	virtual void        CreateMetaTableType(const char* strName, int iType) = 0;
	virtual const char* CheckString(int iStackPos = -1) = 0;
	virtual double      CheckNumber(int iStackPos = -1) = 0;
	virtual int         ObjLen(int iStackPos = -1) = 0;
	virtual const QAngle& GetAngle(int iStackPos = -1) = 0;
	virtual const Vector& GetVector(int iStackPos = -1) = 0;
	virtual void        PushAngle(const QAngle& val) = 0;
	virtual void        PushVector(const Vector& val) = 0;
	virtual void        SetState(lua_State* L) = 0;
	virtual int         CreateMetaTable(const char* strName) = 0;
	virtual bool        PushMetaTable(int iType) = 0;
	virtual void        PushUserType(void* data, int iType) = 0;
	virtual void        SetUserType(int iStackPos, void* data) = 0;
private:
	virtual void* __method_56();
	virtual void* __method_57();
	virtual void* __method_58();
	virtual void* __method_59();
	virtual void* __method_60();
	virtual void* __method_61();
	virtual void* __method_62();
	virtual void* __method_63();
	virtual void* __method_64();
	virtual void* __method_65();
	virtual void* __method_66();
	virtual void* __method_67();
	virtual void* __method_68();
	virtual void* __method_69();
	virtual void* __method_70();
	virtual void* __method_71();
	virtual void* __method_72();
	virtual void* __method_73();
	virtual void* __method_74();
	virtual void* __method_75();
	virtual void* __method_76();
	virtual void* __method_77();
	virtual void* __method_78();
	virtual void* __method_79();
	virtual void* __method_80();
	virtual void* __method_81();
	virtual void* __method_82();
	virtual void* __method_83();
	virtual void* __method_84();
	virtual void* __method_85();
	virtual void* __method_86();
	virtual void* __method_87();
	virtual void* __method_88();
	virtual void* __method_89();
	virtual void* __method_90();
	virtual void* __method_91();
	virtual void* __method_92();
	virtual void* __method_93();
	virtual void* __method_94();
	virtual void* __method_95();
	virtual void* __method_96();
	virtual void* __method_97();
	virtual void* __method_98();
	virtual void* __method_99();
	virtual void* __method_100();
	virtual void* __method_101();
	virtual void* __method_102();
	virtual void* __method_103();
	virtual void* __method_104();
	virtual void* __method_105();
	virtual void* __method_106();
	virtual void* __method_107();
	virtual void* __method_108();
	virtual void* __method_109();
	virtual void* __method_110();
	virtual void* __method_111();
public:
	virtual bool RunString(const char* scriptname, const char* source, const char* code, bool a6, bool a7, bool a8, bool a9);
};

class ILuaShared
{
public:
	virtual ~ILuaShared();
	virtual void Init();
	virtual void Shutdown();
	virtual int PrintStats();

	// stores the interface in this + state + 23
	virtual ILuaInterface* CreateInterface(lua_state_t state, int unused);

	// seems that it invalidates pointers to the interface
	// returns a int(__stdcall***)(char) ??
	virtual void* DestroyInterface(ILuaInterface* pInterface);

	// returns this + state + 23
	virtual ILuaInterface* GetInterface(lua_state_t state);

	// in create interface
	// this + <state enum> + 23 = pointer to created interface
	/*ILuaShared* GetInterface(lua_state_t state)
	{
		return (ILuaShared*)(this + state + 23);
	}*/
};