#include "includes.h"



int savedAutoLootValue = 0;


int isFishing(){
	if (GetChanneledCastingId() == GetSpellIdBySpellName("Fishing"))
		return 1;
	else
		return 0;
}

int isBooberValid() {
	if (G_bobberObjectBase) return 1;
	return 0;
}

int isBooberBoobing() {
	if (isBooberValid()){
		byte isbobbing = *(uint32_t*)(G_bobberObjectBase + constIsBobbing);
		return isbobbing;
	}
	return -1;
}

void StartFishing(){
	if (!isFishing()){
		if (CastSpellByName("Fishing") == 0) printf("Cant find Fishing spell\n");
		print("CastFishing");
	}
	
}

void HandleBoobing(){
	if (isFishing() && isBooberValid() && isBooberBoobing()){
		fObjectInteract((uint32_t*)G_bobberObjectBase);
		print("LootFishes");
	}
	
}

void fishBot_Start(){
	savedAutoLootValue = GetAutoLoot();
	ToggleAutoLoot(1); //for fishy fishing yaaaaaaaaay
	G_fishbotEnabled = 1;
}

void fishBot_Shutdown(){
	G_fishbotEnabled = 0;
	ToggleAutoLoot(savedAutoLootValue); //restoring AL value
	
}

void fishbot_Pulse(){
	if (G_fishbotEnabled){
		StartFishing();
		//FindDaBoober();
		HandleBoobing();
	}
}
/////////////////////////////////////////////////////
void fishing_test(){
	//StartFishing();
}