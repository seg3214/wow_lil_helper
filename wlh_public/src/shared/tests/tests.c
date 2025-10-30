#include <stdint.h>
#include <stdarg.h>
#include "../main.h"
////////////////////////////////////
#define BGROLES_TESTING
////////////////////////////////////

// #include "mocks/mocks.h"
// int(__cdecl* test_sir)(uint32_t* pObject, uint32_t* rejuv, uint32_t glast, uint32_t gfirst, uint32_t* bInRange, int zero1, int zero2) = 0x809610;
/*
 int __cdecl sub_819900(void *pCaster, struct SpellEntry *pSpellRow, WGUID guid, bool *bInRange, int, int)
int8_t test_sir2(uint32_t* pObject, uint32_t* rejuv, uint32_t GUIDLAST, uint32_t GUIDFIRST, uint32_t* statcishit, uint32_t zero1, uint32_t zero2)
{
	int8_t rr;
	__asm {
		push zero2
		push zero1
		push statcishit
		push GUIDFIRST
		push GUIDLAST
		push rejuv
		push pObject
		mov ecx, rejuv
		mov eax, 0x00809610
		call eax
		mov rr, al
	}
	return rr;
}

*/
static uint32_t casted;
void DumpObject_Binary(struct stObject2* x, char* filename) {
	FILE* pFile;
	if ((pFile = fopen(filename, "wb")) == NULL) {
		printf("The file DUMP file was not opened\n");

	} else {
		fwrite(x, sizeof(struct stObject2), 1, pFile);
	}
	fclose(pFile);
};
void DumpREADObject_Binary(struct stObject2* x, char* filename) {
	FILE* pFile;
	if ((pFile = fopen(filename, "rb")) == NULL) {
		printf("The file DUMP file was not opened\n");

	} else {
		fread(x, sizeof(struct stObject2), 1, pFile);
	}
	fclose(pFile);
};

void DumpREADObjectAuras_Binary(struct stObject2* x, char* filename) {
	FILE* pFile;
	if ((pFile = fopen(filename, "rb")) == NULL) {
		printf("The file DUMP file was not opened\n");

	} else {
		fread(&x->auras, sizeof(struct stAura), 100, pFile);
	}
	fclose(pFile);
};
void DumpObjectAuras_Binary(struct stObject2* x, char* filename) {
	FILE* pFile;
	if ((pFile = fopen(filename, "wb")) == NULL) {
		printf("The file DUMP file was not opened\n");

	} else {
		fwrite(&x->auras, sizeof(struct stAura), 100, pFile);
	}
	fclose(pFile);
};
void DumpObject_Readable(struct stObject2* x, char* filename) {
	FILE* pFile;
	if ((pFile = fopen(filename, "wb")) == NULL) {
		printf("The file DUMP file was not opened\n");

	} else {
		// sprintf(&st, "%s %d\r\n", n, *pUnitRace);
		// sprintf(&st, "%s \r\n", n);

		char header1[] = {"uint32_t,uint64_t*,uint32_t*,byte*,uint32_t*,uint32_t*,uint32_t,uint32_t,uint32_t,uint32_t,byte*,D3DXVECTOR3,uint32_t*,uint32_t*,float*,float*,float\n"};
		char header2[] = {"objBase,pGUID,pType,pUnitClass,pHealth,pMaxHealth,HealthPercent,Power,MaxPower,PowerPercent,PowerType,pos,CastingId,ChanneledCastingId,pCombatReach,pBoundingRadius,distanceToMe\n"};
		fwrite(&header1, strlen(header1), 1, pFile);
		fwrite(&header2, strlen(header2), 1, pFile);
		char s[1000] = {0};
		sprintf(&s, "%X,%llX,%X,%X,%X,%X,%X,%X,%X,%X,%X,%p,%X,%X,%f,%f,%f\n", x->objBase, *x->pGUID, *x->pType, *x->pUnitClass, *x->pHealth, *x->pMaxHealth, x->HealthPercent, x->Power, x->MaxPower, x->PowerPercent, *x->PowerType, &x->pos, *x->CastingId, *x->ChanneledCastingId, *x->pCombatReach, *x->pBoundingRadius, x->distanceToMe);
		fwrite(&s, strlen(s), 1, pFile);
	}
	fclose(pFile);
}

void test_pulse() {
	//printf("no %lu\n", PerformanceCounter());
	// BGRoles_Pulse();
#if defined WINDOWS

	// uint32_t objBase = (*fGetObjectByGuid)(0xB, -1);
	// printf("objbae %X\n", objBase);
	if ((GetAsyncKeyState(VK_F7) & 0x01)) {
		printf("test_pulse F7\n");
		// Dump_players();
#if defined BGROLES_TESTING
		BGRoles_Pulse(-1);
#endif
		// BGRoles_DumpToFile();
		// if (GUIDTarget) {
		// 	DumpObject_Binary(Target, "DUMP_paladin.bin");
		// }
		// struct stObject2 x = {0};
		// DumpREADObject_Binary(&x, "DUMP_paladin.bin");
	} else if ((GetAsyncKeyState(VK_F8) & 0x01)) {
		printf("test_pulse F8\n");
	}
#elif defined LINUX
	printf("pulse linux\n");
#if defined BGROLES_TESTING
	BGRoles_Pulse(-1);
#endif

#endif
	return;

	// if (!casted)
	// 	casted = PerformanceCounter();
	// const uint32_t wait = 200;
	//
	// uint32_t nowframe = PerformanceCounter();
	// uint32_t autoLengthMS = 500;
	//
	// //int lag = abs(PerformanceCounter() - casted);
	// //uint32_t lag = PerformanceCounter() - casted;
	// if ((nowframe - casted) > autoLengthMS)
	// //if (lag > wait)
	// {
	// 	casted = PerformanceCounter();
	// 	printf("tick \n");
	// }
	//else
	//printf("no %li\n", PerformanceCounter() - casted);
}
extern void as_war_bg_tank();

void test_script() {
#if defined WINDOWS
	if (!GUIDTarget) {
		printf("no target\n");
		return;
	}
	int r = DK_GetMyPetObjectBase();
	printf("test script %d\n", r);
	return;
	int st = IsSilenced(Target);
	// const int32_t* ObjectHeader = *(int32_t**)(Target->objBase + 0x8);
	// const uint32_t* UnitFlags = (uint32_t*)(ObjectHeader + UNIT_FIELD_FLAGS);
	// int st2 = 0;
	// if (*UnitFlags & UNIT_FLAG_STUNNED) {
	// 	st2 = 1;
	// }
	printf("stunned %X\n", st);
	return;
	int gse = GetSpellEntry(48441);
	if (!gse)
		printf("gse error \n");
	uint64_t guid = 0x0A00000000C23685;
	uint32_t gfirst = 0x0A000000;
	uint32_t glast = 0x00C23685;
	uint32_t inrange = 30;
	// char* spid = "Rejuvenation";
	// char* spid = "Faerie Fire";
	char* spid = "Claw";
	// float rrr = test_sir((uint32_t*)Myself->objBase, buffer_SpellEntry, glast, gfirst, &inrange, 0, 0);
	// float rrr = test_sir((uint32_t*)Myself->objBase, buffer_SpellEntry, guid, &inrange, 0, 0);
	// int rrr = test_sir3(spid, Target);
	// printf("rrr=0x%lX \n", rrr);
	return;
	uint8_t _HitTestBoundingModels = Traceline2(Myself, Target, HitTestBoundingModels);
	uint8_t _HitTestWMO = Traceline2(Myself, Target, HitTestWMO);
	uint8_t _HitTestUnknown = Traceline2(Myself, Target, HitTestUnknown);
	uint8_t _HitTestGround = Traceline2(Myself, Target, HitTestGround);
	uint8_t _HitTestLiquid = Traceline2(Myself, Target, HitTestLiquid);
	uint8_t _HitTestUnknown2 = Traceline2(Myself, Target, HitTestUnknown2);
	uint8_t _HitTestMovableObjects = Traceline2(Myself, Target, HitTestMovableObjects);
	printf("===========================\n"
	       "%X _HitTestBoundingModels \n"
	       "%X _HitTestWMO \n"
	       "%X _HitTestUnknown \n"
	       "%X _HitTestGround \n"
	       "%X _HitTestLiquid \n"
	       "%X _HitTestUnknown2 \n"
	       "%X _HitTestMovableObjects \n",
	       _HitTestBoundingModels, _HitTestWMO, _HitTestUnknown, _HitTestGround, _HitTestLiquid, _HitTestUnknown2, _HitTestMovableObjects);
	//if (!_HitTestWMO)
	//	CastSpellByName("Rejuvenation", Target);
	//	else
	//	printf("hit\n");
	return;
#endif
}
