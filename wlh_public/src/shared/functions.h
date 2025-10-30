#pragma once

#pragma region client functions

// uint32_t fGetObjectByGuid(uint64_t guid, int32_t filter) ;
extern uint32_t(__cdecl* fGetObjectByGuid)(uint64_t guid, int32_t filter);
extern int32_t(__cdecl* fGetSpellCooldown)(uint32_t spellId, int32_t isPet, int* duration, int* start, int32_t* isEnabled, int* unk0);
extern int32_t(__cdecl* fCastSpell)(uint32_t spellId, int32_t itemId, uint64_t guid, int32_t isTrade, int32_t a6, int32_t a7, int32_t a8);
extern uint32_t(__cdecl* PerformanceCounter)();
extern byte(__cdecl* fTraceline)(D3DXVECTOR3* start, D3DXVECTOR3* end, D3DXVECTOR3* result, float* distanceTravelled, uint32_t flags, uint32_t zero);

//
// uint32_t(__cdecl* fGetObjectByGuid)(uint64_t guid, int32_t filter) ;
// int32_t(__cdecl* fGetSpellCooldown)(uint32_t spellId, int32_t isPet, int* duration, int* start, int32_t* isEnabled, int* unk0) ;
// int32_t(__cdecl* fCastSpell)(uint32_t spellId, int32_t itemId, uint64_t guid, int32_t isTrade, int32_t a6, int32_t a7, int32_t a8) ;
// uint32_t(__cdecl* PerformanceCounter)() ;
// byte(__cdecl* fTraceline)(D3DXVECTOR3* start, D3DXVECTOR3* end, D3DXVECTOR3* result, float* distanceTravelled, uint32_t flags, uint32_t zero) ;
//
void CGUnit_C__OnSetRawFacingLocal(uint32_t objectBase, uint32_t time, float facing);
char* fGetObjectName(uint32_t* objAddress);
void fUseItem(uint32_t* itembase, uint64_t* targetguid);
int fUnitReaction(uint32_t objectBase, uint32_t unitToCompare);
int32_t fGetLocalizedRow(uint32_t* pDBTable, int32_t index, void* buffer);
int32_t fGetAuraCount(uint32_t objBase);
uint32_t fGetAura(uint32_t objBase, int32_t index);
void fGetObjectLocation(uint32_t* objAddress, D3DXVECTOR3* location);
float CGUnit_C_GetHeight(uint32_t* pObjBase);
byte Traceline(D3DXVECTOR3* start, D3DXVECTOR3* end);
uint8_t Traceline2(struct stObject2* pstObj, struct stObject2* pstObj2, enum CGWorldFrameHitFlags flags);
uint8_t LineOfSight(struct stObject2* pstObj, struct stObject2* pstObj2);
char* fObjectInteract(uint32_t* objAddress);
#pragma endregion

#pragma region other functions
byte GetByte(uint32_t val, uint32_t byteIndex);
int CountBits(uint32_t x);
float distanceTwoVectors(D3DXVECTOR3* v1, D3DXVECTOR3* v2);
void SetFacing(uint32_t objectBase, float angle);
void LookAt(D3DXVECTOR3 loc);
void LookAway(D3DXVECTOR3 loc);
#pragma endregion
