#include "includes.h"

//Pops n elements from the stack. 
#define lua_pop(L,n)  lua_settop(L, -(n)-1)

// lua funcs
UINT32* GetLuaState() {
	UINT32* rr;
	__asm {
		mov eax, 0xxx
			call eax
			mov rr, eax
	}
	return rr;
}

void lua_settop(UINT32* pLuaState, INT32 index) {
	__asm {
		mov eax, index
			push eax
			mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 8
	}
}

void FrameScript__PushString(UINT32 *pLuaState, const char *strtp) {
	//void* cmdcave;
	//cmdcave = calloc(strlen(str) + 1, 1);
	//memcpy(cmdcave, str, strlen(str) + 1);
	//UINT32* pls;
	//pls =(UINT32*) calloc(1, sizeof(UINT32));
	//memcpy(pls, pLuaState, sizeof(UINT32));
	__asm {
		//mov eax, str
		push strtp
			//mov eax, pLuaState
			push pLuaState
			mov eax, 0xxx
			call eax
			add esp, 8
	}
}

void lua_getglobal(UINT32* pLuaState) {
	__asm {
		mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 4
	}
}

void lua_setglobal(UINT32* pLuaState) {
	__asm {
		mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 4
	}
}

INT32 FrameScript_ToBoolean(UINT32 * pLuaState, INT32 index) {
	INT32 rr;
	__asm {
		push 0
			mov eax, index
			push eax
			mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 0xC
			mov rr, eax
	}
	return rr;
}

char* FrameScript_ToLString(UINT32 * pLuaState, INT32 index) {
	char* rr;
	__asm {
		push 0
			mov eax, index
			push eax
			mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 0xC
			mov rr, eax
	}
	return rr;
}

INT32 lua_gettop(UINT32* pLuaState) {
	INT32 rr;
	__asm {
		mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 4
			mov rr, eax
	}
	return rr;
}

INT32 FrameScript_IsNumber(UINT32* pLuaState, INT32 index) {
	INT32  rr;
	__asm {
		mov eax, index
			push eax
			mov eax, pLuaState
			push eax
			mov eax, 0xxx
			call eax
			add esp, 8
			mov rr, eax
	}
	return rr;
}

//end lua funcs

char* wrap_GetGlobalVar_string(const char *varName) {
	UINT32* pLuaState = GetLuaState();
	FrameScript__PushString(pLuaState, varName);
	lua_getglobal(pLuaState);
	char* rr = FrameScript_ToLString(pLuaState, lua_gettop(pLuaState));
	lua_pop(pLuaState, 2);
	return rr;
}

void wrap_SetGlobalVar(const char* varName, const char * varValue) {
	UINT32* pLuaState = GetLuaState();
	FrameScript__PushString(pLuaState, varName);
	FrameScript__PushString(pLuaState, varValue);
	lua_setglobal(pLuaState);
	lua_pop(pLuaState, 1);
}