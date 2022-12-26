#pragma once

//(*fEnumVisibleObjects)(&enumVisObjCallback, 0);
typedef uint32_t(_cdecl *tfEnumVisibleObjects)(void* callback, int32_t filter);
//(*fGetObjectByGuid)(localPlayerGUID, -1);
typedef uint32_t* (_cdecl *tfGetObjectByGuid)(uint64_t guid, int32_t filter);

typedef int32_t(_cdecl *tfCastSpell)(uint32_t spellId, int32_t itemId, uint64_t guid, int32_t isTrade, int32_t a6, int32_t a7, int32_t a8);
//private delegate int CastSpellDelegate(
//	uint spellId, int itemId = 0, ulong guid = 0ul, int isTrade = 0, int a6 = 0, int a7 = 0, int a8 = 0);

extern tfEnumVisibleObjects fEnumVisibleObjects;
extern tfGetObjectByGuid fGetObjectByGuid;
extern tfCastSpell fCastSpell;


//Returns objects name from objects address. maybe same as char* CGUnit_C__GetObjectName_0(uint32_t* objAddress);
char * fGetObjectName(uint32_t* objectBase);
//
void fObjectInteract(uint32_t* objectBase);
//objectBase-object to check; unitToCompare-player;
int fUnitReaction(uint32_t* objectBase, uint32_t *unitToCompare);
//
int fHasAuraBySpellId(uint32_t* objectBase, int32_t spellId);
//Gets aura count on the object; auras are cached
int fGetAuraCount(uint32_t* objectBase);
//Gets Aura Base address by aura index; 0 if not found; function is not used currently
uint32_t *fGetAura(uint32_t* objectBase, int32_t index);
//Sets Auto loot 1-check; 0-uncheck
void ToggleAutoLoot(uint32_t flag);
//Gets AL value 1-enabled; 0-disabled;
uint32_t GetAutoLoot();
//
void SetFPS(int fps);
//
char* wrap_GetGlobalVar_string(const char *varName);
//
void wrap_SetGlobalVar(const char* varName, const char * varValue);

void test2(); //DEBUG DELETE AFTER