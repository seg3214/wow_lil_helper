#include "../../shared/main.h"

struct stObject2 stMyself, stTarget, stFocus, stPet;
const struct stObject2 *Myself = &stMyself, *Target = &stTarget, *Focus = &stFocus, *Pet = &stPet;
extern const uint32_t ar_pet_dkghouls_ids[];
extern const uint32_t ar_pet_exclusions_ids[];

uint32_t(__cdecl* fEnumVisibleObjects)(void* callback, int32_t filter) = constEnumVisibleObjects;

int dump_count;
uint32_t arDumpObjects[dump_size];

int pf_count;
uint32_t arPlayersFriendly[pf_size];

int ph_count;
uint32_t arPlayersHostile[ph_size];

int pph_count;
uint32_t arPlayerPetHostile[pph_size];

int ppf_count;
uint32_t arPlayerPetFriendly[ppf_size];

int vh_count;
uint32_t arVehicleHostile[vh_size];

int vf_count;
uint32_t arVehicleFriendly[vf_size];

int th_count;
uint32_t arTotemHostile[th_size];

int nf_count;
uint32_t arNPCFriendly[nf_size];

int ch_count;
uint32_t arCreatureHostile[ch_size];  //aka mobs

int i_count;
uint32_t arItems[i_size];

int go_count;
uint32_t arGOs[go_size];

int mdkp_count;
uint32_t arMyDKPets[mdkp_size];

int td_count;
uint32_t arTDs[td_size];  //training dummy
int initObject(uint32_t objBase, struct stObject2* stObj) {
	if (!objBase) {
		printf("error null pointer %s():line %d\n", __FUNCTION__, __LINE__);
		return 0;
	}
	const int32_t* ObjectHeader = *(int32_t**)(objBase + 0x8);
	stObj->objBase = objBase;
	stObj->pGUID = (uint64_t*)(ObjectHeader + OBJECT_FIELD_GUID);
	stObj->pType = (uint32_t*)(ObjectHeader + OBJECT_FIELD_TYPE);
	stObj->pUnitClass = ((byte*)(ObjectHeader + UNIT_FIELD_BYTES_0) + 1);
	stObj->pHealth = (uint32_t*)(ObjectHeader + UNIT_FIELD_HEALTH);
	stObj->pMaxHealth = (uint32_t*)(ObjectHeader + UNIT_FIELD_MAXHEALTH);
	stObj->HealthPercent = roundl((*stObj->pHealth / (double)*stObj->pMaxHealth) * 100);

	stObj->PowerType = ((byte*)(ObjectHeader + UNIT_FIELD_BYTES_0) + 3);
	stObj->Power = *((uint32_t*)(ObjectHeader + UNIT_FIELD_POWER1) + *stObj->PowerType);
	stObj->MaxPower = *((uint32_t*)(ObjectHeader + UNIT_FIELD_MAXPOWER1) + *stObj->PowerType);
	if (*stObj->PowerType == 1) {  //rage
		stObj->Power = round((double)stObj->Power / 10);
		stObj->MaxPower = round((double)stObj->MaxPower / 10);
	} else if (*stObj->PowerType == 6)  //runicpower
	{
		stObj->Power = round((double)stObj->Power / 10);
		stObj->MaxPower = round((double)stObj->MaxPower / 10);
	}
	stObj->PowerPercent = roundl((stObj->Power / (double)stObj->MaxPower) * 100);
	fGetObjectLocation((uint32_t*)objBase, &stObj->pos);
	//stObj->pHeight= (float*)((char*)pObjBase + 0x854);
	stObj->CastingId = objBase + constCastingId;
	stObj->ChanneledCastingId = objBase + constChanneledCastingId;
	stObj->pCombatReach = (float*)(ObjectHeader + UNIT_FIELD_COMBATREACH);
	stObj->pBoundingRadius = (float*)(ObjectHeader + UNIT_FIELD_BOUNDINGRADIUS);

	stObj->distanceToMe = distanceTwoVectors(&Myself->pos, &stObj->pos);  //double reading with player in drawing

#pragma region reading auras 2
	const int ac = fGetAuraCount(objBase);
	//printf("fGetAuraCount %d\n",ac);
	int aa = 0;  //added aura count
	for (int i = 0; i < ac; i++) {
		struct AuraRec* a = fGetAura(objBase, i);
		if (!a)
			printf("not a %d\n", i);
		if (!a->AuraId)
			continue;
		//if (!GetSpellEntry(a->AuraId)){
		//	printf("error cant find row %s():line %d\n", __FUNCTION__, __LINE__);
		//	}
		if (!GetSpellEntry(a->AuraId))
			continue;
		stObj->auras[aa].pAuraId = &a->AuraId;
		stObj->auras[aa].pCreatorGuid = &a->CreatorGuid;
		stObj->auras[aa].pFlags = &a->Flags;
		stObj->auras[aa].pStackCount = &a->StackCount;
		if (buffer_SpellEntry->SpellName)
			stObj->auras[aa].pAuraName = buffer_SpellEntry->SpellName;
		else
			stObj->auras[aa].pAuraName = " ";
		if (a->EndTime)
			stObj->auras[aa].RemainingSec = (a->EndTime - PerformanceCounter()) / 1000;
		stObj->auras[aa].Mechanic = buffer_SpellEntry->Mechanic;
		stObj->auras[aa].DispelType = buffer_SpellEntry->Dispel;
		//	printf("auras %s fl: %X\n",stObj->auras[aa].pAuraName,*stObj->auras[aa].pFlags);

		aa++;
	}

	return 1;

#pragma endregion
}
int enumVisObjCallback(uint64_t guid, uint32_t filter) {  //another way to get visible objects
	uint32_t objBase = (*fGetObjectByGuid)(guid, -1);
	if (!objBase)
		return 1;
	int32_t* ObjectHeader = *(int32_t**)(objBase + 0x8);
	uint32_t* type = (uint32_t*)(ObjectHeader + OBJECT_FIELD_TYPE);
	//dumping
	if (!(*type & TYPEMASK_ITEM) && !(*type & TYPEMASK_CONTAINER) && (*type & TYPEMASK_UNIT)) {
		if (dump_count > dump_size) {
			printf("object list size exceeded dump_size %d", dump_size);
			return 1;
		}
		arDumpObjects[dump_count] = objBase;
		dump_count++;
	}
	if (*type & TYPEMASK_UNIT) {
		if (CanAttack(objBase)) {  // unit hostile

			if ((*type & TYPEMASK_PLAYER) && (!ifUnitPet(objBase)) && (!StrContainsSubString(fGetObjectName((uint32_t*)objBase), "Totem")))	 //player hostile
			{
				byte* UnitClass = ((byte*)(ObjectHeader + UNIT_FIELD_BYTES_0) + 1);
				if (*UnitClass == ClassPriest && GetShapeshiftForm(objBase) == FORM_SPIRITOFREDEMPTION)
					return 1;
				if (ph_count > ph_size) {
					printf("object list size exceeded ph_size %d", ph_size);
					return 1;
				}
				arPlayersHostile[ph_count] = objBase;
				ph_count++;
				return 1;
			}

			//if (GetUnitTypeFromGUID(guid) == UnitTypeFromGuidPet){ //pet hostile
			if (ifUnitPet(objBase)) {  //pet hostile

				if ((ifUnitTotem(objBase)) || (StrContainsSubString(fGetObjectName((uint32_t*)objBase), "Totem"))  //wowcircle has different UnitNativeDisplayID for totems
				) {												   //totem and pet
					//printf("guid passed %llX %X %s \n",guid,GetUnitTypeFromGUID(guid),fGetObjectName((uint32_t*)objBase));
					if (th_count > th_size) {
						printf("object list size exceeded th_size %d", th_size);
						return 1;
					}
					arTotemHostile[th_count] = objBase;
					th_count++;
					return 1;
				} else	// not totem and is pet
				{
					int excl_f = 0;
					for (int i = 0; ar_pet_exclusions_ids[i] != 0; i++) {
						const uint32_t* UnitNativeDisplayID = (uint32_t*)(ObjectHeader + UNIT_FIELD_NATIVEDISPLAYID);
						if (*UnitNativeDisplayID == ar_pet_exclusions_ids[i]) {
							//printf("click %X\n",obj->UnitNativeDisplayID);
							excl_f = 1;
						}
					}
					//uint32_t *UnitNativeDisplayID= (uint32_t*)(ObjectHeader + UNIT_FIELD_NATIVEDISPLAYID);
					//printf("guid passed %llX %X %s id:%X\n",guid,GetUnitTypeFromGUID(guid),fGetObjectName(pBase),*UnitNativeDisplayID);
					//printf("guid passed %d\n",excl_f );
					if (excl_f)
						return 1;

					//printf("adding %X\n",obj->UnitNativeDisplayID);
					if (pph_count > pph_size) {
						printf("object list size exceeded pph_size %d", pph_size);
						return 1;
					}
					arPlayerPetHostile[pph_count] = objBase;
					pph_count++;
					return 1;
				}
			}
			if (GetUnitTypeFromGUID(guid) == UnitTypeFromGuidVehicle) {  //vehicle hostile
				if (vh_count > vh_size) {
					printf("object list size exceeded vh_size %d", vh_size);
					return 1;
				}
				arVehicleHostile[vh_count] = objBase;
				vh_count++;
				return 1;
			}
			if (GetUnitTypeFromGUID(guid) == UnitTypeFromGuidUnit) {  //
				if (ch_count > ch_size) {
					printf("object list size exceeded ch_size %d", ch_size);
					return 1;
				}
				arCreatureHostile[ch_count] = objBase;
				ch_count++;
				return 1;
			}
			//

			const uint32_t Training_Dummy = 0x3ECA;
			const uint32_t* UnitNativeDisplayID = (uint32_t*)(ObjectHeader + UNIT_FIELD_NATIVEDISPLAYID);
			if (*UnitNativeDisplayID == Training_Dummy) {
				if (td_count > td_size) {
					printf("object list size exceeded td_size %d", td_size);
					return 1;
				}
				arTDs[td_count] = objBase;
				td_count++;
				return 1;
			}

			//
		} else {  // unit friendly
			if (fUnitCanAssist((uint32_t*)Myself->objBase, (uint32_t*)objBase, 0)) {
				// printf("unit fr %s %X mypet: %X\n", fGetObjectName((uint32_t*)objBase), *UnitNativeDisplayID, ttt);
				if (*type & TYPEMASK_PLAYER) {	//player friendly
					const byte* UnitClass = ((byte*)(ObjectHeader + UNIT_FIELD_BYTES_0) + 1);
					if (*UnitClass == ClassPriest && GetShapeshiftForm(objBase) == FORM_SPIRITOFREDEMPTION)
						return 1;

					if (pf_count > pf_size) {
						printf("object list size exceeded pf_size %d", pf_size);
						return 1;
					}
					arPlayersFriendly[pf_count] = objBase;
					pf_count++;
					return 1;
				} else {
					if (ifUnitMyPet(objBase)) {
						uint32_t* UnitNativeDisplayID = (uint32_t*)(ObjectHeader + UNIT_FIELD_NATIVEDISPLAYID);
						for (int i = 0; ar_pet_dkghouls_ids[i] != 0; i++) {
							if (*UnitNativeDisplayID == ar_pet_dkghouls_ids[i]) {
								if (mdkp_count > mdkp_size) {
									printf("object list size exceeded mdkp_size %d", mdkp_size);
									return 1;
								}
								arMyDKPets[mdkp_count] = objBase;
								mdkp_count++;
								return 1;
							}
						}
					}
				}
			}
			if (GetUnitTypeFromGUID(guid) == UnitTypeFromGuidVehicle) {  //vehicle friendly
				if (vf_count > vf_size) {
					printf("object list size exceeded vf_size %d", vf_size);
					return 1;
				}
				arVehicleFriendly[vf_count] = objBase;
				vf_count++;
				return 1;
			}

			{ /*
				const uint32_t UNITNPCFLAGS = *(uint32_t*)(ObjectHeader + UNIT_NPC_FLAGS);
			if (UNITNPCFLAGS & ( 0x2))
			{
				if (nf_count > nf_size){ printf("object list size exceeded nf_size %d", nf_size); return 1; }
				arNPCFriendly[nf_count] = objBase;
				nf_count++;
				return 1;
			}
			*/
			}
		}

	} else if (*type & TYPEMASK_ITEM) {  //item
		if (i_count > i_size) {
			printf("object list size exceeded i_size %d", i_size);
			return 1;
		}
		arItems[i_count] = objBase;
		i_count++;
		return 1;
	} else if (*type & TYPEMASK_GAMEOBJECT) {  //GO
		if (go_count > go_size) {
			printf("object list size exceeded go_size %d", go_size);
			return 1;
		}
		arGOs[go_count] = objBase;
		go_count++;
		return 1;
	}

	return 1;
}
int InitObjects() {
	//if (!GUIDSelf) return 0;
	const int32_t* ObjectHeader = *(int32_t**)((uint32_t)BaseSelf + 0x8);
	const uint64_t* myGUID = (uint64_t*)(ObjectHeader + OBJECT_FIELD_GUID);
	memset(Myself, 0, sizeof(struct stObject2));
	initObject((*fGetObjectByGuid)(*myGUID, -1), Myself);

	memset(Target, 0, sizeof(struct stObject2));
	if (GUIDTarget)
		initObject((*fGetObjectByGuid)(GUIDTarget, -1), Target);

	memset(Focus, 0, sizeof(struct stObject2));
	if (GUIDFocus)
		initObject((*fGetObjectByGuid)(GUIDFocus, -1), Focus);

	memset(Pet, 0, sizeof(struct stObject2));
	if (GUIDPet)
		initObject((*fGetObjectByGuid)(GUIDPet, -1), Pet);

	//struct stObject2 stobj = {0};
	//initObject(BaseSelf, &stobj);
	//printf("voj %s %X\n", fGetObjectName(Target.base),*Target.type);
	//printf("voj %X\n", *Target.type);
	//DWORD dwStart = GetTickCount();
	dump_count = 0;
	memset(&arDumpObjects, 0, sizeof(arDumpObjects));
	pf_count = 0;
	memset(&arPlayersFriendly, 0, sizeof(arPlayersFriendly));
	ph_count = 0;
	memset(&arPlayersHostile, 0, sizeof(arPlayersHostile));
	th_count = 0;
	memset(&arTotemHostile, 0, sizeof(arTotemHostile));
	nf_count = 0;
	memset(&arNPCFriendly, 0, sizeof(arNPCFriendly));
	pph_count = 0;
	memset(&arPlayerPetHostile, 0, sizeof(arPlayerPetHostile));
	vh_count = 0;
	memset(&arVehicleHostile, 0, sizeof(arVehicleHostile));
	vf_count = 0;
	memset(&arVehicleFriendly, 0, sizeof(arVehicleFriendly));
	ch_count = 0;
	memset(&arCreatureHostile, 0, sizeof(arCreatureHostile));
	i_count = 0;
	memset(&arItems, 0, sizeof(arItems));
	go_count = 0;
	memset(&arGOs, 0, sizeof(arGOs));
	mdkp_count = 0;
	memset(&arMyDKPets, 0, sizeof(arMyDKPets));
	td_count = 0;  //training dummy
	memset(&arTDs, 0, sizeof(arTDs));

	(*fEnumVisibleObjects)(&enumVisObjCallback, 0);

	if (DUMPREQUEST) {
		DUMPREQUEST = 0;
		static uint32_t tstart = 0;
		if (!tstart)
			tstart = PerformanceCounter();

		uint32_t y = abs(tstart - PerformanceCounter());
		if (y > 500) {
			tstart = 0;
			printf("DUMPREQUEST\n");
			FILE* pFile;
			if ((pFile = fopen("DUMP_arPlayers.txt", "wb")) == NULL) {
				printf("The file DUMP_arPlayers was not opened\n");

			} else {
				char* l0 = "arPlayersHostile: ----------------------------------\r\n";
				fwrite(l0, 1, strlen(l0), pFile);

				for (int i = 0; arPlayersHostile[i] != 0; i++) {
					char st[100] = {0};

					//const int32_t* ObjectHeader = *(int32_t**)(arPlayersHostile[i] + 0x8);
					//byte* pUnitRace = ((byte*)(ObjectHeader + UNIT_FIELD_BYTES_0) + 0);

					//char* n = fGetObjectName((uint32_t*)arPlayersHostile[i]);
					//	sprintf(&st, "%s %s\r\n", n, *pUnitRace);
					//fwrite(&st, 1, strlen(st), pFile);
				}

				char* l1 = "arPlayersFriendly: ----------------------------------\r\n";
				fwrite(l1, 1, strlen(l1), pFile);

				for (int i = 0; arPlayersFriendly[i] != 0; i++) {
					char st[100] = {0};

					uint32_t ob = arPlayersFriendly[i];
					const int32_t* ObjectHeader = *(int32_t**)(ob + 0x8);
					byte* pUnitRace = ((byte*)(ObjectHeader + UNIT_FIELD_BYTES_0) + 0);

					char* n = fGetObjectName((uint32_t*)arPlayersFriendly[i]);
					sprintf(&st, "%s %d\r\n", n, *pUnitRace);
					//sprintf(&st, "%s \r\n", n);
					fwrite(&st, 1, strlen(st), pFile);
				}

				fclose(pFile);
			}
		}
	}
	//printf("----------------------\n");
	//for (int i = 0; arTotemHostile[i] != 0; i++){printf("player  %s %X \n", fGetObjectName(arTotemHostile[i]),arTotemHostile[i]);	}
	//for (int i = 0; arVehicleHostile[i] != 0; i++){		printf("player  %s %X \n", fGetObjectName(arVehicleHostile[i]),arVehicleHostile[i]);		}
	//for (int i = 0; arItems[i] != 0; i++){	printf("player  %s %X \n", fGetObjectName(arItems[i]),arItems[i]);}
	return 1;
}
