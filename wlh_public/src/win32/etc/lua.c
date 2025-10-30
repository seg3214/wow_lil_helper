#include "../shared/main.h"

enum LuaConstant { LuaMultRet = -1, LuaTypeNil = 0, LuaTypeBoolean = 1, LuaTypeNumber = 3, LuaTypeString = 4 };

//Pops n elements from the stack.
#define lua_pop(L, n) lua_SetTop(L, -(n) - 1)

#pragma region typedefs lua
typedef void (*tfFrameScript__PushNumber)(uint32_t* pLuaState, double n);
tfFrameScript__PushNumber lua_PushNumber = (tfFrameScript__PushNumber)FrameScript__PushNumber;	//push double works
typedef void (*tfFrameScript__PushInteger)(uint32_t* pLuaState, int32_t n);
tfFrameScript__PushInteger lua_PushInteger = (tfFrameScript__PushInteger)FrameScript__PushInteger;  //works
typedef void (*tfFrameScript__PushString)(uint32_t* pLuaState, const char* str);
tfFrameScript__PushString lua_PushString = (tfFrameScript__PushString)FrameScript__PushString;	//works
typedef void (*tfFrameScript__PushNil)(uint32_t* pLuaState);
tfFrameScript__PushNil lua_PushNil = (tfFrameScript__PushNil)FrameScript__PushNil;  //works
typedef void (*tfFrameScript__PushBoolean)(uint32_t* pLuaState, int32_t b);
tfFrameScript__PushBoolean lua_PushBoolean = (tfFrameScript__PushBoolean)FrameScript__PushBoolean;  //works

typedef int32_t (*tfFrameScript__lua_Type)(uint32_t* pLuaState, int32_t index);
tfFrameScript__lua_Type lua_Type = (tfFrameScript__lua_Type)FrameScript__lua_Type;  //works

typedef double (*tfFrameScript__LuaToNumber)(uint32_t* pLuaState, int32_t index);
tfFrameScript__LuaToNumber lua_ToNumber = (tfFrameScript__LuaToNumber)FrameScript__LuaToNumber;	 //works
typedef int32_t (*tfFrameScript__LuaToInteger)(uint32_t* pLuaState, int32_t index);
tfFrameScript__LuaToInteger lua_ToInteger = (tfFrameScript__LuaToInteger)FrameScript__LuaToInteger;  //works
typedef int32_t (*tfFrameScript__LuaToBoolean)(uint32_t* pLuaState, int32_t index);
tfFrameScript__LuaToBoolean lua_ToBoolean = (tfFrameScript__LuaToBoolean)FrameScript__LuaToBoolean;  //works
typedef char* (*tfFrameScript__LuaToLString)(uint32_t* pLuaState, int32_t index, int32_t zero);
tfFrameScript__LuaToLString lua_ToLString = (tfFrameScript__LuaToLString)FrameScript__LuaToLString;  //works

typedef int (*tfFrameScript__IsString)(uint32_t* pLuaState, int32_t index);
tfFrameScript__IsString lua_IsString = (tfFrameScript__IsString)FrameScript__IsString;	//IS IT CONVERTABLE TO string(double integer or string). works
typedef int (*tfFrameScript__IsNumber)(uint32_t* pLuaState, int32_t index);
tfFrameScript__IsNumber lua_IsNumber = (tfFrameScript__IsNumber)FrameScript__IsNumber;	//IS IT CONVERTABLE TO number(double or integer). works

typedef uint32_t* (*tfGetLuaState)();
tfGetLuaState lua_GetLuaState = (tfGetLuaState)FrameScript__GetLuaState;  //works
typedef int32_t (*tfFrameScript__GetTop)(uint32_t* pLuaState);
tfFrameScript__GetTop lua_GetTop = (tfFrameScript__GetTop)FrameScript__GetTop;	//works
typedef void (*tfFrameScript__SetTop)(uint32_t* pLuaState, int32_t index);
tfFrameScript__SetTop lua_SetTop = (tfFrameScript__SetTop)FrameScript__SetTop;	//works

typedef int32_t (*tfFrameScript__Pcall)(uint32_t* pLuaState, int32_t nargs, int32_t nresults, int32_t errfunc);
tfFrameScript__Pcall lua_Pcall = (tfFrameScript__Pcall)FrameScript__Pcall;  //works
typedef int32_t (*tfFrameScript__LuaLoadBuffer)(uint32_t* pLuaState, char* buffer, int32_t bufferLength, char* chunkName);
tfFrameScript__LuaLoadBuffer lua_LuaLoadBuffer = (tfFrameScript__LuaLoadBuffer)FrameScript__LuaLoadBuffer;  //works
typedef int32_t (*tfFrameScript__SetGlobal)(uint32_t* pLuaState);
tfFrameScript__SetGlobal lua_SetGlobal = (tfFrameScript__SetGlobal)FrameScript__LuaSetGlobal;  //works

typedef int32_t (*tfFrameScript__GetGlobal)(uint32_t* pLuaState);
tfFrameScript__GetGlobal lua_GetGlobal = (tfFrameScript__GetGlobal)FrameScript__LuaGetGlobal;  //works
#pragma endregion

void SetGlobalVar(const char* varName, const char* varValue) {
	uint32_t* pLuaState = lua_GetLuaState();
	lua_PushString(pLuaState, varName);
	lua_PushString(pLuaState, varValue);
	lua_SetGlobal(pLuaState);
	lua_pop(pLuaState, 1);
}

char* GetGlobalVar_string(const char* varName) {
	uint32_t* pLuaState = lua_GetLuaState();
	lua_PushString(pLuaState, varName);
	lua_GetGlobal(pLuaState);
	char* rr = lua_ToLString(pLuaState, lua_GetTop(pLuaState), 0);
	lua_pop(pLuaState, 2);
	return rr;
}

char* Lua_HandleError(uint32_t* pLuaState) {
	char* p = lua_ToLString(pLuaState, 1, 0);
	if (!p)
		return "Unknown Error";
	lua_pop(pLuaState, 1);
	return p;
}

char* StackObjectToString2(uint32_t* pLuaState, int32_t index) {
	int32_t otype = lua_Type(pLuaState, index);
	static char buffer[50];
	switch (otype) {
		case LuaTypeNil:
			return "nil";
			break;
		case LuaTypeBoolean:
			return lua_ToBoolean(pLuaState, index) > 0 ? "true" : "false";
			break;
		case LuaTypeNumber:
			DoubleToString(lua_ToNumber(pLuaState, index), (char*)&buffer);
			return (char*)&buffer;
			break;
		case LuaTypeString:
			return lua_ToLString(pLuaState, index, 0);
			break;
		default:
			return "<unknown lua type>";
			break;
	}
}
double StackObjectToType(uint32_t* pLuaState, int32_t index) {
	int32_t otype = lua_Type(pLuaState, index);
	//static char buffer[50];
	switch (otype) {
		case LuaTypeNil:
			return 0;
			break;
		case LuaTypeBoolean:
			return lua_ToBoolean(pLuaState, index) > 0 ? 1 : 0;
			break;
		case LuaTypeNumber:
			return lua_ToNumber(pLuaState, index);
			break;
		case LuaTypeString:
			return (int32_t)lua_ToLString(pLuaState, index, 0);
			break;
		default:
			return 0xDEADBEEF;
			break;
	}
}
double* ExecuteInternal(char* query, uint32_t withResults) {  //returns array indexes 0-n
	char* nquery = 0;
	if (withResults) {
		char* wrs = "return ";
		int x = strlen(query) + strlen(wrs) + 1;
		nquery = (char*)malloc(x);
		memset(nquery, 0, sizeof(x));
		strcat(nquery, wrs);
		strcat(nquery, query);
		query = nquery;
	}
	static double fdf[25];
	memset(fdf, 0, sizeof(fdf));
	uint32_t* pLuaState = lua_GetLuaState();
	int32_t top = lua_GetTop(pLuaState);

	char* memory = (char*)malloc(strlen(query) + 1);
	memcpy(memory, query, strlen(query) + 1);
	memory[strlen(query)] = 0;

	if (lua_LuaLoadBuffer(pLuaState, memory, strlen(query), "cleanCore") > 0) {
		printf("lua_LuaLoadBuffer ERROR: %s\n", Lua_HandleError(pLuaState));
		return 0;
	}
	if (lua_Pcall(pLuaState, 0, withResults ? LuaMultRet : 0, 0) > 0) {
		printf("lua_Pcall ERROR: %s\n", Lua_HandleError(pLuaState));
		return 0;
	}
	int32_t returnValueCount = lua_GetTop(pLuaState) - top;
	for (int i = 1; i <= returnValueCount; i++) {
		fdf[i - 1] = StackObjectToType(pLuaState, i);
	}
	lua_pop(pLuaState, returnValueCount);
	free(memory);
	free(nquery);
	return (double*)&fdf;
}
char* luaResToString(double* result, int index) {
	return (char*)(uint32_t)result[index];
}
int64_t luaResToInteger(double* result, int index) {
	return (int64_t)result[index];
}
double luaResToDouble(double* result, int index) {
	return result[index];
}
