#include "includes.h"

int DEBUGMODE = 1;//modify this

uint32_t G_fishbotEnabled = 0; //dont modify;
uint32_t G_booberTOKEN = 0;
uint32_t G_bobberObjectBase = 0;
uint32_t G_WorldLoaded = 0;
char  G_tstamp[20];//timestamp

uint32_t G_MapID = 0;
char *G_InternalMapName = 0;
int32_t G_zoneID = 0;
char *G_zoneText = 0;
char *G_MinimapZoneText = 0;
uint32_t G_IsIndoors = 0;

uint32_t G_PlayerBase = 0;
uint64_t G_PlayerGUID = 0;
char *G_PlayerName = 0;
char *G_RealmName = 0;
char *G_AccountName = 0;

uint32_t* G_ObjectManagerBase = 0;

