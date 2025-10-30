#include "../../../shared/main.h"
#include "player_spec_private.c"

#define MAXPlayerSpecDataVars 500
struct PlayerSpecVars {
	uint64_t GUID[MAXPlayerSpecDataVars];
	uint32_t freeSlot;
	enum eTalentSpecs spec[MAXPlayerSpecDataVars];
};
struct PlayerSpecVars PlayerSpecVars = {0};
int dps_classes[] = {3, 4, 8, 9};

int PlayerSpec_IfNothingToDo() {
	for (int i = 0; i < BGRoles.cacheFreeSlot; i++) {
		if (!BGRoles.arBGPlayers[i].playerSpec) {
			return 0;
		}
	}
	return 1;
}

struct stbgplayer_specaurasdata* PlayerSpec_IfAuraIsInList(char* s) {
	int size = sizeof(arBGPlayer_SpecData) / sizeof(struct stbgplayer_specaurasdata);
	for (int i = 0; i < size; i++) {
		if (strcmp(s, arBGPlayer_SpecData[i].aura_name) == 0) {
			// printf("hello %s\n", arBGPlayer_SpecData[i].aura_name);
			return &arBGPlayer_SpecData[i];
			break;
		}
	}
	return 0;
}
int PlayerSpec_IfExist(uint64_t* pGUID) {
	for (int i = 0; i < PlayerSpecVars.freeSlot; i++) {
		if (*pGUID == PlayerSpecVars.GUID[i]) {
			return 1;
		}
	}
	return 0;
}
void PlayerSpec_Add1(uint64_t* pGUID, enum eTalentSpecs spec) {
	if (PlayerSpecVars.freeSlot >= MAXPlayerSpecDataVars) {
		printf("error PlayerSpecVars size\n");
		return;
	}
	// printf("guid3[]=%llX\n", *pGUID);
	PlayerSpecVars.GUID[PlayerSpecVars.freeSlot] = *pGUID;
	// printf("guid4[]=%llX\n", PlayerSpecVars.GUID[PlayerSpecVars.freeSlot]);
	PlayerSpecVars.spec[PlayerSpecVars.freeSlot] = spec;
	PlayerSpecVars.freeSlot++;
}
void PlayerSpec_CheckAuras1(struct stObject2* o) {
	for (int i = 0; o->auras[i].pAuraId != 0; i++) {
		struct stAura* pAura = &o->auras[i];
		struct stbgplayer_specaurasdata* pad = PlayerSpec_IfAuraIsInList(pAura->pAuraName);
		if (!pad)
			continue;
		if (!PlayerSpec_IfExist(pAura->pCreatorGuid)) {
			if (!(*o->pUnitClass == ClassMage && *o->pGUID == *pAura->pCreatorGuid)) {
				PlayerSpec_Add1(pAura->pCreatorGuid, pad->spec_that_has_this_aura);
			}
		}

		// printf("guid1[%d]=%llX %f\n", i, p->GUID, p->pos[3]);
	}
}
void PlayerSpec_CheckBGPlayerGUID(struct stBGPlayer* p) {
	if (p->playerSpec) {
		// printf("skipping guid%llX\n", p->GUID);
		return;
	}
	// printf("trying to bond guid %llX\n", p->GUID);

	//quick fix to mark all dps classes without finding out spec that is not needed at the moment
	int as = sizeof(dps_classes) / sizeof(dps_classes[0]);
	for (int i = 0; i < as; i++) {
		if (p->unitClass == dps_classes[i]) {
			p->playerSpec = SPEC_DPS;
			p->tags_player |= CACHE_PLAYER_SPEC_DPS;
			return;
		}
	}
	for (int i = 0; i < PlayerSpecVars.freeSlot; i++) {
		if (PlayerSpecVars.GUID[i] == p->GUID) {
			p->playerSpec = PlayerSpecVars.spec[i];
			if (p->playerSpec & SPEC_PROTO) {
				p->tags_player |= CACHE_PLAYER_SPEC_TANK;
			} else if (p->playerSpec & SPEC_HEALER) {
				p->tags_player |= CACHE_PLAYER_SPEC_HEALER;
			}
			break;
		}
	}
}
void PlayerSpec_Pulse(struct stBGPlayer* p, struct stObject2* o) {
	PlayerSpec_CheckAuras1(o);
	PlayerSpec_CheckBGPlayerGUID(p);
}
