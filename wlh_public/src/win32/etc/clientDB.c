#include "../shared/main.h"

extern const uint32_t constRegisterBase;
extern const uint32_t SpellCount;
extern const uint32_t SpellBook;
uint32_t* pDBSpellTable = 0;
uint32_t* pDBSkillTable = 0;
struct SpellEntry* buffer_SpellEntry = 0;
struct SkillLineEntry* buffer_SkillLineEntry = 0;
struct stSpellIKnow arKnownSpells[szKnownSpells];
struct clientDBTables arClientDBTables[250];

extern int32_t fGetLocalizedRow(uint32_t* pDBTable, int32_t index, void* buffer);
extern double* ExecuteInternal(char* query, uint32_t withResults);  //returns array indexes 0-n
extern void SetGlobalVar(const char* varName, const char* varValue);
extern char* GetGlobalVar_string(const char* varName);
extern double luaResToDouble(double* result, int index);
extern int64_t luaResToInteger(double* result, int index);

void ReadClientDBTables() {
	printf("%s Reading client DB \n", format_time());
	int i = 0;
	memset(arClientDBTables, 0, sizeof(arClientDBTables));
	for (byte* tableBase = (byte*)constRegisterBase; *tableBase != 0xC3; tableBase += 0x11) {
		uint32_t id = *(uint32_t*)(tableBase + 1);
		//uint32_t tablePtr=(*(int32_t*)(tableBase + 0xB) + 0x18);
		/*if (i < 1){
        byte* tablePtr1 = tableBase + 0xB;
        printf("tablePtr1 %X \n", tablePtr1);
        uint32_t *tablePtr2 = (uint32_t*)(tableBase + 0xB);
        printf("tablePtr2 %X \n", tablePtr2);
        uint32_t* tablePtr3 = (uint32_t*)(*(int32_t*)(tableBase + 0xB) + 0x18);
        printf("tablePtr3 %X \n", tablePtr3);
        }*/
		//uint32_t* tablePtr = (uint32_t*)(*(int32_t*)(tableBase + 0xB) + 0x18);
		uint32_t* tablePtr = *(uint32_t**)(tableBase + 0xB);

		//printf("tablePtr %X \n", tablePtr);
		arClientDBTables[i].pBase = tablePtr;  //pointer to table header
		arClientDBTables[i].id = id;
		/*if (arClientDBTables[i].id == DBSpell){
        struct WoWClientDB *dbh = (struct WoWClientDB*)tablePtr;
        uint32_t* fishrec=GetRow(dbh, 7620); //get pointer to record under index of 7620
        uint32_t* afterfishrec = GetRow(dbh, 7621);
        dump_to_file(fishrec, (afterfishrec - fishrec)*4); //dump "fishing" record . its memory structure not DBC file structure.
        //printf("fishrec %X\n", fishrec);
        //printf("afterfishrec %X\n", afterfishrec);
        //printf("- %X\n", (afterfishrec - fishrec)*4);
        //printf("numRows %d minIndex %d maxIndex %d FirstRow %d\n", arClientDBTables[i].header.numRows, arClientDBTables[i].header.minIndex, arClientDBTables[i].header.maxIndex, arClientDBTables[i].header.FirstRow);
        printf("numRows %d minIndex %d maxIndex %d FirstRow %d\n", dbh->numRows, dbh->minIndex, dbh->maxIndex, dbh->FirstRow);
        }*/
		//printf("tablePtr %X   \n", tablePtr);
		//if (index == DBAreaTrigger) printf("here %X\n", id);
		i++;
	}
}

uint32_t* GetDBPointer(uint32_t id) {
	for (int i = 0; i < sizeof(arClientDBTables) / sizeof(struct clientDBTables); i++) {
		if (arClientDBTables[i].id == id) {
			//printf("DB %X %X\n", arClientDBTables[i].id, arClientDBTables[i].pBase);
			return arClientDBTables[i].pBase;
		}
	}
	printf("Cant find DB with ID: %X\n", id);
	return 0;
}

int GetSpellEntry(int32_t Id) {	 //returns 0 on error; 1 on success. data in buffer_SpellEntry
	memset(buffer_SpellEntry, 0, 4 * 4 * 256);
	int res = fGetLocalizedRow(pDBSpellTable, Id, buffer_SpellEntry);
	if (!res) {
		//printf("%s Get Row Failed id: %d\n", __FUNCTION__,Id);
		return 0;
	}
	return 1;
}

void ReadKnownSpells() {
	if (!pDBSpellTable)
		pDBSpellTable = GetDBPointer(DBSpell);
	if (!buffer_SpellEntry)
		buffer_SpellEntry = (struct SpellEntry*)malloc(4 * 4 * 256);  //why this size? ~680 sizeof(struct SpellEntry)

	memset(arKnownSpells, 0, sizeof(arKnownSpells));
	uint32_t spellcount = *((uint32_t*)SpellCount);
	for (int i = 0; i < spellcount; i++) {
		memset(buffer_SpellEntry, 0, 4 * 4 * 256);
		uint32_t spellid = *(uint32_t*)(SpellBook + (i * 4));
		int res = fGetLocalizedRow(pDBSpellTable, spellid, buffer_SpellEntry);
		if (!res) {
			printf("%s Get Row Failed id: %d\n", __FUNCTION__, spellid);
			return;
		}

		arKnownSpells[i].spellId = buffer_SpellEntry->Id;
		arKnownSpells[i].spellName = buffer_SpellEntry->SpellName;
		arKnownSpells[i].spellRank = buffer_SpellEntry->Rank;

		char st[100] = {0};
		sprintf(&st, "GetSpellInfo(%d)", buffer_SpellEntry->Id);
		double* resei = ExecuteInternal(&st, 1);

		arKnownSpells[i].Attributes = buffer_SpellEntry->Attributes;
		arKnownSpells[i].AttributesEx = buffer_SpellEntry->AttributesEx;
		arKnownSpells[i].DmgClass = buffer_SpellEntry->DmgClass;
		arKnownSpells[i].PowerCost = luaResToInteger(resei, 3);
		arKnownSpells[i].IsHealthFunnel = luaResToInteger(resei, 4);
		arKnownSpells[i].CastTime = luaResToInteger(resei, 6);
		arKnownSpells[i].MinRange = luaResToInteger(resei, 7);
		arKnownSpells[i].MaxRange = luaResToInteger(resei, 8);
		arKnownSpells[i].Mechanic = buffer_SpellEntry->Mechanic;
		arKnownSpells[i].TriggersGCD = buffer_SpellEntry->StartRecoveryTime > 0 && buffer_SpellEntry->StartRecoveryTime <= 1500;

		//sprintf(&st,"GetSpellInfo(%d)",62124);
		//char* (*zzdf)[50] = (char* (*)[50])ExecuteInternal(&st, 1);
		//		for (int i = 0; (*zzdf)[i] != 0; i++){
		//printf("THIS %s\n\n", (*zzdf)[i]);
		//	}
		//		if ((*zzdf)[3])
		//		arKnownSpells[i].PowerCost = atoi((*zzdf)[3]);

		//printf("%s id: %d\n", buffer_SpellEntry->SpellName, buffer_SpellEntry->Id);
	}
	//dump_to_file(buffer_SpellEntry,sizeof(struct SpellEntry));/////////
}
