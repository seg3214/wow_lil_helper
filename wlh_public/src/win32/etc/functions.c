#include "../shared/main.h"

extern const uint32_t constUnitReaction;
extern const uint32_t constGetObjectName;
extern const uint32_t constGetLocalizedRow;
extern const uint32_t constGetObjectLocation;
extern const uint32_t constUseItem;
extern const uint32_t constGetAuraCount;
extern const uint32_t constGetAura;
extern const uint32_t constCGUnit_C__OnSetRawFacingLocal;

extern uint32_t(__cdecl* fGetObjectByGuid)(uint64_t guid, int32_t filter);
extern int32_t(__cdecl* fGetSpellCooldown)(uint32_t spellId, int32_t isPet, int* duration, int* start, int32_t* isEnabled, int* unk0);
extern int32_t(__cdecl* fCastSpell)(uint32_t spellId, int32_t itemId, uint64_t guid, int32_t isTrade, int32_t a6, int32_t a7, int32_t a8);
extern uint32_t(__cdecl* PerformanceCounter)();
extern byte(__cdecl* fTraceline)(D3DXVECTOR3* start, D3DXVECTOR3* end, D3DXVECTOR3* result, float* distanceTravelled, uint32_t flags, uint32_t zero);

const uint32_t constObjectInteract = 44;

//client functions
void CGUnit_C__OnSetRawFacingLocal(uint32_t objectBase, uint32_t time, float facing) {
	__asm {
		push facing;
		push time;
		mov ecx, objectBase;
		mov eax, constCGUnit_C__OnSetRawFacingLocal;
		call eax;
	}
}
char* fGetObjectName(uint32_t* objAddress) {
	uint32_t pfunc = *(uint32_t*)(*objAddress + 4 * constGetObjectName);
	char* rr;
	__asm {
		mov ecx, objAddress;
		mov eax, pfunc;
		call eax;
		mov rr, eax;
	}
	return rr;
}
void fUseItem(uint32_t* itembase, uint64_t* targetguid) {
	__asm {
		push 0;
		push targetguid;
		mov ecx, itembase;
		mov eax, constUseItem;
		call eax;
	}
}
int fUnitReaction(uint32_t objectBase, uint32_t unitToCompare) {
	int rr;
	__asm {
		push unitToCompare;
		mov ecx, objectBase;
		mov eax, constUnitReaction;
		call eax;
		mov rr, eax;
	}
	return rr;
}
int32_t fGetLocalizedRow(uint32_t* pDBTable, int32_t index, void* buffer) {
	int32_t rr;
	__asm {
		push buffer;
		push index;
		mov ecx, pDBTable;
		mov eax, constGetLocalizedRow;
		call eax;
		mov rr, eax;
	}
	return rr;
}
int32_t fGetAuraCount(uint32_t objBase) {
	int32_t rr;
	__asm {
		mov ecx, objBase;
		mov eax, constGetAuraCount;
		call eax;
		mov rr, eax;
	}
	return rr;
}
uint32_t fGetAura(uint32_t objBase, int32_t index) {
	int32_t rr;
	__asm {
		push index;
		mov ecx, objBase;
		mov eax, constGetAura;
		call eax;
		mov rr, eax;
	}
	return rr;
}
void fGetObjectLocation(uint32_t* objAddress, D3DXVECTOR3* location) {
	uint32_t pfunc = *(uint32_t*)(*objAddress + 4 * constGetObjectLocation);
	__asm {
		push location;
		mov ecx, objAddress;
		mov eax, pfunc;
		call eax;
	}
}
byte Traceline(D3DXVECTOR3* start, D3DXVECTOR3* end) {
	float distanceTravelled = 1.0f;
	D3DXVECTOR3 result;

	return (*fTraceline)(start, end, &result, &distanceTravelled, 0x100011, 0);
}
float CGUnit_C_GetHeight(uint32_t* pObjBase) {
	if (!pObjBase)
		return 0.0f;
	return *(float*)((char*)pObjBase + 0x854);
}
uint8_t Traceline2(struct stObject2* pstObj, struct stObject2* pstObj2, enum CGWorldFrameHitFlags flags) {
	float distanceTravelled = 1.0f;
	D3DXVECTOR3 result;
	D3DXVECTOR3 objpos1 = pstObj->pos;
	D3DXVECTOR3 objpos2 = pstObj2->pos;

	float oh1 = CGUnit_C_GetHeight((uint32_t*)pstObj->objBase);
	float oh2 = CGUnit_C_GetHeight((uint32_t*)pstObj2->objBase);

	objpos1.z += oh1;
	objpos2.z += oh2;

	return (*fTraceline)(&objpos1, &objpos2, &result, &distanceTravelled, flags, 0);
}
uint8_t LineOfSight(struct stObject2* pstObj, struct stObject2* pstObj2) {
	if (Traceline2(pstObj, pstObj2, (HitTestBoundingModels | HitTestWMO | HitTestMovableObjects)))
		return 0;
	return 1;
}
char* fObjectInteract(uint32_t* objAddress) {
	uint32_t pfunc = *(uint32_t*)(*objAddress + 4 * constObjectInteract);
	char* rr;
	__asm {
		mov ecx, objAddress;
		mov eax, pfunc;
		call eax;
		mov rr, eax;
	}
	return rr;
}

//other functions
byte GetByte(uint32_t val, uint32_t byteIndex) {  //index 1-4 from the right
	if (!(byteIndex > 0 && byteIndex < 5))
		return 0;
	//int z=(byteIndex - 4)*-1;
	return (val >> 8 * (byteIndex - 1)) & 0xFF;
}
int CountBits(uint32_t x) {
	int count = 0;
	int r = 0;
	while (x) {
		count += x & 1;
		x >>= 1;
	}
	return count;
}
float distanceTwoVectors(D3DXVECTOR3* v1, D3DXVECTOR3* v2) {
	return sqrt(pow((float)(v1->x - v2->x), 2) + pow((float)(v1->y - v2->y), 2) + pow((float)(v1->z - v2->z), 2));
}
void SetFacing(uint32_t objectBase, float angle) {
	const float pi2 = (float)(PI * 2);
	if (angle < 0.0f)
		angle += pi2;
	if (angle > pi2)
		angle -= pi2;
	CGUnit_C__OnSetRawFacingLocal(objectBase, PerformanceCounter(), angle);
}
void LookAt(D3DXVECTOR3 loc) {
	D3DXVECTOR3 local = Myself->pos;
	D3DXVECTOR3 diffVector = {loc.x - local.x, loc.y - local.y, loc.z - local.z};
	float an = atan2(diffVector.y, diffVector.x);
	SetFacing(Myself->objBase, an);
}
void LookAway(D3DXVECTOR3 loc) {
	D3DXVECTOR3 local = Myself->pos;
	D3DXVECTOR3 diffVector = {loc.x - local.x, loc.y - local.y, loc.z - local.z};
	float an = atan2(diffVector.y, diffVector.x) + PI;
	SetFacing(Myself->objBase, an);
}
