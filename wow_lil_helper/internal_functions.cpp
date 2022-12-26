#include "includes.h"



typedef char *(__thiscall *tfGetObjectName)(uint32_t*);
typedef char *(__thiscall *tfObjectInteract)(uint32_t*);
typedef int (__thiscall *tfUnitReaction)(uint32_t*, uint32_t*);
typedef int (__thiscall *tfHasAuraBySpellId)(uint32_t*, int32_t);
typedef int (__thiscall *tfGetAuraCount)(uint32_t*);
typedef uint32_t *(__thiscall *tfGetAura)(uint32_t*, int32_t);



tfEnumVisibleObjects fEnumVisibleObjects = (tfEnumVisibleObjects)EnumVisibleObjects;
tfGetObjectByGuid fGetObjectByGuid = (tfGetObjectByGuid)GetObjectByGuid;
tfCastSpell fCastSpell = (tfCastSpell)CastSpell;


int enumVisObjCallback(uint64_t guid, uint32_t filter){ //another way to get visible objects
	printf("guid %I64X\n", guid);
	return 1;
}


char * fGetObjectName(uint32_t* objectBase){
	uint32_t pfunc = *(uint32_t*)(*objectBase + 4 * constGetObjectName);
	tfGetObjectName ftocall = (tfGetObjectName)pfunc;
	return (*ftocall)(objectBase);
}

void fObjectInteract(uint32_t* objectBase){
	uint32_t pfunc = *(uint32_t*)(*objectBase + 4 * constObjectInteract);
	tfObjectInteract ftocall = (tfObjectInteract)pfunc;
	(*ftocall)(objectBase);
}

int fUnitReaction(uint32_t* objectBase, uint32_t *unitToCompare){
	tfUnitReaction ftocall = (tfUnitReaction)constUnitReaction;
	return (*ftocall)(objectBase, unitToCompare);
}

int fGetAuraCount(uint32_t* objectBase){
	tfGetAuraCount ftocall = (tfGetAuraCount)constGetAuraCount;
	return (*ftocall)(objectBase);
}

uint32_t *fGetAura(uint32_t* objectBase, int32_t index){
	tfGetAura ftocall = (tfGetAura)constGetAura;
	return (*ftocall)(objectBase, index);
}

int fHasAuraBySpellId(uint32_t* objectBase, int32_t spellId){
	tfHasAuraBySpellId ftocall = (tfHasAuraBySpellId)constHasAuraBySpellId;
	return (*ftocall)(objectBase, spellId);
}

void ToggleAutoLoot(uint32_t flag){
	uint32_t pp=*(uint32_t*)constAutoLoot_Activate_Pointer;
	uint32_t* pALValue=(uint32_t*) (pp + constAutoLoot_Activate_Offset);
	if (flag == 1) *pALValue = 0x1;
	else if (flag == 0) *pALValue = 0x0;
}

uint32_t GetAutoLoot(){
	uint32_t pp = *(uint32_t*)constAutoLoot_Activate_Pointer;
	uint32_t* pALValue = (uint32_t*)(pp + constAutoLoot_Activate_Offset);
	return *pALValue;
}

void SetFPS(int fps){
	*(uint32_t*)pGFX_MaxFPS = fps;
}

