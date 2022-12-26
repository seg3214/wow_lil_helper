#pragma once

#define ar_stClientObject_length 1000
#define ar_PlayerObjectIndex_length 100
#define max_auras 60
#define max_knownSpells 200

//program global vars
extern uint32_t G_fishbotEnabled;
extern uint32_t G_booberTOKEN;
extern uint32_t G_bobberObjectBase;
extern uint32_t G_WorldLoaded;
extern char  G_tstamp[20];//timestamp

//client vars and consts
extern uint32_t G_MapID;
extern char *G_InternalMapName;
extern int32_t G_zoneID;
extern char *G_zoneText;
extern char *G_MinimapZoneText;
extern uint32_t G_IsIndoors;

extern uint32_t G_PlayerBase;
extern uint64_t G_PlayerGUID;
extern char *G_PlayerName;
extern char *G_RealmName;
extern char *G_AccountName;

extern uint32_t* G_ObjectManagerBase;

//object related
struct stClientObject{
	uint32_t pBase;
	uint64_t guid;
	uint32_t type;
	char* ObjectName;
	uint64_t createdby;
	float posx;
	float posy;
	float posz;
	float height;
};

struct stPlayerObject{
	struct stClientObject* pstClientObject;
	uint32_t hostile;// 1 or 0
	float distToMe;
	struct AuraRec arPlayerAuras[max_auras];
};

struct SpellIKnow{
	uint32_t spellId;
	char * spellName;
	char * spellRank;
};

extern struct stClientObject* ar_stClientObject;
extern struct stPlayerObject *pMyselfstPlayerObject ;
extern struct stPlayerObject ar_Players[];
extern struct SpellIKnow arknownSpells[max_knownSpells];

//
//dx vars
extern LPDIRECT3DDEVICE9 pDevice;
