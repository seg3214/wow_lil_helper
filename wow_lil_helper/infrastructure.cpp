#include "includes.h"

uint32_t worldLoadedTimerBusy = 0;



void ReadGameVars(){
	format_time(G_tstamp);
	printf("%s Reading game vars \n", G_tstamp);
	
	G_ObjectManagerBase = (uint32_t*)(*(uint32_t*)constStaticClientConnection + constObjectManagerOffset);
	if (!*G_ObjectManagerBase) {
		printf("Object Manager does not exist");
		return;
	}
	G_PlayerGUID = *(uint64_t*)(*G_ObjectManagerBase + constlocalGuidOffset);
	G_PlayerName = (char*)constPlayerName;
	G_RealmName = (char*)constRealmName;
	G_AccountName = (char*)constAccountName;
	
	
}

void ReadGameVars2(){
	G_MapID = *(uint32_t*)constMapID;
	//printf("MapID %d\n", MapID);
	G_InternalMapName = (char*)constInternalMapName;
	//printf("InternalMapName %s\n", InternalMapName);
	G_zoneID = *(int32_t*)constzoneID;
	//printf("zoneID %d\n", zoneID);
	G_zoneText = *(char**)constzoneText;
	//printf("zoneText %s\n", zoneText);
	G_MinimapZoneText = *(char**)constGetMinimapZoneText;
	//printf("MinimapZoneText %s\n", MinimapZoneText);
	G_IsIndoors = *(uint32_t*)constIsIndoors;
	// printf("IsIndoors %d\n", IsIndoors);
}

void PulseEndScene(){
	static int doonce_EndScene_flag = 0;
	static DWORD frame200 = 0;//200
	static DWORD frame500 = 0;//500
	static DWORD frame01 = 0;//1000
	static DWORD frame05 = 0;//5000
	static DWORD frame30 = 0;//30000
	static DWORD nowframe = 0;

	if (doonce_EndScene_flag == 0) {
		doonce_EndScene_flag = 1;
		frame200 = frame500 = frame01 = frame05 = frame30 = GetTickCount();

		 drawing_Init();
		/*
		if (WorldLoaded){
			ReadKnownSpells();
			ReadGameObjects();
		}*/
	}
	
	nowframe = GetTickCount();

	// Block runs wildly every frame in client
	if (G_WorldLoaded){
		ReadGameObjects();
		
//		drawing_Pulse();
	}
	
	// end of wild block

	if (nowframe - frame200 > 200) { //runs every 0.2 seconds
		frame200 = nowframe;
		//if (WorldLoaded) ReadGameObjects();
		//printf("frame200\n");
		//wrap_SetGlobalVar("shit", "0");
//		if (WorldLoaded) scripting_pulse();
		if (G_WorldLoaded) {
			ReadGameVars2();
			aura_pulse();
		}

		
	}
	else
		if (nowframe - frame500 > 500) { //runs every 0.5 seconds
			frame500 = nowframe;
			

		}
		else
			if (nowframe - frame01 > 1000) { //runs every 1 second
				frame01 = nowframe;

				
				if (G_WorldLoaded) fishbot_Pulse();
			}
			else
				if (nowframe - frame05 > 5000) { //runs every 5 seconds
					frame05 = nowframe;
					if (G_WorldLoaded) ReadKnownSpells();
				}
				else
					if (nowframe - frame30 > 30000) { //runs every 30 seconds
						if (G_WorldLoaded){
						frame30 = nowframe;
						UINT32* hwact = (UINT32*)0xxxx; //antiafk 
						memcpy(hwact, &frame30, 4); //antiafk 
						}
					}
}

// Fires when world is loaded
void WorldJustLoaded(){
	if (worldLoadedTimerBusy) {
		worldLoadedTimerBusy = 0;
		G_WorldLoaded = 1;
		//printf("WorldJustLoaded ONCE\n");
	}
	format_time(G_tstamp);
	printf("%s World just loaded \n", G_tstamp);

	//put here non repeatable with pulse 
	//Random_init(); //init seed of random generator
	ReadGameVars();
	clientDB_Init();

	ReadKnownSpells();
	ReadGameObjects();
	
	
	
	
}

// Fires when world is unloaded
void CopeWithWorldGone(){
	format_time(G_tstamp);
	printf("%s World gone out \n", G_tstamp);
}

void CheckUpOnWorldLoaded(){
	if (*(uint32_t*)constWorldLoaded == 1 && G_WorldLoaded == 1){ //world is still running.		business as usual
		//printf("wordl stil runing \n");
	}
	else
		if (*(uint32_t*)constWorldLoaded == 0 && G_WorldLoaded == 1){ //world just have gone out.		stop something ?
			G_WorldLoaded = 0;
			CopeWithWorldGone();
			//printf("world gone out\n");
		}
		else
			if (*(uint32_t*)constWorldLoaded == 1 && G_WorldLoaded == 0){ //world just loaded.   start up a timer> start da business
				if (!worldLoadedTimerBusy){
					ActivateTimer(1000, &WorldJustLoaded);
					worldLoadedTimerBusy = 1;
				}
				//printf("world loaded\n");
			}
}

void program_started(){
	utils_start();
	format_time(G_tstamp);
	//SetFPS(60);
	printf("%s Hi !\n", G_tstamp);

	//fishBot_Start();//is like we clicked start fishbot
	
}

void cleanup(){ //freeing memory etc
	if (G_fishbotEnabled) fishBot_Shutdown();
	objects_cleanup();
	drawing_cleanup();
	utils_stop();
}

