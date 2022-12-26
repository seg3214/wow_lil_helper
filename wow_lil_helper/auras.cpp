#include "includes.h"


AuraRec ar_PlayerAuras[ar_PlayerObjectIndex_length*max_auras]; //AuraRec.CreatorGuid = 0  end of array

int32_t ar_HostilePlayerAuras[ar_PlayerObjectIndex_length][max_auras]; 
int32_t ar_FriendlyPlayerAuras[ar_PlayerObjectIndex_length][max_auras]; 

int fHasAuraBySpellName(uint32_t objectBase,char *SpellName){
	for (int i = 0; i < sizeof(arknownSpells) / sizeof(SpellIKnow); i++){
		if (arknownSpells[i].spellId != 0)
			if (strcmp(SpellName, arknownSpells[i].spellName) == 0){
				return fHasAuraBySpellId((uint32_t*)objectBase, arknownSpells[i].spellId);
			}
	}
	return 0;
}


void readObjectAuras(uint32_t objectBase, struct AuraRec (*auraBuffer)[max_auras]){ 
	uint32_t auraCount;
	uint32_t *auraTable; //change to not *
	int32_t result;
	

	auraCount = *(uint32_t *)(objectBase + constAURA_COUNT_1);
	if (auraCount == -1){
		auraCount = *(uint32_t *)(objectBase + constAURA_COUNT_2);
		auraTable = (uint32_t *)(objectBase + constAURA_TABLE_2);

	}
	else{
		auraTable = (uint32_t *)(objectBase + constAURA_TABLE_1);
	}
	int ac= 0;
	memset((*auraBuffer), 0xcc, sizeof(struct AuraRec)*max_auras);
	for (int i = 0; i < auraCount; i++){
		AuraRec *Aura = (AuraRec*)((uint32_t)auraTable + (constAURA_SIZE * i)); 
		if (!Aura->AuraId) continue;
		
		memcpy((*auraBuffer+ac), Aura, sizeof(AuraRec));
		
		ac++;
	}
}

void aura_pulse(){
		for (int i = 0; ar_Players[i].pstClientObject != 0; i++){
			readObjectAuras(ar_Players[i].pstClientObject->pBase, &ar_Players[i].arPlayerAuras);
		}
		auras_test();//del
}

////////////////////////////////////

int32_t fCGUnit_C__GetAura(uint32_t objectBase, uint32_t index){

	uint32_t v2;
	int32_t result;

	if (*(uint32_t *)(objectBase + constAURA_COUNT_1) == -1)
		v2 = *(uint32_t *)(objectBase + constAURA_COUNT_2);
	else
		v2 = *(uint32_t *)(objectBase + constAURA_COUNT_1);
	if (index >= v2)
	{
		result = 0;
	}
	else
	{
		if (*(uint32_t *)(objectBase + constAURA_COUNT_1) == 0xxx)
			result = *(uint32_t *)(objectBase + constAURA_TABLE_2) + 24 * index;
		else
			result = objectBase + 24 * index + constAURA_TABLE_1;
	}
	return result;

	//return (*ftocall)(objectBase);
}

void DEBUG_print_objectAuras( ){
	printf("%d\n", pMyselfstPlayerObject->arPlayerAuras[0].AuraId);
	printf("%X\n", pMyselfstPlayerObject->arPlayerAuras->CreatorGuid);
	printf("%X\n", pMyselfstPlayerObject->arPlayerAuras->StackCount);
	printf("2 %d\n", pMyselfstPlayerObject->arPlayerAuras[1].AuraId);
}

void auras_test(){

	
}

