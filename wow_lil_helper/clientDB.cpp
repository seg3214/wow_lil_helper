#include "includes.h"


// int ClientDb_GetLocalizedRow(IntPtr instance, int index, IntPtr rowPtr);
typedef int32_t(__thiscall *tfGetLocalizedRow)(uint32_t*, int32_t, void*);
tfGetLocalizedRow fGetLocalizedRow = (tfGetLocalizedRow)constGetLocalizedRow;

struct clientDBTables{
	uint32_t *pBase;
	uint32_t id;
};

struct clientDBTables arClientDBTables[250];



void ReadClientDBTables()
{
	format_time(G_tstamp);
	printf("%s Reading client DB \n", G_tstamp);
	int i = 0;
	memset(arClientDBTables, 0, sizeof(arClientDBTables));
	for (byte* tableBase = (byte*)constRegisterBase;
		*tableBase != 0xC3;
		tableBase += 0x11){
		uint32_t id = *(uint32_t*)(tableBase + 1);
		uint32_t* tablePtr = (uint32_t*)(*(int32_t*)(tableBase + 0xxx) + 0xxx);

		arClientDBTables[i].pBase = tablePtr;
		arClientDBTables[i].id = id;

		i++;
	}
}

uint32_t* GetDBPointer(ClientDB id){
	//uint32_t *pDBTable = 0;
	for (int i = 0; i < sizeof(arClientDBTables) / sizeof(clientDBTables); i++){
		if (arClientDBTables[i].id == id)
		{
			//printf("DB %X %X\n", arClientDBTables[i].id, arClientDBTables[i].pBase);
			return arClientDBTables[i].pBase;
		}
	}
	printf("Cant find DB with ID: %X\n", id);
	return 0;
}

int32_t GetLocalizedRow(uint32_t* pDBTable, int32_t id, void* buffer){
	int32_t tmp = fGetLocalizedRow((uint32_t*)((uint32_t)pDBTable - 0xxx), id, buffer);
	if (tmp != 0)
	{
		return 1;
	}
	else{
		printf("Get Row Failed id:%X\n", id);
		return 0;
	}
}

void clientDB_Init(){
	ReadClientDBTables();
}

void DEBUG_print_ClientDBTables()
{
	for (int i = 0; i < sizeof(arClientDBTables) / sizeof(clientDBTables); i++){
		if (arClientDBTables[i].id != 0)
			printf("clientDB #%d id:%X @ %X\n", i, arClientDBTables[i].id, arClientDBTables[i].pBase);

	}
}


void clientDB_test(){
	//clientDB_Init();
	uint32_t *pDBTable = 0;
	//DEBUG_print_ClientDBTables();
	pDBTable = GetDBPointer(DBSpell);
	if (pDBTable == 0){
		printf("Exiting fGetSpellbookRow due to non existent client DB\n");
		return;
	}
	void *buffer = malloc(4 * 4 * 256);
	printf("buffer at %X \n",buffer);
	int32_t res = GetLocalizedRow(pDBTable, 7620, buffer);
	if (res == 0){
		printf("Get Row Failed");
		free(buffer);
		return;
	}
	
	struct SpellRec *sp;
	sp = (struct SpellRec*)buffer;
	printf("spell name %s \n",sp->SpellName);
	printf("spell id %X \n", sp->Id);
	printf("spell rank %s \n", sp->Rank);
	
	free(buffer);
	
}