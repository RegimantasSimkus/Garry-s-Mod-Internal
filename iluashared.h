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

class ILuaInterface
{
	virtual void* __method_1();
	virtual void* __method_2();
	virtual void* __method_3();
	virtual void* __method_4();
	virtual void* __method_5();
	virtual void* __method_6();
	virtual void* __method_7();
	virtual void* __method_8();
	virtual void* __method_9();
	virtual void* __method_10();
	virtual void* __method_11();
	virtual void* __method_12();
	virtual void* __method_13();
	virtual void* __method_14();
	virtual void* __method_15();
	virtual void* __method_16();
	virtual void* __method_17();
	virtual void* __method_18();
	virtual void* __method_19();
	virtual void* __method_20();
	virtual void* __method_21();
	virtual void* __method_22();
	virtual void* __method_23();
	virtual void* __method_24();
	virtual void* __method_25();
	virtual void* __method_26();
	virtual void* __method_27();
	virtual void* __method_28();
	virtual void* __method_29();
	virtual void* __method_30();
	virtual void* __method_31();
	virtual void* __method_32();
	virtual void* __method_33();
	virtual void* __method_34();
	virtual void* __method_35();
	virtual void* __method_36();
	virtual void* __method_37();
	virtual void* __method_38();
	virtual void* __method_39();
	virtual void* __method_40();
	virtual void* __method_41();
	virtual void* __method_42();
	virtual void* __method_43();
	virtual void* __method_44();
	virtual void* __method_45();
	virtual void* __method_46();
	virtual void* __method_47();
	virtual void* __method_48();
	virtual void* __method_49();
	virtual void* __method_50();
	virtual void* __method_51();
	virtual void* __method_52();
	virtual void* __method_53();
	virtual void* __method_54();
	virtual void* __method_55();
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