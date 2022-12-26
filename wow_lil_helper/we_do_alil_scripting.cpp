#include "includes.h"

#define runsPerPulse 2
char *varName = "shit";
int running = 0;

void running_done(){
	running = 0;
}

void warlock_bg_aflic(){
	CastSpellByName("Detect Invisibility");
	//printf("runCount %d\n", runCount);
	printf("hello from warlock_bg_aflic\n");
	running_done();
}

void scripting_pulse(){
	if (running) return;

	char *letsgo = wrap_GetGlobalVar_string(varName);
	
	if (!letsgo) wrap_SetGlobalVar(varName, "none");
	if (!letsgo) return;
	

	//printf("%s\n", letsgo);
	if (strcmp(letsgo, "none") != 0){
		running = 1;
		wrap_SetGlobalVar(varName, "none");

		if (strcmp(letsgo, "warlock_bg_aflic") == 0){
			
			warlock_bg_aflic();
		}
		else if (letsgo){

		}

	}
	
}