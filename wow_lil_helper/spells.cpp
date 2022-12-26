#include "includes.h"



struct SpellIKnow arknownSpells[max_knownSpells];

void ReadKnownSpells(){
	uint32_t *pDBTable = 0;
	pDBTable = GetDBPointer(DBSpell);
	if (pDBTable == 0){
		printf("Exiting fGetSpellbookRow due to non existent client DB\n");
		return;
	}
	//printf("Reading known spells\n");
	//int spi = 0;
	struct SpellRec *buffer = (struct SpellRec*)malloc(4 * 4 * 256);
	memset(arknownSpells, 0, sizeof(arknownSpells));
	//struct SpellRec *sp;
	//sp = (struct SpellRec*)buffer;
	uint32_t spellcount = *((uint32_t*)SpellCount);
	//printf("spellcount=%d\n", spellcount);
	for (int i = 0; i < spellcount; i++){
		memset(buffer,0,4*4*256);
		uint32_t spellid = *(uint32_t*)(SpellBook + (i * 4));
		//printf("%d :spellid=%d\n",i, spellid);
		//printf("buffer at %X \n", buffer);
		//int32_t res = fGetSpellbookRow(spellid, buffer);
		int32_t res = GetLocalizedRow(pDBTable, spellid, buffer);
		if (res == 0){
			free(buffer);
			return;
		}
		//printf("i=%d ", i);
		arknownSpells[i].spellId = buffer->Id;
		arknownSpells[i].spellName = buffer->SpellName;
		arknownSpells[i].spellRank = buffer->Rank;
		//printf("%s \n", buffer->SpellName);
		//printf("spell id %X \n", buffer->Id);
		//printf("spell Rank %s \n", sp->Rank);
		//spi++;
		
	}
	free(buffer);
	//dump_to_file(&arknownSpells, sizeof(arknownSpells));
	}

char* GetSpellNameById(uint32_t spellId){
	for (int i = 0; i < sizeof(arknownSpells) / sizeof(SpellIKnow); i++){
		if (arknownSpells[i].spellId != 0)
			if (arknownSpells[i].spellId == spellId)
				return arknownSpells[i].spellName;
	}
	printf("Cant Find Spell GetSpellNameById()\n");
	return 0;
}

uint32_t GetSpellIdBySpellName(char *spellName){
	for (int i = 0; i < sizeof(arknownSpells) / sizeof(SpellIKnow); i++){
		if (arknownSpells[i].spellId != 0)
			if (strcmp(spellName, arknownSpells[i].spellName) == 0)
				return arknownSpells[i].spellId;
	}
	printf("Cant Find Spell GetSpellIdBySpellName()\n");
	return 0;
}

void CastSpellById(uint32_t spellId){
	//uint32_t spellId = 7620;
	(*fCastSpell)(spellId, 0, 0, 0, 0, 0, 0);
}

int32_t CastSpellByName(char *spellName){
	for (int i = 0; i < sizeof(arknownSpells) / sizeof(SpellIKnow); i++){
		if (arknownSpells[i].spellId != 0)
			if (strcmp(spellName, arknownSpells[i].spellName) == 0){
				CastSpellById(arknownSpells[i].spellId);
				return 1;
		}
	}
	printf("Cant find spell '%s'\n",spellName);
	return 0;
}

uint32_t GetChanneledCastingId(){
	return *(uint32_t*)(G_PlayerBase + constChanneledCastingId);
}
/////////////////////////////////////////////
void DEBUG_print_knownSpells()
{
	for (int i = 0; i < sizeof(arknownSpells) / sizeof(SpellIKnow); i++){
		if (arknownSpells[i].spellId != 0)
			printf("arknownSpells #%d id:%d  %s %s\n", i, arknownSpells[i].spellId, arknownSpells[i].spellName, arknownSpells[i].spellRank);

	}
}

void spells_test(){
	//ReadKnownSpells();
	CastSpellById(132);
	//CastSpellByName("Fel Armor");
	//printf("GetSpellNameById %s \n",GetSpellNameById(76220));
	//DEBUG_print_knownSpells();
}
