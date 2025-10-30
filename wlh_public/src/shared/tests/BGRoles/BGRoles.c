#include "../../main.h"
#include "BGRoles_private.h"
#include "BGRoles_private.c"
#include <stdarg.h>
#include <assert.h>
struct stBGRoles BGRoles = {0};
int initialized = 0;
int bg_id = 0;
float pve_index = 1.03f;
float pve_index2 = 1.16f;
int pve_mana_classes[] = {2, 5, 7, 8, 9, 11};

void _BGRoles_SetPlayerProperties() {
	return;
}

void _BGRoles_Init() {
	if (initialized)
		return;
	// init here
	BGRoles.cacheMaxSlots = szarBGPlayers;
	BGRoles.cacheFreeSlot = 0;
	initialized = 1;
}
// if 1 boundaries are violated . 0 otherwise
int _BGRoles_CheckBoundaries() {
	// tested
	_BGRoles_Init();
	if (BGRoles.cacheFreeSlot >= BGRoles.cacheMaxSlots) {
		printf("BGRoles cache is full\n");
		return 1;
	}
	return 0;
}
//-1 no result. otherwise index in the array
int _BGRoles_GetIndexofPlayer(uint32_t lowGUID) {
	_BGRoles_Init();
	if (lowGUID == 0)
		return -1;

	for (int j = 0; j < BGRoles.cacheFreeSlot; j++) {
		if (lowGUID == (uint32_t)BGRoles.arBGPlayers[j].GUID)

		{
			return j;
		}
	}

	return -1;
}
void _BGRoles_DeletePlayer(uint32_t lowGUID) {
	_BGRoles_Init();
	if (lowGUID == 0)
		return;
	int ix = _BGRoles_GetIndexofPlayer(lowGUID);
	if (ix < 0)
		return;
#if defined(DEBUG_OBJECTCACHE)
	log_line(bg_id, "removing player lowGUID=%X index=%d cfs=%d\n", lowGUID, ix, BGRoles.cacheFreeSlot);
#endif
	memcpy(&BGRoles.arBGPlayers[ix], &BGRoles.arBGPlayers[BGRoles.cacheFreeSlot - 1], sizeof(struct stBGPlayer));
	memset(&BGRoles.arBGPlayers[BGRoles.cacheFreeSlot - 1], 0, sizeof(struct stBGPlayer));
	BGRoles.cacheFreeSlot--;
}

// 1 if lowGUID exists in the cache 0 otherwise.
int _BGRoles_IfPlayerExists(uint32_t lowGUID) {
	_BGRoles_Init();

	if (lowGUID == 0)
		return 0;
	if (_BGRoles_GetIndexofPlayer(lowGUID) < 0) {
		return 0;
	} else {
		return 1;
	}
}
int _BGRoles_DEBUG_Checkduplicates(char* name) {
	for (int i = 0; i < BGRoles.cacheFreeSlot; i++) {
		struct stBGPlayer* p = &BGRoles.arBGPlayers[i];
		if (strcmp(&p->name, name) == 0) {
			return 1;
		}
	}
	return 0;
}

void _BGRoles_SetSinglePlayer(struct stBGPlayer* p, uint32_t objBase) {
#if defined WINDOWS
	struct stObject2 x = {0};
	initObject(objBase, &x);
	p->GUID = *x.pGUID;
	p->unitClass = *x.pUnitClass;
	p->objType = CACHE_OBJECT_PLAYER;
#endif
}
void _BGRoles_UpdateSinglePlayer(struct stBGPlayer* p) {
#if defined WINDOWS
	//returns 0 if object/unit logged off or in stealth.
	p->objBase = (*fGetObjectByGuid)(p->GUID, -1);
	if (!p->objBase) {
#if defined(DEBUG_OBJECTCACHE)
		// log_line(bg_id, "player invisible %s lowGUID=%X cfs=%d\n", p->name, (uint32_t)p->GUID, BGRoles.cacheFreeSlot);
#endif
		p->visible = 0;
		return;
	}
	p->visible = 1;
	fGetObjectLocation((uint32_t*)p->objBase, &p->pos);
	p->canAttack = CanAttack(p->objBase);
	flag_Fix(p->canAttack, &p->tags_player, CACHE_PLAYER_ATTACKABLE);
	p->canAssist = fUnitCanAssist((uint32_t*)Myself->objBase, (uint32_t*)p->objBase, 0);
	flag_Fix(p->canAssist, &p->tags_player, CACHE_PLAYER_ASSISTABLE);

	struct stObject2 x = {0};
	initObject(p->objBase, &x);
	if (*x.PowerType == 0)	//mana
	{
		int as = sizeof(pve_mana_classes) / sizeof(pve_mana_classes[0]);
		for (int i = 0; i < as; i++)
			if (p->unitClass == pve_mana_classes[i]) {
				float in = (float)*x.pMaxHealth / x.MaxPower;
				int pvi1 = in < pve_index;

				if (in < pve_index) {
					p->tags_player |= CACHE_PLAYER_GEAR_PVE;

#if defined(DEBUG_OBJECTCACHE)
					log_line(bg_id, "player PVE GEAR %s lowGUID=%X cfs=%d,hp=%d,mana=%d\n", p->name, (uint32_t)p->GUID, BGRoles.cacheFreeSlot, *x.pMaxHealth, x.MaxPower);
#endif
				} else
					flag_Remove(&p->tags_player, CACHE_PLAYER_GEAR_PVE);

				break;
			}
	}
	flag_Fix(*x.pHealth == 0, &p->tags_player, CACHE_PLAYER_DEAD);
	p->HP_Percent = x.HealthPercent;
#endif
}
void _BGRoles_UpdatePlayers() {
	_BGRoles_Init();
	for (int i = 0; i < BGRoles.cacheFreeSlot; i++) {
		struct stBGPlayer* p = &BGRoles.arBGPlayers[i];
		_BGRoles_UpdateSinglePlayer(p);
	}
}

void _BGRoles_AddPlayer(uint32_t objBase) {
	if (!objBase)
		return;
	_BGRoles_Init();
#if defined WINDOWS
	uint32_t* oh = *(uint32_t**)(objBase + 0x8);
	assert(oh);
	uint64_t GUID = *(uint64_t*)(oh + OBJECT_FIELD_GUID);
	assert(GUID);
	uint32_t lowGUID = (uint32_t)GUID;
	if (_BGRoles_IfPlayerExists(lowGUID))
		return;
	if (_BGRoles_CheckBoundaries())
		return;
	struct stBGPlayer* p = &BGRoles.arBGPlayers[BGRoles.cacheFreeSlot];
	char* c = fGetObjectName((uint32_t*)objBase);
	assert(c);
	if (strcmp(c, "Unknown") == 0) {
#if defined(DEBUG_OBJECTCACHE)
		// log_line(bg_id, "unknown objBase=%X cfs=%d\n", objBase, BGRoles.cacheFreeSlot);
#endif
		return;
	}
	// Fill player structure here
	memcpy(p->name, c, 13);
	_BGRoles_SetSinglePlayer(p, objBase);
	_BGRoles_UpdateSinglePlayer(p);
#if defined(_DEBUG) && defined(DEBUG_OBJECTCACHE)
	int dfound = _BGRoles_DEBUG_Checkduplicates(c);
	int ind = _BGRoles_GetIndexofPlayer(lowGUID);
	if (dfound)
		log_line(bg_id, "dupe found objBase=%X %s giof=%d cfs=%d,%llX\n", objBase, p->name, ind, BGRoles.cacheFreeSlot, p->GUID);
	log_line(bg_id, "adding objBase=%X %s giof=%d cfs=%d,%llX\n", objBase, p->name, ind, BGRoles.cacheFreeSlot, p->GUID);
#endif
#endif
	BGRoles.cacheFreeSlot++;
}

void _BGRoles_FillAssistablePlayers() {
	for (int i = 0; arPlayersFriendly[i] != 0; i++) {
		_BGRoles_AddPlayer(arPlayersFriendly[i]);
	}
}
void _BGRoles_FillHostilePlayers() {
	for (int i = 0; arPlayersHostile[i] != 0; i++) {
		_BGRoles_AddPlayer(arPlayersHostile[i]);
	}
}
void _BGRoles_WipeClean() {
	// tested
#if defined(DEBUG_OBJECTCACHE)
	log_line(bg_id, "wiping %d\n", bg_id);
#endif
	initialized = 0;
	memset(&BGRoles, 0, sizeof(struct stBGRoles));
}
void _BGRoles_ShowInfo(char* format, ...) {
	// tested
#if defined(_DEBUG) && defined(DEBUG_OBJECTCACHE)
	va_list args;
	char buffer[256];
	va_start(args, format);
	vsprintf(buffer, format, args);
	printf("%s", (char*)&buffer);
	va_end(args);
#endif
}
//go through cached objects.
void _BGRoles_Proccess_Cache() {
	for (int i = 0; i < BGRoles.cacheFreeSlot; i++) {
		struct stBGPlayer* p = &BGRoles.arBGPlayers[i];
		if (!p->visible)
			continue;
		if (!PlayerSpec_IfNothingToDo()) {
			struct stObject2 x = {0};
			initObject(p->objBase, &x);

			PlayerSpec_Pulse(p, &x);
		}
		Drawing_Cache_AddEntry(p);
	}
	drawing_cache_finalized = 1;
}
//switch1:0 regular loop;
//switch1:1 reset cache;
//switch1:2 proccess cache;
//switch1:-1 testing;
void BGRoles_Pulse(int switch1) {
#if defined BGROLES
#if defined WINDOWS

	if (vars.InstanceType != INSTANCE_PVP_BG)
		return;
	switch (switch1) {
		case 0: {
			_BGRoles_UpdatePlayers();
			_BGRoles_FillHostilePlayers();
			_BGRoles_FillAssistablePlayers();
			break;
		}
		case 1: {
			bg_id = log_generateID(0);
			_BGRoles_WipeClean();
			_BGRoles_Init();
			break;
		}
		case 2: {
			_BGRoles_Proccess_Cache();
			break;
		}
		case -1: {
			_BGRoles_WipeClean();
			_BGRoles_Init();
			_BGRoles_FillHostilePlayers();
			_BGRoles_FillAssistablePlayers();
			break;
		}
	}
#elif defined LINUX
#endif
#endif
}
