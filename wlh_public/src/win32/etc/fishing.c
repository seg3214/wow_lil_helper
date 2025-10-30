#include "../shared/main.h"

#pragma region Fishing

extern const uint32_t constAutoLoot_Activate_Pointer;
extern const uint32_t constAutoLoot_Activate_Offset;
extern const uint32_t constIsBobbing;
extern const uint64_t* pGUIDLootWindow;

int fishing_in_proggress;
int savedAutoLootValue;

enum enBobberStatusMask {
	bobber_deployed = 0x00000001,
	casting_fishing = 0x00000002,
	bobber_boobing = 0x00000004,
	loot_window_opened = 0x00000008,
};

uint32_t GetAutoLootState() {
	uint32_t pp = *(uint32_t*)constAutoLoot_Activate_Pointer;
	uint32_t* pALValue = (uint32_t*)(pp + constAutoLoot_Activate_Offset);
	return *pALValue;
}
void ToggleAutoLoot(uint32_t flag) {
	uint32_t pp = *(uint32_t*)constAutoLoot_Activate_Pointer;
	uint32_t* pALValue = (uint32_t*)(pp + constAutoLoot_Activate_Offset);
	if (flag == 1)
		*pALValue = 0x1;
	else if (flag == 0)
		*pALValue = 0x0;
}
uint32_t GetChanneledCastingId() {  //casting id is now in object unit structure
	return *(uint32_t*)(Myself->objBase + constChanneledCastingId);
}
uint32_t findBooba() {
	uint32_t boober1 = 0, boober2 = 0;
	int bc = 0;
	for (int i = 0; i < go_count; i++) {
		//for (int i = 0; arClientObjects[i].pBase != 0; i++){
		const int32_t* ObjectHeader = *(int32_t**)(arGOs[i] + 0x8);
		uint64_t* p_CREATED_BY_GUID = (uint64_t*)(ObjectHeader + OBJECT_FIELD_CREATED_BY);

		//fishbot finding the boober
		if (*p_CREATED_BY_GUID == GUIDSelf) {
			//printf("%s %llX \n",fGetObjectName((uint32_t*)arGOs[i]),*p_CREATED_BY_GUID);
			if (strcmp(fGetObjectName((uint32_t*)arGOs[i]), "Fishing Bobber") == 0) {
				bc++;
				if (bc == 1) {
					boober1 = arGOs[i];
				} else if (bc == 2) {
					boober2 = arGOs[i];
				}
			}
		}
		//fishbot finding the boober end
	}

	if (boober2) {
		return boober2;
	} else if (boober1) {
		return boober1;
	}
	return 0;
}
void fishing() {
	if (!(timerflags & 0x2))
		return;

	uint32_t fishing_status = 0;  //not used rn

	if (GetChanneledCastingId() == GetMySpellIdByMySpellName("Fishing")) {
		fishing_status += casting_fishing;
		//printf("fishing\n");
	} else {
		CastSpellByName("Fishing", Myself);
	}

	uint32_t booba_base = findBooba();
	if (booba_base) {
		fishing_status += bobber_deployed;

		//printf("booba found @ %X\n",booba_base);
		byte isbobbing = *(uint32_t*)(booba_base + constIsBobbing);

		if (isbobbing) {
			//printf("booba boobing\n");
			fishing_status += bobber_boobing;
			if (!*pGUIDLootWindow) {
				//	printf("interacting\n");
				fObjectInteract((uint32_t*)booba_base);
			}
		}
	}
}
void fishing_start() {
	printf("fishing starting...\n");
	fishing_in_proggress = 1;
	savedAutoLootValue = GetAutoLootState();
	ToggleAutoLoot(1);
}
void fishing_stop() {
	if (fishing_in_proggress) {
		printf("fishing ending...\n");
		fishing_in_proggress = 0;
		ToggleAutoLoot(savedAutoLootValue);  //restoring AL value
	}
}
#pragma endregion
