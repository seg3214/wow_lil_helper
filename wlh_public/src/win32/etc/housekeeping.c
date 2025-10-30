#include "../shared/main.h"

uint32_t G_WorldLoaded = 0;
uint32_t worldLoadedTimerBusy = 0;
int SHUTTINGDOWN = 0;
int DUMPREQUEST = 0;
uint32_t timerflags;  //mask 0x10=30000 0x8=5000 0x4=2000 0x2=1000 0x1=500
struct stClientVars vars = {0};

#ifndef LINUX
extern void scripting_pulse();
extern void InstallHooks();
extern void UninstallHooks();
extern void ReadKnownSpells();

void ReadClientVars() {
	vars.ZoneID = constZoneID;
	//printf("zoneID %d\n", vars.ZoneID);
	vars.ZoneText = constZoneText;
	//printf("zoneText %s\n", vars.ZoneText );
	vars.ZoneMinimapText = constMinimapZoneText;
	//printf("MinimapZoneText %s\n", vars.ZoneMinimapText );
	vars.IsIndoors = constIsIndoors;
	//printf("IsIndoors %d\n", vars.IsIndoors  );
	vars.MapID = constMapID;

	//none - Not inside an instance
	//arena - Player versus player arena
	//pvp - Player versus player battleground
	//party - 5-man instance
	//raid - Raid instance
	//
	//none	    bg	   lfg
	//nil none	1 pvp	1 party
	double* r = ExecuteInternal("IsInInstance() ", 1);
	vars.IsInInstance = luaResToInteger(r, 0);
	char* itype = luaResToString(r, 1);
	switch (itype[2]) {
		case 'n': {
			vars.InstanceType = INSTANCE_NONE;
			break;
		}
		case 'e': {
			vars.InstanceType = INSTANCE_PVP_ARENA;
			break;
		}
		case 'p': {
			vars.InstanceType = INSTANCE_PVP_BG;
			break;
		}
		case 'r': {
			vars.InstanceType = INSTANCE_DUNGEON_PARTY;
			break;
		}
		case 'i': {
			vars.InstanceType = INSTANCE_DUNGEON_RAID;
			break;
		}
		default:
			vars.InstanceType = INSTANCE_NONE;
	}
}
// Fires when world is loaded
void WorldJustLoaded() {
	if (worldLoadedTimerBusy) {
		worldLoadedTimerBusy = 0;

		G_WorldLoaded = 1;
	}
	//CDataStore_Init();
	SetPHs();  //here because bg bug
	SetGlobalVar(varName, "none");
	scriptName = GetGlobalVar_string(varName);  //changes the pointer to string from original scriptName
	ReadClientVars();

	//reset cache
	BGRoles_Pulse(1);
	ExecuteInternal("local f_clce = CreateFrame(\"frame\",nil, UIParent); f_clce:SetScript(\"OnUpdate\", CombatLogClearEntries);", 0);
	printf("%s World just loaded \n", format_time());
}
// Fires when world is unloaded
void CopeWithWorldGone() {
	printf("%s World gone out \n", format_time());
	ResetPHs();
	//CDataStore_Destroy();
	//drawing_once = 0;
	//if (autoCastinginProgress) { AutoScriptEnd(); }
}

void CheckUpOnWorldLoaded() {
	//printf("world %d guid %d\n",WorldLoaded,GUIDSelf);
	if (WorldLoaded == 1 && G_WorldLoaded == 1) {	      //world is still running.		business as usual
	} else if (WorldLoaded == 0 && G_WorldLoaded == 1) {  //world just have gone out.		stop something ?
		G_WorldLoaded = 0;
		CopeWithWorldGone();
		//printf("world gone out\n");
	} else if (WorldLoaded == 1 && G_WorldLoaded == 0) {  //world just loaded.   start up a timer> start da business
		if (!worldLoadedTimerBusy) {
			ActivateTimer(1000, &WorldJustLoaded);
			worldLoadedTimerBusy = 1;
		}
		//printf("world loaded\n");
	}
}

void PulseEndScene() {
	static int doonce_EndScene_flag = 0;
	static uint64_t nowframe = 0;
	static uint64_t frame05 = 0;  //5000
	static uint64_t frame30 = 0;  //30000

	Drawing_Cache_Reset();

	if (!G_WorldLoaded)
		return;
	if (doonce_EndScene_flag == 0) {
		doonce_EndScene_flag = 1;
		//frame200 = frame500 = frame01 = frame05 = frame30 = GetTickCount();
		frame05 = frame30 = GetTickCount64();
		ReadKnownSpells();
	}
	ReadClientVars();
	if (!InitObjects())
		return;
	nowframe = GetTickCount64();

	BGRoles_Pulse(0);
	BGRoles_Pulse(2);
	Drawing_Pulse();
	scripting_pulse();
	AutoScriptPulse(nowframe);

	if (nowframe - frame05 > 5000) {  //runs every 5 seconds
		frame05 = nowframe;
		ReadKnownSpells();
	} else if (nowframe - frame30 > 30000) {  //runs every 30 seconds
		frame30 = nowframe;
		uint32_t* hwact = (uint32_t*)constAntiAFK;
		memcpy(hwact, &frame30, 4);
	}
}
void PulseTimers() {
	static int doonce_flag = 0;
	static uint64_t nowframe = 0;
	static uint64_t frame005 = 0;  //500
	static uint64_t frame01 = 0;   //1000
	static uint64_t frame02 = 0;   //2000
	static uint64_t frame05 = 0;   //5000
	static uint64_t frame30 = 0;   //30000

	if (doonce_flag == 0) {
		doonce_flag = 1;
		alive = nowframe = frame005 = frame01 = frame02 = frame05 = frame30 = GetTickCount64();
	} else {
		nowframe = GetTickCount64();
		alive = nowframe;
	}
	if ((nowframe - frame005) > 500) {  //runs every 0.5 seconds
		frame005 = nowframe;
		timerflags = timerflags | 0x1;
	}
	if ((nowframe - frame01) > 1000) {  //runs every 1 seconds
		frame01 = nowframe;
		timerflags = timerflags | 0x2;
	}
	if ((nowframe - frame02) > 2000) {  //runs every 2 seconds
		frame02 = nowframe;
		timerflags = timerflags | 0x4;
	}
	if ((nowframe - frame05) > 5000) {  //runs every 5 seconds
		frame05 = nowframe;
		timerflags = timerflags | 0x8;
	}
	if ((nowframe - frame30) > 30000) {  //runs every 30 seconds
		frame30 = nowframe;
		timerflags = timerflags | 0x10;
	}
}
void shutdown() {
	if (buffer_SpellEntry)
		free(buffer_SpellEntry);
	DrawingRelease();

	utils_stop();
	UninstallHooks();
	ResetPHs();
	CDataStore_Destroy();
}
void Loop_EndScene() {
	if (!SHUTTINGDOWN) {
		static int once = 0;
		if (!once) {
			once = 1;
			utils_start();
			printf("%s Hi !\n", format_time());

#ifdef _DEBUG
			printf("DEBUG MODE-----------\n");
#endif
			ReadClientDBTables();
		}
		CheckUpOnTimers();
		PulseTimers();

		CheckUpOnWorldLoaded();
		PulseEndScene();

#ifdef _DEBUG
		test_pulse();
#endif

		timerflags = 0;
		if ((GetAsyncKeyState(VK_F11) & 0x01)) {
			if (!DUMPREQUEST) {
				DUMPREQUEST = 1;
			}
		}
	} else {  //shutting down
		static int once = 0;
		if (!once) {
			once = 1;
		}
	}
}
void Loop_HookThread() {
	CDataStore_Init();
	SetPHs();
	InstallHooks();

	while (!(GetAsyncKeyState(VK_F12) & 0x01)) {
		Sleep(1);
		if (abs(GetTickCount64() - alive) > 1000) {  //crashed
			// printf("crashed\n");
			alive = GetTickCount64();
		}
	}
	SHUTTINGDOWN = 1;
	Sleep(500);
	shutdown();
}
#endif
